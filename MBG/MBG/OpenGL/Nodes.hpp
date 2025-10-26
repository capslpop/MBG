#pragma once

#include <string>
#include <functional>

#include "RenderPass.hpp"
#include "VertexBuffer.hpp"
#include "DoubleTexture2D.hpp"
#include "DoubleTexture3D.hpp"
#include "Texture2D.hpp"
#include "Texture3D.hpp"

enum RENDER_TYPE {
	TRIANGLES,
	TRIANGLE_STRIPS,
	TRIANGLE_LINES,
	LINES,
	POINTS,
};

enum NODE_HINT {
	COMMON,		// This means that this node is likly to happen every frame and should be cauched
	UNCOMMON,	// This means that this node is not likly to happen every frame and should not be cauched
	NONE,
};

// Since a FrameGraph is inheriently defered so will be out memory transfors
// This means that data read or writen will happen once the FrameGraph's run command is executed
template <typename T>
struct NodeMemoryRead {
	T* GPU_Memory = nullptr;
	F function = ;
	std::string name = "MEMORY O";
	NODE_HINT hint = NODE_HINT::NONE;
};

struct NodeDraw {
	RenderPass* render_pass = nullptr;
	RENDER_TYPE render_type = RENDER_TYPE::POINTS;
	size_t offset = 0; // The offset of the vertex position NOT byte position
	size_t size = 0; // number of elements to draw
	size_t instance_count = 1; // number of instances of this mesh
	std::string name = "DRAW MESH";
	NODE_HINT hint = NODE_HINT::NONE;
};

struct NodeMultiDraw {
	RenderPass* render_pass = nullptr;
	RENDER_TYPE render_type = RENDER_TYPE::POINTS;
	size_t count = 0; // The number of draw calls executed
	size_t* offset = nullptr; // The offset of the vertex position NOT byte position
	size_t* size = nullptr; // number of elements to draw
	size_t* instance_cout = nullptr; // number of instances of each of these meshes
	std::string name = "MULTI DRAW MESHES";
	NODE_HINT hint = NODE_HINT::NONE;
};

// TODO: When deleting resources. You need to check that the frameGraph isn't currently using them