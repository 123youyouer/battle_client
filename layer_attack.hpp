#ifndef layer_attack_hpp
#define layer_attack_hpp

#include "globel.h"
#include "layer_base.hpp"
#include "aiming_checker.hpp"
namespace battle_unit
{
	template<class T>
	class aiming_checker_by_sputtering_distance
	{
	public:
		static bool chk(T* src, battle_base_unit* ctr, battle_base_unit* tar)
		{
			if (src->sputtering_area_squ() <= 0
				|| !ctr->interface_flags.chk(rtti_flag_rigid_body)
				|| !tar->interface_flags.chk(rtti_flag_rigid_body))
			{
				return false;
			}
			int tar_r = tar->template get_interface_point<interface_rigid_body>()->radius();
			int d1 = ctr->template get_interface_point<interface_rigid_body>()->distance_squared(tar->template get_interface_point<interface_rigid_body>());
			int d2 = src->sputtering_area_squ() + src->sputtering_area()*tar_r + tar_r*tar_r;
			return d1 <= d2;
			//return ctr->template get_interface_point<interface_rigid_body>()->distance_squared(tar->template get_interface_point<interface_rigid_body>()) <= src->sputtering_area_squ();
		}
	};
	template<class super>
	class layer_attack : public super
	{
	protected:
		uint32 next_shot_timer;
		
		virtual void update_target_hp(interface_rtti* tar)
		{
			if (tar == NULL)
				return;
			tar->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(this->attack_power());
		}
		bool sputtering_able(battle_base_unit* ctr, battle_base_unit* target)
		{
			return ctr != target
				&& aiming_checker_unit<super>::chk(this, target)
				&& aiming_checker_flag<super>::chk(this, target)
				&& aiming_checker_by_sputtering_distance<super>::chk(this, ctr, target);
		}
		void update_aoe(battle_base_unit* center_tar)
		{
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				if (sputtering_able(center_tar, static_cast<battle_base_unit*>(*i)))
				{
					update_target_hp((*i));
					static_cast<battle_base_unit*>(*i)->effect_flags.add(this->attack_visual_flag());
				}					
			}
		}
	public:
		layer_attack()
		{ 
			next_shot_timer = 0; 
		}
		virtual ~layer_attack() {}
		virtual void shot(uint32 millisecond)
		{
			if (next_shot_timer <= millisecond)
			{
				battle_base_unit* tar = this->aiming_target();
				if (tar == NULL)
					return;
				next_shot_timer = this->attack_speed();
				this->status_flags.add(STATUS_FLAGS::battle_obj_flag_shoting);
				if (!this->use_trajectory())
				{
					update_target_hp(tar);
					tar->effect_flags.add(this->attack_visual_flag());
					update_aoe(tar);
				}				
			}
			else
				next_shot_timer -= millisecond;			
		}

	};
	template<class super>
	class layer_attack_delay : public layer_attack<super>
	{
		virtual void try_shot(uint32 millisecond)
		{
			if (this->next_shot_timer <= millisecond)
			{
				battle_base_unit* tar = this->aiming_target();
				if (tar == NULL)
					return;
				this->next_shot_timer = this->attack_speed();
				this->visual_state_flags.add(STATUS_FLAGS::battle_obj_flag_shoting);
			}
			else
				this->next_shot_timer -= millisecond;
		}
	};
	template<class super>
	class layer_bomb : public super
	{
	protected:
		void bomb(battle_base_unit* tar)
		{
			//tar->visual_state_flags.add(this->attack_visual_flag());
			tar->template get_interface_point<battle_unit::interface_attack_able>()->on_demage(this->attack_power());
		}
		virtual bool demage_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<super>::chk(this, tar)
				&& aiming_checker_flag<super>::chk(this, tar)
				&& aiming_checker_by_distance<super>::chk(this, tar);
		}
		void bomb()
		{
			if (this->_bombed)
				return;
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				if (demage_able(static_cast<battle_base_unit*>(*i)))
					bomb(static_cast<battle_base_unit*>(*i));
			}
			this->status_flags.add(STATUS_FLAGS::battle_obj_flag_shoting);
			this->_bombed = true;
			on_bombing();
		}
		virtual void on_bombing() {};
		virtual bool triggered(const uint32& millisecond) = 0;
	public:
		layer_bomb() {}
		virtual void bomb(uint32 millisecond)
		{
			if (triggered(millisecond))
				bomb();
		}
	};
	template<class super>
	class layer_time_bomb : public layer_bomb<super>
	{
	private:
		uint32 _wait_timer;
	protected:
		virtual bool triggered(const uint32& millisecond)
		{
			_wait_timer += millisecond;
			return (this->exploded_timer() <= _wait_timer);
		}
	public:
		layer_time_bomb() : _wait_timer(0)
		{
		}
		virtual ~layer_time_bomb()
		{
		}
	};
	template<class super>
	class layer_impact_bomb : public layer_bomb<super>
	{
	protected:
		virtual bool triggered(const uint32& millisecond)
		{
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_base_unit* u = static_cast<battle_base_unit*>(*i);
				if (aiming_checker_unit<super>::chk(this, u)
					&& aiming_checker_flag<super>::chk(this, u)
					&& aiming_checker_attackable_by_warrior<super>::chk(this, u))
				{
					return true;
				}
			}
			return false;
		}
	public:
		layer_impact_bomb()
		{
		}
		virtual ~layer_impact_bomb()
		{
		}
	};

}

#endif // !layer_attack_hpp#pragma once
