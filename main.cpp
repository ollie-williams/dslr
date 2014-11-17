#include <iostream>

#define __MACOS__
#include "EDSDK/Header/EDSDK.h"

EdsError EDSCALLBACK handleObjectEvent(
    EdsObjectEvent event,
    EdsBaseRef     object,
    EdsVoid*       context  )
{
  std::cout << "Object event.\n";
  return EDS_ERR_OK;
}


EdsError EDSCALLBACK handlePropertyEvent(
    EdsPropertyEvent        inEvent,
    EdsPropertyID           inPropertyID,
    EdsUInt32               inParam,
    EdsVoid *               inContext )
{
  std::cout << "Property event " << std::hex << inPropertyID << "\n";
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK handleStateEvent(
    EdsStateEvent           inEvent,
    EdsUInt32               inEventData,
    EdsVoid *               inContext )
{
  std::cout << "State event " << std::hex << inEvent << "\n";
  return EDS_ERR_OK;
}

EdsError GetCurrentDevice(EdsCameraRef camera, EdsUInt32* device)
{
  // Get current device setting
  return EdsGetPropertyData(camera,
                           kEdsPropID_Evf_OutputDevice,
                           0,
                           sizeof(*device),
                           device);
}

template<EdsPropertyID PropID, typename T>
EdsError SetProperty(EdsCameraRef camera, const T& data)
{
  return EdsSetPropertyData(camera,
                            PropID,
                            0,
                            sizeof(T),
                            &data);
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

int main()
{
  EdsError err;
  
  std::cout << "Initializing library\n";
  err = EdsInitializeSDK();
  if (EDS_ERR_OK != err) {
    throw err;
  }

  EdsCameraListRef cameraList = nullptr;
  err = EdsGetCameraList(&cameraList);
  if (EDS_ERR_OK != err) {
    throw err;
  }

  EdsUInt32 cameraCount = -1;
  err = EdsGetChildCount(cameraList, &cameraCount);
  if (EDS_ERR_OK != err) {
    throw err;
  }
  std::cout << "There are " << cameraCount << " cameras connected.\n";

  if (cameraCount > 0) {

    EdsCameraRef camera = nullptr;
    err = EdsGetChildAtIndex(cameraList, 0, &camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, nullptr);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All, handlePropertyEvent, nullptr);
    if (EDS_ERR_OK != err) {
      throw err;
    }
      
    err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All, handleStateEvent, nullptr);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    std::cout << "Opening session.\n";
    err = EdsOpenSession(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }

#if 0
    std::cout << "Starting live view.\n";
    err = StartLiveView(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }
#endif

    

#if 0
    std::cout << "Ending live view.\n";
    err = EndLiveView(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }
#endif
    
    std::cout << "Closing session.\n";
    err = EdsCloseSession(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    EdsRelease(camera);    
  }

  EdsRelease(cameraList);

  std::cout << "Unitializing library\n";
  EdsTerminateSDK();
  
  return 0;
}







