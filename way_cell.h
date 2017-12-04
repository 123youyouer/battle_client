#ifndef waycell_h
#define waycell_h
#include <vector>
#include "globel.h"
#include <chipmunk/chipmunk_private.h>
#include <algorithm>
#include "way_base.h"
using namespace std;
class way_cell : public pathfinding_result
{
private:
	way_cell* prev;
	int h;
	int g;
	bool used;
	bool open;
	int pass_type;
	int width;
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
	inline void compute_h(way_cell* wp)
	{
		h = abs(wp->x - x) + abs(wp->y - y);
	}
	way_cell* get_prev()
	{
		return prev;
	}
	inline void set_prev(way_cell* prev)
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
	
	way_cell(int index_x, int index_y,int pixel_per_cell, int x_offset, int y_offset)
	{
		x = index_x;
		y = index_y;
		pos.x = x*pixel_per_cell + pixel_per_cell / 2 + x_offset;
		pos.y = y*pixel_per_cell + pixel_per_cell / 2 + y_offset;
		reset_all();
	}
};
class way_grid
{
public:
	static const uint32 neighbor_left = 0;
	static const uint32 neighbor_top = 1;
	static const uint32 neighbor_right = 2;
	static const uint32 neighbor_bottom = 3;
private:
	vector<way_cell*> buf;
public:
	uint32 _w;
	uint32 _h;
	uint32 pixel_per_cell;
public:
	way_grid(uint32 w, uint32 h, uint32 pixel_per_cell = 40, uint32 x_offset = 30, uint32 y_offset = 35)
	{
		this->_w = w;
		this->_h = h;
		this->pixel_per_cell = pixel_per_cell;
		buf.resize(w*h);
		for (int x = 0; x < w; ++x)
			for (int y = 0; y < h; ++y)
			{
				way_cell* p = new way_cell(x, y, pixel_per_cell, x_offset, y_offset);
				p->x = x;
				p->y = y;
				p->set_pass_type(0);
				buf[y*_w + x] = p;
			}
	}
	~way_grid()
	{
		for (int i = 0; i < buf.size(); ++i)
			delete buf[i];
		buf.clear();
	}
	way_cell* get_way_cell(uint32 x, uint32 y)
	{
		if (x < this->_w && x >= 0 && y < this->_h && y >= 0)
		{
			return buf[y*_w + x];
		}
		return NULL;
	}
	int get_way_cell_value(uint32 x, uint32 y)
	{
		if (x < this->_w && x >= 0 && y < this->_h && y >= 0)
		{
			return buf[y*_w + x]->get_pass_type();
		}
		return false;
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
	inline float pos_x0(way_cell* wp,int x_side_offset)
	{
		return wp->x*pixel_per_cell + x_side_offset;
	}
	inline float pos_x1(way_cell* wp, int x_side_offset)
	{
		return wp->x*pixel_per_cell + pixel_per_cell + x_side_offset;
	}
	inline float pos_y0(way_cell* wp, int y_side_offset)
	{
		return wp->y*pixel_per_cell + y_side_offset;
	}
	inline float pos_y1(way_cell* wp, int y_side_offset)
	{
		return wp->y*pixel_per_cell + pixel_per_cell + y_side_offset;
	}
	way_cell* get_neighbor(way_cell* base, int side)
	{
		switch (side)
		{
		case neighbor_left:		return get_way_cell(base->x - 1, base->y);
		case neighbor_top:		return get_way_cell(base->x, base->y + 1);
		case neighbor_right:	return get_way_cell(base->x + 1, base->y);
		case neighbor_bottom:	return get_way_cell(base->x, base->y - 1);
		default:return NULL;
		}
		return NULL;
	}
};
class way_cell_pathfinding
{
private:
	way_grid base_grid;
	vector<way_cell*> open_list;
	vector<way_cell*> closed_list;
	int x_side_offset;
	int y_side_offset;
private:
	way_cell* find_near_waypoint(const cpVect& pos);
	void handle_node(way_cell* src_wp, way_cell* tar_wp, way_cell* cur_node, uint32 target_id);
	bool build_path(vector<pathfinding_result*>& out);
public:
	way_cell_pathfinding(int width, int height)
		:base_grid(12, 20), x_side_offset(30), y_side_offset(35)
	{
	}
public:
	cpVect get_pos(way_cell* wp);
	void add_obstacle(float x, float y, int w, int h, int target_id);
	void del_obstacle(float x, float y, int w, int h);
	bool find_path(const cpVect& start, const cpVect& end, vector<pathfinding_result*>& out, int target_id);
};
#endif
