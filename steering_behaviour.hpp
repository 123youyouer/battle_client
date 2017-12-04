#ifndef steering_behaviour_hpp
#define steering_behaviour_hpp
#include <vector>
#include "layer_base.hpp"
#include "globel.h"

namespace battle_unit
{
	class steering_behaviour_base
	{
	protected:
		float coefficient;
		b2Vec2 result;
		int compute_type;
	public:
		steering_behaviour_base() :coefficient(1.0f), result(b2Vec2_zero) {}
		virtual const b2Vec2& compute_mechanics(interface_rigid_body* src) = 0;
	};
	class steering_behaviour_by_target : public steering_behaviour_base
	{
	public:
		virtual const b2Vec2& compute_mechanics(interface_rigid_body* src)
		{
			try
			{
				const b2Vec2& loc = src->location();
				const b2Vec2& dst = src->template get_interface_point<interface_seeker>()->destination();
				result = (dst - loc);
				result.x = result.x*coefficient;
				result.y = result.y*coefficient;
				return result;
			}
			catch (...)
			{
				return b2Vec2_zero;
			}
		}
	};
}
#endif // !steering_behaviour_hpp#pragma once
