#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
using namespace glm;

//------------------------------------------------------------
// INDIRECT DRAW BUFFER
//	This creates a indirect draw buffer that can be used with
//	multi draw. However, since we are using opengl we need to
//	emulate the behaviour. So nothing exits on the GPU.
//------------------------------------------------------------

// TODO: this class

namespace MBG {

class IndirectDrawBuffer
{
public:
	IndirectDrawBuffer(IndirectDrawBufferParams params) {

	}

	~IndirectDrawBuffer() {

	}

private:

};

}