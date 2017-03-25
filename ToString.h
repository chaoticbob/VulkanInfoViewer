#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include <QString>

#include <vulkan/vulkan.h>

QString toStringVersion(uint32_t version);
QString toStringDeviceType(VkPhysicalDeviceType type);
QString toStringVkFormat(VkFormat format);
QString toStringVkColorSpace(VkColorSpaceKHR colorSpace);
QString toStringVkPresentMode(VkPresentModeKHR mode);
QString toStringFormatFeature(VkFormatFeatureFlagBits value);

QString toStringFormatFeatureShort(VkFormatFeatureFlagBits value);
QString toStringImageTypeShort(VkImageType value);
QString toStringImageUsageFlagShort(VkImageUsageFlagBits value);

QString toStringSampleCounts(VkSampleCountFlags value);

#endif // __TO_STRING_H__
