#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Color.h"
#include <vector>
#include "MathGeoLib/MathGeoLib.h"

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
	void			SetRotation(float angle, const vec& u);
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

	uint VAO = 0;

protected:

	PrimitiveTypes type;

};

// ============================================
class MCube : public Primitive
{
public:
	MCube();
	MCube(float sizeX, float sizeY, float sizeZ, vec center);
	void Render() const;
public:
	vec size;
};

// ============================================
class MSphere : public Primitive
{
public:
	MSphere();
	MSphere(float radius, int rings, int sectors, vec pos);
public:
	float radius;
};

// ============================================
class MCylinder : public Primitive
{
public:
	MCylinder();
	MCylinder(float radius, float height, int rings, int sectors, vec pos);
public:
	float radius;
	float height;
};

// ============================================
class MLine : public Primitive
{
public:
	MLine();
	MLine(float x, float y, float z, vec origin);
	void Render() const;
public:
	vec origin;
	vec destination;
};

// ============================================
class MArrow : public Primitive
{
public:
	MArrow();
	MArrow(float x, float y, float z, vec pos);
	void Render() const;
public:
	vec origin;
	vec destination;
	vec arrowHead1;
	vec arrowHead2;
private:
	float defaultHeadLength = 2;
};

// ============================================
class MAxis : public Primitive
{
public:
	MAxis();
	MAxis(float size, vec pos);
	void Render() const;
public:
	vec origin;
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
	vec normal;
	float constant;
};

// ============================================
class MCapsule : public Primitive
{
public:
	MCapsule();
	MCapsule(float radius, float height, int rings, int sectors, vec pos);
public:
	float radius;
	float height;
};

// ============================================
class MFrustum : public Primitive
{
public:
	MFrustum();
	MFrustum(float distance, float depth, float width, float height, vec center);
public:
	vec center;
	float distance, width, depth, height;
};

// ============================================


#endif // __PRIMITIVE_H__
