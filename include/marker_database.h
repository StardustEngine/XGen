#ifndef _XGEN_MARKER_DATABASE_H_
#define _XGEN_MARKER_DATABASE_H_

#include <llvm/Support/JSON.h>
#include <string>
#include <unordered_map>

namespace XGen {
struct DeclMarker {
    std::string attrs;
};

struct FieldMarker : DeclMarker {
    static FieldMarker FromJSON(const llvm::json::Value& val);
};

struct FunctionMarker : DeclMarker {
    std::unordered_map<std::string, FieldMarker> params;

    static FunctionMarker FromJSON(const llvm::json::Value& val);
};

struct RecordMarker : DeclMarker {
    std::unordered_map<std::string, FieldMarker> fields;
    std::unordered_map<std::string, FunctionMarker> methods;

    static RecordMarker FromJSON(const llvm::json::Value& val);
};

struct EnumeratorMarker : DeclMarker {
    static EnumeratorMarker FromJSON(const llvm::json::Value& val);
};

struct EnumMarker : DeclMarker {
    std::unordered_map<std::string, EnumeratorMarker> values;

    static EnumMarker FromJSON(const llvm::json::Value& val);
};

struct MarkerDatabase {
    std::unordered_map<std::string, RecordMarker> records;
    std::unordered_map<std::string, FunctionMarker> functions;
    std::unordered_map<std::string, EnumMarker> enums;

    static MarkerDatabase FromJSON(const llvm::json::Value& val);
};
}  // namespace XGen

#endif