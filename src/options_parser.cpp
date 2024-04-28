#include "options_parser.h"
#include "precomp_params.h"

#include <clang/tooling/CommonOptionsParser.h>

namespace XGen {
llvm::Error OptionsParser::Init(int& argc, const char** argv,
    llvm::cl::OptionCategory& category, const char* overview)
{
    static llvm::cl::opt<std::string> param_filepath("param",
        llvm::cl::desc("Specify parsing parameter file"),
        llvm::cl::Required, llvm::cl::cat(category),
        llvm::cl::sub(*llvm::cl::AllSubCommands),
        llvm::cl::value_desc("file"));

    static llvm::cl::list<std::string> args_after("extra-arg",
        llvm::cl::desc("Additional argument to append to the compiler command line"),
        llvm::cl::cat(category), llvm::cl::sub(*llvm::cl::AllSubCommands));

    static llvm::cl::list<std::string> args_before("extra-arg-before",
        llvm::cl::desc("Additional argument to prepend to the compiler command line"),
        llvm::cl::cat(category), llvm::cl::sub(*llvm::cl::AllSubCommands));

    llvm::cl::ResetAllOptionOccurrences();
    llvm::cl::HideUnrelatedOptions(category);

    // parse command line.
    {
        std::string error_message;
        llvm::raw_string_ostream ostream(error_message);
        if (!llvm::cl::ParseCommandLineOptions(argc, argv, overview, &ostream)) {
            ostream.flush();
            return llvm::make_error<llvm::StringError>(
                error_message, llvm::inconvertibleErrorCode());
        }
    }

    auto expected_params
        = LoadPrecompParamsFromFile(param_filepath);
    if (!expected_params) {
        return expected_params.takeError();
    }
    PrecompParams& params = expected_params.get();

    // build compilations.
    {
        std::string compilations_buffer;
        for (const std::string& arg : params.compilaions) {
            compilations_buffer += arg + "\n";
        }
        std::string error_message;
        m_compilations = tooling::FixedCompilationDatabase::loadFromBuffer(
            ".", compilations_buffer, error_message);
        if (!m_compilations) {
            std::string msg = llvm::formatv(
                "Error while trying to load compilation database: {0}\n");
            return llvm::make_error<llvm::StringError>(msg,
                llvm::inconvertibleErrorCode());
        }
    }

    // adjust compilations.
    {
        auto adjusting_compilations
            = std::make_unique<tooling::ArgumentsAdjustingCompilations>(std::move(m_compilations));
        m_adjuster = tooling::getInsertArgumentAdjuster(
            args_before, tooling::ArgumentInsertPosition::BEGIN);
        m_adjuster = tooling::combineAdjusters(
            std::move(m_adjuster),
            tooling::getInsertArgumentAdjuster(args_before, tooling::ArgumentInsertPosition::END));
        adjusting_compilations->appendArgumentsAdjuster(m_adjuster);
        m_compilations = std::move(adjusting_compilations);
    }

    // collect all headerfiles into one cpp file.
    {
        m_collection.clear();
        for (const std::string& headerfile : params.headerfiles) {
            m_collection += "#include \"" + headerfile + "\"\n";
        }
    }

    // marker database
    m_markers = params.markers;

    return llvm::Error::success();
}

llvm::Expected<OptionsParser> OptionsParser::Create(
    int& argc, const char** argv,
    llvm::cl::OptionCategory& category,
    const char* overview)
{
    OptionsParser parser;
    llvm::Error err = parser.Init(argc, argv, category, overview);
    if (err) {
        return std::move(err);
    }
    return std::move(parser);
}

OptionsParser::OptionsParser(int& argc, const char** argv,
    llvm::cl::OptionCategory& category, const char* overview)
{
    llvm::Error err = Init(argc, argv, category, overview);
    if (err) {
        llvm::report_fatal_error(
            Twine("OptionsParser: failed to parse command-line arguments. ")
            + llvm::toString(std::move(err)));
    }
}
} // namespace XGen