#include <vector>
#include <mutex>
#include "Nodes.hpp"
#include <concepts>

template <typename T>
concept Node = std::is_same<T, NodeDraw> || std::is_same<T, NodeMultiDraw> || std::is_same<T, NodeMemory>;

class CommandBuffer {
private:
	std::vector<uint8_t> commandVector = {};
	std::vector<unsigned int> commandOrder = {};
	std::mutex mtx;
public:
	void logCommand(NodeDraw cmd);
	void logCommand(NodeMemory cmd);
	void logCommand(NodeMultiDraw cmd);

	template <Node T>
	void addCommand(T cmd);
};