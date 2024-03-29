#include "precomp_params.h"

#include <llvm/Support/MemoryBuffer.h>

namespace XGen {

PrecompParams PrecompParams::FromJSON(const llvm::json::Value& val)
{
    PrecompParams params;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return params;
    }
    // headerfiles
    if (const auto* headerfiles_val = root->getArray("headerfiles")) {
        for (const auto& child_val : *headerfiles_val) {
            if (auto try_get_value = child_val.getAsString()) {
                std::string headerfile = try_get_value->str();
                params.headerfiles.push_back(headerfile);
            }
        }
    }
    // compilations
    if (const auto* compilations_val = root->getArray("compilations")) {
        for (const auto& child_val : *compilations_val) {
            if (auto try_get_value = child_val.getAsString()) {
                std::string compilation = try_get_value->str();
                params.compilaions.push_back(compilation);
            }
        }
    }
    // markers
    if (const auto* markers_val = root->get("markers")) {
        params.markers = std::move(MarkerDatabase::FromJSON(*markers_val));
    }
    return params;
}

llvm::Expected<PrecompParams> LoadPrecompParamsFromFile(const std::string& filepath)
{
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> expected_params
        = llvm::MemoryBuffer::getFile(filepath);
    if (std::error_code error_code = expected_params.getError()) {

        auto error_message = llvm::formatv(
            "Error load file from \"{0}\", message: {1}",
            filepath.c_str(), error_code.message().c_str());
        return llvm::make_error<llvm::StringError>(
            error_message.str(), llvm::inconvertibleErrorCode());
    }
    std::unique_ptr<llvm::MemoryBuffer> params_data = std::move(*expected_params);

    llvm::Expected<llvm::json::Value> err_or_val
        = llvm::json::parse(params_data->getBuffer());
    if (!err_or_val) {
        auto error_message = llvm::formatv(
            "Error parse json from \"{0}\"",
            filepath.c_str());
        return llvm::make_error<llvm::StringError>(
            error_message.str(), llvm::inconvertibleErrorCode());
    }
    llvm::json::Value params_val = std::move(*err_or_val);

    return std::move(PrecompParams::FromJSON(params_val));
}
} // namespace XGen