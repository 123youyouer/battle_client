#ifndef battle_unit_builder_hpp
#define battle_unit_builder_hpp
#include "unit_all.hpp"
#include "unit_wuzhuangzhishengji.hpp"
namespace battle_unit
{	
	class unit_builder
	{
	public:
		template<class T>
		static battle_base_unit* build(int id, int type, int x, int y, char* base_data)
		{
			battle_base_unit* u=static_cast<battle_base_unit*>(new T());
			u->initialization(id, type, base_data);
			u->join_world(x, y);
			return u;
		}
		static battle_base_unit* build_by_type(int id, int type, int x, int y, char* base_data)
		{
			switch (type)
			{
			case object_type::battle_object_type_zibao_kache:
				return build<unit_zibaokache>(id, type, x, y, base_data);
			case object_type::battle_object_type_bomb:
				return build<unit_time_bomb>(id, type, x, y, base_data);
			case object_type::battle_object_type_tianqi_tank:
				return build<unit_tianqi>(id, type, x, y, base_data);
			case object_type::battle_object_type_huixiong_tank:
				return build<unit_huixiong_tank>(id, type, x, y, base_data);
			case object_type::battle_object_type_xiniu_tank:
				return build<unit_xiniu_tank>(id, type, x, y, base_data);
			case object_type::battle_object_type_meiguodabing:
				return build<unit_meiguodabing>(id, type, x, y, base_data);
			case object_type::battle_object_type_bingying:
				return build<unit_bingying>(id, type, x, y, base_data);
			case object_type::battle_object_type_cineng_tank:
				return build<unit_cibao_tank>(id, type, x, y, base_data);
			case object_type::battle_object_type_guanglengtank:
				return build<unit_guangleng_tank>(id, type, x, y, base_data);
			case object_type::battle_object_type_V3:
				return build<unit_v3>(id, type, x, y, base_data);
			case object_type::battle_object_type_dianta:
				return build<unit_dianta>(id, type, x, y, base_data);
			case object_type::battle_object_type_jinuofu:
				return build<unit_jinuofu>(id, type, x, y, base_data);
			case object_type::battle_object_type_zhujidi:
				return build<unit_zhujidi>(id, type, x, y, base_data);
			case object_type::battle_object_type_aiguozhe:
				return build<unit_aiguozhe>(id, type, x, y, base_data);
			case object_type::battle_object_type_aiguozhe_two:
				return build<unit_aiguozhe_two>(id, type, x, y, base_data);
			case object_type::battle_object_type_feixingbing:
				return build<unit_feixingbing>(id, type, x, y, base_data);
			case object_type::battle_object_type_zhongzhuangdabing:
				return build<unit_zhongzhuangdabing>(id, type, x, y, base_data);
			case object_type::battle_object_type_fushegongbing:
				return build<unit_fushegongbing>(id, type, x, y, base_data);
			case object_type::battle_object_type_zhandouyaosai:
				return build<unit_zhandouyaosai>(id, type, x, y, base_data);
			case object_type::battle_object_type_wuzhuangzhishengji:
				return build<unit_wuzhuangzhishengji>(id, type, x, y, base_data);
			case object_type::battle_object_type_gaitetank:
				return build<unit_gaite_tank>(id, type, x, y, base_data);
			case object_type::battle_object_type_tanya:
				return build<unit_tanya>(id, type, x, y, base_data);
			case object_type::battle_object_type_dog:
				return build<unit_dog>(id, type, x, y, base_data);
			case object_type::battle_object_type_kongbufenzi:
				return build<unit_kongbufenzi>(id, type, x, y, base_data);
			case object_type::battle_object_type_guanglengta:
				return build<unit_guanglengta>(id, type, x, y, base_data);
			case object_type::battle_object_type_kuangshouren:
				return build<unit_kuangshouren>(id, type, x, y, base_data);
			case object_type::battle_object_type_gongchengshi:
				return build<unit_gongchengshi>(id, type, x, y, base_data);
			case object_type::battle_object_type_cineng_tank_facory:
				return build<unit_fujidi>(id, type, x, y, base_data);
			case object_type::battle_object_type_fujidi:
				return build<unit_fujidi>(id, type, x, y, base_data);
			case object_type::battle_object_type_jupao:
				return build<unit_jupao>(id, type, x, y, base_data);
			case object_type::battle_object_boss_kongbufenzi:
				return build<unit_kongbufenzi>(id, type, x, y, base_data);
			case object_type::battle_object_boss_kuangshouren:
				return build<unit_boss_kuangshouren>(id, type, x, y, base_data);
			}
			return NULL;
		}
	};
}
#endif#pragma once
