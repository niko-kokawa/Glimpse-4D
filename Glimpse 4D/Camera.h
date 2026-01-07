#pragma once
#include "MeshData.h"
#include "Space.h"

class Camera4D {
public:
	Vec4 position;

	float fov;
	float wNear;
	Rot4 rotation;
	Camera4D(Vec4 position, Rot4 rotor, float fov, float wNear) {
		this->fov = fov;
		this->position = position;
		this->wNear = wNear;
		this->rotation = rotor;
	}
	Vec4 apply(const Vec4& p) const {
		return rotation.rotate(p - position);
	}
};