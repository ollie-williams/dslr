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
  //std::cout << "Property event " << std::hex << inPropertyID << "\n";
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK handleStateEvent(
    EdsStateEvent           inEvent,
    EdsUInt32               inEventData,
    EdsVoid *               inContext )
{
  std::cout << "State event.\n";
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

EdsError SetDevice(EdsCameraRef camera, EdsUInt32 device)
{
  return EdsSetPropertyData(camera,
                           kEdsPropID_Evf_OutputDevice,
                           0,
                           sizeof(device),
                           &device);
}

EdsError StartLiveView(EdsCameraRef camera)
{
  EdsError err;  
  EdsUInt32 device;
  err = GetCurrentDevice(camera, &device);
  if (EDS_ERR_OK != err) {
    return err;
  }
  
  device |= kEdsEvfOutputDevice_PC;
  return SetDevice(camera, device);
}

EdsError EndLiveView(EdsCameraRef camera)
{
  EdsError err;  
  EdsUInt32 device;
  err = GetCurrentDevice(camera, &device);
  if (EDS_ERR_OK != err) {
    return err;
  }

  device &= ~kEdsEvfOutputDevice_PC;
  return SetDevice(camera, device);  
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

    err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, NULL);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All, handlePropertyEvent, NULL);
    if (EDS_ERR_OK != err) {
      throw err;
    }
    
    err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All, handleStateEvent, NULL);
    if (EDS_ERR_OK != err) {
      throw err;
    }

    std::cout << "Opening session.\n";
    err = EdsOpenSession(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }

    std::cout << "Starting live view.\n";
    err = StartLiveView(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }

    std::cout << "Ending live view.\n";
    err = EndLiveView(camera);
    if (EDS_ERR_OK != err) {
      throw err;
    }

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







