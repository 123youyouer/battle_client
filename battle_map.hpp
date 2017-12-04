#ifndef battle_map_hpp
#define battle_map_hpp
#include <math.h>
#include <vector>
#include "globel.h"
#include "rtti.h"
#include <chipmunk/chipmunk_private.h>
//#include "nav_context.h"
#include "waypoint.h"
class way
{
public:
	std::vector<cpVect> line;
};
class interface_static_obstacle
{
public:
	const static int interface_type = rtti_i_static_obstacle;
public:
	interface_static_obstacle(interface_rtti* rtti)
	{
		rtti->interface_flags.add(rtti_flag_static_obstacle);
		rtti->rtti_point[rtti_i_static_obstacle] = dynamic_cast<interface_static_obstacle*>(this);
	};
};
class obstacle : public interface_rtti, interface_static_obstacle
{
private:
	cpBody* _body;
	cpShape* _shape;
	cpBB this_BB;
	bool naved;
public:
	int x;
	int y;
	int w;
	int h;
	int from_id;
public:
	virtual bool is_static()
	{
		return true;
	}
public:
	inline bool is_naved()
	{
		return naved;
	}
	inline void set_naved(bool is)
	{
		naved = is;
	}
	virtual bool is_sensor()
	{
		return false;
	}
	virtual bool movement_able()
	{
		return false;
	}
	virtual void join_world(int x, int y)
	{
	}
	virtual uint16 collision_type()
	{
		return 0;
	}
	virtual uint16 collision_mask()
	{
		return 0;
	}
	virtual void movement(uint32 millisecond)
	{
		return;
	}
	float CalculatePointToLineDistance(cpVect point, cpVect p1, cpVect p2)
	{
		float dis = 0.f;

		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;

		// ��ֱ�ߴ�ֱ��������ʾ����ת����ʾ
		float k = -((p1.x - point.x)*dx + (p1.y - point.y)*dy) / (dx*dx + dy*dy);
		float footX = k*dx + p1.x;
		float footY = k*dy + p1.y;

		//if�����Ƿ������߶���
		if (footY >= fmin(p1.y, p2.y) && footY <= fmax(p1.y, p2.y)
			&& footX >= fmin(p1.x, p2.x) && footX <= fmax(p1.x, p2.x))
		{
			dis = sqrtf((footX - point.x)*(footX - point.x) + (footY - point.y)*(footY - point.y));
		}
		else
		{
			float dis1 = sqrtf((p1.x - point.x)*(p1.x - point.x) + (p1.y - point.y)*(p1.y - point.y));
			float dis2 = sqrtf((p2.x - point.x)*(p2.x - point.x) + (p2.y - point.y)*(p2.y - point.y));

			dis = (dis1 < dis2 ? dis1 : dis2);
		}

		return dis;
	}
	inline bool intersects_segment(int forgot_id, cpVect p1, cpVect p2)
	{
		if (forgot_id == this->from_id)
			return false;
		return cpBBIntersectsSegment(this_BB, p1, p2);
	}
	inline bool check_distance_to_segment(int forgot_id, cpVect p1, cpVect p2,cpFloat dis)
	{
		if (forgot_id == this->from_id)
			return true;
		if (intersects_segment(forgot_id, p1, p2))
			return false;
		cpVect p;
		for (int i = 0; i < 4; ++i)
		{
			//cpFloat dis = 0;
			switch (i)
			{
			case 0:	p.x = this_BB.l; p.y = this_BB.t; break;
			case 1:	p.x = this_BB.l; p.y = this_BB.b; break;
			case 2:	p.x = this_BB.r; p.y = this_BB.t; break;
			case 3:	p.x = this_BB.r; p.y = this_BB.b; break;
			}
			if (CalculatePointToLineDistance(p, p1, p2) <= dis)
				return false;
		}
		return true;
	}
	virtual ~obstacle()
	{
		if (_body == NULL)
			return;
		cpSpaceRemoveShape(globel::globel_data->this_world, this->_shape);
		cpSpaceRemoveBody(globel::globel_data->this_world, this->_body);
		cpShapeFree(this->_shape);
		cpBodyFree(this->_body);
		_body = NULL;
		_shape = NULL;
	}
	obstacle(int from_id, int x, int y, int w, int h, cpBitmask filter_mask, cpSpace* create_body_in_this_world)
		:interface_static_obstacle(dynamic_cast<interface_rtti*>(this)), naved(false)
	{
		this->from_id = from_id;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		if (create_body_in_this_world != NULL)
		{
			_body = cpBodyNewStatic();
			_body = cpSpaceAddBody(create_body_in_this_world, _body);
			cpBodySetPosition(_body, cpv(x, y));
			_shape = cpBoxShapeNew(_body, w, h, 0);
			_shape->filter.group = 0;
			_shape->filter.categories = collision_type::collision_type_obstacle | filter_mask;
			_shape->filter.mask = collision_type::collision_type_obstacle | filter_mask;
			//_shape->filter.categories = collision_type::collision_type_obstacle | collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
			//_shape->filter.mask = collision_type::collision_type_obstacle | collision_type::collision_type_tank | collision_type::collision_type_ground_soilder;
			_shape = cpSpaceAddShape(create_body_in_this_world, _shape);
		}
		this_BB.l = x - w / 2;
		this_BB.r = x + w / 2;
		this_BB.t = y + h / 2;
		this_BB.b = y - h / 2;
	}
};

//class battle_map_base
//{
//public:
//	int map_type_id;
//protected:
//	cpSpace* this_world;
//	uint32 _w;
//	uint32 _h;
//	uint32 _zhujidi_y_0;
//	uint32 _zhujidi_y_1;
//	uint32 _mid_line_y;
//	bool need_recast;
//	nav_context _nav_context;
//	std::vector<way*> ways;
//	std::vector<obstacle*> obstacles;
//protected:
//	void create_edge(cpSpace* this_world, float x0, float y0, float x1, float y1)
//	{
//	}
//	
//public:
//	inline uint32 w() const { return _w; }
//	inline uint32 h() const { return _h; }
//#pragma region obstacle
//	virtual obstacle* create_obstacle(int x, int y, int w, int h, cpSpace* this_world = NULL)
//	{
//		obstacle* p = new obstacle(x, y, w, h, this_world);
//		_nav_context.add_obstacle(x, y, w, h, p);
//		obstacles.push_back(p);
//		need_recast = true;
//		return p;
//	}
//	virtual void delete_obstacle(obstacle* p)
//	{
//		std::vector<obstacle*>::iterator i = obstacles.begin();
//		while (i != obstacles.end())
//		{
//			if (p == *i)
//			{
//				_nav_context.del_obstacle((*i));
//				i = obstacles.erase(i);				
//				break;
//			}
//			else
//				++i;
//		}
//		need_recast = true;
//	}
//	inline bool intersects_segment(cpVect p1, cpVect p2)
//	{
//		for (int i = 0; i < obstacles.size(); ++i)
//		{
//			if (obstacles[i]->intersects_segment(p1, p2))
//				return true;
//		}
//		return false;
//	}
//	bool check_segment_distance_to_obstacles(cpVect p1, cpVect p2, cpFloat dis)
//	{
//		if (p1 == p2)
//			return true;
//		for (int i = 0; i < obstacles.size(); ++i)
//		{
//			if (!obstacles[i]->check_distance_to_segment(p1, p2, dis))
//				return false;
//		}
//		return true;
//	}
//	virtual void add_obstacle(int* data, int len)
//	{
//		for (int i = 0; i < len; i++)
//		{
//			int x = data[i * 4 + 0];
//			int y = data[i * 4 + 1];
//			int w = data[i * 4 + 2];
//			int h = data[i * 4 + 3];
//			create_obstacle(x, y, w, h, this_world);
//			_mid_line_y = y;
//		}
//		need_recast = true;
//	}
//	bool recast()
//	{
//		if (!need_recast)
//			return false;
//		_nav_context.reset();
//		_nav_context.build_triangle();
//		need_recast = false;
//		return true;
//	}
//	void build_path(cpVect src, cpVect tar, cpFloat min_width, vector<vector2>& out)
//	{
//		_nav_context.find_path(vector2(src.x, src.y), vector2(tar.x, tar.y), min_width, out);
//	}
//#pragma endregion
//	
//	inline bool check_zhujidi_line(cpVect p1, bool to_north)
//	{
//		if (to_north)
//		{
//			return p1.y > _zhujidi_y_0;
//		}
//		else
//		{
//			return p1.y < _zhujidi_y_1;
//		}
//	}
//	virtual int get_way_center_x(cpVect p1) = 0;
//	virtual cpVect get_way_point(cpVect p1, bool to_north)
//	{
//		way* selected_way = NULL;
//		int dis = 100000;
//		for (int i = 0; i < ways.size(); ++i)
//		{
//			int this_dis = abs(ways[i]->line[0].x - p1.x);
//			if (this_dis < dis)
//			{
//				dis = this_dis;
//				selected_way = ways[i];
//			}
//		}
//		if (selected_way == NULL)
//			return cpvzero;
//		dis = 100000;
//		if (to_north)
//		{
//			for (int i = 0; i < selected_way->line.size(); ++i)
//			{
//				if (selected_way->line[i].y >(p1.y + 40))
//				{
//					return selected_way->line[i];
//				}
//			}
//			return cpvzero;
//		}
//		else
//		{
//			for (int i = selected_way->line.size() - 1; i >= 0; --i)
//			{
//				if (selected_way->line[i].y < (p1.y - 40))
//				{
//					return selected_way->line[i];
//				}
//			}
//			return cpvzero;
//		}
//
//	}
//	virtual cpVect get_way_point_fly(cpVect p1, bool to_north)
//	{
//		if (to_north)
//			return cpv(p1.x, p1.y + 5);
//		else
//			return cpv(p1.x, p1.y - 5);
//	}
//	virtual void add_path(int* data, int len)
//	{
//		way* new_way = new way();
//		for (int i = 0; i < len; i++)
//		{
//			int x = data[i * 2 + 0];
//			int y = data[i * 2 + 1];
//			new_way->line.push_back(cpv(x, y));
//		}
//		ways.push_back(new_way);
//	}
//	virtual void init_map(cpSpace* this_world)
//	{
//		this->this_world = this_world;
//		create_edge(this_world, 0, 0, 0, h());
//		create_edge(this_world, 0, h(), w(), h());
//		create_edge(this_world, w(), h(), w(), 0);
//		create_edge(this_world, w(), 0, 0, 0);
//	}
//	
//	battle_map_base(uint32 w, uint32 h) :
//		_w(w),
//		_h(h),
//		_zhujidi_y_0(0),
//		_zhujidi_y_1(0),
//		_nav_context(w, h),
//		need_recast(false){}
//};

class battle_map_base
{
public:
	int map_type_id;
	bool need_reset_cached_path;
protected:
	cpSpace* this_world;
	uint32 _w;
	uint32 _h;
	uint32 _zhujidi_y_0;
	uint32 _zhujidi_y_1;
	uint32 _mid_line_y;
	std::vector<way*> ways;
	std::vector<obstacle*> obstacles;
public:
	waypoint_pathfinding pathfinding;
protected:
	void create_edge(cpSpace* this_world, float x0, float y0, float x1, float y1)
	{
	}
	
public:
	inline uint32 w() const { return _w; }
	inline uint32 h() const { return _h; }
#pragma region obstacle
	virtual obstacle* create_obstacle(int x, int y, int w, int h, int id, cpBitmask filter_mask, cpSpace* this_world = NULL)
	{
		need_reset_cached_path = true;
		obstacle* p = new obstacle(id, x, y, w, h, filter_mask, this_world);
		pathfinding.add_obstacle(x, y, w, h, id);
		obstacles.push_back(p);
		return p;
	}
	virtual void delete_obstacle(obstacle* p)
	{
		need_reset_cached_path = true;
		std::vector<obstacle*>::iterator i = obstacles.begin();
		while (i != obstacles.end())
		{
			if (p == *i)
			{
				pathfinding.del_obstacle((*i)->x, (*i)->y, (*i)->w, (*i)->h);
				i = obstacles.erase(i);				
				break;
			}
			else
				++i;
		}
	}
	inline bool intersects_segment(int forgot_id, cpVect p1, cpVect p2)
	{
		for (int i = 0; i < obstacles.size(); ++i)
		{
			if (obstacles[i]->intersects_segment(forgot_id, p1, p2))
				return true;
		}
		return false;
	}
	bool check_segment_distance_to_obstacles(int forgot_id, cpVect p1, cpVect p2, cpFloat dis)
	{
		if (p1 == p2)
			return true;
		for (int i = 0; i < obstacles.size(); ++i)
		{
			if (!obstacles[i]->check_distance_to_segment(forgot_id, p1, p2, dis))
				return false;
		}
		return true;
	}
	virtual void add_obstacle(int* data, int len)
	{
		for (int i = 0; i < len; i++)
		{
			int x = data[i * 4 + 0];
			int y = data[i * 4 + 1];
			int w = data[i * 4 + 2];
			int h = data[i * 4 + 3];
			create_obstacle(x, y, w, h, pathfinding_result::pass_type_terrain, collision_type::collision_type_terrain, this_world);
			_mid_line_y = y;
		}
	}
	void build_path(cpVect src, cpVect tar, int target_id, vector<pathfinding_result*>& out)
	{
		pathfinding.find_path(src, tar, out, target_id);
	}
#pragma endregion
	
	inline bool check_zhujidi_line(cpVect p1, bool to_north)
	{
		if (to_north)
		{
			return p1.y > _zhujidi_y_0;
		}
		else
		{
			return p1.y < _zhujidi_y_1;
		}
	}
	virtual int get_way_center_x(cpVect p1) = 0;
	virtual cpVect get_way_point(cpVect p1, bool to_north, bool is_soilder)
	{
		way* selected_way = NULL;
		int dis = 100000;
		for (int i = 0; i < ways.size(); ++i)
		{
			int this_dis = fabs(ways[i]->line[0].x - p1.x);
			if (this_dis < dis)
			{
				dis = this_dis;
				selected_way = ways[i];
			}
		}
		if (selected_way == NULL)
			return cpvzero;
		dis = 100000;
		if (to_north)
		{
			for (int i = 0; i < selected_way->line.size(); ++i)
			{
				if (selected_way->line[i].y >(p1.y + 40))
				{
					return selected_way->line[i];
				}
			}
			return cpvzero;
		}
		else
		{
			for (int i = selected_way->line.size() - 1; i >= 0; --i)
			{
				if (selected_way->line[i].y < (p1.y - 40))
				{
					return selected_way->line[i];
				}
			}
			return cpvzero;
		}

	}
	virtual cpVect get_way_point_fly(cpVect p1, bool to_north)
	{
		if (to_north)
			return cpv(p1.x, p1.y + 5);
		else
			return cpv(p1.x, p1.y - 5);
	}
	virtual void add_path(int* data, int len)
	{
		way* new_way = new way();
		for (int i = 0; i < len; i++)
		{
			int x = data[i * 2 + 0];
			int y = data[i * 2 + 1];
			new_way->line.push_back(cpv(x, y));
		}
		ways.push_back(new_way);
	}
	virtual void init_map(cpSpace* this_world)
	{
		this->this_world = this_world;
		create_edge(this_world, 0, 0, 0, h());
		create_edge(this_world, 0, h(), w(), h());
		create_edge(this_world, w(), h(), w(), 0);
		create_edge(this_world, w(), 0, 0, 0);
	}
	
	battle_map_base(uint32 w, uint32 h) :
		_w(w),
		_h(h),
		_zhujidi_y_0(635),
		_zhujidi_y_1(230),
		pathfinding(w, h),
		need_reset_cached_path(false)
	{}
};
class battle_map_oneway : public battle_map_base
{
protected:
	int x1;
	int x2;
	int offset;
public:
	battle_map_oneway(uint32 w, uint32 h) :battle_map_base(w, h), offset(10)
	{
	}
	virtual int get_way_center_x(cpVect p1)
	{
		
		if (p1.x < ((x1 + x2) / 2))
		{
			return p1.x + offset;
		}
		else
		{
			return p1.x - offset;
		}
		if (p1.x<x2&&p1.x>x1)
		{
			return p1.x;
		}
		else if (p1.x <= x1)
		{
			return p1.x + offset;
		}
		else
		{
			return p1.x - offset;
		}
	}
	virtual cpVect get_way_point(cpVect p1, bool to_north, bool is_soilder)
	{
		cpVect res = cpvzero;
		if (to_north)
		{
			if (p1.x<=x2&& p1.x>=x1)
			{
				res.x = p1.x;
				res.y = is_soilder ? p1.y + offset : h() - 1;
			}
			else if (p1.x < x1)
			{
				res.x = (x1 + x2) / 2;
				res.y = is_soilder ? p1.y + offset : h() - 1;
			}
			else
			{
				res.x = (x1 + x2) / 2;
				res.y = is_soilder ? p1.y + offset : h() - 1;
			}
		}
		else
		{
			if (p1.x<=x2&&p1.x>=x1)
			{
				res.x = p1.x;
				res.y = is_soilder ? p1.y - offset : 1;
			}
			else if (p1.x < x1)
			{
				res.x = (x1 + x2) / 2;
				res.y = is_soilder ? p1.y - offset : 1;;
			}
			else
			{
				res.x = (x1 + x2) / 2;
				res.y = is_soilder ? p1.y - offset : 1;;
			}
		}
		if (this->check_zhujidi_line(p1, to_north))
		{
			res.x = this->w() / 2;
		}
		return res;
	}
	virtual void add_path(int* data, int len)
	{
		if (len != 2)
			return;
		x1 = data[0];
		x2 = data[1];
	}
};
class battle_map_twoway : public battle_map_base
{
protected:
	int x01;
	int x02;
	int x11;
	int x12;
	int offset;
	cpVect get_way_point(cpVect p1, bool to_north, int x1, int x2)
	{
		cpVect res = cpvzero;
		if (to_north)
		{
			if (p1.x<=x2&&p1.x>=x1)
			{
				res.x = p1.x;
				res.y = p1.y + offset;
			}
			else if (p1.x < x1)
			{
				res.x = p1.x + offset;;
				res.y = p1.y + offset;
			}
			else
			{
				res.x = p1.x - offset;;
				res.y = p1.y + offset;
			}
		}
		else
		{
			if (p1.x<=x2&&p1.x>=x1)
			{
				res.x = p1.x;
				res.y = p1.y - offset;
			}
			else if (p1.x < x1)
			{
				res.x = p1.x + offset;
				res.y = p1.y - offset;
			}
			else
			{
				res.x = p1.x - offset;
				res.y = p1.y - offset;
			}
		}
		if (this->check_zhujidi_line(p1, to_north))
		{
			res.x = this->w() / 2;
		}
		return res;
	}
	virtual int get_way_center_x(cpVect p1, int x1, int x2)
	{
		if (p1.x < ((x1 + x2) / 2))
		{
			return p1.x + offset;
		}
		else
		{
			return p1.x - offset;
		}
		if (p1.x<x2&&p1.x>x1)
		{
			return p1.x;
		}
		else if (p1.x <= x1)
		{
			return p1.x + offset;
		}
		else
		{
			return p1.x - offset;
		}
	}
public:
	battle_map_twoway(uint32 w, uint32 h) :battle_map_base(w, h), offset(10)
	{
	}
	virtual int get_way_center_x(cpVect p1)
	{
		if (p1.x < w() / 2)
		{
			return get_way_center_x(p1, x01, x02);
		}
		else
		{
			return get_way_center_x(p1, x11, x12);
		}
	}
	virtual cpVect get_way_point(cpVect p1, bool to_north, bool is_soilder)
	{
		if (!is_soilder)
		{
			if (to_north)
			{
				p1.y = is_soilder ? this->_mid_line_y : h() - 1;
				p1.x = p1.x < w() / 2 ? w() / 4 : w() * 3 / 4;
				return p1;
			}
			else
			{
				p1.y = is_soilder ? this->_mid_line_y : 1;
				p1.x = p1.x < w() / 2 ? w() / 4 : w() * 3 / 4;
				return p1;
			}
		}
		else if (p1.x < w() / 2)
		{
			return get_way_point(p1, to_north, x01, x02);
		}
		else
		{
			return get_way_point(p1, to_north, x11, x12);
		}
	}
	virtual void add_path(int* data, int len)
	{
		if (len != 4)
			return;
		x01 = data[0];
		x02 = data[1];
		x11 = data[2];
		x12 = data[3];
	}
};
/*
class battle_map_bak1 : public battle_map_base
{
private:
std::vector<way*> ways;
private:
void create_ways()
{
for (int i = 1; i < 10; ++i)
{
way* new_way = new way();
new_way->line.push_back(b2Vec2(54 * i, 160));
new_way->line.push_back(b2Vec2(54 * i, 320));
new_way->line.push_back(b2Vec2(54 * i, 480));
new_way->line.push_back(b2Vec2(54 * i, 640));
new_way->line.push_back(b2Vec2(54 * i, 800));
ways.push_back(new_way);
}
}
inline const way& choose_way(const b2Vec2& p1)
{
int i = p1.x / 54;
if (i >= ways.size())
return *(ways[ways.size() - 1]);
else
return *(ways[i]);
}
public:
battle_map_bak1(int w,int h)
:battle_map_base(w, h)
{

}
virtual const b2Vec2& get_way_point(const b2Vec2& p1, bool to_north)
{
const way& cur_way = choose_way(p1);
if (to_north)
{
return cur_way.line[cur_way.line.size() - 1];
}
else
{
return cur_way.line[0];
}
}
virtual void init_map(b2World* this_world)
{
create_ways();
create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);
}
};
class battle_map_1 : public battle_map_base
{
protected:
std::vector<way*> ways;
protected:
void create_ways()
{
way* way1 = new way();
way1->line.push_back(b2Vec2(110, 50));
way1->line.push_back(b2Vec2(110, 405));
way1->line.push_back(b2Vec2(110, 535));
way1->line.push_back(b2Vec2(110, 820));
ways.push_back(way1);

way1 = new way();
way1->line.push_back(b2Vec2(450, 50));
way1->line.push_back(b2Vec2(450, 405));
way1->line.push_back(b2Vec2(450, 535));
way1->line.push_back(b2Vec2(450, 820));
ways.push_back(way1);
}
inline const way& choose_way(const b2Vec2& p1)
{
if (p1.x < (w() / 2))
return *ways[0];
else
return *ways[1];
}
public:
battle_map_1(int w, int h)
:battle_map_base(w, h)
{
map_type_id = 0;
}
virtual const b2Vec2& get_way_point(const b2Vec2& p1, bool to_north)
{
const way& cur_way = choose_way(p1);
if (to_north)
{
for (int i = 0; i < cur_way.line.size(); i++)
{
if (p1.y < cur_way.line[i].y)
return cur_way.line[i];
}
return cur_way.line[cur_way.line.size() - 1];
}
else
{
for (int i = 0; i < cur_way.line.size(); i++)
{
if (p1.y > cur_way.line[i].y)
return cur_way.line[i];
}
return cur_way.line[0];
}
}

virtual void init_map(b2World* this_world)
{
create_ways();
create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(20, 470, 20, 60, this_world);
create_obstacle(250, 470, 100, 60, this_world);
create_obstacle(520, 470, 20, 60, this_world);
}
};
class battle_map_3 : public battle_map_1
{
public:
battle_map_3(int w, int h)
:battle_map_1(w, h)
{

}
virtual void init_map(b2World* this_world)
{
create_ways();
create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(20, 592, 20, 60, this_world);
create_obstacle(250, 592, 100, 60, this_world);
create_obstacle(520, 592, 20, 60, this_world);
}
};
class battle_map_5 : public battle_map_1
{
public:
battle_map_5(int w, int h)
:battle_map_1(w, h)
{

}
virtual void init_map(b2World* this_world)
{
create_ways();
create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(20, 348, 20, 60, this_world);
create_obstacle(250, 348, 100, 60, this_world);
create_obstacle(520, 348, 20, 60, this_world);
}
};
class battle_map_0 : public battle_map_base
{
protected:
b2Vec2 p01;
b2Vec2 p02;
b2Vec2 tmp;
way my_way;
const b2Vec2& get_way_point_n(const b2Vec2& p1)
{
if ((p1.x > (p01.x + 100)) && (p1.x < (p02.x - 100)))
{
tmp.x = p1.x;
tmp.y = p1.y + 10;
return tmp;
}
else
{
for (int i = 0; i < my_way.line.size(); i++)
{
if (p1.y < my_way.line[i].y)
return my_way.line[i];
}
return my_way.line[my_way.line.size() - 1];
}
}
const b2Vec2& get_way_point_s(const b2Vec2& p1)
{
if ((p1.x > (p01.x + 100)) && (p1.x < (p02.x - 100)))
{
tmp.x = p1.x;
tmp.y = p1.y - 10;
return tmp;
}
else
{
for (int i = 0; i < my_way.line.size(); i++)
{
if (p1.y > my_way.line[i].y)
return my_way.line[i];
}
return my_way.line[0];
}
}
void create_ways()
{
my_way.line.push_back(b2Vec2(270, 50));
my_way.line.push_back(b2Vec2(270, 405));
my_way.line.push_back(b2Vec2(270, 535));
my_way.line.push_back(b2Vec2(270, 820));
}
public:
battle_map_0(int w, int h)
:battle_map_base(w, h)
{
p01.x = 63;
p01.y = 343;
p02.x = 477;
p02.y = 517;
}
virtual const b2Vec2& get_way_point(const b2Vec2& p1, bool to_north)
{
if (to_north)
return get_way_point_n(p1);
else
return get_way_point_s(p1);
}
virtual void init_map(b2World* this_world)
{
create_ways();

create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(31, 470, 32, 60, this_world);
create_obstacle(508, 470, 32, 60, this_world);
}
};
class battle_map_2 : public battle_map_0
{
public:
battle_map_2(int w, int h)
:battle_map_0(w, h)
{

}
virtual void init_map(b2World* this_world)
{
create_ways();

create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(31, 592, 32, 60, this_world);
create_obstacle(508, 592, 32, 60, this_world);
}
};
class battle_map_4 : public battle_map_0
{
public:
battle_map_4(int w, int h)
:battle_map_0(w, h)
{

}
virtual void init_map(b2World* this_world)
{
create_ways();

create_edge(this_world, 0, 0, 0, h());
create_edge(this_world, 0, h(), w(), h());
create_edge(this_world, w(), h(), w(), 0);
create_edge(this_world, w(), 0, 0, 0);

create_obstacle(31, 348, 32, 60, this_world);
create_obstacle(508, 348, 32, 60, this_world);
}
};
*/
#endif