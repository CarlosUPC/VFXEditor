#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"

ModuleCamera::ModuleCamera(bool start_enabled)
	: Module(start_enabled)
{
	frustum.type = math::FrustumType::PerspectiveFrustum;

	position = { 0.0f,10.0f,10.0f };
	frustum.pos = position;
	frustum.front = { 0.0f,0.0f,-1.0f };
	frustum.up = { 0.0f,1.0f,0.0f };

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;

	frustum.verticalFov = DEGTORAD * 90.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (16.0f / 9.0f));

	LookAt(reference);


}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	return true;
}

update_state ModuleCamera::Update()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 newPos(0, 0, 0);
		float speed = 0.2f;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2;
		else if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
			speed /= 2;

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= frustum.front * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += frustum.WorldRight() * speed;

		position += newPos;
		reference += newPos;

		reference = position - getMovementFactor();
	}

	frustum.pos = position;

	Z = -frustum.front;
	Y = frustum.up;
	X = frustum.WorldRight();

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}


void ModuleCamera::OnResize(int width, int height)
{
	float newAR = (float)width / (float)height;
	setAspectRatio(newAR);
}

void ModuleCamera::setAspectRatio(float aspectRatio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
}

float* ModuleCamera::getProjectionMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float* ModuleCamera::getViewMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

void ModuleCamera::LookAt(const float3& Spot)
{
	reference = Spot;

	//caluclate direction to look
	float3 dir = Spot - frustum.pos;

	//caluclate the new view matrix
	float3x3 viewMat = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3(0.0f, 1.0f, 0.0f));

	//set new front and up for the frustum
	frustum.front = viewMat.MulDir(frustum.front).Normalized();
	frustum.up = viewMat.MulDir(frustum.up).Normalized();
}

float3 ModuleCamera::getMovementFactor()
{
	int dx = -App->input->GetMouseDeltaX();
	int dy = -App->input->GetMouseDeltaY();

	float3 newPosition = position - reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * 0.01f;

		Quat rotation = Quat::RotateY(DeltaX);
		frustum.front = rotation.Mul(frustum.front).Normalized();
		frustum.up = rotation.Mul(frustum.up).Normalized();
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * 0.01f;

		Quat rotation = Quat::RotateAxisAngle(frustum.WorldRight(), DeltaY);

		if (rotation.Mul(frustum.up).Normalized().y > 0.0f)
		{
			frustum.up = rotation.Mul(frustum.up).Normalized();
			frustum.front = rotation.Mul(frustum.front).Normalized();
		}
	}

	return -frustum.front * newPosition.Length();
}