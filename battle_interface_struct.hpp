#ifndef battle_interface_struct_hpp
#define battle_interface_struct_hpp
#include "globel.h"
#pragma pack(1)
#pragma region input data

struct input_obj_data_base
{
public:
	uint08	_camps;
	uint32	_attack_flags;
	uint32	_be_attack_flags;
	uint32	_attack_special_effects;
    uint32	_curHp;
	uint32	_hp;
	uint32	_threat;	
	uint32	_aoe_area;
	uint32	_radius;
};
struct input_obj_data_unit : input_obj_data_base
{
public:
	uint32	_attack_distance;
	uint32	_seek_distance;
	uint32	_max_speed;
	uint32	_attack_power;
	uint32	_attack_power_scale_to_building;
	uint32	_attack_speed;
};
struct input_obj_data_test_tank : public input_obj_data_unit
{
public:
	uint32	_max_attack_angle;
};
struct input_obj_data_bomb : input_obj_data_base
{
public:
	uint32	_bomb_distance;
	uint32	_bomb_power;
	uint32	_exploded_timer;
};
struct input_obj_data_bingying : public input_obj_data_base
{
public:
	int cd;
	int width;
	int height;
};
struct input_obj_data_gongchengshi : public input_obj_data_unit
{
public:
	int duringTime;
	
};
#pragma endregion

#pragma region output data 
typedef struct output_obj_data_base
{
public:
	uint32 ID;
	uint32 TYPE;
	float x;
	float y;
	float face;
	uint32 cur_hp;
	uint32 status_flag = 0;
	uint32 effect_flag;
	uint32 target_id;
} output_obj_data_base;
struct output_obj_data_test_tank : public output_obj_data_base
{
public:
	float aiming;
};
struct output_obj_data_bomb : public output_obj_data_base
{
	uint32 bombed;
};
struct output_obj_data_fortress :public output_obj_data_base
{
public:
	float aiming;
};
#pragma endregion
#endif
