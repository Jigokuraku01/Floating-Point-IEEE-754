#include "input_query.hpp"

#include "i_possible_float_numbers.hpp"

[[nodiscard]]
PossibleFloat InputQuery::get_first_number() const noexcept {
    return m_first_number;
}
[[nodiscard]]
PossibleFloat InputQuery::get_second_number() const noexcept {
    return m_second_number;
}

[[nodiscard]]
PossibleFloat InputQuery::get_third_number() const noexcept {
    return m_third_number;
}
[[nodiscard]]
PossibleOperations InputQuery::get_cur_operation() const noexcept {
    return m_cur_operation;
}
[[nodiscard]]
PossibleRounding InputQuery::get_cur_rounding() const noexcept {
    return m_cur_rounding;
}

InputQuery::InputQuery(PossibleFloat first_number, PossibleFloat second_number,
                       PossibleFloat third_number,
                       PossibleRounding cur_rounding,
                       PossibleOperations cur_operation)
    : m_first_number(first_number), m_second_number(second_number),
      m_third_number(third_number), m_cur_rounding(cur_rounding),
      m_cur_operation(cur_operation) {
}
