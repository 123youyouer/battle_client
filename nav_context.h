#ifndef nav_context_h
#define nav_context_h
#include <vector>
#include "vector2.h"
using namespace std;

class polygon;
class triangle;
class cell;
class nav_context
{
public:
	int width;
	int height;
	int radius;
public:
	polygon* 				m_out_polygon;
	vector<polygon*>  		m_polygons;
	vector<vector2>   		m_lines;
	vector<triangle*> 		m_triangles;
	vector<cell*>     		m_cells;
protected:
	void link_cells();
	void reset_cell();
public:
	nav_context(int w, int h);
	virtual ~nav_context();
	void build_triangle();
	void find_path(vector2 start, vector2 end, float unit_width, vector<vector2>& out);
	void add_obstacle(int x, int y, int w, int h, void* user_data);
	void del_obstacle(void* user_data);
	void reset();
};
#endif // !nav_context_h

