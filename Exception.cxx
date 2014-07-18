#include "Exception.hxx"

Exception::Exception()
    : std::runtime_error("")
{
}

char const * Exception::what() const noexcept
{
    return message_.c_str();
}
