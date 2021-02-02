#pragma once


enum NodeType
{
	PBR,
	UNKNOWN
};


class ShaderNode
{
public:
	ShaderNode();
	~ShaderNode();


	void Draw();

public:
	NodeType type;
};
