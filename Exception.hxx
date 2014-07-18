#ifndef EXCEPTION_HXX
#define EXCEPTION_HXX

#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>

class Exception
    : public std::runtime_error
{
public:
    Exception();
    
    template <typename Type>
    Exception & operator << (Type&& value)
    {
        std::ostringstream converter;
        converter << std::forward<Type>(value);
        message_ += converter.str();
        return (*this);
    }
    
    virtual char const * what() const noexcept override;
    
private:
    std::string message_;
};

#endif // EXCEPTION_HXX
