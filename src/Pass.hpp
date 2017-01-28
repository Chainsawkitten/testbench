#pragma once
#include <memory>
#include "Base/Material.hpp"

class Pass
{
public:
	Pass();
	~Pass();
	std::shared_ptr<Material> material = nullptr;
};

