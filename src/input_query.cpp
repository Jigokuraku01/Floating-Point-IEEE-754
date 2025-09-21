#include "input_query.hpp"

#include "i_possible_float_numbers.hpp"

[[nodiscard]]
PossibleFloat InputQuery::get_first_number() const noexcept {
    return _first_number;
}
[[nodiscard]]
PossibleFloat InputQuery::get_second_number() const noexcept {
    return _second_number;
}

[[nodiscard]]
PossibleFloat InputQuery::get_third_number() const noexcept {
    return _third_number;
}
[[nodiscard]]
PossibleOperations InputQuery::get_cur_operation() const noexcept {
    return _cur_operation;
}
[[nodiscard]]
PossibleRounding InputQuery::get_cur_rounding() const noexcept {
    return _cur_rounding;
}

InputQuery::InputQuery(PossibleFloat first_number, PossibleFloat second_number,
                       PossibleFloat third_number,
                       PossibleRounding cur_rounding,
                       PossibleOperations cur_operation)
    : _first_number(first_number), _second_number(second_number),
      _third_number(third_number), _cur_rounding(cur_rounding),
      _cur_operation(cur_operation) {
}
