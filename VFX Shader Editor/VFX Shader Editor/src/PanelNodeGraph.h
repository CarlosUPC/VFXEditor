#pragma once

#include "Panel.h"

class PanelNodeGraph : public Panel
{
public:

	PanelNodeGraph(const char* name);
	~PanelNodeGraph();

	void Draw();

private:
	bool grid = true;

};
