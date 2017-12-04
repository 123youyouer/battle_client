#ifndef unit_all_hpp
#define unit_all_hpp

#include "layer_base.hpp"


namespace battle_unit
{
	class unit_cibao_tank : public unit_ground_tank
	{
	public:
		virtual ~unit_cibao_tank() {};
	};
	class unit_bingying : public unit_factory
	{
	protected:
		int cd;
		int width;
		int height;
		obstacle* ob = NULL;
	private:

	public:
		virtual bool is_box()
		{
			return true;
		}
		virtual void attach_contact_force(interface_rigid_body* o, cpVect& force, bool north)
		{
			cpVect c1 = o->location();
			cpVect c0 = this->location();
			if (north)
			{
				if (c1.y > (c0.y + height / 2))
					return;
				else if (c1.y > (c0.y - height / 2))
					force.y += max_speed() * 10;
				else if(c1.x<=c0.x)
					force.x -= max_speed() * 10;
				else
					force.x += max_speed() * 10;
			}
			else
			{
				if (c1.y < (c0.y - height / 2))
					return;
				else if (c1.y < (c0.y + height / 2))
					force.y -= max_speed() * 10;
				else if (c1.x <= c0.x)
					force.x -= max_speed() * 10;
				else
					force.x += max_speed() * 10;
			}
		}
		virtual void join_world(int x, int y)
		{
			this->_body = cpBodyNewStatic();
			cpBodySetPosition(this->_body, cpv(x, y));
			cpBodySetUserData(this->_body, static_cast<interface_rtti*>(this));
			this->_shape = cpBoxShapeNew(_body, 60, 60, 0);
			cpShapeSetElasticity(this->_shape, 0);
			cpShapeSetSensor(this->_shape, this->is_sensor());
			this->_shape->filter.group = 0;
			this->_shape->filter.categories = this->collision_type();
			this->_shape->filter.mask = this->collision_mask();
			cpSpaceAddBody(this_world(), this->_body);
			this->_shape = cpSpaceAddShape(this_world(), this->_shape);
			ob = globel::globel_data->cur_map->create_obstacle(
				x, 
				y, 
				70, 
				70,
				this->id(),
				collision_type::collision_type_terrain | collision_type::collision_type_building,
				this_world());
			
		}
		virtual void remove_from_world()
		{
			unit_factory::remove_from_world();
			globel::globel_data->cur_map->delete_obstacle(ob);
			delete ob;
		}
		virtual bool movement_able()
		{
			return false;
		}
		virtual void serialization(char* data)
		{
			unit_factory::serialization(data);
			input_obj_data_bingying* d = reinterpret_cast<input_obj_data_bingying*>(data);
			cd = d->cd;
			width = d->width;
			height = d->height;
		}
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_building | collision_type::collision_type_terrain;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_building | collision_type::collision_type_terrain;
		}
		virtual void movement(uint32 millisecond)
		{
			return;
		}
		virtual ~unit_bingying() {}
	};
	class unit_dianta : public unit_tower
	{
	public:
		virtual ~unit_dianta() {}
	};
	class unit_boss_dianta : public unit_tower
	{
	public:
		virtual void join_world(int x, int y)
		{
			this->radius(60);
			_tower_2::join_world(x, y);
			ob = globel::globel_data->cur_map->create_obstacle(
				x,
				y,
				120,
				120,
				this->id(),
				collision_type::collision_type_building,
				globel::globel_data->this_world
			);

		}
	};
	class unit_dog : public unit_ground_soilder
	{
	protected:
		virtual void update_target_hp(interface_rtti* tar)
		{
			battle_base_unit* u = (battle_base_unit*)tar;
			switch (u->type())
			{
			case object_type::battle_object_type_zhongzhuangdabing:
			case object_type::battle_object_type_fushegongbing:
			case object_type::battle_object_type_meiguodabing:
			case object_type::battle_object_type_dog:
			case object_type::battle_object_type_tanya:
			case object_type::battle_object_type_feixingbing:
				u->template get_interface_point<battle_unit::interface_attack_able>()->kill_now();
				break;
			case object_type::battle_object_type_zhujidi:
				unit_ground_soilder::update_target_hp(tar);
				break;
			default:
				unit_ground_soilder::update_target_hp(tar);
				return;
			}
		}
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<unit_dog>::chk(this, tar)
				&& aiming_checker_flag<unit_dog>::chk(this, tar)
				&& aiming_checker_attackable_by_warrior<unit_dog>::chk(this, tar)
				&& aiming_checker_attackable_only_soilder<unit_dog>::chk(this, tar);
		}
		virtual battle_base_unit* select_target(battle_base_unit* t1, battle_base_unit*t2)
		{
			return aiming_selector_by_threat::select(t1, t2);
		}
		virtual bool target_seek_able(battle_base_unit* tar)
		{
			if (!unit_ground_soilder::target_seek_able(tar))
				return false;
			return aiming_checker_attackable_only_soilder<unit_dog>::chk(this, tar);
		}
	public:
		unit_dog()
		{
			this->use_distance(true);
		}
		virtual ~unit_dog() {}
	};
	class unit_tanya : public unit_ground_soilder
	{
	private:
		bool is_priority(battle_base_unit* t)
		{
			switch (t->type())
			{
			case object_type::battle_object_type_zhongzhuangdabing:
			case object_type::battle_object_type_fushegongbing:
			case object_type::battle_object_type_meiguodabing:
			case object_type::battle_object_type_dog:
			case object_type::battle_object_type_tanya:
			case object_type::battle_object_type_feixingbing:
			case object_type::battle_object_type_kuangshouren:
				return true;
			}
			return false;
		}
	protected:
		virtual battle_base_unit* select_target(battle_base_unit* t1, battle_base_unit*t2)
		{
			if (t1 == NULL)
				return t2;
			if (t2 == NULL)
				return t1;
			if (!t1->interface_flags.chk(rtti_flag_attack_able)
				|| !t2->interface_flags.chk(rtti_flag_attack_able))
				return NULL;
			if (is_priority(t1) == is_priority(t2))
				return aiming_selector_by_threat::select(t1, t2);
			else
				return is_priority(t1) ? t1 : t2;
		}
	public:
		virtual ~unit_tanya() {}
	};
	class unit_feixingbing : public unit_fly_soilder
	{
	protected:
		virtual bool use_pathfinding()
		{
			return false;
		}
	public:
		virtual ~unit_feixingbing() {}
	};
	class unit_fushegongbing : public unit_ground_soilder
	{
	public:
		virtual ~unit_fushegongbing() {}
	};
	class unit_gaite_tank : public unit_ground_tank
	{
	public:
		virtual ~unit_gaite_tank() {}
	};
	class unit_gongchengshi : public unit_ground_soilder
	{
	protected:
		int duringTime;
	protected:
		virtual void serialization(char* data)
		{
			unit_ground_soilder::serialization(data);
			input_obj_data_gongchengshi* d = reinterpret_cast<input_obj_data_gongchengshi*>(data);
			duringTime = d->duringTime;
		}
		virtual bool aiming_able(battle_base_unit* tar)
		{			
			if (!aiming_checker_unit<unit_gongchengshi>::chk(this, tar))
				return false;
			if (!aiming_checker_flag<unit_gongchengshi>::chk(this, tar))
				return false;
			if (!tar->interface_flags.chk(rtti_flag_occupied))
				return false;
			if (!aiming_checker_attackable_by_warrior<unit_gongchengshi>::chk(this, tar))
				return false;
			return true;
		}
		virtual bool target_seek_able(battle_base_unit* tar)
		{			
			if (!unit_ground_soilder::target_seek_able(tar))
				return false;
			if (!tar->interface_flags.chk(rtti_flag_occupied))
				return false;
			if (tar->template get_interface_point<interface_occupied>()->check_occupied())
				return false;
			return true;
		}
	public:
		virtual void shot(uint32 millisecond)
		{
			if (this->aiming_target() == NULL)
				return;
			this->aiming_target()->template get_interface_point<battle_unit::interface_occupied>()->occupy(duringTime);
			this->status_flags.add(STATUS_FLAGS::battle_obj_flag_gongchengshi);
			this->hp(0);
		}
		unit_gongchengshi()
		{
			this->use_distance(true);
		}
	public:
		virtual ~unit_gongchengshi() {}
	};
	class unit_guanglengta : public unit_tower
	{
	public:
		virtual ~unit_guanglengta() {}
	};
	class unit_guangleng_tank : public unit_ground_tank
	{
	public:
		virtual ~unit_guangleng_tank() {}
	};
	class unit_huixiong_tank : public unit_ground_tank
	{
	public:
		virtual ~unit_huixiong_tank() {}
	};
	class unit_xiniu_tank : public unit_ground_tank
	{
	private:
		const static int max_reset_count = 180;
		const static int max_sprint_count = 36;
		int rest_frame_count = max_reset_count;
		int sprint_frame_count = max_sprint_count;
		cpVect old_v;
		bool stat = 1;
	private:
		bool anybody_near()
		{
			for (std::vector<interface_rtti*>::iterator i = globel::globel_data->the_battle_units.begin(); i != globel::globel_data->the_battle_units.end(); ++i)
			{
				battle_base_unit* u = static_cast<battle_base_unit*>(*i);
				if (u->id() == this->id())
					continue;
				if (u->camps() == this->camps())
					continue;
				cpVect dis = (u->get_interface_point<interface_rigid_body>()->location() - this->location());
				if (cpvlengthsq(dis) <= 4900)
					return true;
			}
			return false;
		}
		cpVect compute_max_speed(const cpVect& v)
		{
			return cpv(v.x * 3 * max_reset_count / max_sprint_count, v.y * 3 * max_reset_count / max_sprint_count);
		}
		void on_rest(const cpVect& v)
		{
			cpVect max = compute_max_speed(v);
			float f_max = (rest_frame_count);
			f_max /= max_reset_count;
			max = cpvmult(max, f_max);
			cpBodySetVelocity(this->body(), max);

			cpBodySetVelocity(this->body(), cpvzero);
			rest_frame_count--;
			if (rest_frame_count == 0)
			{
				sprint_frame_count = max_sprint_count;
				stat = 1;
			}				
		}
		void on_sprint(const cpVect& v)
		{
			cpVect max = compute_max_speed(v);
			float f_max = max_sprint_count-sprint_frame_count;
			f_max /= max_sprint_count;
			max=cpvmult(max, f_max);
			cpBodySetVelocity(this->body(), max);
			sprint_frame_count--;
			if (sprint_frame_count == 0)
			{
				rest_frame_count = max_reset_count;
				stat = 0;
			}
		}
	public:
		virtual cpFloat get_body_mass()
		{
			return 120;
		}
		virtual void movement(uint32 millisecond)
		{
			unit_ground_tank::movement(millisecond);
			/*cpVect v = cpBodyGetVelocity(this->body());
			v = cpvnormalize(v);
			v = cpvmult(v, this->max_speed());
			if (v.x == 0 && v.y == 0)
				return;
			if (!anybody_near())
				return;
			if (stat == 0)
				on_rest(v);
			else
				on_sprint(v);*/
			
		}
	public:
		virtual ~unit_xiniu_tank() {}
	};
	class unit_zhandouyaosai : public unit_ground_tank
	{
	public:
		virtual cpFloat get_body_mass()
		{
			return 120;
		}
	public:
		virtual ~unit_zhandouyaosai() {}
	};
	class unit_jinuofu : public unit_aircraft
	{
	protected:
		virtual bool use_pathfinding()
		{
			return false;
		}
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_sky2;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_sky2;
		}
	public:
		unit_jinuofu()
		{
			this->use_distance(true);
		}
		virtual ~unit_jinuofu() {}
	};

	typedef unit<interface_big_bomb, interface_attack_able, interface_seeker>							_kongbufenzi_0;
	typedef layer_seeker_target<_kongbufenzi_0>															_kongbufenzi_1;
	typedef layer_seeker_waypoint_by_ground<_kongbufenzi_1>												_kongbufenzi_2;
	typedef	layer_impact_bomb<_kongbufenzi_2>															_kongbufenzi_3;
	typedef layer_movement_soilder<_kongbufenzi_3>														_kongbufenzi_4;

	class unit_kongbufenzi : public _kongbufenzi_4
	{
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_ground_soilder | collision_type::collision_type_terrain;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_ground_soilder | collision_type::collision_type_terrain;
		}
		virtual void serialization(char* data)
		{
			int et = 0;
			unit_serialization<battle_base_unit>::serialization(this, data);
			unit_serialization<battle_unit::interface_attack_able>::serialization(this->template get_interface_point<battle_unit::interface_attack_able>(), data);
			unit_serialization<battle_unit::interface_rigid_body>::serialization(this->template get_interface_point<battle_unit::interface_rigid_body>(), data);
			unit_serialization<battle_unit::interface_seeker>::serialization(this->template get_interface_point<battle_unit::interface_seeker>(), data);
			this->attack_distance(reinterpret_cast<input_obj_data_unit*>(data)->_aoe_area);
			this->attack_power(reinterpret_cast<input_obj_data_unit*>(data)->_attack_power);
			this->attack_flags.set(reinterpret_cast<input_obj_data_unit*>(data)->_attack_flags);
			this->exploded_timer(et);
		}
		virtual void deserialization(char* data)
		{
			_kongbufenzi_4::deserialization(data);
		}
	protected:
		virtual void on_bombing()
		{
			this->hp(0);
		}
	public:
		unit_kongbufenzi()
		{
			this->use_distance(true);
		}
		virtual ~unit_kongbufenzi() {}
	};

	class unit_kuangshouren : public unit_ground_soilder
	{
	protected:
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<unit_kuangshouren>::chk(this, tar)
				&& aiming_checker_flag<unit_kuangshouren>::chk(this, tar)
				&& aiming_checker_attackable_by_warrior<unit_kuangshouren>::chk(this, tar);
		}
	public:
		virtual cpFloat get_body_mass()
		{
			return 20;
		}
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_ground_soilder | collision_type::collision_type_tank | collision_type::collision_type_terrain;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_ground_soilder | collision_type::collision_type_tank | collision_type::collision_type_terrain;
		}
		unit_kuangshouren()
		{
			this->use_distance(true);
		}
		virtual ~unit_kuangshouren() {}
	};
	class unit_meiguodabing : public unit_ground_soilder
	{
	public:
		virtual ~unit_meiguodabing() {}
	};
	
	class unit_tianqi : public unit_ground_tank
	{
	public:
		virtual cpFloat get_body_mass()
		{
			return 40;
		}
	public:
		virtual ~unit_tianqi() {}
	};
	class unit_v3 : public unit_ground_tank
	{
	public:
		virtual ~unit_v3() {}
	};
	
	class unit_zhongzhuangdabing : public unit_ground_soilder
	{
	public:
		virtual ~unit_zhongzhuangdabing() {}
	};
	class unit_zhujidi : public unit_tower
	{
		obstacle* ob;
	public:
		virtual bool is_sensor()
		{
			return true;
		}
		virtual void join_world(int x, int y)
		{
			_tower_2::join_world(x, y);
			//ob = globel::globel_data->cur_map->create_obstacle(x, y, 100, 80, globel::globel_data->this_world);
			//globel::globel_data->cur_map->_nav_context.reset();
			//globel::globel_data->cur_map->_nav_context.add_obstacle(x, y, 100, 80);
			//globel::globel_data->cur_map->_nav_context.build_triangle();

		}
		virtual void remove_from_world()
		{
			_tower_2::remove_from_world();
			if (ob != NULL)
			{
				globel::globel_data->cur_map->delete_obstacle(ob);
				delete ob;
			}				
		}
	public:
		unit_zhujidi() :ob(NULL) {}
		virtual ~unit_zhujidi() {}
	};
	class unit_aiguozhe : public unit_tower
	{
	public:
		virtual ~unit_aiguozhe() {}
	};
	class unit_aiguozhe_two : public unit_tower
	{
	public:
		virtual ~unit_aiguozhe_two() {}
	};
	class unit_fujidi : public unit_tower
	{
	public:
		virtual ~unit_fujidi() {}
	};
	class unit_jupao : public unit_tower
	{
	public:
		virtual ~unit_jupao() {}
	};
	class unit_zibaokache : public unit_ground_tank
	{
	public:
		virtual ~unit_zibaokache() {}
	};
	typedef unit<interface_big_bomb>																			_bomb_0;
	typedef layer_time_bomb<_bomb_0>																	_bomb_1;
	class unit_time_bomb : public _bomb_1
	{
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
		}
		virtual bool movement_able()
		{
			return false;
		}
		virtual void movement(uint32 millisecond)
		{
		}
	public:
		virtual ~unit_time_bomb() {}
	};
	class unit_boss_kuangshouren : public unit_ground_soilder
	{
	protected:
		virtual bool aiming_able(battle_base_unit* tar)
		{
			return aiming_checker_unit<unit_boss_kuangshouren>::chk(this, tar)
				&& aiming_checker_flag<unit_boss_kuangshouren>::chk(this, tar)
				&& aiming_checker_attackable_by_warrior<unit_boss_kuangshouren>::chk(this, tar);
		}
		virtual bool seek_target()
		{
			return false;
		}
		virtual cpVect waypoint_location()
		{
			cpVect res = this->location();
			if (this->camps() == 0)
				res.y += 10;
			else
				res.y -= 10;
			return res;
		}
		virtual bool seek_waypoint()
		{
			return true;
		}
	public:
		virtual cpFloat get_body_mass()
		{
			return 4000;
		}
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_all;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_all;
		}
		virtual bool is_boss() 
		{ 
			return true; 
		}
		unit_boss_kuangshouren()
		{
			this->use_distance(false);
		}
		virtual ~unit_boss_kuangshouren() {}
	};
	
}
#endif // !cibao_tank_hpp#pragma once
