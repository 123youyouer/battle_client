#ifndef contact_filter_hpp
#define contact_filter_hpp
#include "globel.h"
/*
class contact_filter : public b2ContactFilter
{
protected:
	bool my_ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		const b2Filter& f1 = fixtureA->GetFilterData();
		const b2Filter& f2 = fixtureB->GetFilterData();

		if (f1.groupIndex != f2.groupIndex)
			return false;
		return ((f1.maskBits&f2.categoryBits) != 0);
	}
public:
	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		b2Vec2 v1 = fixtureA->GetBody()->GetLinearVelocity();
		b2Vec2 v2 = fixtureB->GetBody()->GetLinearVelocity();

		if ((v1 != b2Vec2_zero) && (v2 != b2Vec2_zero))
			return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
		if (v1 == b2Vec2_zero)
			return my_ShouldCollide(fixtureB, fixtureA);
		else
			return my_ShouldCollide(fixtureA, fixtureB);					
	}
};
*/
#endif
