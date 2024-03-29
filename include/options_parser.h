#ifndef _XGEN_OPTIONS_PARSER_H_
#define _XGEN_OPTIONS_PARSER_H_

#include "marker_database.h"

#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Error.h>

namespace XGen {

using namespace clang;

class OptionsParser {
public:
    static llvm::Expected<OptionsParser> Create(
        int& argc, const char** argv,
        llvm::cl::OptionCategory& Category,
        const char* overview = "");

    const std::string& GetCollection() { return m_collection; }
    tooling::CompilationDatabase& GetCompilations() { return *m_compilations; }
    tooling::ArgumentsAdjuster GetArgumentsAdjuster() { return m_adjuster; }
    MarkerDatabase& GetMarkerDatabase() { return m_markers; }

protected:
    OptionsParser(int& argc, const char** argv,
        llvm::cl::OptionCategory& Category, const char* overview);

private:
    OptionsParser() = default;

    llvm::Error Init(int& argc, const char** argv,
        llvm::cl::OptionCategory& Category, const char* overview);

    std::string m_collection;
    std::unique_ptr<tooling::CompilationDatabase> m_compilations;
    MarkerDatabase m_markers;
    tooling::ArgumentsAdjuster m_adjuster;
};
} // namespace XGen

#endif
