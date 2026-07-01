#include <stdio.h>
#include <vector>
#include "vk_helper.h"

std::vector<VkPhysicalDeviceGroupPropertiesKHR> enumerate_device_groups(VkInstance instance) {
    PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR =
        (PFN_vkEnumeratePhysicalDeviceGroupsKHR)
        vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroupsKHR");

    if (!vkEnumeratePhysicalDeviceGroupsKHR) {
        printf("VK_KHR_device_group not available\n");
        return std::vector<VkPhysicalDeviceGroupPropertiesKHR>();
    }

    uint32_t groupCount = 0;

    VkResult result = vkEnumeratePhysicalDeviceGroupsKHR(
        instance,
        &groupCount,
        NULL
    );

    if (result != VK_SUCCESS || groupCount == 0) {
        printf("No physical device groups found\n");
        return std::vector<VkPhysicalDeviceGroupPropertiesKHR>();
    }

    std::vector<VkPhysicalDeviceGroupPropertiesKHR> groups(groupCount);

    for (uint32_t i = 0; i < groupCount; i++) {
        groups[i].sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES_KHR;
        groups[i].pNext = NULL;
    }

    result = vkEnumeratePhysicalDeviceGroupsKHR(
        instance,
        &groupCount,
        groups.data()
    );

    if (result != VK_SUCCESS) {
        printf("Failed to enumerate device groups\n");
        return std::vector<VkPhysicalDeviceGroupPropertiesKHR>();
    }

    for (uint32_t i = 0; i < groupCount; i++) {
        printf("Device group %u:\n", i);
        printf("  Physical device count: %u\n", groups[i].physicalDeviceCount);
        printf("  Subset allocation: %s\n",
            groups[i].subsetAllocation ? "true" : "false");

        for (uint32_t j = 0; j < groups[i].physicalDeviceCount; j++) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(groups[i].physicalDevices[j], &props);

            printf("  Device %u: %s\n", j, props.deviceName);
        }
    }
    return groups;
}
