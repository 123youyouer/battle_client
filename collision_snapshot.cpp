#include "collision_snapshot.h"

collision_snapshot::collision_snapshot(unit_body* u1, unit_body* u2) :pushed(false)
{
	a = u1;
	b = u2;
	force_from_engine = a->speed;
}
bool collision_snapshot::is_faraway()
{
	return false;
}
void collision_snapshot::apply()
{
	if (pushed)
		return;
	if (is_faraway())
		return;
	collision_force new_f;
	new_f.source = a;
	new_f.value = b->point - a->point;
	new_f.value.normalize();
	new_f.value *= a->max_speed;
	pushed = true;
	collision_manager::static_collision_manager->transmit(b, new_f);
}
void collision_snapshot::transmit(const collision_force& f)
{
	if (pushed)
		return;
	if (f.source == b)
		return;
	force_from_collision += f.value;
	force_from_collision.normalize();
	force_from_collision *= a->max_speed;
	if (is_faraway())
		return;
	collision_force new_f;
	new_f.source = f.source;
	new_f.value = b->point - a->point;
	new_f.value.normalize();
	new_f.value *= a->max_speed;
	pushed = true;
	collision_manager::static_collision_manager->transmit(b, new_f);
}


collision_manager* collision_manager::static_collision_manager = new collision_manager();

void collision_manager::apply()
{
	for (int i = 0; i < all_collision_snapshot_size; ++i)
	{
		if (all_collision_snapshot[i] == NULL)
			continue;
		all_collision_snapshot[i]->apply();
	}
}
void collision_manager::transmit(unit_body* body, const collision_force& f)
{
	for (int i = 0; i < all_collision_snapshot_size; ++i)
	{
		if (all_collision_snapshot[i] == NULL)
			continue;
		if (all_collision_snapshot[i]->a != body)
			continue;
		all_collision_snapshot[i]->transmit(f);
	}
}