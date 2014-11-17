namespace dslr {

class Exception : public std::runtime_error {  
 public:
  Exception(const EdsError error);

  EdsError getError() const { return error_; }

 private:
  static std::string Message(const EdsError error);
  
  const EdsError error_;
};

} // dslr
