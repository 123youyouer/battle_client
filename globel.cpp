#include "globel.h"
#include "battle_unit_builder.hpp"
#include "contact_filter.hpp"
#include "contact_listener.hpp"
#include "battle_energy.hpp"

globel* globel::globel_data = new globel();
void globel::to_csharp(int type, int len, char* data)
{
	if (this->func_event_callback != NULL)
		this->func_event_callback(type, len, data);
}
void globel::on_unit_removed(int id)
{
	if (this->func_event_callback == NULL)
		return;
	to_csharp(4, 4, (char*)&id);
}
void globel::Log(char* szBuf)
{
	if (this->func_event_callback != NULL)
	{
	}
}
void globel::reset_lua(int battle_type, int battle_id)
{
}
void globel::reset_units()
{
	std::vector<interface_rtti*>::iterator i = the_battle_units.begin();
	while (!the_battle_units.empty())
	{
		delete (static_cast<battle_unit::battle_base_unit*>(*i));
		i = the_battle_units.erase(i);		
	}
}
void globel::set_pvp_flag(int flag)
{
	this->pvp_flag = flag;
}
void globel::reset(uint32 map, int w, int h, int millisecond_per_frame)
{
	this->pvp_flag = 0;
	this->millisecond_per_frame = millisecond_per_frame;
	reset_units();
	this_world = cpSpaceNew();
	cpSpaceSetGravity(this_world, cpvzero);
	cpCollisionHandler* handle = cpSpaceAddDefaultCollisionHandler(this_world);
	handle->beginFunc = contact_listener::on_collision_begin;
	handle->postSolveFunc = contact_listener::on_collision_postsolve;
	handle->preSolveFunc = contact_listener::on_collision_presolve;
	handle->separateFunc = contact_listener::on_collision_sSeparate;
	switch (map)
	{
	case 0:
		cur_map = new battle_map_oneway(w, h);
		break;
	case 2:
		cur_map = new battle_map_oneway(w, h);
		break;
	case 4:
		cur_map = new battle_map_oneway(w, h);
		break;
	case 1:
		cur_map = new battle_map_twoway(w, h);
		break;
	case 3:
		cur_map = new battle_map_twoway(w, h);
		break;
	case 5:
		cur_map = new battle_map_twoway(w, h);
		break;
	default:
		cur_map = new battle_map_oneway(w, h);
		break;
	}
	current_frame = 0;
	cur_map->init_map(this_world);

	battle_energy_0->reset(10.0f, 10.0f, 1.0f / 50.0f, 90 * 50, 2.0f);
	battle_energy_1->reset(10.0f, 10.0f, 1.0f / 50.0f, 90 * 50, 2.0f);
}
void globel::set_energy_data(int camps, int max, int first, float grow_perframe, int fast_start_frame, float fast_ratio)
{
	if (camps == 0)
		battle_energy_0->reset(max, first, grow_perframe, fast_start_frame, fast_ratio);
	if (camps == 1)
		battle_energy_1->reset(max, first, grow_perframe, fast_start_frame, fast_ratio);
}
void globel::set_energy(int camps, int value)
{
	float v = value;
	if (camps == 0)
		battle_energy_0->set_energy(v);
	else if (camps == 1)
		battle_energy_1->set_energy(v);
}
int globel::get_cell(int x, int y)
{
	return this->cur_map->pathfinding.get_grid_cell(x, y);
}
int globel::frame_bgn(int frame_index, int millisecond)
{
	return 0;
}
int	globel::frame_upd(int frame_index, int millisecond)
{
	if (frame_index <= globel::globel_data->current_frame)
		return 0;
	globel::globel_data->current_frame = frame_index;
	globel::globel_data->millisecond_per_frame = millisecond;
	battle_energy_0->update(frame_index);
	battle_energy_1->update(frame_index);
	std::vector<interface_rtti*>::iterator i;
	if (globel::globel_data->cur_map->need_reset_cached_path)
	{
		globel::globel_data->cur_map->need_reset_cached_path = false;
		for (i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
		{
			battle_unit::battle_base_unit* unit = static_cast<battle_unit::battle_base_unit*>(*i);
			if ((globel::globel_data->pvp_flag != 0) && (unit->camps() != 0))
				continue;
			if (!unit->interface_flags.chk(rtti_flag_seeker))
				continue;
			unit->template get_interface_point<battle_unit::interface_seeker>()->clear_path();
		}
	}
	cpSpaceStep(this_world, ((float)millisecond) / 1000);
	
	for (i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
	{
		battle_unit::battle_base_unit* u = static_cast<battle_unit::battle_base_unit*>(*i);
		if (u->camps() == 0)
			u->round_bigin(millisecond);
		else if(globel::globel_data->pvp_flag==0)
			u->round_bigin(millisecond);
	}
	for (i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
	{
		battle_unit::battle_base_unit* u = static_cast<battle_unit::battle_base_unit*>(*i);
		if (u->camps() == 0)
			u->update(millisecond);
		else if (globel::globel_data->pvp_flag == 0)
			u->update(millisecond);
		else
			u->update_enemy_online();
	}	
	for (i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
	{
		battle_unit::battle_base_unit* u = static_cast<battle_unit::battle_base_unit*>(*i);
		if (u->camps() == 0)
			u->round_end(millisecond);
		else if (globel::globel_data->pvp_flag == 0)
			u->round_end(millisecond);
	}
	return 0;
}
int	globel::frame_end(int frame_index, int millisecond)
{
	return 0;
}
char* globel::build_unit(int id, int type, int x, int y, char* base_data)
{
	battle_unit::battle_base_unit* res = battle_unit::unit_builder::build_by_type(id, type, x, y, base_data);
	globel::globel_data->the_battle_units.push_back(static_cast<interface_rtti*>(res));
	return (char*)res;
}
void globel::deserialization(char* unit, char* out)
{
	((battle_unit::battle_base_unit*)(unit))->deserialization(out);
}
bool globel::death_object(char* handle)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (u == NULL)
		return false;
	bool res = false;
	if (u->interface_flags.chk(rtti_flag_attack_able))
		res |= u->template get_interface_point<battle_unit::interface_attack_able>()->death();
	if (u->interface_flags.chk(rtti_flag_time_bomb))
		res |= u->template get_interface_point<battle_unit::interface_big_bomb>()->bombed();
	return res;
}
void globel::demage(char* tar, int effect_flag, int value)
{
	if (tar == NULL)
		return;
	battle_unit::battle_base_unit* u = (battle_unit::battle_base_unit*)(tar);
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return;
	u->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(value);
	u->effect_flags.add(effect_flag);
}
void globel::aoe2(int camps, int attack_flags, int x, int y, int radius, int value, int effect_flag, int scale_to_zhujidi)
{
	int radius_squ = radius*radius;
	std::vector<interface_rtti*>::iterator i = the_battle_units.begin();
	for (; i != the_battle_units.end(); ++i)
	{
		battle_unit::battle_base_unit* u = static_cast<battle_unit::battle_base_unit*>(*i);
		if (u == NULL)
			continue;
		if (u->camps() == camps)
			continue;
		if (!u->interface_flags.chk(rtti_flag_attack_able))
			continue;
		if (!u->template get_interface_point<battle_unit::interface_attack_able>()->be_attack_flags.chk(attack_flags))
			continue;
		int r = u->template get_interface_point<battle_unit::interface_rigid_body>()->radius();
		int aoe_squ = radius_squ + 2 * radius*r + r*r;
		if (aoe_squ < globel_func::distance_squ(x
			, y
			, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().x
			, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().y))
			continue;
		if (u->is_building_shild())
			u->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(value*scale_to_zhujidi / 100);
		else
			u->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(value);
		//u->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(value);
		u->effect_flags.add(effect_flag);
	}
}
bool globel::aoe_check(char* handle, int attack_flags, int x, int y, int radius)
{
	int radius_squ = radius*radius;
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return false;
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return false;
	if (!u->template get_interface_point<battle_unit::interface_attack_able>()->be_attack_flags.chk(attack_flags))
		return false;
	int r = u->template get_interface_point<battle_unit::interface_rigid_body>()->radius();
	int aoe_squ = radius_squ + 2 * radius*r + r*r;
	if (aoe_squ < globel_func::distance_squ(x
		, y
		, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().x
		, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().y))
		return false;
	return true;
}
void globel::aoe(int camps, int attack_flags, int x, int y, int radius, int value, int effect_flag)
{
	int radius_squ = radius*radius;
	std::vector<interface_rtti*>::iterator i = the_battle_units.begin();
	for (; i != the_battle_units.end(); ++i)
	{
		battle_unit::battle_base_unit* u = static_cast<battle_unit::battle_base_unit*>(*i);
		if (u == NULL)
			continue;
		if (u->camps() == camps)
			continue;
		if (!u->interface_flags.chk(rtti_flag_attack_able))
			continue;
		if (!u->template get_interface_point<battle_unit::interface_attack_able>()->be_attack_flags.chk(attack_flags))
			continue;
		int r = u->template get_interface_point<battle_unit::interface_rigid_body>()->radius();
		int aoe_squ = radius_squ + 2 * radius*r + r*r;
		if (aoe_squ < globel_func::distance_squ(x
			, y
			, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().x
			, u->template get_interface_point<battle_unit::interface_rigid_body>()->location().y))
			continue;
		u->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(value);
		u->effect_flags.add(effect_flag);
	}
}
void globel::set_object_BeAttackEffect(char* handle, int effect)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return;
	u->effect_flags.add(effect);
}
void globel::set_object_HP(char* handle, int value)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return;
	u->template get_interface_point<battle_unit::interface_attack_able>()->hp(value);
}
void globel::set_object_location(char* handle, int x, int y)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!u->interface_flags.chk(rtti_flag_rigid_body))
		return;
	u->template get_interface_point<battle_unit::interface_rigid_body>()->location(x, y);
}
void globel::set_object_movment(char* handle, int speed, int face, int aiming_face)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (u->interface_flags.chk(rtti_flag_rigid_body))
		u->template get_interface_point<battle_unit::interface_rigid_body>()->face(((float)face) / 100);
	if (u->interface_flags.chk(rtti_flag_tank))
		u->template get_interface_point<battle_unit::interface_tank>()->cur_aiming_angle(((float)face) / 100);	
}
void globel::set_object_status(char* handle, int flag)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	u->status_flags.set(flag);
}
int	globel::get_object_HP(char* handle)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!u->interface_flags.chk(rtti_flag_attack_able))
		return 0;
	return u->template get_interface_point<battle_unit::interface_attack_able>()->hp();
}
int	globel::get_stat_flags(char* handle)
{
	battle_unit::battle_base_unit* u = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	return u->status_flags.get();
}
float globel::get_current_energy(int camps)
{
	if (camps == 0)
		return battle_energy_0->get_current_energy();
	else
		return battle_energy_1->get_current_energy();
}
bool globel::cast_energy(int camps, float v)
{
	if (camps == 0)
		return battle_energy_0->cast_energy(v);
	else
		return battle_energy_1->cast_energy(v);
}
void globel::add_path(int* data, int len)
{
	cur_map->add_path(data, len);
}
void globel::add_obstacle(int* data, int len)
{
	cur_map->add_obstacle(data, len);
}
void globel::set_callback(void* func)
{
	func_event_callback = (event_callback)func;
}
void globel::remove_object(char* handle)
{
	battle_unit::battle_base_unit* will_del = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	
	std::vector<interface_rtti*>::iterator i = the_battle_units.begin();
	for (; i != the_battle_units.end();)
	{
		battle_unit::battle_base_unit* this_obj = static_cast<battle_unit::battle_base_unit*>(*i);
		if (this_obj == will_del)
		{
			will_del->remove_from_world();
			i = globel::globel_data->the_battle_units.erase(i);
		}
		else
		{
			static_cast<battle_unit::battle_base_unit*>(*i)->on_any_obj_removed(will_del);
			++i;
		}
	}
	on_unit_removed(will_del->id());
	delete (will_del);
}
void globel::use_trajectory(char* handle, bool use)
{
	battle_unit::battle_base_unit* tar = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	if (!tar->interface_flags.chk(rtti_flag_attacker))
		return;
	tar->get_interface_point<battle_unit::interface_attacker>()->use_trajectory(use);
}
void globel::set_building_shild(char* handle, bool use)
{
	battle_unit::battle_base_unit* tar = reinterpret_cast<battle_unit::battle_base_unit*>(handle);
	tar->set_building_shild(use);
}
bool globel::put_down_check(int x, int y, int radius)
{
	cpVect v;
	v.x = x; v.y = y;
	cpPointQueryInfo cpqi;
	cpShapeFilter csf;
	csf.group = 0;
	csf.categories = collision_type::collision_type_obstacle;
	csf.mask = collision_type::collision_type_obstacle;
	cpSpacePointQueryNearest(this_world, v, radius, csf, &cpqi);
	return (cpqi.shape == NULL);
}
globel::globel()
{
	holder = new char[1024]();
	lua = NULL;
	battle_energy_0 = new battle_energy();
	battle_energy_1 = new battle_energy();
}