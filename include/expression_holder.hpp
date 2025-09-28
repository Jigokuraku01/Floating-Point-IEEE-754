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
    std::uint64_t round_to_bin_and_shift(std::uint64_t inpValue,
                                         std::uint64_t cntOfBits,
                                         std::uint32_t sign);

    std::uint64_t divide_int(std::uint64_t big_first_numb,
                             std::uint64_t big_second_numb, std::uint32_t sign,
                             bool should_shift_numer = true,
                             std::uint64_t shift = 0);

    PossibleFloat divide(PossibleFloat first_float, PossibleFloat second_float);
    PossibleFloat mult(PossibleFloat first_float, PossibleFloat second_float);
    static PossibleFloat plus(PossibleFloat first_float,
                              PossibleFloat second_float);
    static PossibleFloat minus(PossibleFloat first_float,
                               PossibleFloat second_float);
    PossibleFloat fma(PossibleFloat first_float, PossibleFloat second_float,
                      PossibleFloat third_float);
    PossibleFloat mad(PossibleFloat first_float, PossibleFloat second_float,
                      PossibleFloat third_float);
    //first one is formatted number in formant 1{mant_cnt} << mant_cnt_bits, second is multiplier
    [[nodiscard]]
    std::pair<std::uint32_t, std::int32_t> format_big_number_to_mant_format(
        std::uint64_t inp_number, std::uint32_t mant_cnt, std::uint32_t sign);
    //sign have to be gained from PossibleFloat
    //mant in format 1.{some_numbers}. Size of some_numbers = mant_cnt. This function should round with exp
    void format_int_exp_and_sign_to_possible_float(PossibleFloat& ans_float,
                                                   std::uint64_t mant,
                                                   std::int64_t exp);
    static std::pair<bool, PossibleFloat>
    divide_checks(PossibleFloat first_float, PossibleFloat second_float);
    static std::pair<bool, PossibleFloat>
    mult_checks(PossibleFloat first_float, PossibleFloat second_float);
    static std::pair<bool, PossibleFloat>
    plus_checks(PossibleFloat first_float, PossibleFloat second_float);

    static std::pair<std::string, std::string>
    format_to_output(const PossibleFloat& inp_float);

    const InputQuery m_curInpQuery;
};

std::uint64_t get_all_ones_at_inp_bit_cnt(std::uint32_t inpCnt);
std::int64_t cut_number(std::int64_t inpNumber, std::int64_t cnt_bits);