#include "CommandBuffer.hpp"
#include <mutex>
#include <thread>



template <Node T>
void CommandBuffer::addCommand(T cmd) {
	CommandBuffer::mtx.lock();
	size_t oldSize = CommandBuffer::commandVector.size();
	CommandBuffer::commandVector.resize(oldSize + sizeof(T));
	std::memcpy(CommandBuffer::commandVector.data() + oldSize, &cmd, sizeof(T));
	CommandBuffer::logCommand(cmd);
	CommandBuffer::mtx.unlock();
}

void CommandBuffer::logCommand(NodeMemory cmd) {
	CommandBuffer::commandOrder.push_back(NODE_MEMORY);
}

void CommandBuffer::logCommand(NodeDraw cmd) {
	CommandBuffer::commandOrder.push_back(NODE_DRAW);

}

void CommandBuffer::logCommand(NodeMultiDraw cmd) {
	CommandBuffer::commandOrder.push_back(NODE_MULTI_DRAW);
}