#ifndef GLOBEL_H
#define GLOBEL_H

#define NULL 0
#define PI 3.1415926f
#define MAX_FORCE	1.0f
#include <chipmunk/chipmunk_private.h>
#include <vector>
#include "rtti.h"

typedef unsigned char uint08;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef void(*event_callback)(int, int, char*);

class battle_object_base;
class battle_map_base;
class lua_wrapper;
class battle_energy;

class object_type
{
public:
	const static int battle_object_type_bomb = 299999;

	const static int battle_object_type_zhongzhuangdabing = 200000;
	const static int battle_object_type_fushegongbing = 200001;
	const static int battle_object_type_huixiong_tank = 200002;
	const static int battle_object_type_V3 = 200003;
	const static int battle_object_type_xiniu_tank = 200004;
	const static int battle_object_type_zhandouyaosai = 200005;
	const static int battle_object_type_wuzhuangzhishengji = 200006;
	const static int battle_object_type_meiguodabing = 200007;
	const static int battle_object_type_guanglengtank = 200008;
	const static int battle_object_type_gaitetank = 200009;
	const static int battle_object_type_dog = 200010;
	const static int battle_object_type_tanya = 200011;
	const static int battle_object_type_feixingbing = 200012;
	const static int battle_object_type_zibao_kache = 200013;
	const static int battle_object_type_jinuofu = 200014;
	const static int battle_object_type_cineng_tank = 200015;
	const static int battle_object_type_unused = 200016;
	const static int battle_object_type_kongbufenzi = 200017;
	const static int battle_object_type_tianqi_tank = 200018;
	const static int battle_object_type_kongzhongzhiyuan = 200019;
	const static int battle_object_type_yuanchengdaodan = 200020;
	const static int battle_object_type_sanbing = 200021;
	const static int battle_object_type_hedanfashe = 200022;
	const static int battle_object_type_goushe = 200023;
	const static int battle_object_type_bingying = 200024;
	const static int battle_object_type_cineng_tank_facory = 200025;
	const static int battle_object_type_gaitejipao = 200026;
	const static int battle_object_type_dianta = 200027;
	const static int battle_object_type_aiguozhe = 200028;
	const static int battle_object_type_jupao = 200029;
	const static int battle_object_type_kuangshijinglianqi = 200030;
	const static int battle_object_type_zhujidi = 200031;
	const static int battle_object_type_fujidi = 200032;
	const static int battle_object_type_gongchengshi = 200036;
	const static int battle_object_type_kuangshouren = 200037;
	const static int battle_object_type_guanglengta = 200038;

	const static int battle_object_type_aiguozhe_two = 200039;

	const static int battle_object_boss_kongbufenzi = 299993;
	const static int battle_object_boss_kuangshouren = 299998;
};
class collision_type
{
public:
	const static uint16 collision_type_terrain			= 1;
	const static uint16 collision_type_tank				= 1 << 1;
	const static uint16 collision_type_ground_soilder	= 1 << 2;
	const static uint16 collision_type_aircraft			= 1 << 3;
	const static uint16 collision_type_sky1				= 1 << 4;
	const static uint16 collision_type_sky2				= 1 << 5;
	const static uint16 collision_type_building = 1 << 6;
	const static uint16 collision_type_obstacle = 1 << 7;
	const static uint16 collision_type_all = 0xFFFF;
};
class globel
{
public:
	static globel* globel_data;
private:
	void reset_lua(int battle_type, int battle_id);
public:
	int pvp_flag;
	char* holder = NULL;
	uint32 millisecond_per_frame;
	uint32 current_frame;
	battle_energy* battle_energy_0;
	battle_energy* battle_energy_1;
	lua_wrapper* lua;
	event_callback func_event_callback;
	std::vector<interface_rtti*> the_battle_units;
	cpSpace* this_world;
	battle_map_base* cur_map;
	
	void set_pvp_flag(int flag);
	void reset(uint32 map, int w, int h, int millisecond_per_frame);
	void create_edge(float x0, float y0, float x1, float y1);
	void to_csharp(int type, int len, char* data);
	void Log(char* szBuf);
	void on_unit_removed(int id);
	void reset_units();
	void set_energy_data(int camps, int max, int first, float grow_perframe, int fast_start_frame, float fast_ratio);
	void set_energy(int camps, int value);
	int get_cell(int x, int y);
	int frame_bgn(int frame_index, int millisecond);
	int	frame_upd(int frame_index, int millisecond);
	int	frame_end(int frame_index, int millisecond);
	char* build_unit(int id, int type, int x, int y, char* base_data);
	void deserialization(char* unit, char* out);
	bool death_object(char* handle);
	void remove_object(char* handle);
	void demage(char* tar,int effect_flag, int value);
	void aoe(int camps, int attack_flags, int x, int y, int radius, int value, int effect_flag);
	void aoe2(int camps, int attack_flags, int x, int y, int radius, int value, int effect_flag, int scale_to_zhujidi);
	void set_callback(void* func);
	int	get_stat_flags(char* handle);
	float get_current_energy(int camps);
	bool cast_energy(int camps, float v);
	void add_path(int* data, int len);
	void add_obstacle(int* data, int len);
	void use_trajectory(char* handle, bool use);
	void set_building_shild(char* handle, bool use);
	bool put_down_check(int x, int y, int radius);

	int	get_object_HP(char* handle);
	void set_object_BeAttackEffect(char* handle, int effect);
	void set_object_HP(char* handle,int value);
	void set_object_location(char* handle, int x, int y);
	void set_object_movment(char* handle, int speed, int face, int aiming_face);
	void set_object_status(char* handle, int flag);
	bool aoe_check(char* handle, int attack_flags, int x, int y, int radius);
	globel();

};
struct globel_func
{
	static float distance(float x1, float y1, float x2, float y2)
	{
		return sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	static float distance_squ(float x1, float y1, float x2, float y2)
	{
		return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	}
	static float direction_to_east(cpVect v)
	{
		float res = cpvtoangle(v) * 180 / PI;
		if (res < 0)
		{
			res = 360.0f + res;
		}
		else if (res >= 360.0f)
		{
			res = fmod(res, 360.0f);
		}
		return res;
			
	}
	/*static float direction_to_east(float x1, float y1, float x2, float y2)
	{
		float dx = x2 - x1;
		float dy = y2 - y1;
		if (dx == 0 && dy == 0)	return 0;
		if (dx == 0 && dy>0)	return 90;
		if (dx == 0 && dy<0)	return 270;
		if (dx>0 && dy == 0)	return 0;
		if (dx<0 && dy == 0)	return 180;
		if (dx>0 && dy>0)	return atanf(abs((y2 - y1) / (x2 - x1))) * 180 / PI;
		if (dx>0 && dy<0)	return 360 - (atanf(abs((y2 - y1) / (x2 - x1))) * 180 / PI);
		if (dx<0 && dy>0)	return 180 - (atanf(abs((y2 - y1) / (x2 - x1))) * 180 / PI);
		if (dx<0 && dy<0)	return 180 + (atanf(abs((y2 - y1) / (x2 - x1))) * 180 / PI);
		return 0;
	}*/
	static float compute_min_angle(float a1, float a2)
	{
		float f1f2 = (a1 > a2) ? (a1 - a2) : 360 - (a1 - a2);
		float f2f1 = (a2 > a1) ? (a2 - a1) : 360 - (a2 - a1);
		float res = f1f2 > f2f1 ? f2f1 : f1f2;
		if (res >= 360.0f)
			res -= 360.0f;
		return res;
	}
	static float direction_to_north(float x1, float y1, float x2, float y2)
	{
		float differ1 = x2 - x1;
		float differ2 = y2 - y1;
		float angel = 0;
		if (differ1 >= 0)
		{
			if (differ2 == 0)
				angel = 90;
			else if (differ2 > 0)
				angel = atanf(abs(differ1 / differ2)) * (180 / PI);
			else
				angel = 180 - atanf(abs(differ1 / differ2)) * (180 / PI);
		}
		else
		{
			if (differ2 == 0)
				angel = 270;
			else if (differ2 > 0)
				angel = 360 - atanf(abs(differ1 / differ2)) * (180 / PI);
			else
			{
				angel = atanf(abs(differ1 / differ2)) * (180 / PI) + 180;
			}
		}
		return angel;
	}
	static bool point_equal(void* p1, void* p2)
	{
		return p1 == p2;
	}
};
#endif // !GLOBEL_H#pragma once
