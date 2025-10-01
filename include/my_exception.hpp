#pragma once

#include <string>

class MyException {
  public:
    MyException(int64_t code, std::string error);

    [[nodiscard]]
    int64_t get_code() const;
    [[nodiscard]]
    std::string get_error() const;

  private:
    int64_t m_code;
    std::string m_error;
};