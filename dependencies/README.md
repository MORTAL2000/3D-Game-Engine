Build
=============


**Bullet**

* Modified generated makefile not to build the demos (demos were causing errors)
* Modified btQuickprof.cpp (replaced getTickCount64 by get_tick_count())

**unsigned long long**
get_tick_count(**void**) 
{
if WINVER >= 0x0600
	return GetTickCount64();
else
	return GetTickCount();
endif
}
	
**GWEN**

* Modified Gwen.cpp line 49:
commented out debugbreak, because it is not supported (?)

* Modified Controls/Text.cpp, line 258:
commented out pop_back, added "str.resize(str.size() - 1);"
	
* Removed Windows.cpp <Shlo...> link and commented open folder out 

* Removed GWEN-DLL in generated Makefile due to errors
	
Author
---
Alexander Koch 2015