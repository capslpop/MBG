#pragma once

#include <vector>

#include "Nodes.hpp"

class FrameGraph {
private:
	enum NODE_TYPE {
		NODE_MEMORY_READ,
		NODE_MEMORY_WRITE,
		NODE_DRAW,
		NODE_MULTI_DRAW,
	};

public:
	FrameGraph();
	~FrameGraph();

	/*
		These are all of the memory syncronization nodes
	*/
	void addNode(NodeMemory node);
	void addNode(NodeMemoryWrite node);
	void addNode(NodeDraw node);
	void addNode(NodeMultiDraw node);

	/*
		This is the most complex function of the entire project!
		It takes all of the data and information that we have been gathering this whole time
		and compiles the instructions into raw openGL code

		Since it is only openGL the only optimisation we do is remove dupicate
		binding operations

		Render Pass
		1.) When there is a render pass we must bind all of the required resources
		2.) Then run the correct draw command with the correct draw properties
		
		Memory Sync
		1.) Bind the resource
		2.) Send the data

		Finally once we have linked all of the nodes in openGL
		we compile them inside of a vector and the deduplicate 
		the extra binding instruction -- this is technicially
		opptional so we will save this to the last thing we do
	*/
	void compile();

	/*
		Now we just itterate through that vector we created 
		and run each of the openGL commands!
	*/
	void run();
	
	/*
		This function renders all of the fram graph info to the screen
		with there timing and structure. When the frame graph updates so
		does the UI. This should use ImGUI to render everything to the screen
	*/
	void renderDebugInfo();

private:
	void threadSafePushToCommandBuffer(NODE_TYPE node_type, size_t byte_size, void* ptr); // This copies the current struct into the command buffer

	std::vector<uint8_t> bytes;
};
