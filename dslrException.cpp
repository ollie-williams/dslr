#include "base.h"
#include "dslrException.h"

namespace dslr {

Exception::Exception(const EdsError error)
    : std::runtime_error(GetMessage(error))
    , error_(error)
{}

std::string Exception::GetMessage(const EdsError error)
{
  return "todo: obtain messages";
}


} // namespace dslr
