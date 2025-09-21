#include "expression_holder.hpp"
// NOLINTBEGIN
#include "i_possible_float_numbers.hpp"
#include "input_query.hpp"
#include "my_exception.hpp"
#include <cstdlib>
#include <format>
#include <string>
#include <utility>

ExpressionHolder::ExpressionHolder(InputQuery inpQuery)
    : _curInpQuery(std::move(inpQuery)) {
}

std::pair<std::string, std::string> ExpressionHolder::solve_and_return_ans() {
    PossibleFloat ans = _curInpQuery.get_first_number(); /*
    if (_curInpQuery.get_cur_operation() != PossibleOperations::NO_OPER) {
        switch (_curInpQuery.get_cur_operation()) {
            case (PossibleOperations::DIV): {
                ans = divide(_curInpQuery.get_first_number(),
                             _curInpQuery.get_second_number());
                break;
            }
            case (PossibleOperations::PLUS): {
                ans = plus(_curInpQuery.get_first_number(),
                           _curInpQuery.get_second_number());
                break;
            }
            case (PossibleOperations::MINUS): {
                ans = minus(_curInpQuery.get_first_number(),
                            _curInpQuery.get_second_number());
                break;
            }
            case (PossibleOperations::MUL): {
                ans = mult(_curInpQuery.get_first_number(),
                           _curInpQuery.get_second_number());
                break;
            }
            case (PossibleOperations::FMA): {
                ans = fma(_curInpQuery.get_first_number(),
                          _curInpQuery.get_second_number(),
                          _curInpQuery.get_third_number());
                break;
            }
            case (PossibleOperations::MAD): {
                ans = mad(_curInpQuery.get_first_number(),
                          _curInpQuery.get_second_number(),
                          _curInpQuery.get_third_number());
                break;
            }
            default: {
                throw MyException(EXIT_FAILURE, "invalid operation");
            }
        }
    }*/

    return format_to_output(ans);
}

std::pair<std::string, std::string>
ExpressionHolder::format_to_output(const PossibleFloat inp_float) {
    std::pair<std::string, std::string> ans;
    ans.second = std::format(
        "0x{:0{}X}", inp_float.get_numb(),
        (inp_float.get_exp_cnt() + inp_float.get_mant_cnt() + 4) / 4);
    if (inp_float.get_mant() == 0 && inp_float.get_exp() == 0) {
        if (inp_float.get_bit_for_sign() == 0) {
            ans.first = "0x0p+0";
        }
        else {
            ans.first = "-0x0p+0";
        }
        return ans;
    }
    if (inp_float.get_exp() ==
        get_all_ones_at_inp_bit_cnt(inp_float.get_exp_cnt())) {
        if (inp_float.get_mant() != 0) {
            ans.first = "nan";
            return ans;
        }
        if (inp_float.get_bit_for_sign() == 0) {
            ans.first = "inf";
        }
        else {
            ans.first = "-inf";
        }
    }

    if (inp_float.get_exp() == 0) {
        std::uint32_t mant = inp_float.get_mant();
        std::uint32_t last_one_bit_index;
        for (std::uint32_t i = 0; i < inp_float.get_mant_cnt(); ++i) {
            if ((mant & (1 << i)) == 1) {
                last_one_bit_index = i;
            }
        }

        auto shift = static_cast<std::int32_t>(
            ((((inp_float.get_mant() + 3) / 4) * 4) + 1 - last_one_bit_index));
        mant <<= shift;
        mant &= get_all_ones_at_inp_bit_cnt((inp_float.get_mant() / 4) * 4);
        std::string sign_str;
        if (inp_float.get_bit_for_sign() == 1) {
            sign_str = "-";
        }
        std::int32_t act_exp = static_cast<std::int32_t>(inp_float.get_exp()) -
                               ((1 << (inp_float.get_exp_cnt() - 1)) - 1);
        auto hex_digits_for_mantissa =
            static_cast<std::int32_t>((inp_float.get_mant() + 3) / 4);
        ans.first = std::format("{}0x1.{:0{}x}p{}", sign_str, mant,
                                hex_digits_for_mantissa, act_exp - shift);
        return ans;
    }

    std::string sign_str;
    if (inp_float.get_bit_for_sign() == 1) {
        sign_str = "-";
    }
    std::uint32_t mant = inp_float.get_mant();
    auto hex_digits_for_mantissa =
        static_cast<std::int32_t>((inp_float.get_mant_cnt() + 3) / 4);
    mant <<= (hex_digits_for_mantissa * 4 -
              static_cast<std::int32_t>(inp_float.get_mant_cnt()));
    std::int32_t act_exp = static_cast<std::int32_t>(inp_float.get_exp()) -
                           ((1 << (inp_float.get_exp_cnt() - 1)) - 1);
    ans.first = std::format("{}0x1.{:0{}x}p{}", sign_str, mant,
                            hex_digits_for_mantissa, act_exp);

    return ans;
}
/*
PossibleFloat ExpressionHolder::mad(PossibleFloat first_float,
                                    PossibleFloat second_float,
                                    PossibleFloat third_float) {
    return plus(mult(first_float, second_float), third_float);
}

std::int64_t ExpressionHolder::round_to_bin_and_shift(std::int64_t inpValue,
                                                      std::int64_t cntOfBits) {
}

std::int64_t ExpressionHolder::divide_int(std::int64_t big_first_numb,
                                          std::int64_t big_second_numb,
                                          bool should_shift_numer = true) {
}

PossibleFloat ExpressionHolder::divide(PossibleFloat first_float,
                                       PossibleFloat second_float) {
}
PossibleFloat ExpressionHolder::mult(PossibleFloat first_float,
                                     PossibleFloat second_float) {
}
PossibleFloat ExpressionHolder::plus(PossibleFloat first_float,
                                     PossibleFloat second_float) {
}
PossibleFloat ExpressionHolder::minus(PossibleFloat first_float,
                                      PossibleFloat second_float) {
}
PossibleFloat ExpressionHolder::fma(PossibleFloat first_float,
                                    PossibleFloat second_float,
                                    PossibleFloat third_float) {
}
*/
std::uint64_t get_all_ones_at_inp_bit_cnt(std::uint32_t inpCnt) {
    if (inpCnt >= 64) {
        return ~(static_cast<std::uint64_t>(0));
    }
    return (static_cast<std::uint64_t>(1) << inpCnt) -
           static_cast<std::uint64_t>(1);
}
// NOLINTEND