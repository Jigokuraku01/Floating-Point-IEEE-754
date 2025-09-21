#pragma once

#include "i_possible_float_numbers.hpp"
#include <cstdint>
enum class PossibleOperations : std::uint8_t {
    PLUS,
    MINUS,
    MUL,
    DIV,
    MAD,
    FMA,
    NO_OPER,
};

enum class PossibleRounding : std::uint8_t {
    TOWARD_ZERO,
    TOWARD_NEAREST_EVEN,
    TOWARD_POS_INFINITY,
    TOWARD_NEG_INFINITY,
};
class InputQuery {
  public:
    InputQuery(PossibleFloat first_number, PossibleFloat second_number,
               PossibleFloat third_number,
               PossibleRounding cur_rounding = PossibleRounding::TOWARD_ZERO,
               PossibleOperations cur_operation = PossibleOperations::NO_OPER);

    [[nodiscard]]
    PossibleFloat get_first_number() const noexcept;
    [[nodiscard]]
    PossibleFloat get_second_number() const noexcept;
    [[nodiscard]]
    PossibleFloat get_third_number() const noexcept;
    [[nodiscard]]
    std::int64_t get_cnt_for_integer() const noexcept;
    [[nodiscard]]
    std::int64_t get_cnt_for_fractional() const noexcept;
    [[nodiscard]]
    PossibleOperations get_cur_operation() const noexcept;
    [[nodiscard]]
    PossibleRounding get_cur_rounding() const noexcept;

  private:
    const PossibleFloat _first_number;
    const PossibleFloat _second_number;
    const PossibleFloat _third_number;
    PossibleRounding _cur_rounding;
    PossibleOperations _cur_operation;
};