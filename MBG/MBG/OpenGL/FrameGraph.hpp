#pragma once

#include <vector>

#include "Info.hpp"
#include "CommandBuffer.hpp"
#include "RenderPass.hpp"

#include "Memory/DescriptorSetBuffer.hpp"
#include "Memory/IndirectDrawBuffer.hpp"

#include "Window.hpp"

namespace MBG {

//------------------------------------------------------------
// FRAME GRAPH
//	these are all the nodes that the frame graph will use
//------------------------------------------------------------

/*
struct NodeMemoryWriteTexture2D {
	Texture2DBuffer* GPU_Memory = nullptr;	// GPU memory pointer
	void* data = nullptr;					// CPU memory pointer
	uint size = 0;							// Size in bytes
	std::string name = "Write 2D Texture";	// Name of node
};
*/

class Window;
class RenderPass;
class DescriptorSetBuffer;
class IndirectDrawBuffer;

struct NodeDraw {
	RenderPass* render_pass = nullptr;					// Shader pass that should be used
	const RenderStates render_states{};						// All the current rendering states
	DescriptorSetBuffer* descriptor_set = nullptr;		// All of the uniform data that is to be used by the shader
	uint draw_count = 1;								// Number of times this draw is executed
	std::string name = "Draw";							// Name of node
};

struct NodeMultiDraw {
	RenderPass* render_pass = nullptr;					// Shader pass that should be used
	const RenderStates render_states{};						// All the current rendering states
	DescriptorSetBuffer* descriptor_set = nullptr;		// All of the uniform data that is to be used by the shader
	IndirectDrawBuffer* indirect_draw_buffer = nullptr;	// All offsets into the vertex data [In the case that multiple vaos are bound we itterate through all of them]
	std::string name = "Multi Draw";					// Name of node
};

class FrameGraph {
public:
	FrameGraph(Window& window) : window_(&window) {}
	~FrameGraph() = default;

public:
	//void addNode(const NodeMemoryWriteTexture2D& node);

	void addNode(const NodeDraw& node); // Draw geometry
	void addNodeDisplay(); // Display to window

	void build(); // Build the command buffer
	void run(); // Runs all the commands you have acumulated [ONLY RUNS ONE FRAME! WHICH ENDS WITH CALLING DISPLAY]
	
	void renderRealTimeGraph(); // Renders the current frame's pipeline to the screen with IMGUI

private:
	CommandBuffer command_buffer_;
	//CommandBuffer compiled_command_buffer_;
	Window* window_;
	
	void bindRenderStates(const RenderStates& s, uvec2 frame_size);
};

}
