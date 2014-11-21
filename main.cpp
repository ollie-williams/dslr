#include "base.h"
#include "camera_list.h"
#include "camera.h"

#include <CoreFoundation/CFRunLoop.h>

using namespace dslr;

void experiment()
{
  DEBUGSTREAM << '\n';
  CameraList cameraList;  
  std::cout << "There are " << cameraList.Count() << " cameras connected.\n";
  if (0 == cameraList.Count()) {
    return;
  }

  DEBUGSTREAM << '\n';
  Camera camera = cameraList.get(0);
  DEBUGSTREAM << '\n';

  // Save images to host
  camera.SetProperty(kEdsPropID_SaveTo, kEdsSaveTo_Host);
  DEBUGSTREAM << '\n';
  camera.SetCapacity(36864*9999, 512*8, true);
  DEBUGSTREAM << '\n';
  
  // Wait
  //sleep(2);
  while(true) {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
  }

}

int main()
{    
  EdsError err = EdsInitializeSDK();
  if (EDS_ERR_OK != err) {
    std::cerr << "Unable to initialize SDK.\n";
    return -1;
  }

  try {
    experiment();
  }
  catch(Exception e) {
    std::cerr << "DSLR exception. msg = \n" << e.what() << std::endl;
  }
  catch(...) {
    std::cerr << "Unidentified exception." << std::endl;
  }
  
  EdsTerminateSDK();  
  return 0;
}







