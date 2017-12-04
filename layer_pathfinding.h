#ifndef layer_pathfinding_hpp
#define layer_pathfinding_hpp
#include <vector>
#include "vector2.h"
#include "battle_map.hpp"
using namespace std;
template<class super>
class layer_navmesh : public super
{
protected:
	cpVect this_frame_destination;
	void optimize_path(std::vector<pathfinding_result*>& the_path)
	{
		if (the_path.empty())
			return;
		battle_map_base* the_map = this->base_map();
		if (the_map->check_segment_distance_to_obstacles(
			this->seeked_target() == NULL ? 0 : this->seeked_target()->id(),
			this->location(), 
			this->destination(), 
			this->radius()))
		{
			the_path.clear();
			return;
		}
		vector<pathfinding_result*>::iterator pos;
		for (vector<pathfinding_result*>::iterator i = the_path.end() - 1; i >= the_path.begin(); --i)
		{
			cpVect d = (*i)->pos;
			if (the_map->check_segment_distance_to_obstacles(
				this->seeked_target() == NULL ? 0 : this->seeked_target()->id(),
				this->location(), 
				d, 
				this->radius()))
			{
				the_path.erase(the_path.begin(), i);
				return;
			}
		}
	}
	cpVect compute_target_pull()
	{
		std::vector<pathfinding_result*>& the_path = this->path();
		optimize_path(the_path);
		if (the_path.empty())
			return this->destination() - this->location();
		battle_map_base* the_map = this->base_map();
		while (!the_path.empty())
		{
			this_frame_destination = the_path[0]->pos;
			if (cpvdistsq(this->location(), this_frame_destination) > 1)
				return this_frame_destination - this->location();
			else
				the_path.erase(the_path.begin());
		}
		return this->destination() - this->location();
	}
};
#endif
