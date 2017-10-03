#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QTreeWidget>
#include <QStandardItem>

#if defined(_WIN32)
  #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__linux__)
  #define VK_USE_PLATFORM_XCB_KHR
#endif
#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace Ui {
class MainWindow;
}

//! \class MainWindow
//!
//!
class MainWindow : public QMainWindow {
private:
  Q_OBJECT
public:
  using LayerExtensions = std::map<std::string, std::vector<VkExtensionProperties>>;
  using GpuProperties = std::map<VkPhysicalDevice, VkPhysicalDeviceProperties>;

  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();  

private:
  void filterTreeWidgetItemsSimple(const QString& widgetName, const QString& filterText);

protected:
  virtual void resizeEvent(QResizeEvent* event);

private slots:
  void on_gpus_currentIndexChanged(int index);

  void on_limitsFilter_textChanged(const QString &arg1);

  void on_formatFilter_textChanged(const QString &arg1);

  void on_featuresFilter_textChanged(const QString &arg1);

  void on_itemChanged(QStandardItem* item);
  void on_tilingLinearImageType_currentIndexChanged(int index);

  void on_tilingOptimalImageType_currentIndexChanged(int index);

  void on_tilingLinearFormatFilter_textChanged(const QString &arg1);

  void on_tilingOptimalFormatFilter_textChanged(const QString &arg1);

  void on_bufferFormatFilter_textChanged(const QString &arg1);

  void on_expandAllBtn_clicked();

  void on_collapseAllBtn_clicked();

private:
  void  createVulkanInstance();
  void  destroyVulkanInstance();
  void  createVulkanSurface();
  void  destroyVulkanSurface();

private:
  void  enumerateInstanceLayers();
  void  populateInstanceLayers();

  void  enumerateInstanceExtensions();
  void  populateInstanceExtensions();
  void  enumerateDeviceExtensions();
  void  populateDeviceExtensions();

  void  enumerateGpus();
  void  populateGpus();

  void  populateGeneral(VkPhysicalDevice gpu);
  void  populateLimits(VkPhysicalDevice gpu);
  void  populateSparse(VkPhysicalDevice gpu);
  void  populateFeatures(VkPhysicalDevice gpu);
  void  populateSurface(VkPhysicalDevice gpu);
  void  populateQueues(VkPhysicalDevice gpu);
  void  populateMemory(VkPhysicalDevice gpu);
  void  populateFormats(VkPhysicalDevice gpu);

  void  updateSurfaceExtents(VkPhysicalDevice gpu);

private:
  Ui::MainWindow *ui;

  std::vector<VkLayerProperties>      mInstanceLayers;
  LayerExtensions                     mInstanceLayerExtensions;
  std::vector<VkExtensionProperties>  mDeviceExtensions;

  VkInstance                          mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR                        mSurface = VK_NULL_HANDLE;

  std::vector<VkPhysicalDevice>       mGpus;
  VkPhysicalDevice                    mCurrentGpu = VK_NULL_HANDLE;
  GpuProperties                       mGpuProperties;

  struct FilterInputs {
    VkImageTiling tiling = static_cast<VkImageTiling>(UINT32_MAX);
    QLineEdit*    formatFilter = nullptr;
    QComboBox*    imageTypeFilter = nullptr;
    QComboBox*    usageFlagsFilter = nullptr;
    QComboBox*    createFlagsFilter = nullptr;
    QTreeWidget*  target = nullptr;
  };

  FilterInputs  mTilingLinearFilterInputs = {};
  FilterInputs  mTilingOptimalFilterInputs = {};

  std::map<QAbstractItemModel*, FilterInputs*> mFilterInputTargets;
};

#endif // MAINWINDOW_H
