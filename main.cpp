#include <iostream>

#define __MACOS__
#include "EDSDK/Header/EDSDK.h"

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

  EdsRelease(cameraList);

  std::cout << "Unitializing library\n";
  EdsTerminateSDK();
  
  return 0;
}
