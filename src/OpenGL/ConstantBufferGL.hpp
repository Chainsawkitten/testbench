#pragma once
#include <GL/glew.h>
#include "../Base/ConstantBuffer.hpp"

class ConstantBufferGL : public ConstantBuffer
{
public:
	ConstantBufferGL(std::string NAME, unsigned int location);
	~ConstantBufferGL() final;
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);

private:

	std::string name;
	GLuint location;
	GLuint handle;
	GLuint index;
	void* buff = nullptr;
	void* lastMat;
};

