#include "base.h"
#include "dslrException.h"
#include "camera_list.h"
#include "camera.h"

namespace dslr {

CameraList::CameraList()
    : list_(nullptr)
{  
  EdsError err = EdsGetCameraList(&list_);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    throw Exception(err);
  }
}

CameraList::~CameraList()
{
  if (list_) {
    EdsRelease(list_);
    list_ = nullptr;
  }
}

int CameraList::Count() const
{
  EdsUInt32 count = -1;
  EdsError err =  EdsGetChildCount(list_, &count);
  if (err != EDS_ERR_OK) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    throw Exception(err);
  }
  return count;
}

Camera CameraList::get(const int index)
{
  EdsCameraRef camera = nullptr;
  EdsError err = EdsGetChildAtIndex(list_, 0, &camera);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    throw Exception(err);
  }

  Camera retval = Camera(camera);
  retval.Initialize();
  return retval;
}

} // namespace dslr
