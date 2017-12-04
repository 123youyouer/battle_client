#ifndef way_base_h
#define way_base_h
#include <chipmunk/chipmunk_private.h>
class pathfinding_result
{
public:
	static const int pass_type_terrain = -2;
	static const int pass_type_waypoint = -1;
	static const int pass_type_passable = 0;
public:
	cpVect pos;
};
#endif // !way_base_h

