#include <vulkan/vulkan.h>

class Cacus {
public:
	Cacus(const char **extensionNames, size_t extensionCount);
	~Cacus();

private:
	bool checkValidationLayersSupport() const;

	VkInstance instance;
};