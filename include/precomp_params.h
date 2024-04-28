#ifndef _XGEN_PARSE_PARAMS_H_
#define _XGEN_PARSE_PARAMS_H_

#include "marker_database.h"

namespace XGen {

struct PrecompParams {
    std::vector<std::string> headerfiles;
    std::vector<std::string> compilaions;
    MarkerDatabase markers;

    static PrecompParams FromJSON(const llvm::json::Value& val);
};

llvm::Expected<PrecompParams> LoadPrecompParamsFromFile(const std::string& filepath);
} // namespace XGen

#endif