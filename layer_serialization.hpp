#ifndef layer_serialization_hpp
#define layer_serialization_hpp
#include "layer_base.hpp"
#include "battle_interface_struct.hpp"
namespace battle_unit
{
	template<class T>
	class unit_serialization
	{
	public:
		static void serialization(T* unit, const char* data) {}
	};
	template<>
	class unit_serialization<battle_base_unit>
	{
	public:
		static void serialization(battle_base_unit* unit, char* data)
		{
			unit->camps(reinterpret_cast<input_obj_data_base*>(data)->_camps);
		}
	};
	template<>
	class unit_serialization<interface_attacker>
	{
	public:
		static void serialization(interface_attacker* unit, char* data)
		{
			unit->attack_distance(reinterpret_cast<input_obj_data_unit*>(data)->_attack_distance);
			unit->attack_flags.set(reinterpret_cast<input_obj_data_unit*>(data)->_attack_flags);
			unit->attack_visual_flag(reinterpret_cast<input_obj_data_base*>(data)->_attack_special_effects);
			unit->attack_power(reinterpret_cast<input_obj_data_unit*>(data)->_attack_power);
			unit->attack_speed(reinterpret_cast<input_obj_data_unit*>(data)->_attack_speed);			
			unit->sputtering_area(reinterpret_cast<input_obj_data_unit*>(data)->_aoe_area);
		}
	};
	template<>
	class unit_serialization<interface_attack_able>
	{
	public:
		static void serialization(interface_attack_able* unit, char* data)
		{
            uint32 hp = (int)reinterpret_cast<input_obj_data_base*>(data)->_curHp;
            if(hp > 0){
                unit->hp(hp);
            }else{
                unit->hp(reinterpret_cast<input_obj_data_base*>(data)->_hp);
            }
			
			unit->threat(reinterpret_cast<input_obj_data_base*>(data)->_threat);
			unit->be_attack_flags.set(reinterpret_cast<input_obj_data_base*>(data)->_be_attack_flags);
			
		}
	};
	template<>
	class unit_serialization<interface_rigid_body>
	{
	public:
		static void serialization(interface_rigid_body* unit, char* data)
		{
			unit->face(reinterpret_cast<input_obj_data_base*>(data)->_camps == 0 ? 90 : 270);
			unit->max_speed(reinterpret_cast<input_obj_data_unit*>(data)->_max_speed);
			unit->radius(reinterpret_cast<input_obj_data_unit*>(data)->_radius);
		}
	};
	template<>
	class unit_serialization<interface_seeker>
	{
	public:
		static void serialization(interface_seeker* unit, char* data)
		{
			unit->seek_distance(reinterpret_cast<input_obj_data_unit*>(data)->_seek_distance);
		}
	};
	template<>
	class unit_serialization<interface_tank>
	{
	public:
		static void serialization(interface_tank* unit, char* data)
		{
			unit->max_attack_angle(reinterpret_cast<input_obj_data_test_tank*>(data)->_max_attack_angle);
		}
	};
	template<>
	class unit_serialization<interface_big_bomb>
	{
	public:
		static void serialization(interface_big_bomb* unit, char* data)
		{
			unit->exploded_timer(reinterpret_cast<input_obj_data_bomb*>(data)->_exploded_timer);
			unit->attack_distance(reinterpret_cast<input_obj_data_bomb*>(data)->_bomb_distance);
			unit->attack_power(reinterpret_cast<input_obj_data_bomb*>(data)->_bomb_power);
			unit->attack_flags.set(reinterpret_cast<input_obj_data_bomb*>(data)->_attack_flags);
		}
	};
	template<class T>
	class unit_deserialization
	{
	public:
		static void deserialization(T* unit, const char* data) {}
	};
	template<>
	class unit_deserialization<battle_base_unit>
	{
	public:
		static void deserialization(battle_base_unit* unit, char* data)
		{
			output_obj_data_base* d = (output_obj_data_base*)data;
			d->ID = unit->id();
			d->TYPE = unit->type();
			d->status_flag = unit->status_flags.get();
			d->effect_flag = unit->effect_flags.get();
		}
	};
	template<>
	class unit_deserialization<interface_attacker>
	{
	public:
		static void deserialization(interface_attacker* unit, char* data)
		{
			output_obj_data_base* d = (output_obj_data_base*)data;
			d->target_id = unit->has_aiming_target() ? unit->aiming_target()->id() : 0;
		}
	};
	template<>
	class unit_deserialization<interface_attack_able>
	{
	public:
		static void deserialization(interface_attack_able* unit, char* data)
		{
			output_obj_data_base* d = (output_obj_data_base*)data;
			d->cur_hp = unit->hp();
		}
	};
	template<>
	class unit_deserialization<interface_rigid_body>
	{
	public:
		static void deserialization(interface_rigid_body* unit, char* data)
		{
			output_obj_data_base* d = (output_obj_data_base*)data;
			d->face = unit->face();
			d->x = unit->location().x;
			d->y = unit->location().y;
            
//            printf("deserialization id ::: %i x ::: %f y ::: %f face ::: %f \n",d->ID,d->x,d->y,d->face);
		}
	};
	template<>
	class unit_deserialization<interface_tank>
	{
	public:
		static void deserialization(interface_tank* unit, char* data)
		{
			output_obj_data_test_tank* d = (output_obj_data_test_tank*)data;
			d->aiming = unit->cur_aiming_angle();
		}
	};
	template<>
	class unit_deserialization<interface_big_bomb>
	{
	public:
		static void deserialization(interface_big_bomb* unit, char* data)
		{
			output_obj_data_bomb* d = (output_obj_data_bomb*)data;
			d->bombed = unit->bombed() ? 1 : 0;
		}
	};
}
#endif // !layer_serialization_hpp#pragma once
