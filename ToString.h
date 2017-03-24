#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include <QString>

#include <vulkan/vulkan.h>

QString toStringVkFormat(VkFormat format);
QString toStringVkColorSpace(VkColorSpaceKHR colorSpace);
QString toStringVkPresentMode(VkPresentModeKHR mode);

#endif // __TO_STRING_H__
