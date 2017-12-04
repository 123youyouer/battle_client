#ifndef collision_snapshot_h
#define collision_snapshot_h
#include "unit_body.hpp"

class collision_force
{
public:
	unit_body* source;
	vec_2d value;
};
class collision_snapshot
{
public:
	unit_body* a;
	unit_body* b;
	vec_2d force_from_engine;
	vec_2d force_from_collision;
	bool pushed;
public:
	bool is_faraway();
	void apply();
	void transmit(const collision_force& f);
	collision_snapshot(unit_body* u1, unit_body* u2);
};
class collision_manager
{
private:
	static const int all_collision_snapshot_size = 1024;
public:
	static collision_manager* static_collision_manager;
	collision_snapshot* all_collision_snapshot[all_collision_snapshot_size];
	void transmit(unit_body* body, const collision_force& f);
	void apply();
};
#endif // !collision_snapshot_h

