#include "way_cell.h"
bool compare_way_cell_for_pathfinding(way_cell *left, way_cell *right)
{
	return left->get_f() > right->get_f();
}
way_cell* way_cell_pathfinding::find_near_waypoint(const cpVect& pos)
{
	int x = 0;
	if (pos.x < x_side_offset)
		x = 0;
	else if (pos.x > x_side_offset + (base_grid._w)*base_grid.pixel_per_cell)
		x = base_grid._w - 1;
	else
		x = (int)(pos.x - x_side_offset) / base_grid.pixel_per_cell;

	int y = 0;
	if (pos.y < y_side_offset)
		y = 0;
	else if (pos.y > y_side_offset + (base_grid._h)*base_grid.pixel_per_cell)
		y = base_grid._h - 1;
	else
		y = (int)(pos.y - y_side_offset) / base_grid.pixel_per_cell;

	return base_grid.get_way_cell(x, y);
}
void way_cell_pathfinding::handle_node(way_cell* src_wp, way_cell* tar_wp, way_cell* cur_node, uint32 target_id)
{
	for (int i = 0; i < 4; ++i)
	{
		way_cell* adjacent_node = base_grid.get_neighbor(cur_node, i);
		if (adjacent_node == NULL)
			continue;
		if ((adjacent_node->get_pass_type() != 0)
			&& (adjacent_node->get_pass_type() != target_id))
			continue;
		if (!adjacent_node->get_used())
		{
			adjacent_node->set_used();
			adjacent_node->set_open();
			adjacent_node->set_prev(cur_node);
			adjacent_node->compute_h(src_wp);
			adjacent_node->set_g(cur_node->get_g() + 1);
			open_list.push_back(adjacent_node);
			push_heap(open_list.begin(), open_list.end(), compare_way_cell_for_pathfinding);
		}
		else if (adjacent_node->get_open())
		{
			if ((cur_node->get_g() + 1 + adjacent_node->get_h()) < adjacent_node->get_f())
			{
				adjacent_node->set_g(cur_node->get_g() + 1);
				adjacent_node->set_prev(cur_node);
				make_heap(open_list.begin(), open_list.end(), compare_way_cell_for_pathfinding);
			}
		}

	}
}
bool way_cell_pathfinding::build_path(vector<pathfinding_result*>& out)
{
	way_cell* path_node = closed_list.back();
	while (path_node != NULL)
	{
		out.push_back(path_node);
		path_node = path_node->get_prev();
	}
	return true;
}
cpVect way_cell_pathfinding::get_pos(way_cell* wp)
{
	cpVect res;
	if (wp->x == 0)
		res.x = wp->x*base_grid.pixel_per_cell + x_side_offset;
	else if (wp->x == base_grid._w - 1)
		res.x = wp->x*base_grid.pixel_per_cell + x_side_offset + x_side_offset;
	else
		res.x = wp->x*base_grid.pixel_per_cell + x_side_offset / 2 + x_side_offset;
	if (wp->y == 0)
		res.y = wp->y*base_grid.pixel_per_cell + y_side_offset;
	else if (wp->y == base_grid._h - 1)
		res.y = wp->y*base_grid.pixel_per_cell + y_side_offset + y_side_offset;
	else
		res.y = wp->y*base_grid.pixel_per_cell + y_side_offset / 2 + y_side_offset;
	return res;
}
void way_cell_pathfinding::add_obstacle(float x, float y, int w, int h, int target_id)
{
	w /= 2;
	h /= 2;
	for (int index_x = 0; index_x < base_grid._w; ++index_x)
	{
		for (int index_y = 0; index_y < base_grid._h; ++index_y)
		{
			way_cell* wp = base_grid.get_way_cell(index_x, index_y);
			if (wp == NULL)
				continue;
			float x0 = base_grid.pos_x0(wp, x_side_offset);
			float x1 = base_grid.pos_x1(wp, x_side_offset);
			float y0 = base_grid.pos_y0(wp, y_side_offset);
			float y1 = base_grid.pos_y1(wp, y_side_offset);

			float min_x = min(fabs(x0 - x), fabs(x1 - x));
			float min_y = min(fabs(y0 - y), fabs(y1 - y));

			if((min_x<w)
				&& (min_y<h))
			{
				wp->set_pass_type(target_id);
			}
		}
	}
}
void way_cell_pathfinding::del_obstacle(float x, float y, int w, int h)
{
	add_obstacle(x, y, w, h, 0);
}
bool way_cell_pathfinding::find_path(const cpVect& start, const cpVect& end, vector<pathfinding_result*>& out, int target_id)
{
	base_grid.reset_pathfinding_cache();
	open_list.clear();
	closed_list.clear();

	way_cell* src_wp = find_near_waypoint(start);
	way_cell* tar_wp = find_near_waypoint(end);
	tar_wp->set_used();
	open_list.push_back(tar_wp);
	push_heap(open_list.begin(), open_list.end(), compare_way_cell_for_pathfinding);
	while (!open_list.empty())
	{
		pop_heap(open_list.begin(), open_list.end(), compare_way_cell_for_pathfinding);
		way_cell* cur_node = open_list.back();
		open_list.pop_back();
		closed_list.push_back(cur_node);
		if (cur_node == src_wp)
			return build_path(out);
		else
			handle_node(src_wp, tar_wp, cur_node, target_id);
	}
	return false;
}