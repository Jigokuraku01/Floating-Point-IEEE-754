#pragma once

#include <string>

class MyException {
  public:
    MyException(int32_t code, std::string error);

    [[nodiscard]]
    int32_t get_code() const;
    [[nodiscard]]
    std::string get_error() const;

  private:
    int32_t _code;
    std::string _error;
};