#ifndef waypoint_h
#define waypoint_h
#include <vector>
#include "globel.h"
#include <chipmunk/chipmunk_private.h>
#include <algorithm>
#include "way_base.h"
using namespace std;
class grid_way_point : public pathfinding_result
{
public:
	static const int pass_type_terrain = -2;
	static const int pass_type_waypoint = -1;
	static const int pass_type_passable = 0;
	static const int pass_type_outside = -999;
private:
	grid_way_point* prev;
	int h;
	int g;
	bool used;
	bool open;
	int pass_type;
public:
	int x;
	int y;	
public:
	inline int get_h()
	{
		return h;
	}
	inline void set_g(int v)
	{
		g = v;
	}
	inline int get_g()
	{
		return g;
	}
	inline void compute_h(grid_way_point* wp)
	{
		h = (wp->x - x)*(wp->x - x) + (wp->y - y)*(wp->y - y);
	}
	grid_way_point* get_prev()
	{
		return prev;
	}
	inline void set_prev(grid_way_point* prev)
	{
		this->prev = prev;
	}
	inline bool get_open()
	{
		return open;
	}
	inline void set_open()
	{
		open = true;
	}
	inline int get_f()
	{
		return g + h;
	}
	inline void set_pass_type(int pass_type)
	{
		this->pass_type = pass_type;
	}
	inline int get_pass_type()
	{
		return pass_type;
	}
	inline bool get_used()
	{
		return used;
	}
	inline void set_used()
	{
		used = true;
	}
	inline void reset_pathfinding_data()
	{
		h = 0; g = 0;
		prev = NULL;
		open = false;
		used = false;
	}
	inline void reset_all()
	{
		reset_pathfinding_data();
		pass_type = 0;
	}

	grid_way_point(int index_x, int index_y, int pixel_per_cell, int x_offset, int y_offset)
	{
		x = index_x;
		y = index_y;
		pos.x = x*pixel_per_cell + x_offset;
		pos.y = y*pixel_per_cell + y_offset;
		reset_all();
	}
};
class grid
{
public:
	static const uint32 neighbor_left = 0;
	static const uint32 neighbor_top = 1;
	static const uint32 neighbor_right = 2;
	static const uint32 neighbor_bottom = 3;
private:
	vector<grid_way_point*> buf;
public:
	uint32 _w;
	uint32 _h;
	uint32 pixel_per_cell;
public:
	
	grid(uint32 w, uint32 h, uint32 pixel_per_cell = 40, uint32 x_offset = 30, uint32 y_offset = 35)
	{
		this->_w = w;
		this->_h = h;
		this->pixel_per_cell = pixel_per_cell;
		buf.resize(w*h);
		for(int x=0;x<w;++x)
			for (int y = 0; y < h; ++y)
			{
				grid_way_point* p = new grid_way_point(x,y, pixel_per_cell, x_offset, y_offset);
				p->set_pass_type(0);
				buf[y*_w + x] = p;
			}
	}
	~grid()
	{
		for (int i = 0; i < buf.size(); ++i)
			delete buf[i];
		buf.clear();
	}
	grid_way_point* get_waypoint(uint32 x, uint32 y)
	{
		if (x < this->_w && x >= 0 && y < this->_h && y >= 0)
		{
			return buf[y*_w + x];
		}
		return NULL;
	}
	int get_waypoint_value(uint32 x, uint32 y)
	{
		if (x < this->_w && x >= 0 && y < this->_h && y >= 0)
		{
			return buf[y*_w + x]->get_pass_type();
		}
		return grid_way_point::pass_type_outside;
	}
	void get_location(uint32 x, uint32 y, uint32& loc_x, uint32& loc_y)
	{
		loc_x = x*pixel_per_cell + pixel_per_cell / 2;
		loc_y = y*pixel_per_cell + pixel_per_cell / 2;
	}
	void reset_pathfinding_cache()
	{
		for (int i = 0; i < _w*_h; ++i)
		{
			buf[i]->reset_pathfinding_data();
		}
	}
	void reset_all()
	{
		for (int i = 0; i < _w*_h; ++i)
		{
			buf[i]->reset_all();
		}
	}
	grid_way_point* get_neighbor(grid_way_point* base,int side)
	{
		switch (side)
		{
		case neighbor_left:		return get_waypoint(base->x - 1, base->y);
		case neighbor_top:		return get_waypoint(base->x, base->y + 1);
		case neighbor_right:	return get_waypoint(base->x + 1, base->y);
		case neighbor_bottom:	return get_waypoint(base->x, base->y - 1);
		default:return NULL;
		}
		return NULL;
	}
};
//extern bool compare_waypoint_for_pathfinding(grid_way_point *left, grid_way_point *right);
class waypoint_pathfinding
{
protected:
	
private:
	grid base_grid;
	vector<grid_way_point*> open_list;
	vector<grid_way_point*> closed_list;
	int x_side_offset;
	int y_side_offset;
private:
	grid_way_point* find_near_waypoint(const cpVect& pos);
	void handle_node(grid_way_point* src_wp, grid_way_point* tar_wp, grid_way_point* cur_node, uint32 target_id);
	bool build_path(vector<pathfinding_result*>& out);
public:
	waypoint_pathfinding(int width, int height)
		:base_grid(13, 21, 40, 30, 35), x_side_offset(30), y_side_offset(35)
	{
	}
	int get_grid_cell(int x, int y)
	{
		return base_grid.get_waypoint_value(x, y);
	}
public:
	cpVect get_pos(grid_way_point* wp);
	void add_obstacle(float x, float y, int w, int h, int target_id);
	void del_obstacle(float x, float y, int w, int h);
	bool find_path(const cpVect& start, const cpVect& end, vector<pathfinding_result*>& out, int target_id);
};
#endif
