#pragma once
#include <vector>
#include "Base/Material.hpp"
#include "Base/RenderState.hpp"

class Renderer;

class Technique
{
public:
	Technique();
	~Technique();

	void enable(Renderer* renderer);

	Material* material = nullptr;
	RenderState* renderState = nullptr;
};

