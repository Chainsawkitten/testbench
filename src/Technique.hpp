#pragma once
#include <vector>
#include "Base/ResourceBinding.hpp"
#include "Base/Material.hpp"
#include "Base/RenderState.hpp"

class Renderer;

class Technique
{
public:
	Technique();
	~Technique();
	//void setBindings(std::vector<ResourceBinding*>* rb);

	void enable(Renderer* renderer);

	Material* material = nullptr;
	RenderState* renderState = nullptr;

	// zero size vector
	std::vector<ResourceBinding*> bindings;
};

