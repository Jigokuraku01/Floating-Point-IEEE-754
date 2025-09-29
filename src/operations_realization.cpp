#include "expression_holder.hpp"
#include "i_possible_float_numbers.hpp"
#include <cstdint>
#include <utility>

PossibleFloat ExpressionHolder::divide(const PossibleFloat first_float,
                                       const PossibleFloat second_float) {
    std::pair<bool, PossibleFloat> checked_info =
        divide_checks(first_float, second_float);
    if (checked_info.first) {
        return checked_info.second;
    }
    NormalFloatNumerHandler normal_first_float = first_float.get_normal_form();
    NormalFloatNumerHandler normal_second_float = first_float.get_normal_form();
    PossibleFloat ans(first_float.get_exp_cnt(), first_float.get_mant_cnt());
    ans.set_bit_sign(normal_first_float.get_sign() ^
                     normal_second_float.get_sign());
    std::uint64_t big_first_mant = normal_first_float.get_norm_mant();
    if (first_float.get_exp() != 0) {
        big_first_mant += (1 << first_float.get_mant_cnt());
    }
    std::uint64_t big_second_mant = normal_second_float.get_norm_mant();
    if (second_float.get_exp() != 0) {
        big_second_mant += (1 << second_float.get_mant_cnt());
    }
    std::uint64_t divided_ans =
        divide_int(big_first_mant, big_second_mant, ans.get_bit_for_sign(),
                   true, ans.get_mant_cnt());
    std::pair<std::uint32_t, std::int32_t> formatted_numb =
        format_big_number_to_mant_format(divided_ans << ans.get_mant_cnt(),
                                         ans.get_mant_cnt(),
                                         ans.get_bit_for_sign());
    std::int32_t act_exp = normal_first_float.get_norm_exp() -
                           normal_second_float.get_norm_exp() +
                           formatted_numb.second;
    format_int_exp_and_sign_to_possible_float(ans, formatted_numb.first,
                                              act_exp);

    return ans;
}
PossibleFloat ExpressionHolder::mult(const PossibleFloat first_float,
                                     const PossibleFloat second_float) {
    std::pair<bool, PossibleFloat> checked_info =
        mult_checks(first_float, second_float);
    if (checked_info.first) {
        return checked_info.second;
    }
    NormalFloatNumerHandler normal_first_float = first_float.get_normal_form();
    NormalFloatNumerHandler normal_second_float =
        second_float.get_normal_form();

    PossibleFloat ans(first_float.get_exp_cnt(), first_float.get_mant_cnt());
    ans.set_bit_sign(normal_first_float.get_sign() ^
                     normal_second_float.get_sign());

    std::uint64_t big_first_mant = normal_first_float.get_norm_mant();

    big_first_mant += (1 << first_float.get_mant_cnt());

    std::uint64_t big_second_mant = normal_second_float.get_norm_mant();
    big_second_mant += (1 << second_float.get_mant_cnt());

    std::uint64_t pos_mult = big_first_mant * big_second_mant;

    std::pair<std::uint32_t, std::int32_t> formatted_numb =
        format_big_number_to_mant_format(pos_mult, ans.get_mant_cnt(),
                                         ans.get_bit_for_sign());
    std::int32_t act_exp = normal_first_float.get_norm_exp() +
                           normal_second_float.get_norm_exp() +
                           formatted_numb.second;
    if (act_exp >= ans.get_min_non_denormalized_exp()) {
        format_int_exp_and_sign_to_possible_float(ans, formatted_numb.first,
                                                  act_exp);
    }
    else {
        std::uint32_t index_of_last_one = UINT32_MAX;
        for (std::uint32_t i = 0; i < sizeof(pos_mult) * 8; ++i) {
            if (((1LL << i) & pos_mult) != 0) {
                index_of_last_one = i;
            }
        }
        format_int_exp_and_sign_to_possible_float(
            ans, formatted_numb.first, act_exp, true, pos_mult,
            index_of_last_one - (ans.get_mant_cnt()));
    }

    return ans;
}
PossibleFloat ExpressionHolder::plus(const PossibleFloat first_float,
                                     const PossibleFloat second_float) {
    std::pair<bool, PossibleFloat> checked_info =
        plus_checks(first_float, second_float);
    if (checked_info.first) {
        return checked_info.second;
    }
    NormalFloatNumerHandler normal_first_float = first_float.get_normal_form();
    NormalFloatNumerHandler normal_second_float = first_float.get_normal_form();
    PossibleFloat ans(first_float.get_exp_cnt(), first_float.get_mant_cnt());
    ans.set_bit_sign(normal_first_float.get_sign() ^
                     normal_second_float.get_sign());

    return ans;
}

PossibleFloat ExpressionHolder::minus(const PossibleFloat first_float,
                                      const PossibleFloat second_float) {
    PossibleFloat tmp_float = second_float;
    tmp_float.set_bit_sign(tmp_float.get_bit_for_sign() ^ 1);
    return plus(first_float, tmp_float);
}

PossibleFloat ExpressionHolder::fma(const PossibleFloat first_float,
                                    const PossibleFloat second_float,
                                    const PossibleFloat third_float) {
    //будет доработано позже. Сейчас заглушка
    return mad(first_float, second_float, third_float);
}

PossibleFloat ExpressionHolder::mad(const PossibleFloat first_float,
                                    const PossibleFloat second_float,
                                    const PossibleFloat third_float) {
    return plus(mult(first_float, second_float), third_float);
}
