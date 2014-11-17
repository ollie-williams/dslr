namespace dslr {

class Exception;

class Camera {
 public:

  ~Camera()
  {
    EdsRelease(camera_);
  }  
  
  template<typename T>
  void SetProperty(const EdsPropertyID propId, const T& data)
  {
    EdsError err = EdsSetPropertyData(camera_, propId, 0, sizeof(T), &data);
    if (EDS_ERR_OK != err) {
      throw Exception(err);
    }
  }

  template<typename T>
  void GetProperty(const EdsPropertyID propId, T* const data) const
  {
    EdsError err = EdsGetPropertyData(camera_, propId, 0, sizeof(T), data);
    if (EDS_ERR_OK != err) {
      throw Exception(err);
    }
  }

 private:
  void Initialize();

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
      
  EdsCameraRef camera_;
};

} // namespace dslr
