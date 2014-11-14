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


  std::cout << "Unitializing library\n";
  EdsTerminateSDK();
  
  return 0;
}
