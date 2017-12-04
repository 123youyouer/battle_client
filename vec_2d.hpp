#ifndef VEC_2D_HPP
#define VEC_2D_HPP
#include <math.h>
class vec_2d
{
public:
	float x;
	float y;
public:
	vec_2d():x(0),y(0) {}
	vec_2d(float X, float Y) :x(X), y(Y) { }
	vec_2d	operator+	(const vec_2d& v) const 
	{ 
		return vec_2d(x + v.x, y + v.y);
	}
	vec_2d	operator-	(const vec_2d& v) const 
	{ 
		return vec_2d(x - v.x, y - v.y);
	}
	vec_2d	operator-	(void) const 
	{ 
		return vec_2d(-x, -y); 
	}
	vec_2d	operator*	(float s) const 
	{ 
		return vec_2d(x*s, y*s);
	}
	float	operator*	(const vec_2d& v) const 
	{ 
		return x*v.x + y*v.y;
	}
	vec_2d	operator/	(float s) const 
	{ 
		return vec_2d(x / s, y / s);
	}
	void	operator+=	(const vec_2d& v) 
	{ 
		x += v.x; y += v.y;
	}
	void	operator-=	(const vec_2d& v) 
	{
		x -= v.x; y -= v.y;
	}
	void	operator*=	(const vec_2d& v) 
	{ 
		x *= v.x; y *= v.y;
	}
	void	operator*=	(float s) 
	{ 
		x *= s; y *= s;
	}
	void	operator/=	(float s) 
	{ 
		x /= s; y /= y; 
	}
	inline bool is_zero()
	{
		return fabsf(x) < 0.1&&fabsf(y) < 0.1;
	}
	inline float det(const vec_2d& v) const 
	{ 
		return x*v.y - y*v.x;
	}
	inline float normalize() 
	{ 
		float len = this->len();
		this->operator/=(len);
		return len;
	}
	inline float len() const 
	{ 
		return sqrtf(x*x + y*y);
	}
	inline float len_squ() const
	{
		return x*x + y*y;
	}
	inline void clip(float max)
	{
		float _len = len();
		if (_len <= max)
			return;
		(*this) *= (max / _len);
	}
	inline void add_polar(float angle, float len)
	{
		float radian = angle / 180 * 3.1415926;
		x += len*cosf(radian);
		y += len*sinf(radian);
	}
	inline void set_polar(float angle, float len)
	{
		float radian = angle / 180 * 3.1415926;
		x = (len*cosf(radian));
		y = (len*sinf(radian));
	}
public:
	static float angle_between(float x1, float y1, float x2, float y2)
	{
		float s = x1*y2 - x2*y1;
		float c = x1*x2 - y1*y2;
		return atan2f(s, c) * 180 / 3.1415926;
	}
	static float radian_between(float x1, float y1, float x2, float y2)
	{
		float s = x1*y2 - x2*y1;
		float c = x1*x2 - y1*y2;
		return atan2f(s, c);
	}
};
#endif