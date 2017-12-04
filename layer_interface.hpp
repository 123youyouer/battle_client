#ifndef layer_interface_hpp
#define layer_interface_hpp

#include <chipmunk\chipmunk_private.h>
#include "battle_map.hpp"
#include "flags.h"
#include "rtti.h"
#include "vector2.h"
namespace battle_unit
{
	class STATUS_FLAGS
	{
	public:
		const static uint32 none;
		const static uint32 battle_obj_flag_aiming;
		const static uint32 battle_obj_flag_shoting;
		const static uint32 battle_obj_flag_occupied;
	/*	const static uint32 battle_obj_flag_attacked_by_small_tank;
		const static uint32 battle_obj_flag_attacked_by_cibao_tank;
		const static uint32 battle_obj_flag_attacked_by_big_tank;
		const static uint32 battle_obj_flag_attacked_by_big_gun;
		const static uint32 battle_obj_flag_attacked_by_small_gun;*/
		const static uint32 battle_obj_flag_gongchengshi;
	};
	const uint32 STATUS_FLAGS::none = 0;
	const uint32 STATUS_FLAGS::battle_obj_flag_aiming = 1 << 1;
	const uint32 STATUS_FLAGS::battle_obj_flag_shoting = 1 << 2;
	const uint32 STATUS_FLAGS::battle_obj_flag_occupied = 1 << 3;
	const uint32 STATUS_FLAGS::battle_obj_flag_gongchengshi = 1 << 4;
	/*const uint32 VISUAL_FLAGS::battle_obj_flag_attacked_by_small_tank = 1 << 2;
	const uint32 VISUAL_FLAGS::battle_obj_flag_attacked_by_cibao_tank = 1 << 3;
	const uint32 VISUAL_FLAGS::battle_obj_flag_attacked_by_big_tank = 1 << 4;
	const uint32 VISUAL_FLAGS::battle_obj_flag_attacked_by_big_gun = 1 << 5;
	const uint32 VISUAL_FLAGS::battle_obj_flag_attacked_by_small_gun = 1 << 6;*/
	
	

	class battle_base_unit : public interface_rtti
	{
	protected:
		uint32		_id;
		uint32		_type;
		uint32		_camps;
		bool		_building_shild;
		battle_map_base* _base_map;
	protected:
		virtual void any_obj_removed(battle_unit::battle_base_unit* o) = 0;
	public:
		inline battle_map_base* base_map()
		{
			return globel::globel_data->cur_map;
		}
		battle_base_unit() { _building_shild = false; }
		flags<unsigned int> status_flags;
		flags<unsigned int> effect_flags;
		inline bool	operator == (const battle_base_unit& o)const { return _id == o._id; }
		inline bool equal(const battle_base_unit& o)const { return *this == o; }
		inline bool equal(const battle_base_unit* p)const { return this->_id == p->_id; }
		inline bool ally(const battle_base_unit& o)const { return this->_camps == o._camps; }
		inline const uint32& id()const { return _id; }
		inline void id(const uint32& v) { _id = v; }
		inline const uint32& type()const { return _type; }
		inline void type(const uint32& v) { _type = v; }
		inline const uint32& camps()const { return _camps; }
		inline void camps(const uint32& v) { _camps = v; }
		virtual bool is_boss() { return false; }
		virtual void round_bigin(int millisecond) = 0;
		virtual void update_enemy_online() = 0;
		virtual void update(int millisecond) = 0;
		virtual void round_end(int millisecond) = 0;
		virtual void serialization(char* data) = 0;
		virtual void deserialization(char* data) = 0;
		virtual void remove_from_world() = 0;
		virtual void initialization(int id, int type, char* data) = 0;
		virtual void join_world(int x, int y) = 0;
		virtual bool is_building_shild() { return _building_shild; }
		inline void set_building_shild(bool v) { _building_shild = v; }
		//virtual bool is_building() = 0;
		void on_any_obj_removed(battle_unit::battle_base_unit* o)
		{
			if (o != this)
				any_obj_removed(o);
		}
	};
	class interface_attack_able
	{
	public:
		const static int interface_type = rtti_i_attack_able;
	private:
		uint32	_hp;
		uint32	_threat;
	public:
		interface_attack_able(interface_rtti* rtti)
		{
			rtti->interface_flags.add(rtti_flag_attack_able);
			rtti->rtti_point[rtti_i_attack_able] = dynamic_cast<interface_attack_able*>(this);
		}
		flags<unsigned int> be_attack_flags;
		inline const uint32& hp()const
		{
			return _hp;
		}
		inline void hp(const uint32& v)
		{
			_hp = v;
		}
		inline const uint32& threat()const
		{
			return _threat;
		}
		inline void threat(const uint32& v)
		{
			_threat = v;
		}
		inline void kill_now()
		{
			hp(0);
		}
		virtual void on_demage(int v)
		{
			int the_v = v;
			_hp = _hp > the_v ? (_hp - the_v) : 0;
		}
		virtual	bool death()const
		{
			return _hp <= 0;
		}
	};
	class interface_attacker
	{
	public:
		const static int interface_type = rtti_i_attacker;
	private:
		uint32	_attack_distance;
		uint32	_attack_distance_squ;
		uint32	_sputtering_area;
		uint32	_sputtering_area_squ;
	protected:
		bool	_use_trajectory;
		uint32	_attack_power;
		uint32	_attack_speed;
		uint32	_attack_special_effects;
		battle_base_unit*	_aiming_target;
	public:
		inline bool use_trajectory()
		{
			return _use_trajectory;
		}
		inline void use_trajectory(bool use)
		{
			_use_trajectory = use;
		}
		inline const uint32& attack_visual_flag()
		{
			return _attack_special_effects;
		}
		interface_attacker(interface_rtti* rtti)
		{
			_aiming_target = NULL;
			_use_trajectory = false;
			rtti->interface_flags.add(rtti_flag_attacker);
			rtti->rtti_point[rtti_i_attacker] = dynamic_cast<interface_attacker*>(this);
		}
		flags<unsigned int> attack_flags;
		inline const uint32& sputtering_area()
		{
			return _sputtering_area;
		}
		inline const uint32& sputtering_area_squ()
		{
			return _sputtering_area_squ;
		}
		inline const void sputtering_area(const uint32& v)
		{
			_sputtering_area = v;
			_sputtering_area_squ = v*v;
		}
		inline const uint32& attack_distance()
		{
			return _attack_distance;
		}
		inline const uint32& attack_distance_squ()
		{
			return _attack_distance_squ;
		}
		inline void attack_distance(const uint32& v)
		{
			_attack_distance = v;
			_attack_distance_squ = v*v;
		}
		inline const uint32& attack_speed()const
		{
			return _attack_speed;
		}
		inline void attack_speed(const uint32& v)
		{
			_attack_speed = v;
		}
		inline const uint32& attack_power()const
		{
			return _attack_power;
		}
		inline void attack_power(const uint32& v)
		{
			_attack_power = v;
		}
		inline battle_base_unit* aiming_target()
		{
			return _aiming_target;
		}
		inline void aiming_target(battle_base_unit* v)
		{
			_aiming_target = v;
		}
		inline bool has_aiming_target()
		{
			return aiming_target() != NULL;
		}
		inline void attack_visual_flag(const uint32& v)
		{
			_attack_special_effects = v;
		}
		virtual void demage(interface_attack_able* v)
		{
			v->on_demage(this->attack_power());
		}
		virtual void aiming(uint32 millisecond) = 0;
		virtual void shot(uint32 millisecond) = 0;
		virtual bool is_prepare_shot() = 0;
	};
	class interface_seeker
	{
	public:
		const static int interface_type = rtti_i_seeker;
	private:
		uint32	_seek_distance;
		uint32	_seek_distance_squ;
		cpVect	_destination;
		cpVect	_last_destination;
		battle_base_unit* _seeked_target;
		uint32	_use_dis;
	protected:
		std::vector<pathfinding_result*> seek_path;
	protected:
		virtual bool seek_target() = 0;
		virtual bool seek_waypoint() = 0;
		virtual cpVect target_location() = 0;
		virtual cpVect waypoint_location() = 0;
		virtual bool need_seek() = 0;
		virtual bool use_pathfinding() = 0;
		virtual bool use_soilder_waypoint() = 0;
		virtual bool find_path(vector<pathfinding_result*>& path) = 0;
	public:
		virtual void clear_path() = 0;
		inline bool use_distance()
		{
			return _use_dis != 0;
		}
		inline void use_distance(bool use)
		{
			_use_dis = use ? 1 : 0;
		}
		inline battle_base_unit* seeked_target()
		{
			return _seeked_target;
		}
		inline void seeked_target(battle_base_unit* v)
		{
			_seeked_target = v;
		}
		inline std::vector<pathfinding_result*>& path()
		{
			return seek_path;
		}
		inline cpVect destination()
		{
			return _destination;
		}
		inline const uint32& seek_distance()
		{
			return _seek_distance;
		}
		inline void seek_distance(const uint32& v)
		{
			_seek_distance = v;
			_seek_distance_squ = v*v;
		}
		inline const uint32& seek_distance_squ()
		{
			return _seek_distance_squ;
		}
		interface_seeker(interface_rtti* rtti)
		{
			_last_destination.x = _last_destination.y = 0;
			_seeked_target = NULL;
			rtti->interface_flags.add(rtti_flag_seeker);
			rtti->rtti_point[rtti_i_seeker] = dynamic_cast<interface_seeker*>(this);
		}
		virtual ~interface_seeker() {}
		
		void try_find_path()
		{
			if (!use_pathfinding())
				return;
			if (seek_path.empty()
				|| cpvdistsq(_last_destination, _destination) > 10000)
			{
				_last_destination = _destination;
				find_path(this->seek_path);
			}
		}
		virtual void seek(uint32 millisecond)
		{
			if (need_seek())
			{
				if (seek_target())
				{
					_destination = target_location();
				}
				else
				{
					seek_waypoint();
					_destination = waypoint_location();
				}
			}
			try_find_path();			
		}
	};
	class interface_big_bomb
	{
	public:
		const static int interface_type = rtti_i_time_bomb;
	private:
		uint32	_exploded_timer;
		uint32	_attack_distance;
		uint32	_attack_distance_squ;
		uint32	_attack_power;
	protected:
		bool	_bombed;
	public:
		flags<unsigned int> attack_flags;
		inline bool bombed()
		{
			return _bombed;
		}
		inline void bombed(bool v)
		{
			_bombed = v;
		}
		inline const uint32& attack_distance()
		{
			return _attack_distance;
		}
		inline const uint32& attack_distance_squ()
		{
			return _attack_distance_squ;
		}
		inline void attack_distance(const uint32& v)
		{
			_attack_distance = v;
			_attack_distance_squ = v*v;
		}
		inline const uint32& attack_power()const
		{
			return _attack_power;
		}
		inline void attack_power(const uint32& v)
		{
			_attack_power = v;
		}

		interface_big_bomb(interface_rtti* rtti) :_bombed(false)
		{
			rtti->interface_flags.add(rtti_flag_time_bomb);
			rtti->rtti_point[rtti_i_time_bomb] = dynamic_cast<interface_big_bomb*>(this);
		}
		inline const uint32& exploded_timer()
		{
			return _exploded_timer;
		}
		inline void exploded_timer(const uint32& v)
		{
			_exploded_timer = v;
		}
		virtual void bomb(uint32 millisecond) = 0;
	};
	class interface_tank
	{
	public:
		const static int interface_type = rtti_i_tank;
	protected:
		float	_cur_aiming_angle;
		float	_max_attack_angle;
	public:
		inline float cur_aiming_angle()
		{
			return _cur_aiming_angle;
		}
		inline void cur_aiming_angle(float v)
		{
			_cur_aiming_angle = v;
		}
		inline float max_attack_angle()
		{
			return _max_attack_angle;
		}
		inline void max_attack_angle(float v)
		{
			_max_attack_angle = v;
		}
		interface_tank(interface_rtti* rtti)
		{
			rtti->interface_flags.add(rtti_flag_tank);
			rtti->rtti_point[rtti_i_tank] = dynamic_cast<interface_tank*>(this);			
		}
		virtual ~interface_tank() {}
	};
	class interface_rigid_body
	{
	public:
		const static int interface_type = rtti_i_rigid_body;
	protected:
		float	_radius;
		float	_the_face;
		float	_max_speed;
		cpVect	_last_location;
		cpBody*	_body;
		cpShape* _shape;
		cpVect	_impulse;

		cpVect	_box_contact_force;
		cpVect	_sensor_contact_force;
		cpVect	_unit_contact_force;
		cpVect	_obstacle_contact_force;
	protected:
		void reset_contact_force()
		{
			_box_contact_force.x = _box_contact_force.y = 0;
			_sensor_contact_force.x = _sensor_contact_force.y = 0;
			_unit_contact_force.x = _unit_contact_force.y = 0;
			_obstacle_contact_force.x = _obstacle_contact_force.y = 0;
		}
	public:
		interface_rigid_body(interface_rtti* rtti)
		{
			rtti->interface_flags.add(rtti_flag_rigid_body);
			rtti->rtti_point[rtti_i_rigid_body] = dynamic_cast<interface_rigid_body*>(this);
		}
		virtual void attach_contact_force(interface_rigid_body* o, cpVect& force, bool north)
		{
			cpVect f = cpvzero;
			float dis = 0;
			if (this->is_static())
			{
				force = force + o->location() - this->location();
				return;
			}
			else
			{
				f = o->location() - this->location();
				dis = cpvlength(f);
				f = cpvnormalize(f);
				f = cpvmult(f, ((o->radius() + this->radius()) > dis ? (o->radius() + this->radius() - dis) : 0));

				force = force + f;
				if (o->location().x < this->location().x)
					if (north)
						f = cpv(f.y, -f.x);
					else
						f = cpv(-f.y, f.x);
				else
					if (north)
						f = cpv(-f.y, f.x);
					else
						f = cpv(f.y, -f.x);
				force = force + f;
			}
		}
		inline cpSpace* this_world()
		{
			return globel::globel_data->this_world;
		}
		virtual ~interface_rigid_body()
		{
			cpShapeFree(_shape);
			cpBodyFree(_body);
			_body = NULL;
			_shape = NULL;
		}
		inline float max_speed()
		{
			return _max_speed;
		}
		inline void max_speed(float v)
		{
			_max_speed = v;
		}
		inline cpVect location()
		{
			return _body->p;
		}
		inline cpBody* body()
		{
			return _body;
		}
		inline cpShape* shape()
		{
			return _shape;
		}
		inline void location(const uint32& x, const uint32& y)
		{
			cpBodySetPosition(_body, cpv(x, y));
		}
		inline float radius()
		{
			return _radius;
		}
		inline void radius(float v)
		{
			_radius = v;
		}
		inline float face()
		{
			return _the_face;
		}
		inline void face(float v)
		{
			_the_face = v;
		}
		inline float distance_squared(const interface_rigid_body* o)const
		{
			return cpvdistsq(this->_body->p, o->_body->p);
		}
		inline float distance(const interface_rigid_body* o)const
		{
			return cpvdist(this->_body->p, o->_body->p);
		}
		virtual bool is_box()
		{
			return false;
		}
		virtual bool is_static()
		{
			return false;
		}
		virtual bool is_sensor()
		{
			return false;
		}
		virtual void on_contact_obstacle() = 0;
		virtual void on_contact(interface_rtti* i) = 0;
		virtual uint16 collision_type() = 0;
		virtual uint16 collision_mask() = 0;		
		virtual void movement(uint32 millisecond) = 0;
		virtual bool movement_able() = 0;
	};
	class interface_world_listener
	{
	public:
		const static int interface_type = rtti_i_world_listener;
	public:
		interface_world_listener(interface_rtti* rtti)
		{
			rtti->interface_flags.add(rtti_flag_world_listener);
			rtti->rtti_point[rtti_i_world_listener] = dynamic_cast<interface_world_listener*>(this);
		}
	};
	class interface_occupied
	{
	public:
		const static int interface_type = rtti_i_occupied;
	private:
		uint32 _occupied_duration = 0;
	public:
		interface_occupied(interface_rtti* rtti)
		{
			_occupied_duration = 0;
			printf("_occupied_duration :: %i", _occupied_duration);
			rtti->interface_flags.add(rtti_flag_occupied);
			rtti->rtti_point[rtti_i_occupied] = dynamic_cast<interface_occupied*>(this);
		}
		inline const uint32& occupied_duration()
		{
			return _occupied_duration;
		}
		inline void occupied_duration(const uint32& v)
		{
			_occupied_duration = v;
		}
		void update_occupied(const uint32& millisecond)
		{
			if (_occupied_duration < millisecond)
				_occupied_duration = 0;
			else
				occupied_duration(_occupied_duration - millisecond);
		}
		virtual bool check_occupied()
		{
			return occupied_duration() > 0;
		}
		void occupy(const uint32& how_long)
		{
			occupied_duration(how_long);
		}
	};

	
};
#endif // !layer_interface_pp#pragma once
