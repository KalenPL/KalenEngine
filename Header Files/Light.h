#ifndef LIGHT_H
#define LIGHT_H

#include "Utility.h"

struct DirectionalLight
{
	DirectionalLight()
	{
		this->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
		this->Pad = 1.0f;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;
};


struct PointLight
{
	PointLight()  //{ ZeroMemory(this, sizeof(this)); }
	{
		this->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		this->Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		this->Range = 0.0f;
		this->Att = XMFLOAT3(0.001f, 0.001f, 0.001f);
		this->Pad = 1.0f;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Att;
	float Pad;
};
#endif //LIGHT_H