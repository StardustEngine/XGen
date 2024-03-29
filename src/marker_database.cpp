#include "marker_database.h"

namespace XGen {
FieldMarker FieldMarker::FromJSON(const llvm::json::Value& val)
{
    FieldMarker field_marker;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return field_marker;
    }
    if (auto attrs_val = root->getString("attrs")) {
        field_marker.attrs = attrs_val->str();
    }
    return field_marker;
}

FunctionMarker FunctionMarker::FromJSON(const llvm::json::Value& val)
{
    FunctionMarker function_marker;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return function_marker;
    }
    if (auto attrs_val = root->getString("attrs")) {
        function_marker.attrs = attrs_val->str();
    }
    if (const auto* params_val = root->getObject("params")) {
        for (const auto& [name, param_val] : *params_val) {
            function_marker.params[name.str()] = FieldMarker::FromJSON(param_val);
        }
    }
    return function_marker;
}

RecordMarker RecordMarker::FromJSON(const llvm::json::Value& val)
{
    RecordMarker record_marker;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return record_marker;
    }
    if (auto attrs_val = root->getString("attrs")) {
        record_marker.attrs = attrs_val->str();
    }
    if (const auto* fields_val = root->getObject("fields")) {
        for (const auto& [name, field_val] : *fields_val) {
            record_marker.fields[name.str()] = FieldMarker::FromJSON(field_val);
        }
    }
    if (const auto* functions_val = root->getObject("functions")) {
        for (const auto& [name, function_val] : *functions_val) {
            record_marker.methods[name.str()] = FunctionMarker::FromJSON(function_val);
        }
    }
    return record_marker;
}

EnumMarker EnumMarker::FromJSON(const llvm::json::Value& val)
{
    EnumMarker enum_marker;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return enum_marker;
    }
    if (auto attrs = root->getString("attrs")) {
        enum_marker.attrs = attrs->str();
    }
    return enum_marker;
}

MarkerDatabase MarkerDatabase::FromJSON(const llvm::json::Value& val)
{
    MarkerDatabase marker_database;

    const auto* root = val.getAsObject();
    if (root == nullptr) {
        return marker_database;
    }
    if (const auto* records_val = root->getObject("records")) {
        for (const auto& [name, record_val] : *records_val) {
            marker_database.records[name.str()] = RecordMarker::FromJSON(record_val);
        }
    }
    if (const auto* functions_val = root->getObject("functions")) {
        for (const auto& [name, function_val] : *functions_val) {
            marker_database.functions[name.str()] = FunctionMarker::FromJSON(function_val);
        }
    }
    if (const auto* enums_val = root->getObject("enums")) {
        for (const auto& [name, enum_val] : *enums_val) {
            marker_database.enums[name.str()] = EnumMarker::FromJSON(enum_val);
        }
    }
    return marker_database;
}
}  // namespace XGen