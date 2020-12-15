#include "Resource.h"

Resource::Resource(){}

Resource::Resource(Type type) 
{
	this->type = type;
}

const u64& Resource::GetResourceID() const
{
	return id;
}

const Type& Resource::GetResourceType() const
{
	return type;
}
