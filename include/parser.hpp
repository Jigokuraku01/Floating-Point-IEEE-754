#pragma once
#include "input_query.hpp"
#include <string>
#include <vector>

class Parser {
  public:
    static InputQuery parse_input_query(const std::vector<std::string>&& argv);

  private:
    static std::uint64_t parse_to_int(const std::string& inpStr,
                                      std::int64_t base);
};
