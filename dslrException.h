namespace dslr {

class Exception : public std::runtime_error {  
 public:
  Exception(const EdsError error);

 private:
  static std::string GetMessage(const EdsError error);
  
  const EdsError error_;
};

} // dslr
