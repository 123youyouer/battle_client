#ifndef layer_aiming_hpp
#define layer_aiming_hpp

#include "layer_base.hpp"
#include "aiming_checker.hpp"


namespace battle_unit
{
	template<class super>
	class layer_aiming_base : public super
	{
	protected:
		battle_base_unit* last_target = NULL;
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<super>::chk(this, tar)
				&& aiming_checker_flag<super>::chk(this, tar)
				&& aiming_checker_by_distance<super>::chk(this, tar);
		}
		virtual battle_base_unit* select_target(battle_base_unit* t1, battle_base_unit*t2)
		{
			return aiming_selector_by_threat_and_distance::select(this, last_target, t1, t2);
		}
	public:
		layer_aiming_base() {  }
		virtual ~layer_aiming_base() { }		
		virtual void aiming(battle_base_unit* tar)
		{
			if (!aiming_able(tar))
				return;
			battle_base_unit* one = select_target(this->aiming_target(), tar);
			if (one != tar)
				return;
			this->status_flags.add(STATUS_FLAGS::battle_obj_flag_aiming);
			this->aiming_target(tar);
		}
		virtual void aiming(uint32 millisecond)
		{
			this->status_flags.del(STATUS_FLAGS::battle_obj_flag_aiming);
			last_target = this->aiming_target();
			this->aiming_target(NULL);
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				aiming(static_cast<battle_base_unit*>(*i));
			}
		}
	};
	template<class super>
	class layer_aiming_threat : public layer_aiming_base<super>
	{
	protected:
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<super>::chk(this, tar)
				&& aiming_checker_flag<super>::chk(this, tar)
				&& aiming_checker_by_distance<super>::chk(this, tar);
		}
	public:
		layer_aiming_threat() {  }
		virtual ~layer_aiming_threat() { }
	};
	template<class super>
	class layer_aiming_tank : public layer_aiming_threat<super>
	{
	protected:
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return layer_aiming_threat<super>::aiming_able(tar)
				&& aiming_checker_by_face<super>::chk(this, tar);
		}
	public:
		layer_aiming_tank() {}
		virtual ~layer_aiming_tank() {}
		virtual void aiming(uint32 millisecond)
		{
			layer_aiming_threat<super>::aiming(millisecond);
			if (this->has_aiming_target())
			{
				this->status_flags.add(STATUS_FLAGS::battle_obj_flag_aiming);
				cpVect v = ((battle_base_unit*)this->aiming_target())->template get_interface_point<battle_unit::interface_rigid_body>()->location() - this->location();
				this->cur_aiming_angle(globel_func::direction_to_east(v));
			}
				
			else
			{
				this->status_flags.del(STATUS_FLAGS::battle_obj_flag_aiming);
				this->cur_aiming_angle(this->face());
			}
				
		}
	};
	template<class super>
	class layer_aiming_soilder : public layer_aiming_threat<super>
	{
	public:
		virtual void aiming(uint32 millisecond)
		{
			layer_aiming_threat<super>::aiming(millisecond);
			if (this->has_aiming_target())
			{
				cpVect v = ((battle_base_unit*)this->aiming_target())->template get_interface_point<battle_unit::interface_rigid_body>()->location() - this->location();
				this->face(globel_func::direction_to_east(v));
				/*this->face(globel_func::direction_to_east(
					this->location().x
					, this->location().y
					, ((battle_base_unit*)this->aiming_target())->template get_interface_point<battle_unit::interface_rigid_body>()->location().x
					, ((battle_base_unit*)this->aiming_target())->template get_interface_point<battle_unit::interface_rigid_body>()->location().y));*/
			}
		}
	};
}
#endif
