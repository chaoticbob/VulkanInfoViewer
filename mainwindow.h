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

//  struct GpuPropertiesData {
//    VkPhysicalDeviceProperties                      device_properties;
//    VkPhysicalDeviceDescriptorIndexingPropertiesEXT descriptor_indexing_properties;
//    std::vector<VkExtensionProperties>              extensions;
//  };
//  using GpuProperties = std::map<VkPhysicalDevice, GpuPropertiesData>;

  struct GpuProperties {
    VkPhysicalDevice                                physicalDevice;
    VkPhysicalDeviceProperties                      deviceProperties;
    VkPhysicalDeviceDescriptorIndexingPropertiesEXT descriptorIndexingProperties;
    std::vector<VkExtensionProperties>              extensions;
    // Extra properties
    VkPhysicalDeviceShaderCorePropertiesAMD         amdShaderCoreProperties;
    std::string                                     description;
  };

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

  void  enumerateGpus();
  QString getFullGpuName(const GpuProperties* pProperties) const;
  void  populateGpus();

  void  populateGeneral(const GpuProperties* pGpuProperties);
  void  populateDeviceExtensions(const GpuProperties* pGpuProperties);
  void  populateLimits(const GpuProperties* pGpuProperties);
  void  populateSparse(const GpuProperties* pGpuProperties);
  void  populateFeatures(const GpuProperties* pGpuProperties);
  void  populateSurface(const GpuProperties* pGpuProperties);
  void  populateQueues(const GpuProperties* pGpuProperties);
  void  populateMemory(const GpuProperties* pGpuProperties);
  void  populateFormats(const GpuProperties* pGpuProperties);

  void  updateSurfaceExtents(const GpuProperties* pGpuProperties);

private:
  Ui::MainWindow *ui;

  std::vector<VkLayerProperties>      mInstanceLayers;
  LayerExtensions                     mInstanceLayerExtensions;

  VkInstance                          mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR                        mSurface = VK_NULL_HANDLE;

  std::vector<GpuProperties>          mGpuProperties;
  const GpuProperties*                mCurrentGpuProperties = nullptr;

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
