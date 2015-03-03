#ifndef LUA_MATRIX_H
#define LUA_MATRIX_H

#include "../LuaBase.h"
#include "../LuaClass.h"
#include "../LuaConsole.h"

/**
 *	Lua implementation of a 4x4 Matrix
 */
class LuaMatrix : public LuaBase
{
public:
	LuaMatrix(lua_State*);
	LuaMatrix(const mat4&);
	~LuaMatrix();

	int get(lua_State*);
	int set(lua_State*);

	virtual void mul(lua_State*, LuaBase*);
	virtual std::string tostring();

	mat4 getModelObject();
	static const std::string className;
	static const LuaClass<LuaMatrix>::Function functions[];
	static const LuaClass<LuaMatrix>::Property properties[];
private:
	mat4 m_matrix;
};

#endif
