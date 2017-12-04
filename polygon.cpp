#include "polygon.h"

polygon::polygon(vector<vector2> &vertexs) :
	m_vertexs(vertexs), user_data(NULL)
{

}
polygon::polygon(): user_data(NULL)
{

}
polygon::~polygon()
{

}
void polygon::add_vector2(int x, int y)
{
	vector2 v;
	v.m_x = x;
	v.m_y = y;
	m_vertexs.push_back(v);
}