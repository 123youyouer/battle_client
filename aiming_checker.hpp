#ifndef aiming_checker_hpp
#define aiming_checker_hpp
#include <vector>
#include "layer_base.hpp"
#include "globel.h"
namespace battle_unit
{	
	template<class T>
	float distance_squ(T* src, interface_rigid_body* tar)
	{
		return src->distance_squared	(tar);
	};
	template<class T>
	float angle(T* src, battle_base_unit* tar)
	{
		return globel_func::direction_to_east(tar->template get_interface_point<interface_rigid_body>()->location()
			- src->location());
	}
	template<class T>
	class aiming_checker_unit
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			if (src == NULL
				|| tar == NULL
				|| src->ally(*tar)
				|| !tar->interface_flags.chk(rtti_flag_attack_able)
				|| tar->template get_interface_point<interface_attack_able>()->death())
			{
				return false;
			}
			return true;
		}
	};
	template<class T>
	class aiming_checker_flag
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			return src->attack_flags	.chk(tar->template get_interface_point<interface_attack_able>()->be_attack_flags);
		}
	};
	template<class T>
	class aiming_checker_by_distance
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			int r1 = src->radius() + tar->template get_interface_point<interface_rigid_body>()->radius() + src->attack_distance();
			r1 *= r1;
			return (r1 >= distance_squ<T>(src, tar->template get_interface_point<battle_unit::interface_rigid_body>()));
		}
	};
	template<class T>
	class aiming_checker_attackable_by_warrior
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			int r1 = src->radius() + tar->template get_interface_point<interface_rigid_body>()->radius() + 15;
			int d1 = distance_squ<T>(src, tar->template get_interface_point<interface_rigid_body>());
			int d2 = r1*r1;
			return  (d1 <= d2);
		}
	};
	template<class T>
	class aiming_checker_attackable_only_soilder
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			switch (tar->type())
			{
			case object_type::battle_object_type_zhongzhuangdabing:
			case object_type::battle_object_type_fushegongbing:
			case object_type::battle_object_type_meiguodabing:
			case object_type::battle_object_type_dog:
			case object_type::battle_object_type_tanya:
			case object_type::battle_object_type_feixingbing:
			case object_type::battle_object_type_zhujidi:
			case object_type::battle_object_boss_kuangshouren:
				return true;
			default:
				return false;
			}
			return false;
		}
	};
	template<class T>
	class seek_checker_by_distance
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			return  (distance_squ<T>(src, tar->template get_interface_point<battle_unit::interface_rigid_body>()) <= src->seek_distance_squ());
		}
	};
	template<class T>
	class aiming_checker_by_face
	{
	public:
		static bool chk(T* src, battle_base_unit* tar)
		{
			float a1 = src->face();
			float a2 = angle(src, tar);

			return fabs(a2 - a1) < src->max_attack_angle();
		}
	};

	class aiming_selector_by_threat
	{
	public:
		static battle_base_unit* select(battle_base_unit* t1, battle_base_unit* t2)
		{
			if (t1 == NULL)
				return t2;
			if (t2 == NULL)
				return t1;
			if (!t1->interface_flags.chk(rtti_flag_attack_able)
				|| !t2->interface_flags.chk(rtti_flag_attack_able))
				return NULL;
			return t1->template get_interface_point<interface_attack_able>()->threat() < t2->template get_interface_point<interface_attack_able>()->threat() ? t2 : t1;
		}
	};
	class aiming_selector_by_threat_and_distance
	{
	private:
		
	public:
		static battle_base_unit* select_by_dis(battle_base_unit* src, battle_base_unit* t1, battle_base_unit* t2)
		{
			if (t1 == NULL)
				return t2;
			if (t2 == NULL)
				return t1;
			int d1 = src->template get_interface_point<interface_rigid_body>()->distance_squared(t1->template get_interface_point<interface_rigid_body>());
			int d2 = src->template get_interface_point<interface_rigid_body>()->distance_squared(t2->template get_interface_point<interface_rigid_body>());
			if (d1 <= d2)
				return t1;
			return t2;
		}
		static int select_by_threat(battle_base_unit* src, battle_base_unit* t1, battle_base_unit* t2)
		{
			if (t1 == NULL)
				return 2;
			if (t2 == NULL)
				return 1;
			if (!t1->interface_flags.chk(rtti_flag_attack_able)
				|| !t2->interface_flags.chk(rtti_flag_attack_able))
			{
				if (t1->interface_flags.chk(rtti_flag_attack_able))
					return 1;
				if (t2->interface_flags.chk(rtti_flag_attack_able))
					return 2;
				return -1;
			}
			int threat1 = t1->template get_interface_point<interface_attack_able>()->threat();
			int threat2 = t2->template get_interface_point<interface_attack_able>()->threat();
			if (threat1 < threat2)
				return 2;
			else if (threat1 > threat2)
				return 1;
			else
				return 0;
		}
		static battle_base_unit* select(battle_base_unit* src, battle_base_unit* old, battle_base_unit* t1, battle_base_unit* t2)
		{
			switch (select_by_threat(src, t1, t2))
			{
			case -1:
				return NULL;
			case 0:
				if (t1 == old)
					return t1;
				else if (t2 == old)
					return t2;
				else
					return select_by_dis(src, t1, t2);
			case 1:
				return t1;
			case 2:
				return t2;
			}
			return NULL;
		}
	};

	
}
#endif
