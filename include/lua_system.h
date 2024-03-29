#ifndef _XGEN_LUA_SYSTEM_H_
#define _XGEN_LUA_SYSTEM_H_

#include <llvm/Support/Error.h>
#include <lua.hpp>
#include <string>

namespace XGen {

class LuaSystem {
public:
    LuaSystem();

    llvm::Error RunCode(const std::string& code);
    llvm::Error RunFile(const std::string& filepath);

private:
    lua_State* m_state;
};

} // namespace XGen

#endif