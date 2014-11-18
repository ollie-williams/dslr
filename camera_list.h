#ifndef DSLR_CAMERA_LIST
#define DSLR_CAMERA_LIST

namespace dslr {

class Camera;

class CameraList {
 public:
  CameraList();
  ~CameraList();
    
  int Count() const;
  Camera get(const int index);

 private:
  EdsCameraListRef list_;  
};

} // namespace dslr

#endif
