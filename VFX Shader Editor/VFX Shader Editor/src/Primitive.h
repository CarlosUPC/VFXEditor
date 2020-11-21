#pragma once

#include "Color.h"
#include <vector>
//#include "Globals.h"
//#include "MathGeoLib/Math/MathConstants.h"
//#include "MathGeoLib/Math/float3.h"
//#include "MathGeoLib/Math/float4x4.h"
//#include "MathGeoLib/Math/float3x3.h"
//#include "MathGeoLib/Math/Quat.h"
//#include "MathGeoLib/Math/TransformOps.h"
//#include "MathGeoLib/MathGeoLibFwd.h"


using namespace std;

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Arrow,
	Primitive_Axis,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Capsule,
	Primitive_Frustum
};

class Primitive
{
public:

	Primitive();
	~Primitive();

	virtual void	Render() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3& u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

protected:

	void generateBuffer();

public:

	Color color;
	float4x4 transform;
	bool axis, wire;

	vector<float> shape;
	vector<uint> indices;

	uint my_id = 0;

protected:

	PrimitiveTypes type;

};

// ============================================
class MCube : public Primitive
{
public:
	MCube();
	MCube(float sizeX, float sizeY, float sizeZ, float3 center);
public:
	float3 size;
};

// ============================================
class MSphere : public Primitive
{
public:
	MSphere();
	MSphere(float radius, int rings, int sectors, float3 pos);
public:
	float radius;
};

// ============================================
class MCylinder : public Primitive
{
public:
	MCylinder();
	MCylinder(float radius, float height, int rings, int sectors, float3 pos);
public:
	float radius;
	float height;
};

// ============================================
class MLine : public Primitive
{
public:
	MLine();
	MLine(float x, float y, float z, float3 origin);
	void Render() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class MArrow : public Primitive
{
public:
	MArrow();
	MArrow(float x, float y, float z, float3 pos);
	void Render() const;
public:
	float3 origin;
	float3 destination;
	float3 arrowHead1;
	float3 arrowHead2;
private:
	float defaultHeadLength = 2;
};

// ============================================
class MAxis : public Primitive
{
public:
	MAxis();
	MAxis(float size, float3 pos);
	void Render() const;
public:
	float3 origin;
	float size;
};

// ============================================
class MPlane : public Primitive
{
public:
	MPlane();
	MPlane(float x, float y, float z, float d);

	void Render() const;
public:
	float3 normal;
	float constant;
};

// ============================================
class MCapsule : public Primitive
{
public:
	MCapsule();
	MCapsule(float radius, float height, int rings, int sectors, float3 pos);
public:
	float radius;
	float height;
};

// ============================================
class MFrustum : public Primitive
{
public:
	MFrustum();
	MFrustum(float distance, float depth, float width, float height, float3 center);
public:
	float3 center;
	float distance, width, depth, height;
};
