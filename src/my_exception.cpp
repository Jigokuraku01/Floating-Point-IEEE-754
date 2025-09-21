#include "my_exception.hpp"

#include <string>
MyException::MyException(int32_t code, std::string error)
    : _code(code), _error(std::move(error)) {
}

[[nodiscard]]
int32_t MyException::get_code() const {
    return _code;
}

[[nodiscard]]
std::string MyException::get_error() const {
    return _error;
}
