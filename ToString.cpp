#include "ToString.h"

#include <map>
#include <sstream>

static std::map<VkFormat, QString> sFormatStrings;

QString toStringVersion(uint32_t version)
{
  std::stringstream ss;
  ss << ((version >> 22) & 0x3FF) << ".";
  ss << ((version >> 12) & 0x3FF) << ".";
  ss << ((version >>  0) & 0xFFF);
  return QString::fromStdString(ss.str());
}

QString toStringDeviceType(VkPhysicalDeviceType type)
{
  QString result;
  switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER          : result = "VK_PHYSICAL_DEVICE_TYPE_OTHER"; break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU : result = "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU"; break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU   : result = "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU"; break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU    : result = "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU"; break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU            : result = "VK_PHYSICAL_DEVICE_TYPE_CPU"; break;
  }
  return result;
}

QString toStringVkFormat(VkFormat format)
{
  if (sFormatStrings.empty()) {
    sFormatStrings[VK_FORMAT_UNDEFINED] = "VK_FORMAT_UNDEFINED";
    sFormatStrings[VK_FORMAT_R4G4_UNORM_PACK8] = "VK_FORMAT_R4G4_UNORM_PACK8";
    sFormatStrings[VK_FORMAT_R4G4B4A4_UNORM_PACK16] = "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_B4G4R4A4_UNORM_PACK16] = "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_R5G6B5_UNORM_PACK16] = "VK_FORMAT_R5G6B5_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_B5G6R5_UNORM_PACK16] = "VK_FORMAT_B5G6R5_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_R5G5B5A1_UNORM_PACK16] = "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_B5G5R5A1_UNORM_PACK16] = "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_A1R5G5B5_UNORM_PACK16] = "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    sFormatStrings[VK_FORMAT_R8_UNORM] = "VK_FORMAT_R8_UNORM";
    sFormatStrings[VK_FORMAT_R8_SNORM] = "VK_FORMAT_R8_SNORM";
    sFormatStrings[VK_FORMAT_R8_USCALED] = "VK_FORMAT_R8_USCALED";
    sFormatStrings[VK_FORMAT_R8_SSCALED] = "VK_FORMAT_R8_SSCALED";
    sFormatStrings[VK_FORMAT_R8_UINT] = "VK_FORMAT_R8_UINT";
    sFormatStrings[VK_FORMAT_R8_SINT] = "VK_FORMAT_R8_SINT";
    sFormatStrings[VK_FORMAT_R8_SRGB] = "VK_FORMAT_R8_SRGB";
    sFormatStrings[VK_FORMAT_R8G8_UNORM] = "VK_FORMAT_R8G8_UNORM";
    sFormatStrings[VK_FORMAT_R8G8_SNORM] = "VK_FORMAT_R8G8_SNORM";
    sFormatStrings[VK_FORMAT_R8G8_USCALED] = "VK_FORMAT_R8G8_USCALED";
    sFormatStrings[VK_FORMAT_R8G8_SSCALED] = "VK_FORMAT_R8G8_SSCALED";
    sFormatStrings[VK_FORMAT_R8G8_UINT] = "VK_FORMAT_R8G8_UINT";
    sFormatStrings[VK_FORMAT_R8G8_SINT] = "VK_FORMAT_R8G8_SINT";
    sFormatStrings[VK_FORMAT_R8G8_SRGB] = "VK_FORMAT_R8G8_SRGB";
    sFormatStrings[VK_FORMAT_R8G8B8_UNORM] = "VK_FORMAT_R8G8B8_UNORM";
    sFormatStrings[VK_FORMAT_R8G8B8_SNORM] = "VK_FORMAT_R8G8B8_SNORM";
    sFormatStrings[VK_FORMAT_R8G8B8_USCALED] = "VK_FORMAT_R8G8B8_USCALED";
    sFormatStrings[VK_FORMAT_R8G8B8_SSCALED] = "VK_FORMAT_R8G8B8_SSCALED";
    sFormatStrings[VK_FORMAT_R8G8B8_UINT] = "VK_FORMAT_R8G8B8_UINT";
    sFormatStrings[VK_FORMAT_R8G8B8_SINT] = "VK_FORMAT_R8G8B8_SINT";
    sFormatStrings[VK_FORMAT_R8G8B8_SRGB] = "VK_FORMAT_R8G8B8_SRGB";
    sFormatStrings[VK_FORMAT_B8G8R8_UNORM] = "VK_FORMAT_B8G8R8_UNORM";
    sFormatStrings[VK_FORMAT_B8G8R8_SNORM] = "VK_FORMAT_B8G8R8_SNORM";
    sFormatStrings[VK_FORMAT_B8G8R8_USCALED] = "VK_FORMAT_B8G8R8_USCALED";
    sFormatStrings[VK_FORMAT_B8G8R8_SSCALED] = "VK_FORMAT_B8G8R8_SSCALED";
    sFormatStrings[VK_FORMAT_B8G8R8_UINT] = "VK_FORMAT_B8G8R8_UINT";
    sFormatStrings[VK_FORMAT_B8G8R8_SINT] = "VK_FORMAT_B8G8R8_SINT";
    sFormatStrings[VK_FORMAT_B8G8R8_SRGB] = "VK_FORMAT_B8G8R8_SRGB";
    sFormatStrings[VK_FORMAT_R8G8B8A8_UNORM] = "VK_FORMAT_R8G8B8A8_UNORM";
    sFormatStrings[VK_FORMAT_R8G8B8A8_SNORM] = "VK_FORMAT_R8G8B8A8_SNORM";
    sFormatStrings[VK_FORMAT_R8G8B8A8_USCALED] = "VK_FORMAT_R8G8B8A8_USCALED";
    sFormatStrings[VK_FORMAT_R8G8B8A8_SSCALED] = "VK_FORMAT_R8G8B8A8_SSCALED";
    sFormatStrings[VK_FORMAT_R8G8B8A8_UINT] = "VK_FORMAT_R8G8B8A8_UINT";
    sFormatStrings[VK_FORMAT_R8G8B8A8_SINT] = "VK_FORMAT_R8G8B8A8_SINT";
    sFormatStrings[VK_FORMAT_R8G8B8A8_SRGB] = "VK_FORMAT_R8G8B8A8_SRGB";
    sFormatStrings[VK_FORMAT_B8G8R8A8_UNORM] = "VK_FORMAT_B8G8R8A8_UNORM";
    sFormatStrings[VK_FORMAT_B8G8R8A8_SNORM] = "VK_FORMAT_B8G8R8A8_SNORM";
    sFormatStrings[VK_FORMAT_B8G8R8A8_USCALED] = "VK_FORMAT_B8G8R8A8_USCALED";
    sFormatStrings[VK_FORMAT_B8G8R8A8_SSCALED] = "VK_FORMAT_B8G8R8A8_SSCALED";
    sFormatStrings[VK_FORMAT_B8G8R8A8_UINT] = "VK_FORMAT_B8G8R8A8_UINT";
    sFormatStrings[VK_FORMAT_B8G8R8A8_SINT] = "VK_FORMAT_B8G8R8A8_SINT";
    sFormatStrings[VK_FORMAT_B8G8R8A8_SRGB] = "VK_FORMAT_B8G8R8A8_SRGB";
    sFormatStrings[VK_FORMAT_A8B8G8R8_UNORM_PACK32] = "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_SNORM_PACK32] = "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_USCALED_PACK32] = "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_SSCALED_PACK32] = "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_UINT_PACK32] = "VK_FORMAT_A8B8G8R8_UINT_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_SINT_PACK32] = "VK_FORMAT_A8B8G8R8_SINT_PACK32";
    sFormatStrings[VK_FORMAT_A8B8G8R8_SRGB_PACK32] = "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_UNORM_PACK32] = "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_SNORM_PACK32] = "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_USCALED_PACK32] = "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_SSCALED_PACK32] = "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_UINT_PACK32] = "VK_FORMAT_A2R10G10B10_UINT_PACK32";
    sFormatStrings[VK_FORMAT_A2R10G10B10_SINT_PACK32] = "VK_FORMAT_A2R10G10B10_SINT_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_UNORM_PACK32] = "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_SNORM_PACK32] = "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_USCALED_PACK32] = "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_SSCALED_PACK32] = "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_UINT_PACK32] = "VK_FORMAT_A2B10G10R10_UINT_PACK32";
    sFormatStrings[VK_FORMAT_A2B10G10R10_SINT_PACK32] = "VK_FORMAT_A2B10G10R10_SINT_PACK32";
    sFormatStrings[VK_FORMAT_R16_UNORM] = "VK_FORMAT_R16_UNORM";
    sFormatStrings[VK_FORMAT_R16_SNORM] = "VK_FORMAT_R16_SNORM";
    sFormatStrings[VK_FORMAT_R16_USCALED] = "VK_FORMAT_R16_USCALED";
    sFormatStrings[VK_FORMAT_R16_SSCALED] = "VK_FORMAT_R16_SSCALED";
    sFormatStrings[VK_FORMAT_R16_UINT] = "VK_FORMAT_R16_UINT";
    sFormatStrings[VK_FORMAT_R16_SINT] = "VK_FORMAT_R16_SINT";
    sFormatStrings[VK_FORMAT_R16_SFLOAT] = "VK_FORMAT_R16_SFLOAT";
    sFormatStrings[VK_FORMAT_R16G16_UNORM] = "VK_FORMAT_R16G16_UNORM";
    sFormatStrings[VK_FORMAT_R16G16_SNORM] = "VK_FORMAT_R16G16_SNORM";
    sFormatStrings[VK_FORMAT_R16G16_USCALED] = "VK_FORMAT_R16G16_USCALED";
    sFormatStrings[VK_FORMAT_R16G16_SSCALED] = "VK_FORMAT_R16G16_SSCALED";
    sFormatStrings[VK_FORMAT_R16G16_UINT] = "VK_FORMAT_R16G16_UINT";
    sFormatStrings[VK_FORMAT_R16G16_SINT] = "VK_FORMAT_R16G16_SINT";
    sFormatStrings[VK_FORMAT_R16G16_SFLOAT] = "VK_FORMAT_R16G16_SFLOAT";
    sFormatStrings[VK_FORMAT_R16G16B16_UNORM] = "VK_FORMAT_R16G16B16_UNORM";
    sFormatStrings[VK_FORMAT_R16G16B16_SNORM] = "VK_FORMAT_R16G16B16_SNORM";
    sFormatStrings[VK_FORMAT_R16G16B16_USCALED] = "VK_FORMAT_R16G16B16_USCALED";
    sFormatStrings[VK_FORMAT_R16G16B16_SSCALED] = "VK_FORMAT_R16G16B16_SSCALED";
    sFormatStrings[VK_FORMAT_R16G16B16_UINT] = "VK_FORMAT_R16G16B16_UINT";
    sFormatStrings[VK_FORMAT_R16G16B16_SINT] = "VK_FORMAT_R16G16B16_SINT";
    sFormatStrings[VK_FORMAT_R16G16B16_SFLOAT] = "VK_FORMAT_R16G16B16_SFLOAT";
    sFormatStrings[VK_FORMAT_R16G16B16A16_UNORM] = "VK_FORMAT_R16G16B16A16_UNORM";
    sFormatStrings[VK_FORMAT_R16G16B16A16_SNORM] = "VK_FORMAT_R16G16B16A16_SNORM";
    sFormatStrings[VK_FORMAT_R16G16B16A16_USCALED] = "VK_FORMAT_R16G16B16A16_USCALED";
    sFormatStrings[VK_FORMAT_R16G16B16A16_SSCALED] = "VK_FORMAT_R16G16B16A16_SSCALED";
    sFormatStrings[VK_FORMAT_R16G16B16A16_UINT] = "VK_FORMAT_R16G16B16A16_UINT";
    sFormatStrings[VK_FORMAT_R16G16B16A16_SINT] = "VK_FORMAT_R16G16B16A16_SINT";
    sFormatStrings[VK_FORMAT_R16G16B16A16_SFLOAT] = "VK_FORMAT_R16G16B16A16_SFLOAT";
    sFormatStrings[VK_FORMAT_R32_UINT] = "VK_FORMAT_R32_UINT";
    sFormatStrings[VK_FORMAT_R32_SINT] = "VK_FORMAT_R32_SINT";
    sFormatStrings[VK_FORMAT_R32_SFLOAT] = "VK_FORMAT_R32_SFLOAT";
    sFormatStrings[VK_FORMAT_R32G32_UINT] = "VK_FORMAT_R32G32_UINT";
    sFormatStrings[VK_FORMAT_R32G32_SINT] = "VK_FORMAT_R32G32_SINT";
    sFormatStrings[VK_FORMAT_R32G32_SFLOAT] = "VK_FORMAT_R32G32_SFLOAT";
    sFormatStrings[VK_FORMAT_R32G32B32_UINT] = "VK_FORMAT_R32G32B32_UINT";
    sFormatStrings[VK_FORMAT_R32G32B32_SINT] = "VK_FORMAT_R32G32B32_SINT";
    sFormatStrings[VK_FORMAT_R32G32B32_SFLOAT] = "VK_FORMAT_R32G32B32_SFLOAT";
    sFormatStrings[VK_FORMAT_R32G32B32A32_UINT] = "VK_FORMAT_R32G32B32A32_UINT";
    sFormatStrings[VK_FORMAT_R32G32B32A32_SINT] = "VK_FORMAT_R32G32B32A32_SINT";
    sFormatStrings[VK_FORMAT_R32G32B32A32_SFLOAT] = "VK_FORMAT_R32G32B32A32_SFLOAT";
    sFormatStrings[VK_FORMAT_R64_UINT] = "VK_FORMAT_R64_UINT";
    sFormatStrings[VK_FORMAT_R64_SINT] = "VK_FORMAT_R64_SINT";
    sFormatStrings[VK_FORMAT_R64_SFLOAT] = "VK_FORMAT_R64_SFLOAT";
    sFormatStrings[VK_FORMAT_R64G64_UINT] = "VK_FORMAT_R64G64_UINT";
    sFormatStrings[VK_FORMAT_R64G64_SINT] = "VK_FORMAT_R64G64_SINT";
    sFormatStrings[VK_FORMAT_R64G64_SFLOAT] = "VK_FORMAT_R64G64_SFLOAT";
    sFormatStrings[VK_FORMAT_R64G64B64_UINT] = "VK_FORMAT_R64G64B64_UINT";
    sFormatStrings[VK_FORMAT_R64G64B64_SINT] = "VK_FORMAT_R64G64B64_SINT";
    sFormatStrings[VK_FORMAT_R64G64B64_SFLOAT] = "VK_FORMAT_R64G64B64_SFLOAT";
    sFormatStrings[VK_FORMAT_R64G64B64A64_UINT] = "VK_FORMAT_R64G64B64A64_UINT";
    sFormatStrings[VK_FORMAT_R64G64B64A64_SINT] = "VK_FORMAT_R64G64B64A64_SINT";
    sFormatStrings[VK_FORMAT_R64G64B64A64_SFLOAT] = "VK_FORMAT_R64G64B64A64_SFLOAT";
    sFormatStrings[VK_FORMAT_B10G11R11_UFLOAT_PACK32] = "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
    sFormatStrings[VK_FORMAT_E5B9G9R9_UFLOAT_PACK32] = "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
    sFormatStrings[VK_FORMAT_D16_UNORM] = "VK_FORMAT_D16_UNORM";
    sFormatStrings[VK_FORMAT_X8_D24_UNORM_PACK32] = "VK_FORMAT_X8_D24_UNORM_PACK32";
    sFormatStrings[VK_FORMAT_D32_SFLOAT] = "VK_FORMAT_D32_SFLOAT";
    sFormatStrings[VK_FORMAT_S8_UINT] = "VK_FORMAT_S8_UINT";
    sFormatStrings[VK_FORMAT_D16_UNORM_S8_UINT] = "VK_FORMAT_D16_UNORM_S8_UINT";
    sFormatStrings[VK_FORMAT_D24_UNORM_S8_UINT] = "VK_FORMAT_D24_UNORM_S8_UINT";
    sFormatStrings[VK_FORMAT_D32_SFLOAT_S8_UINT] = "VK_FORMAT_D32_SFLOAT_S8_UINT";
    sFormatStrings[VK_FORMAT_BC1_RGB_UNORM_BLOCK] = "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC1_RGB_SRGB_BLOCK] = "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_BC1_RGBA_UNORM_BLOCK] = "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC1_RGBA_SRGB_BLOCK] = "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_BC2_UNORM_BLOCK] = "VK_FORMAT_BC2_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC2_SRGB_BLOCK] = "VK_FORMAT_BC2_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_BC3_UNORM_BLOCK] = "VK_FORMAT_BC3_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC3_SRGB_BLOCK] = "VK_FORMAT_BC3_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_BC4_UNORM_BLOCK] = "VK_FORMAT_BC4_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC4_SNORM_BLOCK] = "VK_FORMAT_BC4_SNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC5_UNORM_BLOCK] = "VK_FORMAT_BC5_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC5_SNORM_BLOCK] = "VK_FORMAT_BC5_SNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC6H_UFLOAT_BLOCK] = "VK_FORMAT_BC6H_UFLOAT_BLOCK";
    sFormatStrings[VK_FORMAT_BC6H_SFLOAT_BLOCK] = "VK_FORMAT_BC6H_SFLOAT_BLOCK";
    sFormatStrings[VK_FORMAT_BC7_UNORM_BLOCK] = "VK_FORMAT_BC7_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_BC7_SRGB_BLOCK] = "VK_FORMAT_BC7_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK] = "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK] = "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK] = "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK] = "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK] = "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK] = "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_EAC_R11_UNORM_BLOCK] = "VK_FORMAT_EAC_R11_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_EAC_R11_SNORM_BLOCK] = "VK_FORMAT_EAC_R11_SNORM_BLOCK";
    sFormatStrings[VK_FORMAT_EAC_R11G11_UNORM_BLOCK] = "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_EAC_R11G11_SNORM_BLOCK] = "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_4x4_UNORM_BLOCK] = "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_4x4_SRGB_BLOCK] = "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_5x4_UNORM_BLOCK] = "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_5x4_SRGB_BLOCK] = "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_5x5_UNORM_BLOCK] = "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_5x5_SRGB_BLOCK] = "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_6x5_UNORM_BLOCK] = "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_6x5_SRGB_BLOCK] = "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_6x6_UNORM_BLOCK] = "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_6x6_SRGB_BLOCK] = "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x5_UNORM_BLOCK] = "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x5_SRGB_BLOCK] = "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x6_UNORM_BLOCK] = "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x6_SRGB_BLOCK] = "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x8_UNORM_BLOCK] = "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_8x8_SRGB_BLOCK] = "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x5_UNORM_BLOCK] = "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x5_SRGB_BLOCK] = "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x6_UNORM_BLOCK] = "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x6_SRGB_BLOCK] = "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x8_UNORM_BLOCK] = "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x8_SRGB_BLOCK] = "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x10_UNORM_BLOCK] = "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_10x10_SRGB_BLOCK] = "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_12x10_UNORM_BLOCK] = "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_12x10_SRGB_BLOCK] = "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_12x12_UNORM_BLOCK] = "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
    sFormatStrings[VK_FORMAT_ASTC_12x12_SRGB_BLOCK] = "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
    sFormatStrings[VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG] = "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG] = "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG] = "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG] = "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG] = "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG] = "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG] = "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
    sFormatStrings[VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG] = "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
  }

  QString result;
  auto it = sFormatStrings.find(format);
  if (it != sFormatStrings.end()) {
    result = it->second;
  }
  return result;
}

QString toStringVkColorSpace(VkColorSpaceKHR colorSpace)
{
  QString result;
  switch (colorSpace) {
    case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: result = "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR"; break;
  }
  return result;
}

QString toStringVkPresentMode(VkPresentModeKHR mode)
{
  QString result;
  switch (mode) {
    case VK_PRESENT_MODE_IMMEDIATE_KHR    : result = "VK_PRESENT_MODE_IMMEDIATE_KHR"; break;
    case VK_PRESENT_MODE_MAILBOX_KHR      : result = "VK_PRESENT_MODE_MAILBOX_KHR"; break;
    case VK_PRESENT_MODE_FIFO_KHR         : result = "VK_PRESENT_MODE_FIFO_KHR"; break;
    case VK_PRESENT_MODE_FIFO_RELAXED_KHR : result = "VK_PRESENT_MODE_FIFO_RELAXED_KHR"; break;
  }
  return result;
}

QString toStringVkTransform(VkSurfaceTransformFlagsKHR transform)
{
  QString result;
  switch (transform) {
    case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR                     : result = "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR                    : result = "VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR                   : result = "VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR                   : result = "VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR            : result = "VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR  : result = "VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR : result = "VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR : result = "VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR"; break;
    case VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR                      : result = "VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR"; break;
  }
  return result;
}

QString toStringVkCompositeAlpha(VkCompositeAlphaFlagsKHR mode)
{
  QString result;
  switch (mode) {
    case VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR          : result = "VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR"; break;
    case VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR  : result = "VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR"; break;
    case VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR : result = "VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR"; break;
    case VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR         : result = "VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR"; break;
  }
  return result;
}

QString toStringFormatFeature(VkFormatFeatureFlagBits value)
{
  QString result;
  switch (value) {
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT               : result = "VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT"; break;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT               : result = "VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT"; break;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT        : result = "VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT"; break;
    case VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT        : result = "VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT"; break;
    case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT        : result = "VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT"; break;
    case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT : result = "VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT"; break;
    case VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT               : result = "VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT"; break;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT            : result = "VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT"; break;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT      : result = "VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT"; break;
    case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT    : result = "VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT"; break;
    case VK_FORMAT_FEATURE_BLIT_SRC_BIT                    : result = "VK_FORMAT_FEATURE_BLIT_SRC_BIT"; break;
    case VK_FORMAT_FEATURE_BLIT_DST_BIT                    : result = "VK_FORMAT_FEATURE_BLIT_DST_BIT"; break;
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT : result = "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT"; break;
    case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR            : result = "VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR"; break;
    case VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR            : result = "VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR"; break;
  }
  return result;
}

QString toStringFormatFeatureShort(VkFormatFeatureFlagBits value)
{
  QString result;
  switch (value) {
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT               : result = "SAMPLED_IMAGE"; break;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT               : result = "STORAGE_IMAGE"; break;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT        : result = "STORAGE_IMAGE_ATOMIC"; break;
    case VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT        : result = "UNIFORM_TEXEL_BUFFER"; break;
    case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT        : result = "STORAGE_TEXEL_BUFFER"; break;
    case VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT : result = "STORAGE_TEXEL_BUFFER_ATOMIC"; break;
    case VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT               : result = "VERTEX_BUFFER"; break;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT            : result = "COLOR_ATTACHMENT"; break;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT      : result = "COLOR_ATTACHMENT_BLEND"; break;
    case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT    : result = "DEPTH_STENCIL_ATTACHMENT"; break;
    case VK_FORMAT_FEATURE_BLIT_SRC_BIT                    : result = "BLIT_SRC"; break;
    case VK_FORMAT_FEATURE_BLIT_DST_BIT                    : result = "BLIT_DST"; break;
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT : result = "SAMPLED_IMAGE_FILTER_LINEAR"; break;
    case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR            : result = "TRANSFER_SRC"; break;
    case VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR            : result = "TRANSFER_DST"; break;
  }
  return result;
}

QString toStringImageTypeShort(VkImageType value)
{
  QString result;
  switch (value) {
    case VK_IMAGE_TYPE_1D : result = "1D"; break;
    case VK_IMAGE_TYPE_2D : result = "2D"; break;
    case VK_IMAGE_TYPE_3D : result = "3D"; break;
  }
  return result;
}

QString toStringImageUsageFlagShort(VkImageUsageFlagBits value)
{
  QString result;
  switch (value) {
    case VK_IMAGE_USAGE_TRANSFER_SRC_BIT             : result = "TRANSFER_SRC"; break;
    case VK_IMAGE_USAGE_TRANSFER_DST_BIT             : result = "TRANSFER_DST"; break;
    case VK_IMAGE_USAGE_SAMPLED_BIT                  : result = "SAMPLED"; break;
    case VK_IMAGE_USAGE_STORAGE_BIT                  : result = "STORAGE"; break;
    case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT         : result = "COLOR_ATTACHMENT"; break;
    case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : result = "DEPTH_STENCIL_ATTACHMENT"; break;
    case VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT     : result = "TRANSIENT_ATTACHMENT"; break;
    case VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT         : result = "INPUT_ATTACHMENT"; break;
  }
  return result;
}

QString toStringSampleCounts(VkSampleCountFlags value)
{
  QString result;
  bool append = false;
  if (value & VK_SAMPLE_COUNT_1_BIT) {
    result += "1";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_2_BIT) {
    result += append ? " / " : "";
    result += "2";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_4_BIT) {
    result += append ? " / " : "";
    result += "4";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_8_BIT) {
    result += append ? " / " : "";
    result += "8";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_16_BIT) {
    result += append ? " / " : "";
    result += "16";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_32_BIT) {
    result += append ? " / " : "";
    result += "32";
    append = true;
  }
  if (value & VK_SAMPLE_COUNT_64_BIT) {
    result += append ? " / " : "";
    result += "64";
  }
  return result;
}

