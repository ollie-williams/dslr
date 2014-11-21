#ifndef DSLR_DSLREXCEPTION
#define DSLR_DSLREXCEPTION

namespace dslr {

std::string DescribeError(const EdsError error);

class Exception : public std::runtime_error {  
 public:
  Exception(const EdsError error);

  EdsError getError() const { return error_; }

 private:
  const EdsError error_;
};

} // dslr

#endif
