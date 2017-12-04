#ifndef unit_body_hpp
#define unit_body_hpp
#include "vec_2d.hpp"
#include "flags.h"
#include "globel.h"
class unit_body
{
public:
	vec_2d point;
	float radius;
	vec_2d speed;
	float max_speed;
	flags<int> contact_mask;
	flags<int> contact_catalog;
protected:
	void update_location(int ms)
	{
		if (speed.is_zero())
			return;
		point += speed*ms / 1000;
	}
	bool check_mask(unit_body* o)
	{
		return contact_catalog.chk(o->contact_mask);
	}
	bool is_contacted(unit_body* o)
	{
		if (!check_mask(o))
			return false;
		float len_squ = (point - o->point).len_squ();
		if (((radius + o->radius)*(radius + o->radius)) <= len_squ)
			return false;
		return true;
	}
	bool is_faraway(unit_body* o)
	{
		if (speed.is_zero())
			return true;
		float len_squ = (point - o->point).len_squ();
		if (len_squ < (point + speed - o->point).len_squ())
			return true;
		return false;
	}
public:
	void move(int ms)
	{
		std::vector<interface_rtti*>::iterator i;
		for (i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
		{
			
		}
	}
	
};
#endif // !unit_body_hpp
