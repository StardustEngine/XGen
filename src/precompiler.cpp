#include "precompiler.h"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

namespace XGen {

class MetadataCollector : public ast_matchers::MatchFinder::MatchCallback {
public:
    MetadataCollector(FileDataMap* datamap, MarkerDatabase* markers)
        : m_datamap(datamap)
        , m_markers(markers)
    {
    }

    void run(const ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (const auto* RD = result.Nodes.getNodeAs<clang::RecordDecl>("record")) {
            RD->dump(); // 打印 RecordDecl
        }
    }

private:
    FileDataMap* m_datamap;
    MarkerDatabase* m_markers;
};

Precompiler::Precompiler(
    const clang::tooling::CompilationDatabase& compilations,
    const std::string& source_code,
    MarkerDatabase* markers)
    : m_tool(compilations, std::vector<std::string>({ "precompile.cpp" }))
    , m_markers(markers)
{
    m_tool.mapVirtualFile("precompile.cpp", source_code);
}

int Precompiler::Run(FileDataMap* datamap)
{
    MetadataCollector collector { datamap, m_markers };
    ast_matchers::MatchFinder finder;
    finder.addMatcher(
        clang::ast_matchers::cxxRecordDecl(
            clang::ast_matchers::unless(clang::ast_matchers::isImplicit()))
            .bind("record"),
        &collector);
    return m_tool.run(tooling::newFrontendActionFactory(&finder).get());
}

} // namespace XGen