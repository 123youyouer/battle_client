#ifndef navi_grid_hpp
#define navi_grid_hpp
#include <vector>
#include <chipmunk/chipmunk_private.h>
#include <math.h>
class navi_result
{
public:
	const static int none = 0;
	const static int force = 1;
	const static int navi = 2;
public:
	int type;
	int x_offset;
	int y_offset;
	navi_result():type(0), x_offset(0), y_offset(0)
	{

	}
};
class navi_rect
{
public:
	int x0;
	int y0;
	int x1;
	int y1;
	inline int mid_y()
	{
		return (y0 + y1) / 2;
	}
	inline int mid_x()
	{
		return (x0 + x1) / 2;
	}
	inline int quadrant(int x, int y)
	{
		int midx = mid_x();
		int midy = mid_y();
		if (x > midx&& y > midy)
			return 1;
		else if (x <= midx&& y > midy)
			return 2;
		else if (x <= midx&& y <= midy)
			return 3;
		else
			return 4;
	}
};
class navi_grid
{
public:
	navi_rect grid_rect;
	navi_rect base_rect;
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius) = 0;
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len) = 0;
	void set_base_rect(int left_x, int top_y, int right_x, int bottom_y)
	{
		base_rect.x0 = left_x;
		base_rect.x1 = right_x;
		base_rect.y0 = top_y;
		base_rect.y1 = bottom_y;
	}
public:
	bool in_grid(int x, int y,int radius)
	{
		return ((grid_rect.x0 <= x&&x < grid_rect.x1) && (grid_rect.y1 <= y&&y < grid_rect.y0));
		/*int minx = __min(abs(x - base_rect.x0), abs(x - base_rect.x1));
		int miny = __min(abs(y - base_rect.y0), abs(y - base_rect.y1));
		if ((minx + 10) < radius || (miny + 10) < radius)
			return ((grid_rect.x0 <= x&&x < grid_rect.x1) && (grid_rect.y1 <= y&&y < grid_rect.y0));
		else
			return false;*/
	}
	void reset(int left_x, int top_y, int right_x, int bottom_y, int edge)
	{
		set_base_rect(left_x, top_y, right_x, bottom_y);
		compute_grid_rect(left_x, top_y, right_x, bottom_y, edge);
	}
	
	navi_result get_navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		if (!in_grid(src_x, src_y, radius))
			return navi_result();
		return navi(src_x, src_y, tar_x, tar_y, radius);
	}
};
class navi_grid_solt_0 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (tar_x < src_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
		case 2:
			if ((src_x + radius) < grid_rect.x1)
			{
				res.y_offset = 10;
			}
			else
			{
				res.x_offset = -10;
				res.y_offset = 10;
			}
			return res;
		case 3:
			res.y_offset = tar_y - src_y;
			return res;
		case 4:
			if ((src_y + radius) < grid_rect.y0)
			{
				res.x_offset = 10;
				res.y_offset = -10;
			}
			else
			{
				res.y_offset = -10;
			}
			return res;		
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y,int edge_len)
	{
		grid_rect.x0 = left_x - edge_len;
		grid_rect.x1 = left_x;
		grid_rect.y0 = bottom_y;
		grid_rect.y1 = bottom_y - edge_len;
	}
};
class navi_grid_solt_1 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (tar_x < src_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else if (tar_y < src_y)
		{
			res.x_offset = 0;
			res.y_offset= tar_y - src_y;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
		case 2:
			res.y_offset = 10;
			return res;
		case 3:
			res.y_offset = tar_y - src_y;
			return res;
		case 4:
			res.y_offset = -10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x - edge_len;
		grid_rect.x1 = left_x;
		grid_rect.y0 = bottom_y + (top_y - bottom_y) / 2;
		grid_rect.y1 = bottom_y;
	}
};
class navi_grid_solt_2 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (tar_x < src_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.y_offset = 10;
			return res;
		case 2:
			res.y_offset = tar_y - src_y;
			return res;
		case 3:
		case 4:
			res.y_offset = -10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x - edge_len;
		grid_rect.x1 = left_x;
		grid_rect.y0 = top_y;
		grid_rect.y1 = bottom_y + (top_y - bottom_y) / 2;
	}
};
class navi_grid_solt_3 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (tar_x < src_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			if ((src_y - radius) > grid_rect.y1)
			{
				res.x_offset = 10;
			}
			else
			{
				res.y_offset = 10;
			}
			return res;
		case 2:
			res.y_offset = tar_y - src_y;
			return res;
		case 3:
		case 4:
			if ((src_x + radius) < grid_rect.x1)
			{
				res.y_offset = -10;
			}
			else
			{
				res.x_offset = 10;
			}
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x - edge_len;
		grid_rect.x1 = left_x;
		grid_rect.y0 = top_y + edge_len;
		grid_rect.y1 = top_y;
	}
};
class navi_grid_solt_4 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y < tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
		case 4:
			res.x_offset = 10;
			return res;
		case 2:
			res.x_offset = tar_x - src_x;
			return res;
		case 3:
			res.x_offset = -10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x;
		grid_rect.x1 = left_x + (right_x - left_x) / 2;
		grid_rect.y0 = top_y + edge_len;
		grid_rect.y1 = top_y;
	}
};
class navi_grid_solt_5 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y < tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.x_offset = tar_x - src_x;
			return res;
		case 2:
			res.x_offset = -10;
			return res;		
		case 3:
			res.x_offset = -10;
			return res;
		case 4:
			res.x_offset = 10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x + (right_x - left_x) / 2;
		grid_rect.x1 = right_x;
		grid_rect.y0 = top_y + edge_len;
		grid_rect.y1 = top_y;
	}
};
class navi_grid_solt_6 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y < tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else if (src_x < tar_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.y_offset = tar_y - src_y;
			return res;
			
		case 2:
			if ((src_y - radius) > grid_rect.y1)
			{
				res.x_offset = -10;
			}
			else
			{
				res.y_offset = 10;
			}
			return res;
		case 3:
		case 4:
			if ((src_x - radius) > grid_rect.x0)
			{
				res.y_offset = -10;
			}
			else
			{
				res.x_offset = 10;
			}
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = right_x;
		grid_rect.x1 = right_x + edge_len;
		grid_rect.y0 = top_y + edge_len;
		grid_rect.y1 = top_y;
	}
};
class navi_grid_solt_7 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_x < tar_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.y_offset = tar_y - src_y;
			return res;
		case 2:
			res.y_offset = 10;
			return res;
		case 3:
			res.y_offset = -10;
			return res;
		case 4:
			res.y_offset = -10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = right_x;
		grid_rect.x1 = right_x + edge_len;
		grid_rect.y0 = top_y;
		grid_rect.y1 = top_y - (top_y - bottom_y) / 2;
	}
};
class navi_grid_solt_8 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_x < tar_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.y_offset = 10;
			return res;
		case 2:
			res.y_offset = 10;
			return res;
		case 3:
			res.y_offset = -10;
			return res;
		case 4:
			res.y_offset = tar_y - src_y;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = right_x;
		grid_rect.x1 = right_x + edge_len;
		grid_rect.y0 = top_y - (top_y - bottom_y) / 2;
		grid_rect.y1 = bottom_y;
	}
};
class navi_grid_solt_9 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y > tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else if (src_x < tar_x)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			if ((src_x - radius) > grid_rect.x0)
			{
				res.y_offset = 10;
			}
			else
			{
				res.x_offset = 10;
				res.y_offset = 10;
			}
			return res;
		case 2:
			if ((src_x - radius) > grid_rect.x0)
			{
				res.y_offset = 10;
			}
			else
			{
				res.x_offset = 10;
				res.y_offset = 10;
			}
			return res;
		case 3:
			if ((src_y + radius) < grid_rect.y0)
			{
				res.x_offset = -10;
			}
			else
			{
				res.y_offset = -10;
				res.x_offset = -10;
			}
			return res;
		case 4:
			res.y_offset = tar_y - src_y;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = right_x;
		grid_rect.x1 = right_x + edge_len;
		grid_rect.y0 = bottom_y;
		grid_rect.y1 = bottom_y - edge_len;
	}
};
class navi_grid_solt_10 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y > tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.x_offset = 10;
			return res;
		case 2:
			res.x_offset = -10;
			return res;
		case 3:
			res.x_offset = -10;
			return res;
		case 4:
			res.x_offset = tar_x - src_x;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = right_x - (right_x - left_x) / 2;
		grid_rect.x1 = right_x;
		grid_rect.y0 = bottom_y;
		grid_rect.y1 = bottom_y - edge_len;
	}
};
class navi_grid_solt_11 : public navi_grid
{
protected:
	virtual navi_result navi(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		navi_result res;
		res.type = navi_result::navi;
		if (src_y > tar_y)
		{
			res.x_offset = tar_x - src_x;
			res.y_offset = tar_y - src_y;
			return res;
		}
		else switch (base_rect.quadrant(tar_x, tar_y))
		{
		case 1:
			res.x_offset = 10;
			return res;
		case 2:
			res.x_offset = -10;
			return res;
		case 3:
			res.x_offset = tar_x - src_x;
			return res;
		case 4:
			res.x_offset = 10;
			return res;
		}
		return navi_result();
	}
	virtual void compute_grid_rect(int left_x, int top_y, int right_x, int bottom_y, int edge_len)
	{
		grid_rect.x0 = left_x;
		grid_rect.x1 = right_x - (right_x - left_x) / 2;
		grid_rect.y0 = bottom_y;
		grid_rect.y1 = bottom_y - edge_len;
	}
};
template<int EDGE>
class navi_grid_manager
{
private:
	int edge_len;
	std::vector<navi_grid*> navi_grid_list;
	void reset(int left_x, int top_y, int right_x, int bottom_y, int index)
	{
		navi_grid* grid;
		
		switch (index)
		{
		case 0:
			grid = new navi_grid_solt_0(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 1:
			grid = new navi_grid_solt_1(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 2:
			grid = new navi_grid_solt_2(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 3:
			grid = new navi_grid_solt_3(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 4:
			grid = new navi_grid_solt_4(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 5:
			grid = new navi_grid_solt_5(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 6:
			grid = new navi_grid_solt_6(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 7:
			grid = new navi_grid_solt_7(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 8:
			grid = new navi_grid_solt_8(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 9:
			grid = new navi_grid_solt_9(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 10:
			grid = new navi_grid_solt_10(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		case 11:
			grid = new navi_grid_solt_11(); grid->reset(left_x, top_y, right_x, bottom_y, EDGE);
			break;
		}
		navi_grid_list.push_back(grid);
	}
public:
	void reset(int left_x, int top_y, int right_x, int bottom_y)
	{
		navi_grid_list.clear();
		for (int i = 0; i < 12; ++i)
		{
			reset(left_x, top_y, right_x, bottom_y, i);
		}
	}
	navi_result get_navi_result(int src_x, int src_y, int tar_x, int tar_y, int radius)
	{
		for (int i = 0; i < navi_grid_list.size(); ++i)
		{
			navi_result res = navi_grid_list[i]->get_navi(src_x, src_y, tar_x, tar_y, radius);
			if (res.type != navi_result::none)
				return res;
		}
		return navi_result();
	}
	virtual ~navi_grid_manager()
	{
		for (int i = 0; i < navi_grid_list.size(); ++i)
		{
			delete navi_grid_list[i];
			navi_grid_list[i] = NULL;
		}
		navi_grid_list.clear();
	}
};
#endif
#pragma once
