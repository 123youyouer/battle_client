#include "export.h"
#include "globel.h"
#include "battle_map.hpp"
#include "lua_wrapper.h"
#include "globel.h"

#include "battle_energy.hpp"


int	__stdcall frame_bgn(int frame_index, int millisecond)
{
	return globel::globel_data->frame_bgn(frame_index, millisecond);
}
int	__stdcall frame_upd(int frame_index, int millisecond)
{
	return globel::globel_data->frame_upd(frame_index, millisecond);
}
int	__stdcall frame_end(int frame_index, int millisecond)
{
	return globel::globel_data->frame_end(frame_index, millisecond);
}
int	__stdcall init_battle(int map, int w, int h, int millisecond_per_frame)
{
	globel::globel_data->reset(map, w, h, millisecond_per_frame);
	return 0;
}
char* __stdcall create_object(int id, int type, int x, int y, char* base_data)
{
	return globel::globel_data->build_unit(id, type, x, y, base_data);
}
void __stdcall get_object_data(char* handle, char* data)
{
	globel::globel_data->deserialization(handle, data);
}
bool __stdcall death_object(char* handle)
{
	return globel::globel_data->death_object(handle);
}
void __stdcall remove_object(char* handle)
{
	return globel::globel_data->remove_object(handle);
}
void __stdcall demage(char* tar, int effect_flag, int value)
{
	globel::globel_data->demage(tar, effect_flag, value);
}
void __stdcall aoe2(int camps, int attack_flags, int effect_flags, int x, int y, int radius, int value, int scale_to_zhujidi)
{
	globel::globel_data->aoe2(camps, attack_flags, x, y, radius, value, effect_flags, scale_to_zhujidi);
}
void __stdcall aoe(int camps, int attack_flags, int effect_flags, int x,int y, int radius, int value)
{
	globel::globel_data->aoe(camps, attack_flags, x, y, radius, value, effect_flags);
}
void __stdcall set_callback(void* func)
{
	globel::globel_data->set_callback(func);
}
void __stdcall set_object_location(char* handle, int x, int y)
{
	globel::globel_data->set_object_location(handle, x, y);
}
void __stdcall set_object_movment(char* handle, int speed, int face, int aiming_face)
{
	globel::globel_data->set_object_movment(handle, speed, face, aiming_face);
}
void __stdcall set_object_BeAttackEffect(char* handle, int effect)
{
	globel::globel_data->set_object_BeAttackEffect(handle, effect);
}
void __stdcall set_object_curHP(char* handle, int value)
{
	globel::globel_data->set_object_HP(handle,value);
}
int	__stdcall get_object_HP(char* handle)
{
	return globel::globel_data->get_object_HP(handle);
}
int	__stdcall get_stat_flags(char* handle)
{
	return globel::globel_data->get_stat_flags(handle);
}
float __stdcall get_current_energy(int camps)
{
	return globel::globel_data->get_current_energy(camps);
}
bool __stdcall cast_energy(int camps, float v)
{
	return globel::globel_data->cast_energy(camps, v);
}
void __stdcall add_path(int* data, int len)
{
	globel::globel_data->add_path(data, len);
}
void __stdcall add_obstacle(int* data, int len)
{
	globel::globel_data->cur_map->add_obstacle(data, len);
}
void __stdcall set_energy_data(int camps, int max, int first, float grow_perframe, int fast_start_frame, float fast_ratio)
{
	globel::globel_data->set_energy_data(camps, max, first, grow_perframe, fast_start_frame, fast_ratio);
}
void __stdcall use_trajectory(char* tar, bool use)
{
	globel::globel_data->use_trajectory(tar, use);
}
void __stdcall set_building_shild(char* handle, bool value)
{
	globel::globel_data->set_building_shild(handle, value);
}
bool __stdcall put_down_check(int x, int y, int radius)
{
	return globel::globel_data->put_down_check(x, y, radius);
}
void __stdcall set_energy(int camps, int value)
{
	globel::globel_data->set_energy(camps, value);
}
int	__stdcall get_cell(int x, int y)
{
	return globel::globel_data->get_cell(x, y);
}
void __stdcall set_pvp_flag(int flag)
{
	globel::globel_data->set_pvp_flag(flag);
}
bool __stdcall aoe_check(char* handle, int attack_flags, int x, int y, int radius)
{
	return globel::globel_data->aoe_check(handle, attack_flags, x, y, radius);
}
void __stdcall set_object_status(char* handle, int flags)
{
	return globel::globel_data->set_object_status(handle, flags);
}