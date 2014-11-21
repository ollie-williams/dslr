#include "base.h"
#include "dslrException.h"
#include "camera.h"

namespace dslr {

Camera::Camera(EdsCameraRef camera)
    : camera_(camera)
{}

Camera::~Camera()
{
  if (camera_) {
    Shutdown();
    EdsRelease(camera_);
    camera_ = nullptr;
  }
}  

void Camera::SetCapacity(const int freeClusters, const int bytesPerSector, const bool reset)
{
  EdsCapacity capacity;
  capacity.numberOfFreeClusters = freeClusters;
  capacity.bytesPerSector = bytesPerSector;
  capacity.reset = reset;
  EdsError err = EdsSetCapacity(camera_, capacity);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
}


void Camera::Initialize()
{
  assert(camera_);
  
  EdsError err;
  
  err = EdsSetObjectEventHandler(camera_, kEdsObjectEvent_All, handleObjectEvent, this);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
  
  err = EdsSetPropertyEventHandler(camera_, kEdsPropertyEvent_All, handlePropertyEvent, this);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
  
  err = EdsSetCameraStateEventHandler(camera_, kEdsStateEvent_All, handleStateEvent, this);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
    
  err = EdsOpenSession(camera_);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
}

void Camera::Shutdown()
{
  assert(camera_);

  EdsError err = EdsCloseSession(camera_);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }  
}

EdsError EDSCALLBACK Camera::handleObjectEvent(
    EdsObjectEvent event,
    EdsBaseRef     object,
    EdsVoid*       context  )
{
  std::cout << "Object event " << std::hex << event << "\n";
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK Camera::handlePropertyEvent(
    EdsPropertyEvent        inEvent,
    EdsPropertyID           inPropertyID,
    EdsUInt32               inParam,
    EdsVoid *               inContext )
{
  std::cout << "Property event " << std::hex << inPropertyID << "\n";
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK Camera::handleStateEvent(
    EdsStateEvent           inEvent,
    EdsUInt32               inEventData,
    EdsVoid *               inContext )
{
  std::cout << "State event " << std::hex << inEvent << "\n";
  return EDS_ERR_OK;
}


} // namespace dslr
