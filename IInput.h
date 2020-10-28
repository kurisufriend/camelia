#pragma once
#include "Vector.h"

class IInput
{
private:
	char pad0[172];
	bool pad1;
public:
	bool cameraInThirdPerson;
private:
	char pad2[2];
public:
	Vector cameraOffset;

	void* getUserCmd() {}
};