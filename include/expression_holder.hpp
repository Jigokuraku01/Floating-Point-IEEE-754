#pragma once
#include "i_possible_float_numbers.hpp"
#include "input_query.hpp"
#include <cstdint>
#include <string>
class ExpressionHolder {
  public:
    explicit ExpressionHolder(InputQuery inpQuery);
    std::pair<std::string, std::string> solve_and_return_ans();

  private:
    PossibleFloat use_oper();
    std::int64_t round_to_bin_and_shift(std::int64_t inpValue,
                                        std::int64_t cntOfBits);

    std::int64_t divide_int(std::int64_t big_first_numb,
                            std::int64_t big_second_numb,
                            bool should_shift_numer = true);
    /*
    PossibleFloat divide(PossibleFloat first_float, PossibleFloat second_float);
    PossibleFloat mult(PossibleFloat first_float, PossibleFloat second_float);
    PossibleFloat plus(PossibleFloat first_float, PossibleFloat second_float);
    PossibleFloat minus(PossibleFloat first_float, PossibleFloat second_float);
    PossibleFloat fma(PossibleFloat first_float, PossibleFloat second_float,
                      PossibleFloat third_float);
    PossibleFloat mad(PossibleFloat first_float, PossibleFloat second_float,
                      PossibleFloat third_float);
*/
    static std::pair<std::string, std::string>
    format_to_output(PossibleFloat inp_float);
    const InputQuery _curInpQuery;
};

std::uint64_t get_all_ones_at_inp_bit_cnt(std::uint32_t inpCnt);
std::int64_t cut_number(std::int64_t inpNumber, std::int64_t cnt_bits);