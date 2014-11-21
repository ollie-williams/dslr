#include "base.h"
#include "dslrException.h"

namespace dslr {

Exception::Exception(const EdsError error)
    : std::runtime_error(DescribeError(error))
    , error_(error)
{}

std::string DescribeError(const EdsError error)
{
  std::stringstream ss;
  ss << "Error code " << std::hex << error;
  switch(error) {
    case EDS_ERR_INVALID_HANDLE:
      ss << ": invalid handle";
      break;
    default:
      break;
  }
  return ss.str();
}


} // namespace dslr
