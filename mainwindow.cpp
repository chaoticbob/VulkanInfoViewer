#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ToString.h"

#include <Windows.h>

#include <cassert>
#include <sstream>

#include <QStandardItemModel>

void HideItem(int row, QComboBox* cb)
{
  cb->setItemData(row, QSize(0,0), Qt::SizeHintRole);
}

void MakeCheckable(QComboBox* cb)
{
  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(cb->model());
  int n = cb->count();
  for (int i = 1; i < n; ++i) {
    auto item = model->item(i);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Unchecked, Qt::CheckStateRole);
  }
}

//! \class MainWindow
//!
//!
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  mTilingLinearFilterInputs.tiling            = VK_IMAGE_TILING_LINEAR;
  mTilingLinearFilterInputs.formatFilter      = findChild<QLineEdit*>("tilingLinearFormatFilter");
  mTilingLinearFilterInputs.imageTypeFilter   = findChild<QComboBox*>("tilingLinearImageType");
  mTilingLinearFilterInputs.usageFlagsFilter  = findChild<QComboBox*>("tilingLinearImageUsage");
  mTilingLinearFilterInputs.createFlagsFilter = findChild<QComboBox*>("tilingLinearImageCreate");
  mTilingLinearFilterInputs.target            = findChild<QTreeWidget*>("tilingLinearFormatsWidget");
  Q_ASSERT(mTilingLinearFilterInputs.formatFilter);
  Q_ASSERT(mTilingLinearFilterInputs.imageTypeFilter);
  Q_ASSERT(mTilingLinearFilterInputs.usageFlagsFilter);
  Q_ASSERT(mTilingLinearFilterInputs.createFlagsFilter);
  mFilterInputTargets[mTilingLinearFilterInputs.imageTypeFilter->model()] = &mTilingLinearFilterInputs;
  mFilterInputTargets[mTilingLinearFilterInputs.usageFlagsFilter->model()] = &mTilingLinearFilterInputs;
  mFilterInputTargets[mTilingLinearFilterInputs.createFlagsFilter->model()] = &mTilingLinearFilterInputs;

  mTilingOptimalFilterInputs.tiling            = VK_IMAGE_TILING_OPTIMAL;
  mTilingOptimalFilterInputs.formatFilter      = findChild<QLineEdit*>("tilingOptimalFormatFilter");
  mTilingOptimalFilterInputs.imageTypeFilter   = findChild<QComboBox*>("tilingOptimalImageType");
  mTilingOptimalFilterInputs.usageFlagsFilter  = findChild<QComboBox*>("tilingOptimalImageUsage");
  mTilingOptimalFilterInputs.createFlagsFilter = findChild<QComboBox*>("tilingOptimalImageCreate");
  mTilingOptimalFilterInputs.target            = findChild<QTreeWidget*>("tilingOptimalFormatsWidget");
  Q_ASSERT(mTilingOptimalFilterInputs.formatFilter);
  Q_ASSERT(mTilingOptimalFilterInputs.imageTypeFilter);
  Q_ASSERT(mTilingOptimalFilterInputs.usageFlagsFilter);
  Q_ASSERT(mTilingOptimalFilterInputs.createFlagsFilter);
  mFilterInputTargets[mTilingOptimalFilterInputs.imageTypeFilter->model()] = &mTilingOptimalFilterInputs;
  mFilterInputTargets[mTilingOptimalFilterInputs.usageFlagsFilter->model()] = &mTilingOptimalFilterInputs;
  mFilterInputTargets[mTilingOptimalFilterInputs.createFlagsFilter->model()] = &mTilingOptimalFilterInputs;

  // Hide the first item
  HideItem(0,  mTilingLinearFilterInputs.usageFlagsFilter);
  HideItem(0,  mTilingLinearFilterInputs.createFlagsFilter);
  HideItem(0,  mTilingOptimalFilterInputs.usageFlagsFilter);
  HideItem(0,  mTilingOptimalFilterInputs.createFlagsFilter);

  // Make items checkable
  MakeCheckable(mTilingLinearFilterInputs.usageFlagsFilter);
  MakeCheckable(mTilingLinearFilterInputs.createFlagsFilter);
  MakeCheckable(mTilingOptimalFilterInputs.usageFlagsFilter);
  MakeCheckable(mTilingOptimalFilterInputs.createFlagsFilter);

  // Connect the check events to MainWindow::on_itemChanged
  connect(mTilingLinearFilterInputs.usageFlagsFilter->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_itemChanged(QStandardItem*)));
  connect(mTilingLinearFilterInputs.createFlagsFilter->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_itemChanged(QStandardItem*)));
  connect(mTilingOptimalFilterInputs.usageFlagsFilter->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_itemChanged(QStandardItem*)));
  connect(mTilingOptimalFilterInputs.createFlagsFilter->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(on_itemChanged(QStandardItem*)));

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
  VkResult res = vkCreateInstance(&createInfo, nullptr, &mInstance);
  assert(res == VK_SUCCESS);
}

void MainWindow::destroyVulkanInstance()
{
  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }
}

void MainWindow::createVulkanSurface()
{
  VkWin32SurfaceCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
  createInfo.hinstance = ::GetModuleHandle(nullptr);
  createInfo.hwnd      = (HWND)this->winId();
  VkResult res = vkCreateWin32SurfaceKHR(mInstance, &createInfo, nullptr, &mSurface);
  assert(res == VK_SUCCESS);
}

void MainWindow::destroyVulkanSurface()
{
  if (mSurface == VK_NULL_HANDLE) {
    return;
  }

  vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
  mSurface = VK_NULL_HANDLE;
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
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::fromUtf8(layer.layerName));
    item->setText(1, toStringVersion(layer.specVersion));
    item->setText(2, QString::number(layer.implementationVersion));
    item->setText(3, QString::fromUtf8(layer.description));
    item->setTextAlignment(1, Qt::AlignHCenter);              \
    item->setTextAlignment(2, Qt::AlignHCenter);
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
      item->setTextAlignment(1, Qt::AlignHCenter);              \
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
  VkResult res = vkEnumeratePhysicalDevices(mInstance, &count, nullptr);
  assert(res == VK_SUCCESS);
  mGpus.resize(count);
  res = vkEnumeratePhysicalDevices(mInstance, &count, mGpus.data());
  assert(res == VK_SUCCESS);

  for (uint32_t i = 0; i < count; ++i) {
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(mGpus[i], &properties);
    mGpuProperties[mGpus[i]] = properties;
  }
}

void MainWindow::populateGpus()
{
  QComboBox* cb = findChild<QComboBox*>("gpus");
  Q_ASSERT(cb);

  cb->clear();

  for (const auto& it : mGpuProperties) {
    const auto& properties = it.second;
    cb->addItem(QString::fromUtf8(properties.deviceName));
  }
}

void MainWindow::populateGeneral(VkPhysicalDevice gpu)
{
  auto it = mGpuProperties.find(gpu);
  if (it == mGpuProperties.end()) {
    return;
  }

  const VkPhysicalDeviceProperties& properties = it->second;

  QLabel* lb = findChild<QLabel*>("apiVersionValue");
  Q_ASSERT(lb);
  lb->setText(toStringVersion(properties.apiVersion));

  lb = findChild<QLabel*>("driverVersionValue");
  Q_ASSERT(lb);
  lb->setText(QString::number(properties.driverVersion));

  lb = findChild<QLabel*>("vendorIdValue");
  Q_ASSERT(lb);
  lb->setText(QString::number(properties.vendorID));

  lb = findChild<QLabel*>("deviceIdValue");
  Q_ASSERT(lb);
  lb->setText(QString::number(properties.deviceID));

  lb = findChild<QLabel*>("deviceTypeValue");
  Q_ASSERT(lb);
  lb->setText(toStringDeviceType(properties.deviceType));

  lb = findChild<QLabel*>("deviceNameValue");
  Q_ASSERT(lb);
  lb->setText(properties.deviceName);

  QString uuid;
  for (size_t i = 0; i < VK_UUID_SIZE; ++i) {
    uuid.append(QString::number(properties.pipelineCacheUUID[i], 16));
  }
  lb = findChild<QLabel*>("pipelineCacheUuidValue");
  Q_ASSERT(lb);
  lb->setText(uuid.toUpper());
}

#define ADD_LIMIT(locale, tw, limits, prop)                 \
  {                                                 \
    QTreeWidgetItem* item = new QTreeWidgetItem();  \
    item->setText(0, QString::fromUtf8(#prop));     \
    item->setText(1, locale.toString(limits.prop)); \
    item->setTextAlignment(1, Qt::AlignRight);      \
    tw->addTopLevelItem(item);                      \
  }

void MainWindow::populateLimits(VkPhysicalDevice gpu)
{
  assert(gpu != VK_NULL_HANDLE);

  QTreeWidget* tw = findChild<QTreeWidget*>("limitsWidget");
  Q_ASSERT(tw);

  tw->clear();

  auto it = mGpuProperties.find(gpu);
  if (it == mGpuProperties.end()) {
    return;
  }
  const auto &limits = it->second.limits;

  QLocale locale;

  ADD_LIMIT(locale, tw, limits, maxImageDimension1D);
  ADD_LIMIT(locale, tw, limits, maxImageDimension2D);
  ADD_LIMIT(locale, tw, limits, maxImageDimension3D);
  ADD_LIMIT(locale, tw, limits, maxImageDimensionCube);
  ADD_LIMIT(locale, tw, limits, maxImageArrayLayers);
  ADD_LIMIT(locale, tw, limits, maxTexelBufferElements);
  ADD_LIMIT(locale, tw, limits, maxUniformBufferRange);
  ADD_LIMIT(locale, tw, limits, maxStorageBufferRange);
  ADD_LIMIT(locale, tw, limits, maxPushConstantsSize);
  ADD_LIMIT(locale, tw, limits, maxMemoryAllocationCount);
  ADD_LIMIT(locale, tw, limits, maxSamplerAllocationCount);
  ADD_LIMIT(locale, tw, limits, bufferImageGranularity);
  ADD_LIMIT(locale, tw, limits, sparseAddressSpaceSize);
  ADD_LIMIT(locale, tw, limits, maxBoundDescriptorSets);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorSamplers);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorUniformBuffers);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorStorageBuffers);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorSampledImages);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorStorageImages);
  ADD_LIMIT(locale, tw, limits, maxPerStageDescriptorInputAttachments);
  ADD_LIMIT(locale, tw, limits, maxPerStageResources);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetSamplers);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetUniformBuffers);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetUniformBuffersDynamic);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetStorageBuffers);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetStorageBuffersDynamic);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetSampledImages);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetStorageImages);
  ADD_LIMIT(locale, tw, limits, maxDescriptorSetInputAttachments);
  ADD_LIMIT(locale, tw, limits, maxVertexInputAttributes);
  ADD_LIMIT(locale, tw, limits, maxVertexInputBindings);
  ADD_LIMIT(locale, tw, limits, maxVertexInputAttributeOffset);
  ADD_LIMIT(locale, tw, limits, maxVertexInputBindingStride);
  ADD_LIMIT(locale, tw, limits, maxVertexOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationGenerationLevel);
  ADD_LIMIT(locale, tw, limits, maxTessellationPatchSize);
  ADD_LIMIT(locale, tw, limits, maxTessellationControlPerVertexInputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationControlPerVertexOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationControlPerPatchOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationControlTotalOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationEvaluationInputComponents);
  ADD_LIMIT(locale, tw, limits, maxTessellationEvaluationOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxGeometryShaderInvocations);
  ADD_LIMIT(locale, tw, limits, maxGeometryInputComponents);
  ADD_LIMIT(locale, tw, limits, maxGeometryOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxGeometryOutputVertices);
  ADD_LIMIT(locale, tw, limits, maxGeometryTotalOutputComponents);
  ADD_LIMIT(locale, tw, limits, maxFragmentInputComponents);
  ADD_LIMIT(locale, tw, limits, maxFragmentOutputAttachments);
  ADD_LIMIT(locale, tw, limits, maxFragmentDualSrcAttachments);
  ADD_LIMIT(locale, tw, limits, maxFragmentCombinedOutputResources);
  ADD_LIMIT(locale, tw, limits, maxComputeSharedMemorySize);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupCount[0]);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupCount[1]);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupCount[2]);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupInvocations);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupSize[0]);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupSize[1]);
  ADD_LIMIT(locale, tw, limits, maxComputeWorkGroupSize[2]);
  ADD_LIMIT(locale, tw, limits, subPixelPrecisionBits);
  ADD_LIMIT(locale, tw, limits, subTexelPrecisionBits);
  ADD_LIMIT(locale, tw, limits, mipmapPrecisionBits);
  ADD_LIMIT(locale, tw, limits, maxDrawIndexedIndexValue);
  ADD_LIMIT(locale, tw, limits, maxDrawIndirectCount);
  ADD_LIMIT(locale, tw, limits, maxSamplerLodBias);
  ADD_LIMIT(locale, tw, limits, maxSamplerAnisotropy);
  ADD_LIMIT(locale, tw, limits, maxViewports);
  ADD_LIMIT(locale, tw, limits, maxViewportDimensions[0]);
  ADD_LIMIT(locale, tw, limits, maxViewportDimensions[1]);
  ADD_LIMIT(locale, tw, limits, viewportBoundsRange[0]);
  ADD_LIMIT(locale, tw, limits, viewportBoundsRange[1]);
  ADD_LIMIT(locale, tw, limits, viewportSubPixelBits);
  ADD_LIMIT(locale, tw, limits, minMemoryMapAlignment);
  ADD_LIMIT(locale, tw, limits, minTexelBufferOffsetAlignment);
  ADD_LIMIT(locale, tw, limits, minUniformBufferOffsetAlignment);
  ADD_LIMIT(locale, tw, limits, minStorageBufferOffsetAlignment);
  ADD_LIMIT(locale, tw, limits, minTexelOffset);
  ADD_LIMIT(locale, tw, limits, maxTexelOffset);
  ADD_LIMIT(locale, tw, limits, minTexelGatherOffset);
  ADD_LIMIT(locale, tw, limits, maxTexelGatherOffset);
  ADD_LIMIT(locale, tw, limits, minInterpolationOffset);
  ADD_LIMIT(locale, tw, limits, maxInterpolationOffset);
  ADD_LIMIT(locale, tw, limits, subPixelInterpolationOffsetBits);
  ADD_LIMIT(locale, tw, limits, maxFramebufferWidth);
  ADD_LIMIT(locale, tw, limits, maxFramebufferHeight);
  ADD_LIMIT(locale, tw, limits, maxFramebufferLayers);
  ADD_LIMIT(locale, tw, limits, framebufferColorSampleCounts);
  ADD_LIMIT(locale, tw, limits, framebufferDepthSampleCounts);
  ADD_LIMIT(locale, tw, limits, framebufferStencilSampleCounts);
  ADD_LIMIT(locale, tw, limits, framebufferNoAttachmentsSampleCounts);
  ADD_LIMIT(locale, tw, limits, maxColorAttachments);
  ADD_LIMIT(locale, tw, limits, sampledImageColorSampleCounts);
  ADD_LIMIT(locale, tw, limits, sampledImageIntegerSampleCounts);
  ADD_LIMIT(locale, tw, limits, sampledImageDepthSampleCounts);
  ADD_LIMIT(locale, tw, limits, sampledImageStencilSampleCounts);
  ADD_LIMIT(locale, tw, limits, storageImageSampleCounts);
  ADD_LIMIT(locale, tw, limits, maxSampleMaskWords);
  ADD_LIMIT(locale, tw, limits, timestampComputeAndGraphics);
  ADD_LIMIT(locale, tw, limits, timestampPeriod);
  ADD_LIMIT(locale, tw, limits, maxClipDistances);
  ADD_LIMIT(locale, tw, limits, maxCullDistances);
  ADD_LIMIT(locale, tw, limits, maxCombinedClipAndCullDistances);
  ADD_LIMIT(locale, tw, limits, discreteQueuePriorities);
  ADD_LIMIT(locale, tw, limits, pointSizeRange[0]);
  ADD_LIMIT(locale, tw, limits, pointSizeRange[1]);
  ADD_LIMIT(locale, tw, limits, lineWidthRange[0]);
  ADD_LIMIT(locale, tw, limits, lineWidthRange[1]);
  ADD_LIMIT(locale, tw, limits, pointSizeGranularity);
  ADD_LIMIT(locale, tw, limits, lineWidthGranularity);
  ADD_LIMIT(locale, tw, limits, strictLines);
  ADD_LIMIT(locale, tw, limits, standardSampleLocations);
  ADD_LIMIT(locale, tw, limits, optimalBufferCopyOffsetAlignment);
  ADD_LIMIT(locale, tw, limits, optimalBufferCopyRowPitchAlignment);
  ADD_LIMIT(locale, tw, limits, nonCoherentAtomSize);

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

#define ADD_SPARSE(tw, sparse, prop)                          \
  {                                                           \
    QTreeWidgetItem* item = new QTreeWidgetItem();            \
    item->setText(0, QString::fromUtf8(#prop));               \
    item->setText(1, (sparse.prop == VK_TRUE) ? "Y" : "");   \
    item->setTextAlignment(1, Qt::AlignHCenter);              \
    tw->addTopLevelItem(item);                                \
  }

void MainWindow::populateSparse(VkPhysicalDevice gpu)
{
  assert(gpu != VK_NULL_HANDLE);

  QTreeWidget* tw = findChild<QTreeWidget*>("sparsePropertiesWidget");
  Q_ASSERT(tw);

  tw->clear();

  auto it = mGpuProperties.find(gpu);
  if (it == mGpuProperties.end()) {
    return;
  }

  const auto &sparseProperties = it->second.sparseProperties;

  ADD_SPARSE(tw, sparseProperties, residencyStandard2DBlockShape);
  ADD_SPARSE(tw, sparseProperties, residencyStandard2DMultisampleBlockShape);
  ADD_SPARSE(tw, sparseProperties, residencyStandard3DBlockShape);
  ADD_SPARSE(tw, sparseProperties, residencyAlignedMipSize);
  ADD_SPARSE(tw, sparseProperties, residencyNonResidentStrict);

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

#define ADD_FEATURE(tw, features, prop)                       \
  {                                                           \
    QTreeWidgetItem* item = new QTreeWidgetItem();            \
    item->setText(0, QString::fromUtf8(#prop));               \
    item->setText(1, (features.prop == VK_TRUE) ? "Y" : ""); \
    item->setTextAlignment(1, Qt::AlignHCenter);              \
    tw->addTopLevelItem(item);                                \
  }

void MainWindow::populateFeatures(VkPhysicalDevice gpu)
{
  assert(gpu != VK_NULL_HANDLE);

  QTreeWidget* tw = findChild<QTreeWidget*>("featuresWidget");
  Q_ASSERT(tw);

  tw->clear();

  VkPhysicalDeviceFeatures features = {};
  vkGetPhysicalDeviceFeatures(gpu, &features);

  ADD_FEATURE(tw, features, robustBufferAccess);
  ADD_FEATURE(tw, features, fullDrawIndexUint32);
  ADD_FEATURE(tw, features, imageCubeArray);
  ADD_FEATURE(tw, features, independentBlend);
  ADD_FEATURE(tw, features, geometryShader);
  ADD_FEATURE(tw, features, tessellationShader);
  ADD_FEATURE(tw, features, sampleRateShading);
  ADD_FEATURE(tw, features, dualSrcBlend);
  ADD_FEATURE(tw, features, logicOp);
  ADD_FEATURE(tw, features, multiDrawIndirect);
  ADD_FEATURE(tw, features, drawIndirectFirstInstance);
  ADD_FEATURE(tw, features, depthClamp);
  ADD_FEATURE(tw, features, depthBiasClamp);
  ADD_FEATURE(tw, features, fillModeNonSolid);
  ADD_FEATURE(tw, features, depthBounds);
  ADD_FEATURE(tw, features, wideLines);
  ADD_FEATURE(tw, features, largePoints);
  ADD_FEATURE(tw, features, alphaToOne);
  ADD_FEATURE(tw, features, multiViewport);
  ADD_FEATURE(tw, features, samplerAnisotropy);
  ADD_FEATURE(tw, features, textureCompressionETC2);
  ADD_FEATURE(tw, features, textureCompressionASTC_LDR);
  ADD_FEATURE(tw, features, textureCompressionBC);
  ADD_FEATURE(tw, features, occlusionQueryPrecise);
  ADD_FEATURE(tw, features, pipelineStatisticsQuery);
  ADD_FEATURE(tw, features, vertexPipelineStoresAndAtomics);
  ADD_FEATURE(tw, features, fragmentStoresAndAtomics);
  ADD_FEATURE(tw, features, shaderTessellationAndGeometryPointSize);
  ADD_FEATURE(tw, features, shaderImageGatherExtended);
  ADD_FEATURE(tw, features, shaderStorageImageExtendedFormats);
  ADD_FEATURE(tw, features, shaderStorageImageMultisample);
  ADD_FEATURE(tw, features, shaderStorageImageReadWithoutFormat);
  ADD_FEATURE(tw, features, shaderStorageImageWriteWithoutFormat);
  ADD_FEATURE(tw, features, shaderUniformBufferArrayDynamicIndexing);
  ADD_FEATURE(tw, features, shaderSampledImageArrayDynamicIndexing);
  ADD_FEATURE(tw, features, shaderStorageBufferArrayDynamicIndexing);
  ADD_FEATURE(tw, features, shaderStorageImageArrayDynamicIndexing);
  ADD_FEATURE(tw, features, shaderClipDistance);
  ADD_FEATURE(tw, features, shaderCullDistance);
  ADD_FEATURE(tw, features, shaderFloat64);
  ADD_FEATURE(tw, features, shaderInt64);
  ADD_FEATURE(tw, features, shaderInt16);
  ADD_FEATURE(tw, features, shaderResourceResidency);
  ADD_FEATURE(tw, features, shaderResourceMinLod);
  ADD_FEATURE(tw, features, sparseBinding);
  ADD_FEATURE(tw, features, sparseResidencyBuffer);
  ADD_FEATURE(tw, features, sparseResidencyImage2D);
  ADD_FEATURE(tw, features, sparseResidencyImage3D);
  ADD_FEATURE(tw, features, sparseResidency2Samples);
  ADD_FEATURE(tw, features, sparseResidency4Samples);
  ADD_FEATURE(tw, features, sparseResidency8Samples);
  ADD_FEATURE(tw, features, sparseResidency16Samples);
  ADD_FEATURE(tw, features, sparseResidencyAliased);
  ADD_FEATURE(tw, features, variableMultisampleRate);
  ADD_FEATURE(tw, features, inheritedQueries);

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void setLabelValue(QLabel* lb, uint32_t value)
{
  Q_ASSERT(lb);
  lb->setText(QString::number(value));
}

void setLabelValue(QLabel* lb, const VkExtent2D& value)
{
  QLocale locale;

  Q_ASSERT(lb);
  lb->setText("(" + QString::number(value.width) + ", " + QString::number(value.height) +")");
}

void MainWindow::updateSurfaceExtents(VkPhysicalDevice gpu)
{
  VkSurfaceCapabilitiesKHR surfCaps = {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, mSurface, &surfCaps);

  setLabelValue(findChild<QLabel*>("minImageExtentValue"), surfCaps.minImageExtent);
  setLabelValue(findChild<QLabel*>("maxImageExtentValue"), surfCaps.maxImageExtent);
  setLabelValue(findChild<QLabel*>("currentExtentValue"), surfCaps.currentExtent);
}

void MainWindow::populateSurface(VkPhysicalDevice gpu)
{
  VkSurfaceCapabilitiesKHR surfCaps = {};
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, mSurface, &surfCaps);

  uint32_t count = 0;
  VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mSurface, &count, nullptr);
  assert(res == VK_SUCCESS);
  std::vector<VkSurfaceFormatKHR> formats(count);
  res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, mSurface, &count, formats.data());
  assert(res == VK_SUCCESS);

  setLabelValue(findChild<QLabel*>("minImageCountValue"), surfCaps.minImageCount);
  setLabelValue(findChild<QLabel*>("maxImageCountValue"), surfCaps.maxImageCount);
  setLabelValue(findChild<QLabel*>("maxImageArrayLayersValue"), surfCaps.maxImageArrayLayers);

  updateSurfaceExtents(gpu);

  // Present modes
  res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, mSurface, &count, nullptr);
  assert(res == VK_SUCCESS);
  std::vector<VkPresentModeKHR> presentModes(count);
  res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, mSurface, &count, presentModes.data());
  assert(res == VK_SUCCESS);
  QTreeWidget* tw = findChild<QTreeWidget*>("presentModesWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (const auto& mode : presentModes) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkPresentMode(mode));
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }

  // Transforms
  tw = findChild<QTreeWidget*>("transformsWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (uint32_t i = 0; i < 32; ++i) {
    VkSurfaceTransformFlagBitsKHR transform = static_cast<VkSurfaceTransformFlagBitsKHR>(1 << i);
    if ((surfCaps.supportedTransforms & transform) == 0) {
      continue;
    }
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkTransform(transform));
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }

  // Composite Alpha
  tw = findChild<QTreeWidget*>("compositeAlphaModesWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (uint32_t i = 0; i < 32; ++i) {
    VkCompositeAlphaFlagBitsKHR mode = static_cast<VkCompositeAlphaFlagBitsKHR>(1 << i);
    if ((surfCaps.supportedCompositeAlpha & mode) == 0) {
      continue;
    }
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkCompositeAlpha(mode));
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }

  // Formats and usage
  tw = findChild<QTreeWidget*>("surfaceFormatsWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (const auto& format : formats) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringVkFormat(format.format));
    item->setText(1, toStringVkColorSpace(format.colorSpace));
    item->setText(2, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) != 0) ? "Y" : "");
    item->setText(3, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) != 0) ? "Y" : "");
    item->setText(4, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT) != 0) ? "Y" : "");
    item->setText(5, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT) != 0) ? "Y" : "");
    item->setText(6, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) != 0) ? "Y" : "");
    item->setText(7, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0) ? "Y" : "");
    item->setText(8, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) != 0) ? "Y" : "");
    item->setText(9, ((surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) != 0) ? "Y" : "");
    for (int c = 2; c < item->columnCount(); ++c) {
      item->setTextAlignment(c, Qt::AlignHCenter);
    }
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
    VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, static_cast<uint32_t>(i), mSurface, &presents);
    assert(res == VK_SUCCESS);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::number(i));
    item->setText(1, QString::number(properties[i].queueCount));
    item->setText(2, (presents == VK_TRUE) ? "Y" : "");
    item->setText(3, ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) ? "Y" : "");
    item->setText(4, ((properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) ? "Y" : "");
    item->setText(5, ((properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) ? "Y" : "");
    item->setText(6, ((properties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) ? "Y" : "");
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

void MainWindow::populateMemory(VkPhysicalDevice gpu)
{
  VkPhysicalDeviceMemoryProperties properties = {};
  vkGetPhysicalDeviceMemoryProperties(gpu, &properties);

  // Memory types
  QTreeWidget* tw = findChild<QTreeWidget*>("memoryTypesWidget");
  Q_ASSERT(tw);
  for (uint32_t i = 0; i < properties.memoryTypeCount; ++i) {
    const auto& type = properties.memoryTypes[i];
    auto item = new QTreeWidgetItem();
    item->setText(0, QString::number(i));
    item->setText(1, QString::number(type.heapIndex));
    item->setText(2, ((type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) ? "Y" : "");
    item->setText(3, ((type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0) ? "Y" : "");
    item->setText(4, ((type.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0) ? "Y" : "");
    item->setText(5, ((type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0) ? "Y" : "");
    item->setText(6, ((type.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0) ? "Y" : "");
    for (int c = 0; c < item->columnCount(); ++c) {
      item->setTextAlignment(c, Qt::AlignHCenter);
    }
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }

  // Memory heaps
  QLocale locale;
  tw = findChild<QTreeWidget*>("memoryHeapsWidget");
  Q_ASSERT(tw);
  for (uint32_t i = 0; i < properties.memoryHeapCount; ++i) {
    const auto& heap = properties.memoryHeaps[i];
    QString bytes = locale.toString(heap.size) + " bytes";
    QString mbytes = locale.toString(heap.size / 1048576.0f) + " MB";

    auto item = new QTreeWidgetItem();
    item->setText(0, QString::number(i));
    //item->setText(1, QString::number(heap.size));
    item->setText(1, mbytes + " (" + bytes +")");
    item->setText(2, ((heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0) ? "Y" : "");
    item->setTextAlignment(0, Qt::AlignHCenter);
    item->setTextAlignment(1, Qt::AlignRight);
    item->setTextAlignment(2, Qt::AlignHCenter);
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

QTreeWidgetItem* buildFormatFeatures(QTreeWidgetItem* parentItem, const VkFormatProperties& properties)
{
  std::vector<VkFormatFeatureFlagBits> flags = {
    VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
    VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
    VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
    VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
    VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
    VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
    VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
    VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
    VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_FORMAT_FEATURE_BLIT_SRC_BIT,
    VK_FORMAT_FEATURE_BLIT_DST_BIT,
    VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
    VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR,
    VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR,
  };

  for (const auto& flag : flags) {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, toStringFormatFeature(flag));

    item->setText(1, ((properties.linearTilingFeatures & flag) != 0) ? "Y" : "");
    item->setText(2, ((properties.optimalTilingFeatures & flag) != 0) ? "Y" : "");
    item->setText(3, ((properties.bufferFeatures & flag) != 0) ? "Y" : "");

    item->setTextAlignment(1, Qt::AlignHCenter);
    item->setTextAlignment(2, Qt::AlignHCenter);
    item->setTextAlignment(3, Qt::AlignHCenter);

    parentItem->addChild(item);
  }

  return nullptr;
}

void populateImageFormats(
  VkPhysicalDevice  gpu,
  QTreeWidget*      tw,
  VkImageTiling     tiling
)
{
  uint32_t start = static_cast<uint32_t>(VK_FORMAT_BEGIN_RANGE);
  uint32_t end = static_cast<uint32_t>(VK_FORMAT_END_RANGE);

  tw->clear();
  for (uint32_t i = start; i <= end; ++i) {
    VkFormat format = static_cast<VkFormat>(i);
    VkFormatProperties properties = {};
    vkGetPhysicalDeviceFormatProperties(gpu, format, &properties);
    VkFormatFeatureFlags features = static_cast<VkFormatFeatureFlags>(0);
    if (tiling == VK_IMAGE_TILING_LINEAR) {
      features = properties.linearTilingFeatures;
    }
    else if (tiling == VK_IMAGE_TILING_OPTIMAL) {
      features = properties.optimalTilingFeatures;
    }
    else {
      features = properties.bufferFeatures;
    }

    if (features == 0) {
      continue;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, QVariant::fromValue(i));
    item->setText(0, toStringVkFormat(format));

    // Populate the buffer usages
    if ((tiling != VK_IMAGE_TILING_LINEAR) && (tiling != VK_IMAGE_TILING_OPTIMAL)) {
      item->setText(1, ((features & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) != 0) ? "Y" : "");
      item->setText(2, ((features & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT) != 0) ? "Y" : "");
      item->setText(3, ((features & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT) != 0) ? "Y" : "");
      item->setText(4, ((features & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) != 0) ? "Y" : "");
      item->setTextAlignment(1, Qt::AlignHCenter);
      item->setTextAlignment(2, Qt::AlignHCenter);
      item->setTextAlignment(3, Qt::AlignHCenter);
      item->setTextAlignment(4, Qt::AlignHCenter);
    }

    tw->addTopLevelItem(item);
  }

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void updateImageFormats(
    VkPhysicalDevice    gpu,
    QTreeWidget*        tw,
    VkImageType         type,
    VkImageTiling       tiling,
    VkImageUsageFlags   usageFlags,
    VkImageCreateFlags  createFlags
)
{
  QLocale locale;

  int n = tw->topLevelItemCount();
  for (int i = 0; i < n; ++i) {
    auto item = tw->topLevelItem(i);
    item->setText(1, "");
    item->setText(2, "");
    item->setText(3, "");
    item->setText(4, "");
    item->setText(5, "");

    VkFormat format = static_cast<VkFormat>(item->data(0, Qt::UserRole).value<uint32_t>());
    VkImageFormatProperties imageFormatProperties = {};
    VkResult res = vkGetPhysicalDeviceImageFormatProperties(gpu, format,
        type, tiling, usageFlags, createFlags, &imageFormatProperties);
    if (res != VK_SUCCESS) {
      continue;
    }

    QString extent = locale.toString(imageFormatProperties.maxExtent.width)  + " / " +
                     locale.toString(imageFormatProperties.maxExtent.height) + " / " +
                     locale.toString(imageFormatProperties.maxExtent.depth);

    QString bytes = locale.toString(imageFormatProperties.maxResourceSize) + " bytes";
    QString gbytes = locale.toString(imageFormatProperties.maxResourceSize / (1024.0 * 1048576.0)) + " GB";

    item->setText(1, extent);
    item->setText(2, locale.toString(imageFormatProperties.maxMipLevels));
    item->setText(3, locale.toString(imageFormatProperties.maxArrayLayers));
    item->setText(4, toStringSampleCounts(imageFormatProperties.sampleCounts));
    item->setText(5, gbytes + " (" + bytes + ")");

    item->setTextAlignment(1, Qt::AlignHCenter);
    item->setTextAlignment(2, Qt::AlignHCenter);
    item->setTextAlignment(3, Qt::AlignHCenter);
    item->setTextAlignment(4, Qt::AlignHCenter);
    item->setTextAlignment(5, Qt::AlignRight);
  }

  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }
}

void MainWindow::populateFormats(VkPhysicalDevice gpu)
{
  uint32_t start = static_cast<uint32_t>(VK_FORMAT_BEGIN_RANGE);
  uint32_t end = static_cast<uint32_t>(VK_FORMAT_END_RANGE);

  // Initial values
  VkImageType imageType = VK_IMAGE_TYPE_2D;
  VkImageUsageFlags usageFlags = static_cast<VkImageUsageFlags>(0);
  VkImageCreateFlags createFlags = static_cast<VkImageCreateFlags>(0);

  QTreeWidget* tw = findChild<QTreeWidget*>("formatsWidget");
  Q_ASSERT(tw);
  tw->clear();
  for (uint32_t i = start; i <= end; ++i) {
    VkFormat format = static_cast<VkFormat>(i);
    VkFormatProperties properties = {};
    vkGetPhysicalDeviceFormatProperties(gpu, format, &properties);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setData(0, Qt::UserRole, QVariant::fromValue(i));
    item->setText(0, toStringVkFormat(format));
    item->setText(1, (properties.linearTilingFeatures != 0) ? "Y" : "");
    item->setText(2, (properties.optimalTilingFeatures != 0) ? "Y" : "");
    item->setText(3, (properties.bufferFeatures != 0) ? "Y" : "");
    buildFormatFeatures(item, properties);
    for (int c = 1; c < item->columnCount(); ++c) {
      item->setTextAlignment(c, Qt::AlignHCenter);
    }
    tw->addTopLevelItem(item);
  }
  for (int i = 0; i < tw->columnCount(); ++i) {
    tw->resizeColumnToContents(i);
  }

  // Tiling Linear
  tw = findChild<QTreeWidget*>("tilingLinearFormatsWidget");
  Q_ASSERT(tw);
  populateImageFormats(gpu, tw, VK_IMAGE_TILING_LINEAR);
  updateImageFormats(gpu, tw, imageType, VK_IMAGE_TILING_LINEAR, usageFlags, createFlags);

  // Tiling Optimal
  tw = findChild<QTreeWidget*>("tilingOptimalFormatsWidget");
  Q_ASSERT(tw);
  populateImageFormats(gpu, tw, VK_IMAGE_TILING_OPTIMAL);
  updateImageFormats(gpu, tw, imageType, VK_IMAGE_TILING_OPTIMAL, usageFlags, createFlags);

  // Buffer
  tw = findChild<QTreeWidget*>("bufferFormatsWidget");
  Q_ASSERT(tw);
  populateImageFormats(gpu, tw, static_cast<VkImageTiling>(UINT32_MAX));
}

void MainWindow::on_gpus_currentIndexChanged(int index)
{
  if (static_cast<size_t>(index) >= mGpus.size()) {
    return;
  }

  mCurrentGpu = mGpus[index];
  populateGeneral(mCurrentGpu);
  populateLimits(mCurrentGpu);
  populateSparse(mCurrentGpu);
  populateFeatures(mCurrentGpu);
  populateSurface(mCurrentGpu);
  populateQueues(mCurrentGpu);
  populateMemory(mCurrentGpu);
  populateFormats(mCurrentGpu);
}

void MainWindow::filterTreeWidgetItemsSimple(const QString &widgetName, const QString &filterText)
{
  QTreeWidget* tw = findChild<QTreeWidget*>(widgetName);
  Q_ASSERT(tw);

  if (! filterText.isEmpty()) {
    for (int parentIndex = 0; parentIndex < tw->topLevelItemCount(); ++parentIndex) {
      auto parentItem = tw->topLevelItem(parentIndex);
      auto parentText = parentItem->text(0);
      bool parentVisible = parentText.contains(filterText, Qt::CaseInsensitive);
      for (int childIndex = 0; childIndex < parentItem->childCount(); ++childIndex) {
        auto childItem = parentItem->child(childIndex);
        auto childText = childItem->text(0);
        bool childVisible = childText.contains(filterText, Qt::CaseInsensitive);
        childItem->setHidden(! childVisible);
        if (childVisible) {
          parentVisible = true;
          parentItem->setExpanded(true);
        }
      }
      parentItem->setHidden(! parentVisible);
    }
  }
  else {
    for (int parentIndex = 0; parentIndex < tw->topLevelItemCount(); ++parentIndex) {
      auto parentItem = tw->topLevelItem(parentIndex);
      parentItem->setHidden(false);
      for (int childIndex = 0; childIndex < parentItem->childCount(); ++childIndex) {
        auto childItem = parentItem->child(childIndex);
        childItem->setHidden(false);
      }
    }
  }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  if (mInstance == VK_NULL_HANDLE) {
    return;
  }

  destroyVulkanSurface();
  createVulkanSurface();

  updateSurfaceExtents(mCurrentGpu);
}

void MainWindow::on_limitsFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("limitsWidget", arg1.trimmed());
}

void MainWindow::on_formatFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("formatsWidget", arg1.trimmed());
}

void MainWindow::on_featuresFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("featuresWidget", arg1.trimmed());
}

VkImageType getImageType(QComboBox* cb)
{
  VkImageType type = static_cast<VkImageType>(cb->currentIndex());
  return type;
}

bool isChecked(QStandardItemModel* model, int row)
{
  int n = model->rowCount();
  Qt::CheckState value = model->item(row)->data(Qt::CheckStateRole).value<Qt::CheckState>();
  return value == Qt::Checked;
}

VkImageUsageFlags getUsageFlags(QComboBox* cb)
{
  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(cb->model());

  VkImageUsageFlags result = static_cast<VkImageUsageFlags>(0);
  result |= isChecked(model, 1) ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0;
  result |= isChecked(model, 2) ? VK_IMAGE_USAGE_TRANSFER_DST_BIT : 0;
  result |= isChecked(model, 3) ? VK_IMAGE_USAGE_SAMPLED_BIT : 0;
  result |= isChecked(model, 4) ? VK_IMAGE_USAGE_STORAGE_BIT : 0;
  result |= isChecked(model, 5) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0;
  result |= isChecked(model, 6) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0;
  result |= isChecked(model, 7) ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT : 0;
  result |= isChecked(model, 8) ? VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT : 0;
  return result;
}

VkImageCreateFlags getCreateFlags(QComboBox* cb)
{
  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(cb->model());

  VkImageCreateFlags result = static_cast<VkImageCreateFlags>(0);
  result |= isChecked(model, 1) ? VK_IMAGE_CREATE_SPARSE_BINDING_BIT : 0;
  result |= isChecked(model, 2) ? VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT : 0;
  result |= isChecked(model, 3) ? VK_IMAGE_CREATE_SPARSE_ALIASED_BIT : 0;
  result |= isChecked(model, 4) ? VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT : 0;
  result |= isChecked(model, 5) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
  result |= isChecked(model, 6) ? VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR : 0;
  return result;
}

void MainWindow::on_itemChanged(QStandardItem *item)
{
  auto& inputs = mFilterInputTargets[item->model()];
  auto& tw = inputs->target;
  VkImageType type = getImageType(inputs->imageTypeFilter);
  VkImageTiling tiling = inputs->tiling;
  VkImageUsageFlags usageFlags = getUsageFlags(inputs->usageFlagsFilter);
  VkImageCreateFlags createFlags = getCreateFlags(inputs->createFlagsFilter);
  updateImageFormats(mCurrentGpu, tw, type, tiling, usageFlags, createFlags);
}

void MainWindow::on_tilingLinearImageType_currentIndexChanged(int index)
{
  auto inputs = &mTilingLinearFilterInputs;
  auto& tw = inputs->target;
  VkImageType type = static_cast<VkImageType>(index);
  VkImageTiling tiling = inputs->tiling;
  VkImageUsageFlags usageFlags = getUsageFlags(inputs->usageFlagsFilter);
  VkImageCreateFlags createFlags = getCreateFlags(inputs->createFlagsFilter);
  updateImageFormats(mCurrentGpu, tw, type, tiling, usageFlags, createFlags);
}

void MainWindow::on_tilingOptimalImageType_currentIndexChanged(int index)
{
  auto inputs = &mTilingOptimalFilterInputs;
  auto& tw = inputs->target;
  VkImageType type = static_cast<VkImageType>(index);
  VkImageTiling tiling = inputs->tiling;
  VkImageUsageFlags usageFlags = getUsageFlags(inputs->usageFlagsFilter);
  VkImageCreateFlags createFlags = getCreateFlags(inputs->createFlagsFilter);
  updateImageFormats(mCurrentGpu, tw, type, tiling, usageFlags, createFlags);
}

void MainWindow::on_tilingLinearFormatFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("tilingLinearFormatsWidget", arg1.trimmed());
}

void MainWindow::on_tilingOptimalFormatFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("tilingOptimalFormatsWidget", arg1.trimmed());
}

void MainWindow::on_bufferFormatFilter_textChanged(const QString &arg1)
{
  filterTreeWidgetItemsSimple("bufferFormatsWidget", arg1.trimmed());
}

void MainWindow::on_expandAllBtn_clicked()
{
  QTreeWidget* tw = findChild<QTreeWidget*>("formatsWidget");
  Q_ASSERT(tw);
  for (int i = 0; i < tw->topLevelItemCount(); ++i) {
    tw->topLevelItem(i)->setExpanded(true);
  }
}

void MainWindow::on_collapseAllBtn_clicked()
{
  QTreeWidget* tw = findChild<QTreeWidget*>("formatsWidget");
  Q_ASSERT(tw);
  for (int i = 0; i < tw->topLevelItemCount(); ++i) {
    tw->topLevelItem(i)->setExpanded(false);
  }
}
