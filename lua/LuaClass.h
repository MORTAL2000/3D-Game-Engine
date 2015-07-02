#ifndef LUA_CLASS_H
#define LUA_CLASS_H

#include "LuaBase.h"

template<class T>
class LuaClass
{
public:
    static void registerClass(lua_State*, bool);
    static T* check(lua_State*, int);
    static T* test(lua_State*, int);
    static void push(lua_State*, T*);

    struct Function
    {
        const char* name;
        int (T::*function)(lua_State*);
    };

    struct Property
    {
        const char* name;
        int (T::*getter)(lua_State*);
        int (T::*setter)(lua_State*);
    };
private:
    static int constructor(lua_State*);
    static int index(lua_State*);
    static int newindex(lua_State*);

    static int add(lua_State*);
    static int sub(lua_State*);
    static int mul(lua_State*);
    static int div(lua_State*);
    static int equals(lua_State*);
    static int tostring(lua_State*);

    static int thunk(lua_State*);
    static int gc_obj(lua_State*);
};

/* register class */
template<class T>
void LuaClass<T>::registerClass(lua_State* lua, bool useOperators)
{
    /* register constructor */
    lua_pushcfunction(lua, &LuaClass<T>::constructor);
    lua_setglobal(lua, T::className.c_str());

    /* register table */
    luaL_newmetatable(lua, T::className.c_str());
    int metatable = lua_gettop(lua);

    lua_pushstring(lua, "__gc");
    lua_pushcfunction(lua, &LuaClass<T>::gc_obj);
    lua_settable(lua, metatable);

    lua_pushstring(lua, "__index");
    lua_pushcfunction(lua, &LuaClass<T>::index);
    lua_settable(lua, metatable);

    lua_pushstring(lua, "__newindex");
    lua_pushcfunction(lua, &LuaClass<T>::newindex);
    lua_settable(lua, metatable);

    if(useOperators)
    {
        lua_pushstring(lua, "__add");
        lua_pushcfunction(lua, &LuaClass<T>::add);
        lua_settable(lua, metatable);

        lua_pushstring(lua, "__sub");
        lua_pushcfunction(lua, &LuaClass<T>::sub);
        lua_settable(lua, metatable);

        lua_pushstring(lua, "__mul");
        lua_pushcfunction(lua, &LuaClass<T>::mul);
        lua_settable(lua, metatable);

        lua_pushstring(lua, "__div");
        lua_pushcfunction(lua, &LuaClass<T>::div);
        lua_settable(lua, metatable);

        lua_pushstring(lua, "__eq");
        lua_pushcfunction(lua, &LuaClass<T>::equals);
        lua_settable(lua, metatable);

        lua_pushstring(lua, "__tostring");
        lua_pushcfunction(lua, &LuaClass<T>::tostring);
        lua_settable(lua, metatable);
    }

    for(int i = 0; T::properties[i].name; i++)
    {
        lua_pushstring(lua, T::properties[i].name);
        lua_pushnumber(lua, i);
        lua_settable(lua, metatable);
    }

    for(int i = 0; T::functions[i].name; i++)
    {
        lua_pushstring(lua, T::functions[i].name);
        lua_pushnumber(lua, i | (1 << 8));
        lua_settable(lua, metatable);
    }
}

/* retrieves wrapped class at index */
template<class T>
T* LuaClass<T>::check(lua_State* lua, int index)
{
    T** obj = static_cast<T**>(luaL_checkudata(lua, index, T::className.c_str()));
    if(!obj) return 0;
    return *obj;
}

template<class T>
T* LuaClass<T>::test(lua_State* lua, int index)
{
	T** obj = static_cast<T**>(luaL_testudata(lua, index, T::className.c_str()));
	if(!obj) return 0;
	return *obj;
}

template<class T>
void LuaClass<T>::push(lua_State* lua, T* instance)
{
    T** a = (T**)lua_newuserdata(lua, sizeof(T*));
    *a = instance;
    luaL_getmetatable(lua, T::className.c_str());
    lua_setmetatable(lua, -2);
}

/* constructor */
template<class T>
int LuaClass<T>::constructor(lua_State* lua)
{
    T* obj = new T(lua);
    T** a = static_cast<T**>(lua_newuserdata(lua, sizeof(T*)));
    *a = obj;

    luaL_getmetatable(lua, T::className.c_str());
    lua_setmetatable(lua, -2);
    return 1;
}

/* index */
template<class T>
int LuaClass<T>::index(lua_State* lua)
{
    lua_getmetatable(lua, 1);
    lua_pushvalue(lua, 2);
    lua_rawget(lua, -2);
    if(lua_isnumber(lua, -1))
    {
        int index = lua_tonumber(lua, -1);
        T** obj = static_cast<T**>(lua_touserdata(lua, 1));
        lua_pushvalue(lua, 3);

        if(index & (1 << 8))
        {
            lua_pushnumber(lua, index ^ (1 << 8));
            lua_pushlightuserdata(lua, obj);
            lua_pushcclosure(lua, &LuaClass<T>::thunk, 2);
            return 1;
        }

        lua_pop(lua, 2);
        lua_remove(lua, 1);
        lua_remove(lua, 1);
        return ((*obj)->*(T::properties[index].getter))(lua);
    }
    return 1;
}

/* newindex */
template<class T>
int LuaClass<T>::newindex(lua_State* lua)
{
    lua_getmetatable(lua, 1);
    lua_pushvalue(lua, 2);
    lua_rawget(lua, -2);

    if(lua_isnumber(lua, -1))
    {
        int index = lua_tonumber(lua, -1);
        T** obj = static_cast<T**>(lua_touserdata(lua, 1));

        if(!obj || !*obj)
        {
            Console::log("Lua :: No object available, intern error");
            return 0;
        }

        if(index >> 8)
        {
            Console::log("Lua :: Trying to set a function");
            return 0;
        }

        lua_pop(lua, 2);
        lua_remove(lua, 1);
        lua_remove(lua, 1);

        return ((*obj)->*(T::properties[index].setter))(lua);
    }

    return 0;
}

/* add operator */
template<class T>
int LuaClass<T>::add(lua_State* lua)
{
    T* object = test(lua, -1);
    T* assignee = test(lua, 1);
    if(object && assignee)
    {
        object->add(lua, assignee);
        return 1;
    }
    else
    {
        Console::log("Lua :: Unsupported operation");
    }
    return 0;
}

/* substract operator */
template<class T>
int LuaClass<T>::sub(lua_State* lua)
{
    T* object = test(lua, -1);
    T* assignee = test(lua, 1);
    if(object && assignee)
    {
        object->sub(lua, assignee);
        return 1;
    }
    else
    {
        Console::log("Lua :: Unsupported operation");
    }
    return 0;
}

/* multiplication operator */
template<class T>
int LuaClass<T>::mul(lua_State* lua)
{
    T* object = test(lua, -1);
    T* assignee = test(lua, 1);
    if(object && assignee)
    {
        object->mul(lua, assignee);
        return 1;
    }
    else
    {
        Console::log("Lua :: Unsupported operation");
    }
    return 0;
}

/* division operator */
template<class T>
int LuaClass<T>::div(lua_State* lua)
{
    T* object = test(lua, -1);
    T* assignee = test(lua, 1);
    if(object && assignee)
    {
        object->div(lua, assignee);
        return 1;
    }
    else
    {
        Console::log("Lua :: Unsupported operation");
    }
    return 0;
}

/* equals operator */
template<class T>
int LuaClass<T>::equals(lua_State* lua)
{
    T* obj1 = test(lua, -1);
    T* obj2 = test(lua, 1);
    lua_pushboolean(lua, obj1->eq(obj2));
    return 1;
}

/* tostring operator */
template<class T>
int LuaClass<T>::tostring(lua_State* lua)
{
    T* obj = test(lua, -1);
    if(obj)
    {
        lua_pushstring(lua, obj->tostring().c_str());
        return 1;
    }
    return 0;
}

/* functions */
template<class T>
int LuaClass<T>::thunk(lua_State* lua)
{
    int i = (int)lua_tonumber(lua, lua_upvalueindex(1));
    T** obj = static_cast<T**>(lua_touserdata(lua, lua_upvalueindex(2)));
    return ((*obj)->*(T::functions[i].function))(lua);
}

/* garbage collection */
template<class T>
int LuaClass<T>::gc_obj(lua_State* lua)
{
    T** obj = static_cast<T**>(lua_touserdata(lua, -1));
    if(obj)
    {
        T* clazz = *obj;
        if(clazz)
        {
            delete clazz;
            clazz = 0;
        }
    }
    return 0;
}

#endif
