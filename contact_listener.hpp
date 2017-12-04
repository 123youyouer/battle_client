#ifndef contact_listener_hpp
#define contact_listener_hpp
#include "layer_interface.hpp"
/*
class contact_listener : public b2ContactListener
{
public:
	virtual void BeginContact(b2Contact* contact) 
	{
		battle_unit::i_battle_base_unit* u0 = (battle_unit::i_battle_base_unit*)(contact->GetFixtureA()->GetBody()->GetUserData());
		battle_unit::i_battle_base_unit* u1 = (battle_unit::i_battle_base_unit*)(contact->GetFixtureB()->GetBody()->GetUserData());
		if (u0 != NULL && u1 != NULL && u0->ally(*u1))
		{
			u0->template get_interface_point<battle_unit::i_rigid_body>()->add_contact(u1->template get_interface_point<battle_unit::i_rigid_body>());
			u1->template get_interface_point<battle_unit::i_rigid_body>()->add_contact(u0->template get_interface_point<battle_unit::i_rigid_body>());
		}
	}
	virtual void EndContact(b2Contact* contact) 
	{
		battle_unit::i_battle_base_unit* u0 = (battle_unit::i_battle_base_unit*)(contact->GetFixtureA()->GetBody()->GetUserData());
		battle_unit::i_battle_base_unit* u1 = (battle_unit::i_battle_base_unit*)(contact->GetFixtureB()->GetBody()->GetUserData());
		if (u0 != NULL && u1 != NULL)
		{
			u0->template get_interface_point<battle_unit::i_rigid_body>()->del_contact(u1->template get_interface_point<battle_unit::i_rigid_body>());
			u1->template get_interface_point<battle_unit::i_rigid_body>()->del_contact(u0->template get_interface_point<battle_unit::i_rigid_body>());
		}
	}
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
	}
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}
};
*/
class contact_listener
{
private:
	static void on_contact(interface_rtti* src, interface_rtti* tar)
	{
	}
public:
	
	static cpBool on_collision_begin(cpArbiter *arb, struct cpSpace *space, void* user_data)
	{
		
		return true;
	}
	static cpBool on_collision_presolve(cpArbiter *arb, cpSpace *space, void* user_data)
	{
		return true;
	}
	static void on_collision_postsolve(cpArbiter *arb, cpSpace *space, void* user_data)
	{
	}
	static void on_collision_sSeparate(cpArbiter *arb, cpSpace *space, void* user_data)
	{
	}
};
#endif // !contact_listener_hpp#pragma once
