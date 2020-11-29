#pragma once

#include "Module.h"
//#include "MathGeoLib/Math/float3.h"
//#include "MathGeoLib/Math/float4x4.h"
//#include "MathGeoLib/Math/Quat.h"
//#include "MathGeoLib/Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera(bool start_enabled = true);
	~ModuleCamera();

	bool Init();
	update_state Update(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	float* getProjectionMatrix();
	float* getViewMatrix();
	void LookAt(const float3& Spot);
	float3 getMovementFactor();
	void setAspectRatio(float aspectRatio);

private:
	float3 X = { 1.0f,0.0f,0.0f };
	float3 Y = { 0.0f,1.0f,0.0f };
	float3 Z = { 0.0f,0.0f,1.0f };
	float3 position = { 0.0f,0.0f,10.0f };
	float3 reference = { 0.0f,0.0f,0.0f };

	Frustum frustum;
};
