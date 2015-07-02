#ifndef LUA_MATERIAL_H
#define LUA_MATERIAL_H

#include "../LuaBase.h"
#include "../LuaClass.h"
#include "../LuaConsole.h"
#include <util/MaterialLibrary.h>

/**
 * 	Lua implementation of the Material class
 */
class LuaMaterial : public LuaBase
{
public:
	LuaMaterial(lua_State*);
	~LuaMaterial();

	int set(lua_State*);
	int setValue(lua_State*);
	UserMaterial* getModelObject();

	static const std::string className;
	static const LuaClass<LuaMaterial>::Function functions[];
	static const LuaClass<LuaMaterial>::Property properties[];
private:
	UserMaterial* m_material;
};

#endif
