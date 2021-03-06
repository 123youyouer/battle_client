#ifndef __polygonH__
#define __polygonH__

#include <vector>
#include "vector2.h"

using namespace std;

class polygon
{
public:
	polygon(vector<vector2> &vertexs);
	polygon();

	~polygon();

public:
	const vector<vector2>& get_vertex()const { return m_vertexs; }

	void set_vertex(const vector<vector2> &vertexs) { m_vertexs = vertexs; }
	void add_vector2(int x, int y);
private:
	vector<vector2> m_vertexs;   	 	//���б�
	rectangle 		rect; 				//��Χ��
public:
	void* user_data;
};

#endif