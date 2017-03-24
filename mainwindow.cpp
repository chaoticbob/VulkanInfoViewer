#include "mainwindow.h"
#include "ui_mainwindow.h"

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

  initGpus(&mVkGpus, &mVkGpuProperties);
  populateGpus(mVkGpuProperties);
}

MainWindow::~MainWindow()
{
  destroyVulkanSurface();
  destroyVulkanInstance();

  delete ui;
}

void MainWindow::createVulkanInstance()
{
  enumerateInstanceLayers(&mInstanceLayers);
  populateInstanceLayers(mInstanceLayers);

  scanInstanceExtensions(mInstanceLayers, &mInstanceLayerExtensions);
  populateInstanceExtensions(mInstanceLayerExtensions);

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

void MainWindow::initGpus(std::vector<VkPhysicalDevice>* gpus, std::vector<VkPhysicalDeviceProperties>* gpuProperties)
{
  uint32_t count = 0;
  VkResult res = vkEnumeratePhysicalDevices(mVkInstance, &count, nullptr);
  assert(res == VK_SUCCESS);
  gpus->resize(count);
  res = vkEnumeratePhysicalDevices(mVkInstance, &count, gpus->data());
  assert(res == VK_SUCCESS);

  gpuProperties->resize(count);

  for (size_t i = 0; i < gpus->size(); ++i) {
    vkGetPhysicalDeviceProperties((*gpus)[i], &(*gpuProperties)[i]);
  }
}

void MainWindow::enumerateInstanceLayers(std::vector<VkLayerProperties> *layers)
{
  uint32_t count = 0;
  VkResult res = vkEnumerateInstanceLayerProperties(&count, nullptr);
  assert(res == VK_SUCCESS);

  layers->resize(count);
  res = vkEnumerateInstanceLayerProperties(&count, layers->data());
  assert(res == VK_SUCCESS);
}

void MainWindow::populateInstanceLayers(const std::vector<VkLayerProperties>& layerProperties)
{
  QTreeWidget* tw = findChild<QTreeWidget*>("layersWidget");
  Q_ASSERT(tw);

  for (const auto& elem : layerProperties) {
    std::stringstream ss;
    ss << ((elem.specVersion >> 22) & 0x3FF) << ".";
    ss << ((elem.specVersion >> 12) & 0x3FF) << ".";
    ss << ((elem.specVersion >>  0) & 0xFFF);

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::fromUtf8(elem.layerName));
    item->setText(1, QString::fromStdString(ss.str()));
    item->setText(2, QString::number(elem.implementationVersion));
    item->setText(3, QString::fromUtf8(elem.description));
    tw->addTopLevelItem(item);
  }
}

void MainWindow::enumerateInstanceExtensions(const char* layerName, std::vector<VkExtensionProperties>* extensions)
{
  uint32_t count = 0;
  VkResult res = vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
  assert(res == VK_SUCCESS);

  extensions->resize(count);
  res = vkEnumerateInstanceExtensionProperties(layerName, &count, extensions->data());
}

void MainWindow::scanInstanceExtensions(const std::vector<VkLayerProperties>& layers, std::map<std::string, std::vector<VkExtensionProperties>>* layerExtensions)
{
  for(const auto& layer : layers) {
    std::string layerName = layer.layerName;
    std::vector<VkExtensionProperties> extensions;
    enumerateInstanceExtensions(layerName.c_str(), &extensions);
    if (! extensions.empty()) {
      (*layerExtensions)[layerName] = extensions;
    }
  }

  std::vector<VkExtensionProperties> extensions;
  enumerateInstanceExtensions(nullptr, &extensions);
  (*layerExtensions)[""] = extensions;
}

void MainWindow::populateInstanceExtensions(const std::map<std::string, std::vector<VkExtensionProperties>>& layerExtensions)
{
  QTreeWidget* tw = findChild<QTreeWidget*>("extensionsWidget");
  Q_ASSERT(tw);

  for (const auto& it : layerExtensions) {
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
}

void MainWindow::populateGpus(const std::vector<VkPhysicalDeviceProperties> &gpuProperties)
{
  QComboBox* cb = findChild<QComboBox*>("gpus");
  Q_ASSERT(cb);

  cb->clear();

  for (const auto& elem : gpuProperties) {
    cb->addItem(QString::fromUtf8(elem.deviceName));
  }
}
