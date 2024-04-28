#ifndef _XGEN_METADATA_H_
#define _XGEN_METADATA_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace XGen {

struct Decl {
    std::string name;
    std::string attrs;
    std::string comment;
};

struct Field : Decl {
    std::string type;
    std::string raw_type;
    std::size_t offset;
};

struct Function : Decl {
    std::string ret_type;
    std::string raw_ret_type;
    std::vector<Field> params;
    bool is_const;
    bool is_nothrow;
};

struct Record : Decl {
    std::vector<std::string> bases;
    std::vector<Field> fields;
    std::vector<Function> methods;
};

struct Enumerator : Decl {
    uint64_t value {};
};

struct Enum : Decl {
    std::vector<Enumerator> values;
};

struct Database {
    std::vector<Record> records;
    std::vector<Function> functions;
    std::vector<Enum> enums;
};

using FileDataMap = std::unordered_map<std::string, Database>;

} // namespace XGen

#endif