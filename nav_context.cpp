#include "nav_context.h"
#include "polygon.h"
#include "delaunay.h"
#include "triangle.h"
#include "cell.h"
#include "navmesh.h"
nav_context::nav_context(int w, int h)
	:m_out_polygon(new polygon(vector<vector2>())), width(w), height(h), radius(50)
{
	vector<vector2> vertexs;
	vertexs.push_back(vector2(0, 5));
	//vertexs.push_back(vector2(0, 0));
	vertexs.push_back(vector2(5, 0));
	vertexs.push_back(vector2(w-5, 0));
	//vertexs.push_back(vector2(w, 0));
	vertexs.push_back(vector2(w, 5));
	vertexs.push_back(vector2(w, h - 5));
	//vertexs.push_back(vector2(w, h));
	vertexs.push_back(vector2(w-5, h));
	vertexs.push_back(vector2(5, h));
	vertexs.push_back(vector2(0, h - 5));
	//vertexs.push_back(vector2(0, h));
	m_out_polygon->set_vertex(vertexs);
	
}
void nav_context::build_triangle()
{
	if (m_out_polygon != NULL)
	{
		m_polygons.insert(m_polygons.begin(), m_out_polygon);
	}
	m_lines.clear();
	delaunay d(m_polygons);
	if (!d.create_delaunay(m_triangles))
	{
		return;
	}

	for (int i = 0; i < m_triangles.size(); ++i)
	{
		const vector2 &cur_A = m_triangles[i]->get_vertex_by_index(0);
		const vector2 &cur_B = m_triangles[i]->get_vertex_by_index(1);
		const vector2 &cur_C = m_triangles[i]->get_vertex_by_index(2);

		cell *cur_cell = new cell(cur_A, cur_B, cur_C);
		cur_cell->setindex(i);
		cur_cell->set_flag(m_triangles[i]->get_flag());
		m_cells.push_back(cur_cell);
	}

	link_cells();

	if (m_out_polygon != NULL)
	{
		m_polygons.erase(m_polygons.begin());
	}
}
void nav_context::link_cells()
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		for (int j = 0; j < m_cells.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}
			m_cells[i]->check_and_link(*m_cells[j]);
		}
	}
}
nav_context::~nav_context()
{
	reset();
	while (!m_polygons.empty())
	{
		delete m_polygons[0];
		m_polygons.erase(m_polygons.begin());
	}
}
void nav_context::reset()
{
	m_lines.clear();

	while (!m_triangles.empty())
	{
		delete m_triangles[0];
		m_triangles.erase(m_triangles.begin());
	}
	while (!m_cells.empty())
	{
		delete m_cells[0];
		m_cells.erase(m_cells.begin());
	}
}
void nav_context::del_obstacle(void* user_data)
{
	for (vector<polygon*>::iterator i = m_polygons.begin(); i < m_polygons.end(); ++i)
	{
		if (user_data == (*i)->user_data)
		{
			delete (*i);
			m_polygons.erase(i);			
			return;
		}
	}
}
void nav_context::add_obstacle(int x, int y, int w, int h, void* user_data)
{	
	polygon* p = new polygon();
	vector2 lt(x - w / 2 > 0 ? x - w / 2 : 0, y + h / 2 < height ? y + h / 2 : height);
	vector2 rt(x + w / 2 < width ? x + w / 2 : width, lt.m_y);
	vector2 lb(lt.m_x, y - h / 2 > 0 ? y - h / 2 : 0);
	vector2 rb(rt.m_x, lb.m_y);

	p->add_vector2(lb.m_x, lb.m_y);
	p->add_vector2(rb.m_x, rb.m_y);
	p->add_vector2(rt.m_x, rt.m_y);
	p->add_vector2(lt.m_x, lt.m_y);
	
	p->user_data = user_data;
	
	//p->add_vector2(lt.m_x, lt.m_y);

	m_polygons.push_back(p);
}
void nav_context::find_path(vector2 start, vector2 end, float unit_width, vector<vector2>& out)
{
	navmesh nav(m_cells);
	if (!nav.find_path(start, end, unit_width, out))
		//if (!nav.find_path(start, end, 0, out))
		out.clear();
	reset_cell();
}
void nav_context::reset_cell()
{
	for (int i = 0; i < m_cells.size(); ++i)
	{
		m_cells[i]->reset_cell();
	}
}