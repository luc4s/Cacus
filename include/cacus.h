#include <vulkan/vulkan.h>

class Cacus {
public:

  /**
   * Default constructor
   */
  Cacus();

  /**
   * @param extensionNames Array of required extensions
   * @param extensionCount Number of extensions
   * @throw Error if extensions are not supported
   */
  Cacus(const char **extensionNames, size_t extensionCount);

  ~Cacus();

private:
  bool checkValidationLayersSupport() const;
  void createInstance();

  VkInstance instance;
};