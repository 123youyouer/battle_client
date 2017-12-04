#ifndef unit_wuzhuangzhishengji_hpp
#include "layer_base.hpp"
namespace battle_unit
{
	class unit_wuzhuangzhishengji : public unit_aircraft
	{
	protected:
		virtual bool use_pathfinding()
		{
			return false;
		}
		virtual battle_base_unit* select_by_direction(battle_base_unit* t1, battle_base_unit*t2)
		{
			if (t1 == NULL)
				return t2;
			if (t2 == NULL)
				return t1;
			if (t1 == last_target)
				return t1;
			else if (t2 == last_target)
				return t2;
			
			float d1 = globel_func::direction_to_east(t1->template get_interface_point<interface_rigid_body>()->location()
				- this->location());
			float d2 = globel_func::direction_to_east(t2->template get_interface_point<interface_rigid_body>()->location()
				- this->location());
			if (fabs(d1 - this->face()) < fabs(d2 - this->face()))
				return t1;
			else
				return t2;
		}
		virtual battle_base_unit* select_target(battle_base_unit* t1, battle_base_unit*t2)
		{
			return select_by_direction(t1, t2);
		}
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_aircraft;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_aircraft;
		}
	public:
		virtual ~unit_wuzhuangzhishengji() {}
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
				apply_face(f, 2.0f);
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
}

#endif // !unit_wuzhuangzhishengji_hpp
