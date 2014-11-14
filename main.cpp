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
  std::cout << "Property event.\n";
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


int main()
{
  EdsError err;
  
  std::cout << "Initializing library\n";
  err = EdsInitializeSDK();
  if (EDS_ERR_OK != err)
  {
    throw err;
  }

  EdsCameraListRef cameraList = nullptr;
  err = EdsGetCameraList(&cameraList);
  if (EDS_ERR_OK != err)
  {
    throw err;
  }

  EdsUInt32 cameraCount = -1;
  err = EdsGetChildCount(cameraList, &cameraCount);
  if (EDS_ERR_OK != err)
  {
    throw err;
  }
  std::cout << "There are " << cameraCount << " cameras connected.\n";

  if (cameraCount > 0) {

    EdsCameraRef camera = nullptr;
    err = EdsGetChildAtIndex(cameraList, 0, &camera);
    if (EDS_ERR_OK != err)
    {
      throw err;
    }

    // Set event handler
    err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, NULL);
    if (EDS_ERR_OK != err)
    {
      throw err;
    }
    
    // Set event handler
    err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All, handlePropertyEvent, NULL);
    if (EDS_ERR_OK != err)
    {
      throw err;
    }
    
    // Set event handler
    err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All, handleStateEvent, NULL);
    if (EDS_ERR_OK != err)
    {
      throw err;
    }

    
    EdsRelease(camera);    
  }

  EdsRelease(cameraList);

  std::cout << "Unitializing library\n";
  EdsTerminateSDK();
  
  return 0;
}







