#include "waypoint.h"
bool compare_waypoint_for_pathfinding(grid_way_point *left, grid_way_point *right)
{
	return left->get_f() > right->get_f();
}
////path_finding
grid_way_point* waypoint_pathfinding::find_near_waypoint(const cpVect& pos)
{
	int x = 0;
	if (pos.x < x_side_offset)
		x = 0;
	else if (pos.x > x_side_offset + (base_grid._w - 1)*base_grid.pixel_per_cell)
		x = base_grid._w - 1;
	else
		x = (int)(pos.x + base_grid.pixel_per_cell / 2 - x_side_offset) / base_grid.pixel_per_cell;

	int y = 0;
	if (pos.y < y_side_offset)
		y = 0;
	else if (pos.y > y_side_offset + (base_grid._h - 1)*base_grid.pixel_per_cell)
		y = base_grid._h - 1;
	else
		y = (int)(pos.y + base_grid.pixel_per_cell / 2 - y_side_offset) / base_grid.pixel_per_cell;

	return base_grid.get_waypoint(x, y);
}

void waypoint_pathfinding::handle_node(grid_way_point* src_wp, grid_way_point* tar_wp, grid_way_point* cur_node, uint32 target_id)
{
	for (int i = 0; i < 4; ++i)
	{
		grid_way_point* adjacent_node = base_grid.get_neighbor(cur_node, i);
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
			push_heap(open_list.begin(), open_list.end(), compare_waypoint_for_pathfinding);
		}
		else if (adjacent_node->get_open())
		{
			if ((cur_node->get_g() + 1 + adjacent_node->get_h()) < adjacent_node->get_f())
			{
				adjacent_node->set_g(cur_node->get_g() + 1);
				adjacent_node->set_prev(cur_node);
				make_heap(open_list.begin(), open_list.end(), compare_waypoint_for_pathfinding);
			}
		}

	}
}
bool waypoint_pathfinding::build_path(vector<pathfinding_result*>& out)
{
	grid_way_point* path_node = closed_list.back();
	while (path_node != NULL)
	{
		out.push_back(path_node);
		path_node = path_node->get_prev();
	}
	return true;
}
cpVect waypoint_pathfinding::get_pos(grid_way_point* wp)
{
	return cpv(
		wp->x*base_grid.pixel_per_cell + x_side_offset,
		wp->y*base_grid.pixel_per_cell + y_side_offset);
}
void waypoint_pathfinding::add_obstacle(float x, float y, int w, int h, int target_id)
{
	w /= 2;
	h /= 2;
	for (int index_x = 0; index_x < base_grid._w; ++index_x)
	{
		for (int index_y = 0; index_y < base_grid._h; ++index_y)
		{
			grid_way_point* wp = base_grid.get_waypoint(index_x, index_y);
			if (wp == NULL)
				continue;
			cpVect pos = get_pos(wp);
			if ((fabs(pos.x - x) < (w + base_grid.pixel_per_cell/2)) && (fabs(pos.y - y) < (h + base_grid.pixel_per_cell/2)))
				wp->set_pass_type(target_id);
		}
	}
}
void waypoint_pathfinding::del_obstacle(float x, float y, int w, int h)
{
	add_obstacle(x, y, w, h, 0);
}
bool waypoint_pathfinding::find_path(const cpVect& start, const cpVect& end, vector<pathfinding_result*>& out, int target_id)
{
	base_grid.reset_pathfinding_cache();
	open_list.clear();
	closed_list.clear();

	grid_way_point* src_wp = find_near_waypoint(start);
	grid_way_point* tar_wp = find_near_waypoint(end);
	tar_wp->set_used();
	open_list.push_back(tar_wp);
	push_heap(open_list.begin(), open_list.end(), compare_waypoint_for_pathfinding);
	while (!open_list.empty())
	{
		pop_heap(open_list.begin(), open_list.end(), compare_waypoint_for_pathfinding);
		grid_way_point* cur_node = open_list.back();
		open_list.pop_back();
		closed_list.push_back(cur_node);
		if (cur_node == src_wp)
			return build_path(out);
		else
			handle_node(src_wp, tar_wp, cur_node, target_id);
	}
	return false;
}
////////////////////////////////////////////////////////////////////