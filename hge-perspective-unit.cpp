#include "hge-perspective-unit.hpp"
hge::math::PerspectiveUnit::PerspectiveUnit():
	persM(Matrix4D<>::perspective(3.0f, 2.0f, 3.0f, 90000000000000000000000.0f))
{
}
hge::math::PerspectiveUnit::~PerspectiveUnit()
{
}
const hge::math::Matrix4D<> &hge::math::PerspectiveUnit::getMatrix()
{
	return persM;
}
