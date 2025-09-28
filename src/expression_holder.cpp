#include "expression_holder.hpp"

#include "i_possible_float_numbers.hpp"
#include "input_query.hpp"
#include "iostream"
#include "my_exception.hpp"
#include <cstdint>
#include <cstdlib>
#include <format>
#include <string>
#include <utility>

ExpressionHolder::ExpressionHolder(InputQuery inpQuery)
    : m_curInpQuery(std::move(inpQuery)) {
}

PossibleFloat ExpressionHolder::use_oper() {
    PossibleFloat ans;
    switch (m_curInpQuery.get_cur_operation()) {
        case (PossibleOperations::DIV): {
            ans = divide(m_curInpQuery.get_first_number(),
                         m_curInpQuery.get_second_number());
            break;
        }
        case (PossibleOperations::PLUS): {
            ans = plus(m_curInpQuery.get_first_number(),
                       m_curInpQuery.get_second_number());
            break;
        }
        case (PossibleOperations::MINUS): {
            ans = minus(m_curInpQuery.get_first_number(),
                        m_curInpQuery.get_second_number());
            break;
        }
        case (PossibleOperations::MUL): {
            ans = mult(m_curInpQuery.get_first_number(),
                       m_curInpQuery.get_second_number());
            break;
        }
        case (PossibleOperations::FMA): {
            ans = fma(m_curInpQuery.get_first_number(),
                      m_curInpQuery.get_second_number(),
                      m_curInpQuery.get_third_number());
            break;
        }
        case (PossibleOperations::MAD): {
            ans = mad(m_curInpQuery.get_first_number(),
                      m_curInpQuery.get_second_number(),
                      m_curInpQuery.get_third_number());
            break;
        }
        default: {
            throw MyException(EXIT_FAILURE, "invalid operation");
        }
    }
    return ans;
}

std::pair<std::string, std::string> ExpressionHolder::solve_and_return_ans() {
    PossibleFloat ans = m_curInpQuery.get_first_number();
    if (m_curInpQuery.get_cur_operation() != PossibleOperations::NO_OPER) {
        ans = use_oper();
    }
    return format_to_output(ans);
}

std::pair<std::string, std::string>
ExpressionHolder::format_to_output(const PossibleFloat& inp_float) {
    std::pair<std::string, std::string> ans;
    ans.second = std::format(
        "0x{:0{}X}", inp_float.get_numb(),
        (inp_float.get_exp_cnt() + inp_float.get_mant_cnt() + 4) / 4);
    if (inp_float.check_if_zero()) {
        if (inp_float.get_bit_for_sign() == 0) {
            ans.first = "0x0p+0";
        }
        else {
            ans.first = "-0x0p+0";
        }
        return ans;
    }
    if (inp_float.check_if_nan()) {
        ans.first = "nan";
        return ans;
    }
    if (inp_float.check_if_inf()) {
        if (inp_float.get_bit_for_sign() == 0) {
            ans.first = "inf";
        }
        else {
            ans.first = "-inf";
        }
        return ans;
    }

    NormalFloatNumerHandler cur_normal_form = inp_float.get_normal_form();
    std::uint32_t mant = cur_normal_form.get_norm_mant();
    std::string sign_str;
    if (inp_float.get_bit_for_sign() == 1) {
        sign_str = "-";
    }
    std::int32_t act_exp = cur_normal_form.get_norm_exp();
    std::string exp_sign = "+";
    if (act_exp < 0) {
        exp_sign = "";
    }
    auto hex_digits_for_mantissa =
        static_cast<std::int32_t>((inp_float.get_mant_cnt() + 3) / 4);
    mant <<= static_cast<std::uint32_t>(hex_digits_for_mantissa * 4) -
             inp_float.get_mant_cnt();
    ans.first = std::format("{}0x1.{:0{}x}p{}{}", sign_str, mant,
                            hex_digits_for_mantissa, exp_sign, act_exp);
    return ans;
}

std::uint64_t ExpressionHolder::round_to_bin_and_shift(std::uint64_t inpValue,
                                                       std::uint64_t cntOfBits,
                                                       std::uint32_t sign) {
    return divide_int(inpValue, 1LL << cntOfBits, sign, false, cntOfBits);
}

std::uint64_t ExpressionHolder::divide_int(std::uint64_t big_first_numb,
                                           std::uint64_t big_second_numb,
                                           std::uint32_t sign,
                                           bool should_shift_numer,
                                           std::uint64_t shift) {
    if (sign >= 2) {
        throw MyException(EXIT_FAILURE, "invalid sign");
    }
    if (should_shift_numer) {
        big_first_numb <<= shift;
    }
    std::uint64_t div_tmp_ans = big_first_numb / big_second_numb;
    if (div_tmp_ans * big_second_numb == big_first_numb) {
        return div_tmp_ans;
    }
    std::uint64_t ans;
    switch (m_curInpQuery.get_cur_rounding()) {
        case (PossibleRounding::TOWARD_ZERO): {
            ans = div_tmp_ans;
            break;
        }
        case (PossibleRounding::TOWARD_POS_INFINITY): {
            if (div_tmp_ans == 0) {
                if (sign == 1) {
                    ans = 0;
                }
                else {
                    ans = 1;
                }
            }
            else if (sign == 1) /*sign = 1 => neg number */ {
                ans = div_tmp_ans;
            }
            else {
                ans = div_tmp_ans + 1;
            }
            break;
        }
        case (PossibleRounding::TOWARD_NEG_INFINITY): {
            if (div_tmp_ans == 0) {
                if (sign == 0) {
                    ans = 0;
                }
                else {
                    ans = 1;
                }
            }
            else if (sign == 0) /*sign = 0 => pos number */ {
                ans = div_tmp_ans;
            }
            else {
                ans = div_tmp_ans + 1;
            }
            break;
        }
        case (PossibleRounding::TOWARD_NEAREST_EVEN): {
            ans = div_tmp_ans;
            std::uint64_t mod =
                big_first_numb - (big_second_numb * div_tmp_ans);
            if (mod * 2 > big_second_numb) {
                ans += 1;
            }
            if (mod * 2 == big_second_numb) {
                if ((ans & 1) == 1) {
                    ans += 1;
                }
            }
            break;
        }
    }
    return ans;
}

void ExpressionHolder::format_int_exp_and_sign_to_possible_float(
    PossibleFloat& ans_float, std::uint64_t mant, std::int64_t exp) {
    if (exp >= ans_float.get_min_non_denormalized_exp() &&
        exp <= ans_float.get_max_exp()) {
        ans_float.set_exp(
            static_cast<std::uint32_t>(exp + ans_float.get_exp_bias()));
        ans_float.set_mant(static_cast<std::uint32_t>(mant));
        return;
    }
    if (exp > ans_float.get_max_exp()) {
        switch (m_curInpQuery.get_cur_rounding()) {
            case (PossibleRounding::TOWARD_ZERO): {
                ans_float =
                    ans_float.create_max_number(ans_float.get_bit_for_sign());
                break;
            }
            case (PossibleRounding::TOWARD_NEAREST_EVEN): {
                ans_float = ans_float.create_inf(ans_float.get_bit_for_sign());
                break;
            }
            case (PossibleRounding::TOWARD_POS_INFINITY): {
                if (ans_float.get_bit_for_sign() ==
                    PossibleFloat::pos_sign_code) {
                    ans_float =
                        ans_float.create_inf(ans_float.get_bit_for_sign());
                }
                else {
                    ans_float = ans_float.create_max_number(
                        ans_float.get_bit_for_sign());
                }
                break;
            }
            case (PossibleRounding::TOWARD_NEG_INFINITY): {
                if (ans_float.get_bit_for_sign() ==
                    PossibleFloat::neg_sign_code) {
                    ans_float =
                        ans_float.create_inf(ans_float.get_bit_for_sign());
                }
                else {
                    ans_float = ans_float.create_max_number(
                        ans_float.get_bit_for_sign());
                }
                break;
            }
            default: {
                throw MyException(EXIT_FAILURE, "invalid rounding");
            }
        }
        return;
    }
    ans_float.set_exp(0);
    auto act_exp_without_denormalized_tail = static_cast<std::int32_t>(
        exp - ans_float.get_min_non_denormalized_exp() +
        static_cast<std::int32_t>(ans_float.get_mant_cnt()));
    ans_float.set_mant(static_cast<std::uint32_t>(round_to_bin_and_shift(
        mant,
        static_cast<std::uint32_t>(
            static_cast<std::int32_t>(ans_float.get_mant_cnt()) -
            act_exp_without_denormalized_tail),
        ans_float.get_bit_for_sign())));
}

std::uint64_t get_all_ones_at_inp_bit_cnt(std::uint32_t inpCnt) {
    if (inpCnt >= 64) {
        return ~(static_cast<std::uint64_t>(0));
    }
    return (static_cast<std::uint64_t>(1) << inpCnt) -
           static_cast<std::uint64_t>(1);
}

std::pair<std::uint32_t, std::int32_t>
ExpressionHolder::format_big_number_to_mant_format(std::uint64_t inp_number,
                                                   std::uint32_t mant_cnt,
                                                   std::uint32_t sign) {
    std::uint32_t max_one_bit_cnt = UINT32_MAX;
    for (std::uint32_t i = 0; i < sizeof(inp_number) * 8; ++i) {
        if ((inp_number >> i) % 2 != 0) {
            max_one_bit_cnt = i;
        }
    }
    if (max_one_bit_cnt == UINT32_MAX) {
        throw MyException(EXIT_FAILURE, "Runtime problem with formatting");
    }
    if (max_one_bit_cnt <= 2 * mant_cnt) {
        return {round_to_bin_and_shift(inp_number
                                           << (2 * mant_cnt - max_one_bit_cnt),
                                       mant_cnt, sign),
                static_cast<std::int32_t>(max_one_bit_cnt) -
                    static_cast<std::int32_t>(mant_cnt)};
    }
    std::uint64_t tmp =
        round_to_bin_and_shift(inp_number, max_one_bit_cnt - mant_cnt, sign) &
        get_all_ones_at_inp_bit_cnt(mant_cnt);

    return {tmp, max_one_bit_cnt - (2 * mant_cnt)};
}
