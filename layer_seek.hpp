#ifndef layer_seek_hpp
#define layer_seek_hpp

#include "layer_base.hpp"
#include "aiming_checker.hpp"

namespace battle_unit
{
	template<class super>
	class layer_seeker_target : public super
	{
	public:
		virtual void clear_path()
		{
			this->seek_path.clear();
		}
	protected:
		virtual bool find_path(vector<pathfinding_result*>& path)
		{
			if (!path.empty())
				path.clear();
			battle_map_base* the_map = this->base_map();
			if (the_map->check_segment_distance_to_obstacles(
				this->seeked_target() == NULL ? 0 : this->seeked_target()->id(),
				this->location(),
				this->destination(),
				this->radius()))
			{
				return false;
			}
			else
			{
				battle_map_base* the_map = this->base_map();
				if (this->seeked_target() != NULL)
					the_map->build_path(this->location(), this->destination(), this->seeked_target()->id(), path);
				else
					the_map->build_path(this->location(), this->destination(), pathfinding_result::pass_type_waypoint, path);
				return true;
			}
			
		}
		virtual battle_base_unit* compare_target(battle_base_unit* t1, battle_base_unit* t2)
		{
			return t1 == NULL ? t2 : t1;
		}
		virtual bool update_old_target()
		{
			if (this->seeked_target() == NULL)
				return false;
			if (this->seeked_target()->template get_interface_point<interface_attack_able>()->death())
			{
				this->seeked_target(NULL);
				return false;
			}
			return true;
		}
		virtual bool target_seek_able(battle_base_unit* tar)
		{
			if (tar->is_boss())
			{
				if (!(aiming_checker_unit<super>::chk(this, tar)
					&& aiming_checker_flag<super>::chk(this, tar)))
					return false;
			}
			else
			{
				if (!(aiming_checker_unit<super>::chk(this, tar)
					&& aiming_checker_flag<super>::chk(this, tar)
					&& seek_checker_by_distance<super>::chk(this, tar)))
					return false;
				if (globel::globel_data->cur_map->intersects_segment
				(tar->id(), this->location(), tar->template get_interface_point<interface_rigid_body>()->location()))
					return false;
			}			
			return true;
		}
		virtual bool seek_nearest_target()
		{
			int old_dissqu = 0;
			if (this->seeked_target() != NULL)
				old_dissqu = distance_squ<super>(this, this->seeked_target()->template get_interface_point<battle_unit::interface_rigid_body>());
			else
				old_dissqu = 0;
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_base_unit* test_target = static_cast<battle_base_unit*>(*i);
				if (!target_seek_able(test_target))
					continue;
				if (this->seeked_target() == NULL)
				{
					this->seeked_target(test_target);
					old_dissqu = distance_squ<super>(this, test_target->template get_interface_point<battle_unit::interface_rigid_body>());
				}
				else
				{
					int new_dissqu = distance_squ<super>(this, test_target->template get_interface_point<battle_unit::interface_rigid_body>());
					if (new_dissqu < old_dissqu)
					{
						this->seeked_target(test_target);
						old_dissqu = new_dissqu;
					}						
				}
			}
			return (this->seeked_target() != NULL);
		}
		virtual bool seek_first_target()
		{
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_base_unit* test_target = static_cast<battle_base_unit*>(*i);
				if (!target_seek_able(test_target))
					continue;
				this->seeked_target(test_target);
				return true;
			}
			return false;
		}
	public:
		layer_seeker_target()
		{
			this->use_distance(false);
		}
		virtual ~layer_seeker_target()
		{
		}
		virtual bool seek_target()
		{
			update_old_target();
			if (this->seeked_target() != NULL)
				return true;
			if (this->use_distance())
				return seek_nearest_target();
			else
				return seek_first_target();
		}
		virtual cpVect target_location()
		{
			if (this->seeked_target() != NULL)
			{
				battle_base_unit* u = static_cast<battle_base_unit*>(this->seeked_target());
				if (u == NULL)
					return cpvzero;

				battle_unit::interface_rigid_body* p = u->template get_interface_point<battle_unit::interface_rigid_body>();
				return p->location();

			}
			else
				return cpvzero;
		}
	};
	template<class super>
	class layer_seeker_waypoint : public super
	{
	protected:
		cpVect _target_location;
	public:
		layer_seeker_waypoint()
		{
			_target_location.x = _target_location.y = 0;
		}
		virtual ~layer_seeker_waypoint()
		{
		}
		virtual bool seek_waypoint() = 0;
		virtual cpVect waypoint_location()
		{
			return _target_location;
		}		
	};
	template<class super>
	class layer_seeker_waypoint_by_ground : public layer_seeker_waypoint<super>
	{
	public:
		layer_seeker_waypoint_by_ground()
		{
		}
		virtual ~layer_seeker_waypoint_by_ground()
		{
		}
		virtual bool seek_waypoint()
		{
			cpVect res = this->base_map()->get_way_point(this->location(), this->camps() == 0, this->use_soilder_waypoint());
			this->_target_location = res;
			return true;
		}
	};
	template<class super>
	class layer_seeker_waypoint_by_fly : public layer_seeker_waypoint<super>
	{
	public:
		layer_seeker_waypoint_by_fly()
		{
		}
		virtual ~layer_seeker_waypoint_by_fly()
		{
		}
		virtual bool seek_waypoint()
		{
			cpVect res = this->base_map()->get_way_point_fly(this->template get_interface_point<battle_unit::interface_rigid_body>()->location(), this->camps() == 0);
			this->_target_location = res;
			return true;
		}
	};
	template<class super>
	class layer_seeker_boss_kuangshouren : public super
	{

	};
	/*template<class super>
	class layer_seeker_zhujidi : public super
	{
	public:
		virtual cpVect waypoint_location()
		{
			return cpVect();
		}
		virtual bool seek_zhujidi()
		{
			if (!this->base_map()->check_zhujidi_line(this->template get_interface_point<battle_unit::interface_rigid_body>()->location(), this->camps() == 0))
				return false;
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_base_unit* test_target = static_cast<battle_base_unit*>(*i);
				if (test_target->ally(*this))
					continue;
				if (test_target->type() != object_type::battle_object_type_zhujidi)
					continue;
				this->_target_location = test_target->template get_interface_point<battle_unit::interface_rigid_body>()->location();
				return true;
			}
			return false;
		}
	};*/
}
#endif
