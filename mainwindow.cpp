#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ToString.h"

#include <Windows.h>

#include <cassert>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  createVulkanInstance();
  createVulkanSurface();
  enumerateGpus();
  populateGpus();

}

MainWindow::~MainWindow()
{
  destroyVulkanSurface();
  destroyVulkanInstance();

  delete ui;
}

void MainWindow::createVulkanInstance()
{
  enumerateInstanceLayers();
  populateInstanceLayers();

  enumerateInstanceExtensions();
  populateInstanceExtensions();

  VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  appInfo.pApplicationName    = "Vulkan Info Viewer";
  appInfo.applicationVersion  = 1;
  appInfo.pEngineName         = "Vulkan Info Viewer";
  appInfo.engineVersion       = 1;
  appInfo.apiVersion          = VK_MAKE_VERSION(1, 0, 3);

  const auto& layerExtensions = mInstanceLayerExtensions[""];
  std::vector<const char*> extensions;
  for (const auto& ext : layerExtensions) {
    extensions.push_back(ext.extensionName);
  }

  VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  createInfo.pApplicationInfo         = &appInfo;
  createInfo.enabledExtensionCount    = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames  = extensions.empty() ? nullptr : extensions.data();
  VkResult res = vkCreateInstance(&createInfo, nullptr, &mVkInstance);
  assert(res == VK_SUCCESS);
}

void MainWindow::destroyVulkanInstance()
{
  if (mVkInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mVkInstance, nullptr);
    mVkInstance = VK_NULL_HANDLE;
  }
}

void MainWindow::createVulkanSurface()
{
  VkWin32SurfaceCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
  createInfo.hinstance = ::GetModuleHandle(nullptr);
  createInfo.hwnd      = (HWND)this->winId();
  VkResult res = vkCreateWin32SurfaceKHR(mVkInstance, &createInfo, nullptr, &mVkSurface);
  assert(res == VK_SUCCESS);
}

void MainWindow::destroyVulkanSurface()
{

}

void MainWindow::enumerateInstanceLayers()
{
  mInstanceLayers.clear();

  uint32_t count = 0;
  VkResult res = vkEnumerateInstanceLayerProperties(&count, nullptr);
  assert(res == VK_SUCCESS);

  mInstanceLayers.resize(count);
  res = vkEnumerateInstanceLayerProperties(&count, mInstanceLayers.data());
  assert(res == VK_SUCCESS);
}

void MainWindow::populateInstanceLayers()
{  
  QTreeWidget* tw = findChild<QTreeWidget*>("layersWidget");
  Q_ASSERT(tw);

  tw->clear();

  for (const auto& layer : mInstanceLayers) {
    std::stringstream ss;
    ss << ((layer.specVersion >> 22) & 0x3FF) << ".";
    ss << ((layer.specVersion >> 12) & 0x3FF) << ".";
    ss << ((layer.specVersion >>  0) & 0xFFF);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::fromUtf8(layer.layerName));
    item->setText(1, QString::fromStdString(ss.str()));
    item->setText(2, QString::number(layer.implementationVersion));
    item->setText(3, QString::fromUtf8(layer.description));
    tw->addTopLevelItem(item);
  }

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void enumerateInstanceExtensions(const char *layerName, std::vector<VkExtensionProperties>* extensions)
{
  uint32_t count = 0;
  VkResult res = vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
  assert(res == VK_SUCCESS);

  extensions->resize(count);
  res = vkEnumerateInstanceExtensionProperties(layerName, &count, extensions->data());
}

void MainWindow::enumerateInstanceExtensions()
{
  mInstanceLayerExtensions.clear();

  for(const auto& layer : mInstanceLayers) {
    std::string layerName = layer.layerName;
    std::vector<VkExtensionProperties> extensions;
    ::enumerateInstanceExtensions(layerName.c_str(), &extensions);
    if (! extensions.empty()) {
      mInstanceLayerExtensions[layerName] = extensions;
    }
  }

  std::vector<VkExtensionProperties> extensions;
  ::enumerateInstanceExtensions(nullptr, &extensions);
  if (! extensions.empty()) {
    mInstanceLayerExtensions[""] = extensions;
  }
}

//void MainWindow::scanInstanceExtensions(const std::vector<VkLayerProperties>& layers, std::map<std::string, std::vector<VkExtensionProperties>>* layerExtensions)
//{
//  for(const auto& layer : layers) {
//    std::string layerName = layer.layerName;
//    std::vector<VkExtensionProperties> extensions;
//    enumerateInstanceExtensions(layerName.c_str(), &extensions);
//    if (! extensions.empty()) {
//      (*layerExtensions)[layerName] = extensions;
//    }
//  }

//  std::vector<VkExtensionProperties> extensions;
//  enumerateInstanceExtensions(nullptr, &extensions);
//  (*layerExtensions)[""] = extensions;
//}

void MainWindow::populateInstanceExtensions()
{
  QTreeWidget* tw = findChild<QTreeWidget*>("extensionsWidget");
  Q_ASSERT(tw);

  for (const auto& it : mInstanceLayerExtensions) {
    const auto& layerName = it.first;
    const auto& extensions = it.second;

    QTreeWidgetItem* topItem = new QTreeWidgetItem();
    if (! layerName.empty()) {
      topItem->setText(0, QString::fromStdString(layerName));
    }
    else {
      topItem->setText(0, "Vulkan Extensions");
    }
    tw->addTopLevelItem(topItem);

    for (const auto& ext : extensions) {
      QTreeWidgetItem* item = new QTreeWidgetItem();
      item->setText(0, QString::fromUtf8(ext.extensionName));
      item->setText(1, QString::number(ext.specVersion));
      topItem->addChild(item);
    }
  }

  tw->expandAll();

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::enumerateGpus()
{
  uint32_t count = 0;
  VkResult res = vkEnumeratePhysicalDevices(mVkInstance, &count, nullptr);
  assert(res == VK_SUCCESS);
  mVkGpus.resize(count);
  res = vkEnumeratePhysicalDevices(mVkInstance, &count, mVkGpus.data());
  assert(res == VK_SUCCESS);

  for (uint32_t i = 0; i < count; ++i) {
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(mVkGpus[i], &properties);
    mVkGpuProperties[mVkGpus[i]] = properties;
  }
}

void MainWindow::populateGpus()
{
  QComboBox* cb = findChild<QComboBox*>("gpus");
  Q_ASSERT(cb);

  cb->clear();

  for (const auto& it : mVkGpuProperties) {
    const auto& properties = it.second;
    cb->addItem(QString::fromUtf8(properties.deviceName));
  }
}

#define ADD_LIMIT(tw, limits, prop)                 \
  {                                                 \
    QTreeWidgetItem* item = new QTreeWidgetItem();  \
    item->setText(0, QString::fromUtf8(#prop));     \
    item->setText(1, QString::number(limits.prop)); \
    tw->addTopLevelItem(item);                      \
  }

void MainWindow::populateLimits(VkPhysicalDevice gpu)
{
  assert(gpu != VK_NULL_HANDLE);

  QTreeWidget* tw = findChild<QTreeWidget*>("limitsWidget");
  Q_ASSERT(tw);

  tw->clear();

  if (mVkGpuProperties.find(gpu) == mVkGpuProperties.end()) {
    return;
  }

  const auto &limits = mVkGpuProperties[gpu].limits;

  ADD_LIMIT(tw, limits, maxImageDimension1D);
  ADD_LIMIT(tw, limits, maxImageDimension2D);
  ADD_LIMIT(tw, limits, maxImageDimension3D);
  ADD_LIMIT(tw, limits, maxImageDimensionCube);
  ADD_LIMIT(tw, limits, maxImageArrayLayers);
  ADD_LIMIT(tw, limits, maxTexelBufferElements);
  ADD_LIMIT(tw, limits, maxUniformBufferRange);
  ADD_LIMIT(tw, limits, maxStorageBufferRange);
  ADD_LIMIT(tw, limits, maxPushConstantsSize);
  ADD_LIMIT(tw, limits, maxMemoryAllocationCount);
  ADD_LIMIT(tw, limits, maxSamplerAllocationCount);
  ADD_LIMIT(tw, limits, bufferImageGranularity);
  ADD_LIMIT(tw, limits, sparseAddressSpaceSize);
  ADD_LIMIT(tw, limits, maxBoundDescriptorSets);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorSamplers);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorUniformBuffers);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorStorageBuffers);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorSampledImages);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorStorageImages);
  ADD_LIMIT(tw, limits, maxPerStageDescriptorInputAttachments);
  ADD_LIMIT(tw, limits, maxPerStageResources);
  ADD_LIMIT(tw, limits, maxDescriptorSetSamplers);
  ADD_LIMIT(tw, limits, maxDescriptorSetUniformBuffers);
  ADD_LIMIT(tw, limits, maxDescriptorSetUniformBuffersDynamic);
  ADD_LIMIT(tw, limits, maxDescriptorSetStorageBuffers);
  ADD_LIMIT(tw, limits, maxDescriptorSetStorageBuffersDynamic);
  ADD_LIMIT(tw, limits, maxDescriptorSetSampledImages);
  ADD_LIMIT(tw, limits, maxDescriptorSetStorageImages);
  ADD_LIMIT(tw, limits, maxDescriptorSetInputAttachments);
  ADD_LIMIT(tw, limits, maxVertexInputAttributes);
  ADD_LIMIT(tw, limits, maxVertexInputBindings);
  ADD_LIMIT(tw, limits, maxVertexInputAttributeOffset);
  ADD_LIMIT(tw, limits, maxVertexInputBindingStride);
  ADD_LIMIT(tw, limits, maxVertexOutputComponents);
  ADD_LIMIT(tw, limits, maxTessellationGenerationLevel);
  ADD_LIMIT(tw, limits, maxTessellationPatchSize);
  ADD_LIMIT(tw, limits, maxTessellationControlPerVertexInputComponents);
  ADD_LIMIT(tw, limits, maxTessellationControlPerVertexOutputComponents);
  ADD_LIMIT(tw, limits, maxTessellationControlPerPatchOutputComponents);
  ADD_LIMIT(tw, limits, maxTessellationControlTotalOutputComponents);
  ADD_LIMIT(tw, limits, maxTessellationEvaluationInputComponents);
  ADD_LIMIT(tw, limits, maxTessellationEvaluationOutputComponents);
  ADD_LIMIT(tw, limits, maxGeometryShaderInvocations);
  ADD_LIMIT(tw, limits, maxGeometryInputComponents);
  ADD_LIMIT(tw, limits, maxGeometryOutputComponents);
  ADD_LIMIT(tw, limits, maxGeometryOutputVertices);
  ADD_LIMIT(tw, limits, maxGeometryTotalOutputComponents);
  ADD_LIMIT(tw, limits, maxFragmentInputComponents);
  ADD_LIMIT(tw, limits, maxFragmentOutputAttachments);
  ADD_LIMIT(tw, limits, maxFragmentDualSrcAttachments);
  ADD_LIMIT(tw, limits, maxFragmentCombinedOutputResources);
  ADD_LIMIT(tw, limits, maxComputeSharedMemorySize);
  ADD_LIMIT(tw, limits, maxComputeWorkGroupCount[3]);
  ADD_LIMIT(tw, limits, maxComputeWorkGroupInvocations);
  ADD_LIMIT(tw, limits, maxComputeWorkGroupSize[3]);
  ADD_LIMIT(tw, limits, subPixelPrecisionBits);
  ADD_LIMIT(tw, limits, subTexelPrecisionBits);
  ADD_LIMIT(tw, limits, mipmapPrecisionBits);
  ADD_LIMIT(tw, limits, maxDrawIndexedIndexValue);
  ADD_LIMIT(tw, limits, maxDrawIndirectCount);
  ADD_LIMIT(tw, limits, maxSamplerLodBias);
  ADD_LIMIT(tw, limits, maxSamplerAnisotropy);
  ADD_LIMIT(tw, limits, maxViewports);
  ADD_LIMIT(tw, limits, maxViewportDimensions[2]);
  ADD_LIMIT(tw, limits, viewportBoundsRange[2]);
  ADD_LIMIT(tw, limits, viewportSubPixelBits);
  ADD_LIMIT(tw, limits, minMemoryMapAlignment);
  ADD_LIMIT(tw, limits, minTexelBufferOffsetAlignment);
  ADD_LIMIT(tw, limits, minUniformBufferOffsetAlignment);
  ADD_LIMIT(tw, limits, minStorageBufferOffsetAlignment);
  ADD_LIMIT(tw, limits, minTexelOffset);
  ADD_LIMIT(tw, limits, maxTexelOffset);
  ADD_LIMIT(tw, limits, minTexelGatherOffset);
  ADD_LIMIT(tw, limits, maxTexelGatherOffset);
  ADD_LIMIT(tw, limits, minInterpolationOffset);
  ADD_LIMIT(tw, limits, maxInterpolationOffset);
  ADD_LIMIT(tw, limits, subPixelInterpolationOffsetBits);
  ADD_LIMIT(tw, limits, maxFramebufferWidth);
  ADD_LIMIT(tw, limits, maxFramebufferHeight);
  ADD_LIMIT(tw, limits, maxFramebufferLayers);
  ADD_LIMIT(tw, limits, framebufferColorSampleCounts);
  ADD_LIMIT(tw, limits, framebufferDepthSampleCounts);
  ADD_LIMIT(tw, limits, framebufferStencilSampleCounts);
  ADD_LIMIT(tw, limits, framebufferNoAttachmentsSampleCounts);
  ADD_LIMIT(tw, limits, maxColorAttachments);
  ADD_LIMIT(tw, limits, sampledImageColorSampleCounts);
  ADD_LIMIT(tw, limits, sampledImageIntegerSampleCounts);
  ADD_LIMIT(tw, limits, sampledImageDepthSampleCounts);
  ADD_LIMIT(tw, limits, sampledImageStencilSampleCounts);
  ADD_LIMIT(tw, limits, storageImageSampleCounts);
  ADD_LIMIT(tw, limits, maxSampleMaskWords);
  ADD_LIMIT(tw, limits, timestampComputeAndGraphics);
  ADD_LIMIT(tw, limits, timestampPeriod);
  ADD_LIMIT(tw, limits, maxClipDistances);
  ADD_LIMIT(tw, limits, maxCullDistances);
  ADD_LIMIT(tw, limits, maxCombinedClipAndCullDistances);
  ADD_LIMIT(tw, limits, discreteQueuePriorities);
  ADD_LIMIT(tw, limits, pointSizeRange[0]);
  ADD_LIMIT(tw, limits, pointSizeRange[1]);
  ADD_LIMIT(tw, limits, lineWidthRange[0]);
  ADD_LIMIT(tw, limits, lineWidthRange[1]);
  ADD_LIMIT(tw, limits, pointSizeGranularity);
  ADD_LIMIT(tw, limits, lineWidthGranularity);
  ADD_LIMIT(tw, limits, strictLines);
  ADD_LIMIT(tw, limits, standardSampleLocations);
  ADD_LIMIT(tw, limits, optimalBufferCopyOffsetAlignment);
  ADD_LIMIT(tw, limits, optimalBufferCopyRowPitchAlignment);
  ADD_LIMIT(tw, limits, nonCoherentAtomSize);

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::populateSurface(VkPhysicalDevice gpu)
{
  VkSurfaceCapabilitiesKHR surfCaps = {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, mVkSurface, &surfCaps);

  uint32_t count = 0;
  VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mVkSurface, &count, nullptr);
  assert(res == VK_SUCCESS);
  std::vector<VkSurfaceFormatKHR> formats(count);
  res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mVkSurface, &count, formats.data());
  assert(res == VK_SUCCESS);

  QLabel* lb = findChild<QLabel*>("minImageCountValue");
  Q_ASSERT(lb);
  lb->setText(QString::number(surfCaps.minImageCount));
  lb = findChild<QLabel*>("maxImageCountValue");
  Q_ASSERT(lb);
  lb->setText(QString::number(surfCaps.maxImageCount));

  res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, mVkSurface, &count, nullptr);
  assert(res == VK_SUCCESS);
  std::vector<VkPresentModeKHR> presentModes(count);
  res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, mVkSurface, &count, presentModes.data());
  assert(res == VK_SUCCESS);
  QTreeWidget* tw = findChild<QTreeWidget*>("presentModesWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (const auto& mode : presentModes) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkPresentMode(mode));
    tw->addTopLevelItem(item);
  }

  tw = findChild<QTreeWidget*>("surfaceFormatsWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (const auto& format : formats) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkFormat(format.format));
    item->setText(1, toStringVkColorSpace(format.colorSpace));
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::populateQueues(VkPhysicalDevice gpu)
{
  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, nullptr);
  std::vector<VkQueueFamilyProperties> properties(count);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &count, properties.data());

  QTreeWidget* tw = findChild<QTreeWidget*>("queuesWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (size_t i = 0; i < properties.size(); ++i) {
    VkBool32 presents = VK_FALSE;
    VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, static_cast<uint32_t>(i), mVkSurface, &presents);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::number(i));
    item->setText(1, QString::number(properties[i].queueCount));
    item->setText(2, (presents == VK_TRUE) ? "Y" : " ");
    item->setText(3, ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) ? "Y" : " ");
    item->setText(4, ((properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) ? "Y" : " ");
    item->setText(5, ((properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) ? "Y" : " ");
    item->setText(6, ((properties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) ? "Y" : " ");
    item->setText(7, " ");
    for (int c = 0; c < item->columnCount(); ++c) {
      item->setTextAlignment(c, Qt::AlignHCenter);
    }
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::populateFormats(VkPhysicalDevice gpu)
{
  QTreeWidget* tw = findChild<QTreeWidget*>("formatsWidget");
  Q_ASSERT(tw);
  tw->clear();

  uint32_t start = static_cast<uint32_t>(VK_FORMAT_BEGIN_RANGE) + 1;
  uint32_t end = static_cast<uint32_t>(VK_FORMAT_END_RANGE);
  for (uint32_t i = start; i <= end; ++i) {
    VkFormat format = static_cast<VkFormat>(i);
    VkFormatProperties properties = {};
    vkGetPhysicalDeviceFormatProperties(gpu, format, &properties);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkFormat(format));
    item->setText(1, (properties.linearTilingFeatures != 0) ? "Y" : " ");
    item->setText(2, (properties.optimalTilingFeatures != 0) ? "Y" : " ");
    item->setText(3, (properties.bufferFeatures != 0) ? "Y" : " ");
    for (int c = 1; c < item->columnCount(); ++c) {
      item->setTextAlignment(c, Qt::AlignHCenter);
    }
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::on_gpus_currentIndexChanged(int index)
{
  if (static_cast<size_t>(index) >= mVkGpus.size()) {
    return;
  }

  VkPhysicalDevice gpu = mVkGpus[index];
  populateLimits(gpu);
  populateSurface(gpu);
  populateQueues(gpu);
  populateFormats(gpu);
}

void MainWindow::on_limitsFilter_textChanged(const QString &arg1)
{
  QTreeWidget* tw = findChild<QTreeWidget*>("limitsWidget");
  Q_ASSERT(tw);

  QString filterText = arg1;
  if (! filterText.isEmpty()) {
    for (int i = 0; i < tw->topLevelItemCount(); ++i) {
      auto item = tw->topLevelItem(i);
      auto text = item->text(0);
      bool visible = text.contains(filterText, Qt::CaseInsensitive);
      item->setHidden(! visible);
    }
  }
  else {
    for (int i = 0; i < tw->topLevelItemCount(); ++i) {
      tw->topLevelItem(i)->setHidden(false);
    }
  }
}

void MainWindow::on_formatFilter_textChanged(const QString &arg1)
{
  QTreeWidget* tw = findChild<QTreeWidget*>("formatsWidget");
  Q_ASSERT(tw);

  QString filterText = arg1;
  if (! filterText.isEmpty()) {
    for (int i = 0; i < tw->topLevelItemCount(); ++i) {
      auto item = tw->topLevelItem(i);
      auto text = item->text(0);
      bool visible = text.contains(filterText, Qt::CaseInsensitive);
      item->setHidden(! visible);
    }
  }
  else {
    for (int i = 0; i < tw->topLevelItemCount(); ++i) {
      tw->topLevelItem(i)->setHidden(false);
    }
  }
}
