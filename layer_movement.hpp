#ifndef layer_movement_hpp
#define layer_movement_hpp

#include "layer_interface.hpp"
#include "layer_pathfinding.h"
#include "globel.h"

namespace battle_unit
{
	template<class super>
	class layer_movement : public layer_navmesh<super>
	{
	private:
		int delay;
		cpVect cur_force;
		cpVect force_target_pull;
		cpVect force_unit_collision;
	protected:
		virtual bool use_pathfinding()
		{
			return true;
		}
		cpVect compute_unit_collision(std::vector<battle_unit::interface_rigid_body*>& body_list)
		{
			cpVect res = cpvzero;
			std::vector<battle_unit::interface_rigid_body*>::iterator i = body_list.begin();
			for (; i != body_list.end(); ++i)
			{
				(*i)->attach_contact_force(this, res, this->camps() == 0);
			}
			return res;
		}
		int compute_current_face(cpVect& vaule)
		{
			if (this->is_static())
				return 0;
			if (this->is_prepare_shot())
			{
				vaule = ((battle_base_unit*)this->aiming_target())->template get_interface_point<battle_unit::interface_rigid_body>()->location() - this->location();
				return 1;
			}

			else
				vaule = this->compute_target_pull();
			return 3;
		}
		int apply_face(cpVect f, float step)
		{
			float f1 = this->face();
			float f2 = globel_func::direction_to_east(f);
			//float f2 = globel_func::direction_to_east(0, 0, f.x, f.y);
			//float step = 2.0f;
			float min = globel_func::compute_min_angle(f1, f2);
			float offset = globel_func::compute_min_angle(f1 + 1, f2);
			offset = offset > min ? -step : step;
			if (min < step * 2)
			{
				this->face(f2);
				return 2;
			}
			else if (min > 135)
			{
				this->face(f1 - offset);
				return 1;
			}
			else
			{
				this->face(f1 + offset);
				return 1;
			}
		}
		void apply_speed(float rate)
		{
			if (cpvdistsq(this->location(), this->this_frame_destination) <
				(this->max_speed()*globel::globel_data->millisecond_per_frame / 1000)*
				(this->max_speed()*globel::globel_data->millisecond_per_frame / 1000))
			{
				this->location(this->this_frame_destination.x, this->this_frame_destination.y);
				cpBodySetVelocity(this->_body, cpvzero);
				return;
			}
			cpBody* b = this->_body;
			float f1 = this->face();
			cpVect v = cpv(cosf(f1 / 180.f*PI), sinf(f1 / 180.f*PI));
			v = cpvmult(v, this->max_speed()*rate);
			cpBodySetVelocity(b, v);
		}
		bool fix_out_range()
		{
			cpBody* body = this->_body;

			cpVect loc = body->p;
			cpVect old = loc;
			if (loc.x <= 0)
				loc.x = 1;
			else if (loc.x >= globel::globel_data->cur_map->w())
				loc.x = globel::globel_data->cur_map->w() - 1;
			if (loc.y <= 0)
				loc.y = 1;
			else if (loc.y >= globel::globel_data->cur_map->h())
				loc.y = globel::globel_data->cur_map->h() - 1;
			if (loc == body->p)
				return true;

			cpBodySetVelocity(body, cpvzero);
			cpBodySetPosition(body, loc);

			loc = loc - old;
			loc = cpvmult(cpvnormalize(loc), this->max_speed());
			cpBodySetVelocity(body, loc);
			return false;
		}
	public:
		virtual ~layer_movement()
		{
		}
		virtual void movement(uint32 millisecond)
		{
			cpBody* b = this->_body;
			cpBodySetVelocity(b, cpvzero);
			if (!this->movement_able())
				return;
			cpVect f = cpvzero;
			if (!fix_out_range())
				return;
			switch (compute_current_face(f))
			{
			case 0:
				cpBodySetVelocity(b, cpvzero);
				return;
			case 1:
				cpBodySetVelocity(b, cpvzero);
				//apply_face(f, 1.0f);
				return;
			case 2:
			case 3:
				cpBodySetVelocity(b, cpvzero);
				break;
			default:
				return;
			}
			switch (apply_face(f, 2.0f))
			{
			case 1:
				apply_speed(0.5f);
				break;
			case 2:
				apply_speed(1.0f);
				break;
			}
		}
	};

	template<class super>
	class layer_movement_soilder : public super
	{
	private:
		int delay;
		cpVect cur_force;
		cpVect force_target_pull;
		cpVect force_unit_collision;
	protected:
		virtual bool use_pathfinding()
		{
			return false;
		}
	protected:
		cpVect compute_target_pull()
		{
			cpVect res = (this->destination() - this->location());
			res = cpvnormalize(res);
			res = cpvmult(res, this->max_speed());
			return res;
		}
		cpVect compute_bird_separation()
		{
			cpVect separation = cpvzero;
			int count = 0;
			cpBody* b1 = this->_body;
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_unit::battle_base_unit* tar = static_cast<battle_unit::battle_base_unit*>(*i);
				if (tar == this)
					continue;
				cpShape* shape = tar->template get_interface_point<battle_unit::interface_rigid_body>()->shape();
				if ((shape->filter.mask&collision_type::collision_type_tank) != 0)
				{
					float dis = this->distance_squared(tar->template get_interface_point<battle_unit::interface_rigid_body>());
					float min = tar->template get_interface_point<battle_unit::interface_rigid_body>()->radius() + this->radius();
					min *= min;
					if (dis > min)
						continue;
					dis = sqrtf(dis);
					min = sqrtf(min);
					separation = separation + cpvmult((this->location() - tar->template get_interface_point<battle_unit::interface_rigid_body>()->location()), (min - dis));
					count++;
				}

			}
			if (count > 0)
			{
				separation = cpvnormalize(separation);
				separation = cpvmult(separation, this->max_speed() / 2);
			}
			return separation;
		}
		bool fix_out_range()
		{
			cpVect loc = this->location();
			cpVect old = loc;
			battle_map_base* map = this->base_map();
			if (loc.x <= 1)
				loc.x = 2;
			if (loc.x >= map->w())
				loc.x = map->w() - 1;
			if (loc.y <= 1)
				loc.y = 2;
			if (loc.y >= map->h())
				loc.y = map->h() - 1;
			cpBody* b = this->_body;
			if (loc == old)
				return false;
			cpBodySetPosition(b, loc);
			return true;
		}
	public:
		layer_movement_soilder()
		{
			delay = 0;
			cur_force = cpvzero;
			force_target_pull = cpvzero;
			force_unit_collision = cpvzero;
		}
		virtual ~layer_movement_soilder()
		{
		}
		void apply_face(float f)
		{
			float f0 = this->face();
			if (f0 != f)
			{
				if (delay <= 0)
				{
					this->face(f);
					delay = 5;
				}
				else
				{
					delay--;
				}

			}
		}
		virtual void movement(uint32 millisecond)
		{
			if (!this->movement_able())
				return;
			fix_out_range();
			cpBody* b = this->_body;
			cpVect f1 = cpvzero;
			cpBodySetVelocity(b, cpvzero);
			if (!this->is_prepare_shot())
			{
				f1 = this->compute_target_pull();
				apply_face(globel_func::direction_to_east(f1));
			}
			else if (this->interface_flags.chk(rtti_flag_attacker))
			{
				battle_base_unit* tar = (this->template get_interface_point<battle_unit::interface_attacker>()->aiming_target());
				if (tar == NULL)
					return;
				this->face(globel_func::direction_to_east
				(tar->template get_interface_point<battle_unit::interface_rigid_body>()->location() - this->location()));
				f1 = this->compute_bird_separation();

			}
			else
			{
				cpVect f2 = this->compute_bird_separation();
				f1 = f2;
			}
			f1 = cpvnormalize(f1);
			f1 = cpvmult(f1, this->max_speed());
			if (cpvdistsq(this->location(), this->destination()) <
				(this->max_speed()*globel::globel_data->millisecond_per_frame / 1000)*
				(this->max_speed()*globel::globel_data->millisecond_per_frame / 1000))
			{
				this->location(this->destination().x, this->destination().y);
				cpBodySetVelocity(this->_body, cpvzero);
				return;
			}
			cpBodySetVelocity(b, f1);
		}
	};
}
#endif // !layer_movement_hpp#pragma once
