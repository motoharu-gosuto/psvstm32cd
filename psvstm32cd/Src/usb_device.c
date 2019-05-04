#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"

// USB Device Core handle declaration.
USBD_HandleTypeDef hUsbDeviceFS;

// Init USB device Library, add supported class and start the library
void MX_USB_DEVICE_Init(void)
{
  // Init Device Library, add supported class and start the library.
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC);

  USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS);

  USBD_Start(&hUsbDeviceFS);
}
