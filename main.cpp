#include "base.h"
#include "camera_list.h"
#include "camera.h"

using namespace dslr;

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







