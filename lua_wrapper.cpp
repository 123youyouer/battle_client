
#include "globel.h"
#include "lua_wrapper.h"

/*
int LUA_create_unit(lua_State* L)
{
	int type = luaL_checknumber(L, 1);
	int camps = luaL_checknumber(L, 2);
	int x = luaL_checknumber(L, 3);
	int y = luaL_checknumber(L, 4);
	int l = luaL_checknumber(L, 5);
	int r = luaL_checknumber(L, 6);

	msg_create_unit msg;
	msg.set_type(type);
	msg.set_camps(camps);
	msg.set_x(x);
	msg.set_y(y);
	msg.set_lvl(l);
	msg.set_ranks(r);

	char* buf = new char[msg.ByteSize()];
	msg.SerializeToArray(buf, msg.ByteSize());
	globel::globel_data->to_csharp(msg_type::create_unit, msg.ByteSize(), buf);
	delete buf;

	return 0;
}
int LUA_cast_card(lua_State* L)
{
	int type = luaL_checknumber(L, 1);
	int camps = luaL_checknumber(L, 2);
	int x = luaL_checknumber(L, 3);
	int y = luaL_checknumber(L, 4);
	int l = luaL_checknumber(L, 5);
	int r = luaL_checknumber(L, 6);

	msg_cast_card msg;
	msg.set_type(type);
	msg.set_camps(camps);
	msg.set_x(x);
	msg.set_y(y);
	msg.set_lvl(l);
	msg.set_ranks(r);

	char* buf = new char[msg.ByteSize()];
	msg.SerializeToArray(buf, msg.ByteSize());
	globel::globel_data->to_csharp(msg_type::cast_card, msg.ByteSize(), buf);
	delete buf;

	return 0;
}
void lua_wrapper::start(const char* path)
{
	luaL_dofile(L, path);
	//luaL_dofile(L, "D:\\lantaoyiyou\\client\\c++plugin\\battle_cpp\\lua_test\\test.lua");
}
void lua_wrapper::init()
{
	int err = 0;
	err = lua_getglobal(L, "init");
	err = lua_pcall(L, 0, 0, 0);
	if (err)
	{
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); // 从栈中弹出错误消息
	}
}
void lua_wrapper::update(int frame)
{
	int err = 0;
	err=lua_getglobal(L, "update");
	lua_pushnumber(L, frame);
	err=lua_pcall(L, 1, 0, 0);
	if (err)
	{
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1); // 从栈中弹出错误消息
	}
}
lua_wrapper::lua_wrapper()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_register(L, "create_unit", LUA_create_unit);
	lua_register(L, "cast_card", LUA_cast_card);
}
lua_wrapper::~lua_wrapper()
{
	lua_close(L);
	L = NULL;
}
*/