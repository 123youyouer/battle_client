#ifndef rtti_h
#define rtti_h
#include "flags.h"
class interface_rtti
{
protected:
	int p;
	
	template<class T>
	void init_rtti()
	{
	}
public:
	void* rtti_point[32];
	

public:
	flags<unsigned int> interface_flags;
	template<typename T>
	inline T* get_interface_point()
	{
		return (T*)rtti_point[T::interface_type];
	}
	interface_rtti();
};
const unsigned int rtti_i_rigid_body = 0;
const unsigned int rtti_i_attacker = 1;
const unsigned int rtti_i_seeker = 2;
const unsigned int rtti_i_tank = 3;
const unsigned int rtti_i_world_listener = 4;
const unsigned int rtti_i_time_bomb = 5;
const unsigned int rtti_i_attack_able = 6;
const unsigned int rtti_i_occupied = 7;
const unsigned int rtti_i_static_obstacle = 8;

const unsigned int rtti_flag_rigid_body = 1 << 0;
const unsigned int rtti_flag_attacker = 1 << 1;
const unsigned int rtti_flag_seeker = 1 << 2;
const unsigned int rtti_flag_tank = 1 << 3;
const unsigned int rtti_flag_world_listener = 1 << 4;
const unsigned int rtti_flag_time_bomb = 1 << 5;
const unsigned int rtti_flag_attack_able = 1 << 6;
const unsigned int rtti_flag_occupied = 1 << 7;
const unsigned int rtti_flag_static_obstacle = 1 << 8;
#endif // !rtti_h
