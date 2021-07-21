#pragma once

enum eDepthState
{
	ENABLED,
	DISABLED,
	READ_NO_WRITE,
	NO_READ_NO_WRITE,
	_DEPTH_COUNT
};

struct FVector2D
{
	FVector2D() : x(0.f), y(0.f)
	{

	}

	FVector2D(float _x, float _y) : x(_x), y(_y)
	{

	}
	~FVector2D()
	{

	}
	float x, y;
};