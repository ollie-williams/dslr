#include "base.h"
#include "dslrException.h"

namespace dslr {

Exception::Exception(const EdsError error)
    : std::runtime_error(Message(error))
    , error_(error)
{}

std::string Exception::Message(const EdsError error)
{
  std::stringstream ss;
  ss << "Error code " << std::hex << "error";
  return ss.str();
}


} // namespace dslr
