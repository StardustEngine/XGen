#ifndef _XGEN_PRECOMPILER_H_
#define _XGEN_PRECOMPILER_H_

#include "marker_database.h"
#include "metadata.h"

#include <clang/Tooling/Tooling.h>

namespace XGen {

using namespace clang;

class Precompiler {
public:
    Precompiler(
        const tooling::CompilationDatabase& compilations,
        const std::string& source_code,
        MarkerDatabase* markers = nullptr);

    int Run(FileDataMap* datamap);

private:
    tooling::ClangTool m_tool;
    MarkerDatabase* m_markers;
};

} // namespace XGen

#endif