#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  void  createVulkanInstance();
  void  destroyVulkanInstance();
  void  createVulkanSurface();
  void  destroyVulkanSurface();
  void  initGpus(std::vector<VkPhysicalDevice>* gpus, std::vector<VkPhysicalDeviceProperties>* gpuProperties);

private:
  void  enumerateInstanceLayers(std::vector<VkLayerProperties>* layers);
  void  populateInstanceLayers(const std::vector<VkLayerProperties>& layers);
  void  enumerateInstanceExtensions(const char* layerName, std::vector<VkExtensionProperties>* extensions);
  void  scanInstanceExtensions(const std::vector<VkLayerProperties>& layers, std::map<std::string, std::vector<VkExtensionProperties>>* layerExtensions);
  void  populateInstanceExtensions(const std::map<std::string, std::vector<VkExtensionProperties>>& layerExtensions);
  void  populateGpus(const std::vector<VkPhysicalDeviceProperties>& gpuProperties);

private:
  Ui::MainWindow *ui;

  std::vector<VkLayerProperties>                            mInstanceLayers;
  std::map<std::string, std::vector<VkExtensionProperties>> mInstanceLayerExtensions;

  VkInstance                    mVkInstance = VK_NULL_HANDLE;
  VkSurfaceKHR                  mVkSurface = VK_NULL_HANDLE;

  std::vector<VkPhysicalDevice>           mVkGpus;
  std::vector<VkPhysicalDeviceProperties> mVkGpuProperties;
};

#endif // MAINWINDOW_H
