#include "hge-model-unit.hpp"
hge::math::ModelUnit::ModelUnit():
	localX(math::Vector3D<>(1.0f, 0.0f, 0.0f)),
	localY(math::Vector3D<>(0.0f, 1.0f, 0.0f)),
	localZ(math::Vector3D<>(0.0f, 0.0f, 1.0f)),
	location(math::Vector3D<>(0.0f, 0.0f, 0.0f)),
	rotateScaleTranslateMatrix(math::Matrix4D<>(1.0f)),
	rotateScaleMatrix(math::Matrix4D<>(1.0f)),
	rotateMatrix(math::Matrix4D<>(1.0f))
{
}

void hge::math::ModelUnit::ModelUnit::translate (const math::Vector3D<> &trn)
{
	location += trn;
	rotateScaleTranslateMatrix.translate(trn);
}

void hge::math::ModelUnit::scale(const float &size)
{
	rotateScaleMatrix.scale(size);
	rotateScaleTranslateMatrix.translate(location);
}

const hge::math::Matrix4D<> &hge::math::ModelUnit::getConstRotateScaleTranslateMatrix()
{
	return rotateScaleTranslateMatrix;
}

const hge::math::Matrix4D<> &hge::math::ModelUnit::getConstRotateScaleMatrix()
{
	return rotateScaleMatrix;
}

const hge::math::Matrix4D<> &hge::math::ModelUnit::getConstRotateMatrix()
{
	return rotateMatrix;
}
