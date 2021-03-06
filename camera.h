#ifndef DSLR_CAMERA
#define DSLR_CAMERA

#include "dslrException.h"

namespace dslr {

class Exception;

class Camera {
  
  EdsCameraRef camera_;
  EdsStreamRef stream_;
  bool isLive_;
  
  friend class CameraList;
  
 public:
  ~Camera();

  bool IsLive() const { return isLive_; }
  
  template<typename T>
  void SetProperty(const EdsPropertyID propId, const T& data);

  template<typename T>
  void GetProperty(const EdsPropertyID propId, T* const data) const;

  void SetCapacity(const int freeClusters, const int bytesPerSector, const bool reset);

 private:
  Camera(EdsCameraRef camera);
  void Initialize();
  void Shutdown();

  static EdsError EDSCALLBACK handleObjectEvent(
      EdsObjectEvent        event,
      EdsBaseRef            object,
      EdsVoid*              context);
  
  static EdsError EDSCALLBACK handlePropertyEvent(
      EdsPropertyEvent        inEvent,
      EdsPropertyID           inPropertyID,
      EdsUInt32               inParam,
      EdsVoid *               inContext );

  static EdsError EDSCALLBACK handleStateEvent(
      EdsStateEvent           inEvent,
      EdsUInt32               inEventData,
      EdsVoid *               inContext );

  EdsError DirItemRequestTransfer(EdsDirectoryItemRef item);
};


template<typename T>
void Camera::SetProperty(const EdsPropertyID propId, const T& data)
{
  EdsError err = EdsSetPropertyData(camera_, propId, 0, sizeof(T), &data);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
}

template<typename T>
void Camera::GetProperty(const EdsPropertyID propId, T* const data) const
{
  EdsError err = EdsGetPropertyData(camera_, propId, 0, sizeof(T), data);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
}


} // namespace dslr

#endif
