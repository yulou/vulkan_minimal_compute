#pragma once
#include <vulkan/vulkan.h>
#include <vector>

std::vector<VkPhysicalDeviceGroupPropertiesKHR> enumerate_device_groups(VkInstance instance);
