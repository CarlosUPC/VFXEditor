#pragma once

#include "Globals.h"

enum class Type
{
	RESOURCE_SHADER,
	RESOURCE_TEXTURE,
	RESOURCE_MATERIAL,
	RESOURCE_MODEL

};

class Resource
{
public:

	Resource();
	Resource(Resource::Type);

	const u64& GetResourceID() const;
	const Type& GetResourceType() const;

private:

	uint64 id;
	Type type;

};
