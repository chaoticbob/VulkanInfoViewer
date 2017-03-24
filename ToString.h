#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include <QString>

#include <vulkan/vulkan.h>

QString toStringVkFormat(VkFormat format);
QString toStringVkColorSpace(VkColorSpaceKHR colorSpace);

#endif // __TO_STRING_H__
