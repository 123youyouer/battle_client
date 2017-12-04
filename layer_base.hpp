#ifndef layer_base_hpp
#define layer_base_hpp
#include "layer_interface.hpp"
#include "layer_aiming.hpp"
#include "layer_seek.hpp"
#include "layer_movement.hpp"
#include "layer_serialization.hpp"
#include "layer_attack.hpp"
namespace battle_unit
{
	template<class... implented_interface>
	class unit
		: public battle_unit::battle_base_unit
		, public battle_unit::interface_rigid_body
		, public implented_interface...
	{
	protected:
		virtual void any_obj_removed(battle_unit::battle_base_unit* o)
		{
			if (this->interface_flags.chk(rtti_flag_attacker))
			{
				if (this->template get_interface_point<battle_unit::interface_attacker>()->aiming_target() == o)
					this->template get_interface_point<battle_unit::interface_attacker>()->aiming_target(NULL);
			}
			if (this->interface_flags.chk(rtti_flag_seeker))
			{
				if (this->template get_interface_point<battle_unit::interface_seeker>()->seeked_target() == o)
					this->template get_interface_point<battle_unit::interface_seeker>()->seeked_target(NULL);
			}
		}
		void build_static_body(int x, int y)
		{
			cpBody* body = cpBodyNewStatic();
			body = cpSpaceAddBody(this_world(), body);
			cpBodySetPosition(body, cpv(x, y));
		}
		virtual bool use_soilder_waypoint()
		{
			return !(this->interface_flags.chk(rtti_flag_tank));
		}
	public:
		unit():battle_unit::interface_rigid_body(dynamic_cast<interface_rtti*>(this)), implented_interface(dynamic_cast<interface_rtti*>(this))...
		{
			init_rtti< unit<implented_interface...> >();
		}
		virtual ~unit() {};
		virtual void initialization(int id, int type, char* data)
		{
			this->id(id);
			this->type(type);
			this->serialization(data);
		}
		virtual cpFloat get_body_mass()
		{
			return 1;
		}
		virtual cpFloat get_body_moment()
		{
			return 1;
		}		
		virtual void join_world(int x, int y)
		{
			if (this->is_static())
			{
				this->_body = cpBodyNewStatic();
			}
			else
			{
				this->_body = cpBodyNew(get_body_mass(), get_body_moment());
			}
			cpBodySetPosition(this->_body, cpv(x, y));
			cpBodySetUserData(this->_body, static_cast<interface_rtti*>(this));
			this->_shape = cpCircleShapeNew(this->_body, this->radius(), cpvzero);
			cpShapeSetElasticity(this->_shape, 0);
			cpShapeSetSensor(this->_shape, this->is_sensor());
			this->_shape->filter.group = 0;
			this->_shape->filter.categories = this->collision_type();
			this->_shape->filter.mask = this->collision_mask();
			cpSpaceAddBody(this_world(), this->_body);
			this->_shape=cpSpaceAddShape(this_world(), this->_shape);
		}
		virtual void serialization(char* data)
		{
			unit_serialization<battle_base_unit>::serialization(this, data);
			if (this->interface_flags.chk(rtti_flag_attacker))
				unit_serialization<battle_unit::interface_attacker>::serialization(this->template get_interface_point<battle_unit::interface_attacker>(), data);
			if (this->interface_flags.chk(rtti_flag_attack_able))
				unit_serialization<battle_unit::interface_attack_able>::serialization(this->template get_interface_point<battle_unit::interface_attack_able>(), data);
			if (this->interface_flags.chk(rtti_flag_rigid_body))
				unit_serialization<battle_unit::interface_rigid_body>::serialization(this->template get_interface_point<battle_unit::interface_rigid_body>(), data);
			if (this->interface_flags.chk(rtti_flag_seeker))
				unit_serialization<battle_unit::interface_seeker>::serialization(this->template get_interface_point<battle_unit::interface_seeker>(), data);
			if (this->interface_flags.chk(rtti_flag_tank))
				unit_serialization<battle_unit::interface_tank>::serialization(this->template get_interface_point<battle_unit::interface_tank>(), data);
			if (this->interface_flags.chk(rtti_flag_time_bomb))
				unit_serialization<battle_unit::interface_big_bomb>::serialization(this->template get_interface_point<battle_unit::interface_big_bomb>(), data);
		}
		virtual void deserialization(char* data)
		{
			unit_deserialization<battle_base_unit>::deserialization(this, data);
			if (this->interface_flags.chk(rtti_flag_attacker))
				unit_deserialization<battle_unit::interface_attacker>::deserialization(this->template get_interface_point<battle_unit::interface_attacker>(), data);
			if (this->interface_flags.chk(rtti_flag_attack_able))
				unit_deserialization<battle_unit::interface_attack_able>::deserialization(this->template get_interface_point<battle_unit::interface_attack_able>(), data);
			if (this->interface_flags.chk(rtti_flag_rigid_body))
				unit_deserialization<battle_unit::interface_rigid_body>::deserialization(this->template get_interface_point<battle_unit::interface_rigid_body>(), data);
			if (this->interface_flags.chk(rtti_flag_tank))
				unit_deserialization<battle_unit::interface_tank>::deserialization(this->template get_interface_point<battle_unit::interface_tank>(), data);
			if (this->interface_flags.chk(rtti_flag_time_bomb))
				unit_deserialization<battle_unit::interface_big_bomb>::deserialization(this->template get_interface_point<battle_unit::interface_big_bomb>(), data);
		}
		void fix_position()
		{
			//cpVect pos = cpv((int)(this->location().x * 1000.0f + 0.5f) / 1000.0f, (int)(this->location().y * 1000.0f + 0.5f) / 1000.0f) ;
			//this->location(pos.x, pos.y);
		}
		virtual void round_bigin(int millisecond)
		{
			status_flags.del(STATUS_FLAGS::battle_obj_flag_shoting);
			effect_flags.reset();
			fix_position();
		}
		virtual void round_end(int millisecond)
		{
			reset_contact_force();
			fix_position();
		}
		virtual void update_enemy_online()
		{
			if (this->interface_flags.chk(rtti_flag_rigid_body))
			{
				cpBodySetVelocity(this->template get_interface_point<battle_unit::interface_rigid_body>()->body(), cpvzero);				
			}
		}
		virtual void update(int millisecond)
		{			
			if (this->interface_flags.chk(rtti_flag_attack_able))
			{
				if (this->template get_interface_point<battle_unit::interface_attack_able>()->death())
					return;
			}
			if (this->interface_flags.chk(rtti_flag_time_bomb))
			{
				if (this->template get_interface_point<battle_unit::interface_big_bomb>()->bombed())
					return;
			}
			if (this->interface_flags.chk(rtti_flag_occupied))
			{
				this->template get_interface_point<battle_unit::interface_occupied>()->update_occupied(millisecond);
				if (this->template get_interface_point<battle_unit::interface_occupied>()->check_occupied())
				{
					this->status_flags.add(STATUS_FLAGS::battle_obj_flag_occupied);
					return;
				}
					
			}
			this->status_flags.del(STATUS_FLAGS::battle_obj_flag_occupied);
			if (this->interface_flags.chk(rtti_flag_attacker))
			{
				this->template get_interface_point<battle_unit::interface_attacker>()->aiming(millisecond);
				this->template get_interface_point<battle_unit::interface_attacker>()->shot(millisecond);
			}
			if (this->interface_flags.chk(rtti_flag_seeker))
			{
				this->template get_interface_point<battle_unit::interface_seeker>()->seek(millisecond);
			}
			
			if (this->interface_flags.chk(rtti_flag_rigid_body))
			{
				this->template get_interface_point<battle_unit::interface_rigid_body>()->movement(millisecond);
			}
			if (this->interface_flags.chk(rtti_flag_time_bomb))
			{
				this->template get_interface_point<battle_unit::interface_big_bomb>()->bomb(millisecond);
			}
		}
		virtual bool movement_able()
		{
			if (this->interface_flags.chk(rtti_flag_occupied))
			{
				if (this->template get_interface_point<battle_unit::interface_occupied>()->check_occupied())
					return false;
			}
			return true;
		}
		virtual bool is_prepare_shot()
		{
			if (this->interface_flags.chk(rtti_flag_attacker))
			{
				if (this->template get_interface_point<battle_unit::interface_attacker>()->has_aiming_target())
					return true;
			}
			return false;
		}
		virtual void remove_from_world()
		{
			cpSpaceRemoveShape(this_world(), this->_shape);
			cpSpaceRemoveBody(this_world(), this->_body);
			cpShapeFree(this->_shape);
			cpBodyFree(this->_body);
			_body = NULL;
			_shape = NULL;
		}
		virtual bool need_seek()
		{
			if (!this->interface_flags.chk(rtti_flag_attacker))
				return true;
			else
				return !this->template get_interface_point<battle_unit::interface_attacker>()->has_aiming_target();
		}
		virtual void on_contact_obstacle()
		{

		}
		virtual void on_contact(interface_rtti* i)
		{
			battle_unit::interface_rigid_body* tar = i->get_interface_point<battle_unit::interface_rigid_body>();
			if (tar->is_box())
				tar->attach_contact_force(this->template get_interface_point<battle_unit::interface_rigid_body>(), _box_contact_force, this->camps() == 0);
			else if(tar->is_sensor())
				tar->attach_contact_force(this->template get_interface_point<battle_unit::interface_rigid_body>(), _sensor_contact_force, this->camps() == 0);
			else
				tar->attach_contact_force(this->template get_interface_point<battle_unit::interface_rigid_body>(), _unit_contact_force, this->camps() == 0);
		}
	};

#pragma region soilder
	typedef unit<interface_attacker, interface_attack_able, interface_seeker>													_soilder_0;
	typedef layer_aiming_soilder<_soilder_0>															_soilder_1;
	typedef layer_seeker_target<_soilder_1>																_soilder_2;
	typedef layer_seeker_waypoint_by_ground<_soilder_2>													_soilder_3;
	typedef	layer_attack<_soilder_3>																	_soilder_4;
	typedef layer_movement_soilder<_soilder_4>															_soilder_5;

	class unit_ground_soilder : public _soilder_5
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
		virtual ~unit_ground_soilder() {}
	};

	typedef layer_seeker_waypoint_by_fly<_soilder_2>													_fly_soilder_3;
	typedef	layer_attack<_fly_soilder_3>																_fly_soilder_4;
	typedef layer_movement<_fly_soilder_4>																_fly_soilder_5;

	class unit_fly_soilder : public _fly_soilder_5
	{
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_sky1;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_sky1;
		}
		virtual ~unit_fly_soilder() {}
	};

#pragma endregion

#pragma region aircraft
	typedef unit<interface_attacker, interface_attack_able, interface_seeker>													_aircraft_0;
	typedef layer_aiming_threat<_aircraft_0>															_aircraft_1;
	typedef layer_seeker_target<_aircraft_1>															_aircraft_2;
	typedef layer_seeker_waypoint_by_fly<_aircraft_2>													_aircraft_3;
	typedef	layer_attack<_aircraft_3>																	_aircraft_4;
	typedef layer_movement<_aircraft_4>																	_aircraft_5;

	class unit_aircraft : public _aircraft_5
	{
	public:
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_aircraft;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_aircraft;
		}
		virtual ~unit_aircraft() {}
	};

#pragma endregion

#pragma region tank
	typedef unit<interface_attacker, interface_attack_able, interface_seeker, interface_tank>											_tank_0;
	typedef layer_aiming_tank<_tank_0>																	_tank_1;
	typedef layer_seeker_target<_tank_1>																_tank_2;
	typedef layer_seeker_waypoint_by_ground<_tank_2>													_tank_3;
	typedef	layer_attack<_tank_3>																		_tank_4;
	typedef layer_movement<_tank_4>																		_tank_5;

	class unit_ground_tank : public _tank_5
	{
	public:
		virtual cpFloat get_body_mass()
		{
			return 10;
		}
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_tank | collision_type::collision_type_terrain | collision_type::collision_type_building;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_tank | collision_type::collision_type_terrain | collision_type::collision_type_building;;
		}
		virtual ~unit_ground_tank() {}
	};
#pragma endregion

#pragma region factory
	typedef unit<interface_attack_able, interface_occupied>																_factory_0;
	class unit_factory : public _factory_0
	{
	public:
		virtual bool is_static()
		{
			return true;
		}
	public:
		virtual bool movement_able()
		{
			return false;
		}
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_terrain | collision_type::collision_type_building;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_terrain | collision_type::collision_type_building;
		}
		virtual void movement(uint32 millisecond)
		{
			return;
		}
		virtual ~unit_factory() {}
	};
#pragma endregion

#pragma region tower
	typedef unit<interface_attacker, interface_attack_able, interface_occupied,interface_tank>											_tower_0;
	typedef layer_aiming_tank<_tower_0>																	_tower_1;
	typedef	layer_attack<_tower_1>																		_tower_2;
	class unit_tower : public _tower_2
	{
	public:
		virtual bool is_static()
		{
			return true;
		}
	public:
		virtual bool is_sensor()
		{
			return false;
		}
		virtual bool movement_able()
		{
			return false;
		}
		virtual uint16 collision_type()
		{
			return collision_type::collision_type_tank;
			return collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
		}
		virtual uint16 collision_mask()
		{
			return collision_type::collision_type_tank;
			return collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
		}
		virtual void movement(uint32 millisecond)
		{
			return;
		}
		obstacle* ob;
	public:
		unit_tower() :ob(NULL) {}
		virtual void join_world(int x, int y)
		{
			this->radius(10);
			_tower_2::join_world(x, y);
			ob = globel::globel_data->cur_map->create_obstacle(
				x,
				y,
				40,
				40,
				this->id(),
				collision_type::collision_type_building,
				globel::globel_data->this_world
			);

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
		virtual ~unit_tower() {}
	};
#pragma endregion

};

#endif // !layer_base_hpp#pragma once
