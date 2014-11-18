#include "base.h"
#include "camera_list.h"
#include "camera.h"

using namespace dslr;

#if 0
EdsError GetCurrentDevice(EdsCameraRef camera, EdsUInt32* device)
{
  // Get current device setting
  return EdsGetPropertyData(camera,
                           kEdsPropID_Evf_OutputDevice,
                           0,
                           sizeof(*device),
                           device);
}

EdsError SetDevice(EdsCameraRef camera, EdsUInt32 device)
{
  return SetProperty<kEdsPropID_Evf_OutputDevice>(camera, device);
}

EdsError StartLiveView(EdsCameraRef camera)
{
#if 0
  EdsError err;  
  EdsUInt32 device;
  err = GetCurrentDevice(camera, &device);
  if (EDS_ERR_OK != err) {
    return err;
  }
  std::cout << "Device = " << device << std::endl;
  device |= kEdsEvfOutputDevice_PC;
#endif 
  return SetDevice(camera, kEdsEvfOutputDevice_PC);
}

EdsError EndLiveView(EdsCameraRef camera)
{
#if 0
  EdsError err;  
  EdsUInt32 device;
  err = GetCurrentDevice(camera, &device);
  if (EDS_ERR_OK != err) {
    return err;
  }

  device &= ~kEdsEvfOutputDevice_TFT;
#endif
  return SetDevice(camera, 0);  
}
#endif

int main()
{
  EdsError err;
  
  err = EdsInitializeSDK();
  if (EDS_ERR_OK != err) {
    throw err;
  }

  CameraList cameraList;
    
  std::cout << "There are " << cameraList.Count() << " cameras connected.\n";

  if (cameraList.Count() > 0) {
    Camera camera = cameraList.get(0);
    camera.SetProperty(kEdsPropID_SaveTo, kEdsSaveTo_Host);
  }
    

  EdsTerminateSDK();  
  return 0;
}







