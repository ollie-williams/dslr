#include "base.h"
#include "dslrException.h"
#include "camera.h"

namespace dslr {

Camera::Camera(EdsCameraRef camera)
    : camera_(camera)
    , stream_(nullptr)
{}

Camera::~Camera()
{
  if (stream_) {
    EdsRelease(stream_);
    stream_ = nullptr;
  }
  
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

  err = EdsCreateMemoryStream(0, &stream_);
  if (EDS_ERR_OK != err) {
    throw Exception(err);
  }
  
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
  Camera* camera = reinterpret_cast<Camera*>(context);
  
  switch(event) {
    case kEdsObjectEvent_DirItemRequestTransfer:
      return camera->DirItemRequestTransfer(object);
    
    default:
      std::cout << "Object event " << std::hex << event << "\n";
      return EDS_ERR_OK;
  }
}

EdsError Camera::DirItemRequestTransfer(EdsDirectoryItemRef item)
{  
  std::cout << "Object event DirItemRequestTransfer\n";

  EdsDirectoryItemInfo itemInfo;
  EdsError err = EdsGetDirectoryItemInfo(item, &itemInfo);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    return err;
  }

  std::cout << "Item info:\n"
            << "      size:" << itemInfo.size << '\n'
            << "  isFolder:" << itemInfo.isFolder << '\n'
            << "   groupID:" << itemInfo.groupID << '\n'
            << "    option:" << itemInfo.option << '\n'
            << "szFileName:" << itemInfo.szFileName << '\n'
            << "    format:" << itemInfo.format << '\n'
            << "  dateTime:" << itemInfo.dateTime << '\n';


  std::cout << "Downloading image...\n";
  err = EdsDownload(item, itemInfo.size, stream_);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    return err;
  }

  std::cout << "Signalling download complete...\n";
  err = EdsDownloadComplete(item);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    return err;
  }
  
  std::cout << "Attempting to save to " << itemInfo.szFileName << "...\n";
  FILE* fp = fopen(itemInfo.szFileName, "w");
  if (nullptr == fp) {
    DEBUGSTREAM << "Error opening file " << itemInfo.szFileName << '\n';
    return EDS_ERR_FILE_IO_ERROR;
  }

  EdsVoid* ptr = nullptr;
  err = EdsGetPointer(stream_, &ptr);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    return err;
  }

  EdsUInt32 length = 0;
  err = EdsGetLength(stream_, &length);
  if (EDS_ERR_OK != err) {
    DEBUGSTREAM << DescribeError(err) << std::endl;
    return err;
  }

  const size_t count = fwrite(ptr, length, 1, fp);
  if (1 != count) {
    DEBUGSTREAM << "Error writing to file " << itemInfo.szFileName << '\n';
    return EDS_ERR_FILE_IO_ERROR;
  } 
  
  fclose(fp);

  std::cout << "Done saving to " << itemInfo.szFileName << "...\n";
  
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK Camera::handlePropertyEvent(
    EdsPropertyEvent        inEvent,
    EdsPropertyID           inPropertyID,
    EdsUInt32               inParam,
    EdsVoid *               inContext )
{
  //std::cout << "Property event " << std::hex << inPropertyID << "\n";
  return EDS_ERR_OK;
}

EdsError EDSCALLBACK Camera::handleStateEvent(
    EdsStateEvent           inEvent,
    EdsUInt32               inEventData,
    EdsVoid *               inContext )
{
  switch(inEvent) {
    case kEdsStateEvent_Shutdown:
      std::cout << "Shutting down..." << std::endl;
      exit(0);
      return EDS_ERR_OK;
    default:
      break;
  }
  
  std::cout << "State event " << std::hex << inEvent << "\n";
  return EDS_ERR_OK;
}


} // namespace dslr
