#include "lua_system.h"
#include "options_parser.h"
#include "precompiler.h"


static llvm::cl::OptionCategory category_option { "XGen Options" };

static llvm::cl::opt<std::string> config_filepath("config",
    llvm::cl::desc("Specify config file"),
    llvm::cl::Optional, llvm::cl::cat(category_option),
    llvm::cl::sub(*llvm::cl::AllSubCommands),
    llvm::cl::value_desc("file"));

static llvm::cl::opt<std::string> output_path("output",
    llvm::cl::desc("Specify output directory path"),
    llvm::cl::Optional, llvm::cl::cat(category_option),
    llvm::cl::sub(*llvm::cl::AllSubCommands),
    llvm::cl::value_desc("directory"));

int main(int argc, char* argv[])
{
    std::vector<const char*> arg_list(argc + 1);
    arg_list[0] = argv[0];
    arg_list[1] = "--extra-arg=-D__meta__";
    for (int i = 1; i < argc; ++i) {
        arg_list[i + 1] = argv[i];
    }
    argc += 1;
    auto expected_parser
        = XGen::OptionsParser::Create(argc, arg_list.data(), category_option);
    if (!expected_parser) {
        llvm::errs() << expected_parser.takeError();
        return 1;
    }
    auto& options_parser = expected_parser.get();

    XGen::Precompiler precompiler {
        options_parser.GetCompilations(),
        options_parser.GetCollection()
    };

    XGen::FileDataMap datamap;
    int result = precompiler.Run(&datamap);

    XGen::LuaSystem lua_system;

    return 0;
}
