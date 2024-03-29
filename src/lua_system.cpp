#include "lua_system.h"

namespace XGen {

LuaSystem::LuaSystem()
    : m_state(luaL_newstate())
{
}

} // namespace XGen