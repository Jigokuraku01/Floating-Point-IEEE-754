#include "expression_holder.hpp"
#include "i_possible_float_numbers.hpp"
#include <utility>

std::pair<bool, PossibleFloat>
ExpressionHolder::divide_checks(const PossibleFloat first_float,
                                const PossibleFloat second_float) {

    std::pair<bool, PossibleFloat> check_nans_result =
        PossibleFloat::check_if_nans(first_float, second_float);
    PossibleFloat ans;
    if (check_nans_result.first) {
        return check_nans_result;
    }
    if ((first_float.check_if_zero() && second_float.check_if_zero()) ||
        (first_float.check_if_inf() && second_float.check_if_inf())) {
        ans = first_float.create_nan();
    }
    else if (second_float.check_if_zero()) {
        ans = first_float.create_inf(second_float.get_bit_for_sign() ^
                                     first_float.get_bit_for_sign());
    }
    else if (second_float.check_if_inf() || first_float.check_if_zero()) {
        ans = first_float.create_zero(first_float.get_bit_for_sign() ^
                                      second_float.get_bit_for_sign());
    }
    else if (first_float.check_if_inf()) {
        ans = first_float.create_inf(first_float.get_bit_for_sign() ^
                                     second_float.get_bit_for_sign());
    }
    if (ans.get_mant_cnt() != 0) {
        return {true, ans};
    }
    return {false, ans};
}

std::pair<bool, PossibleFloat>
ExpressionHolder::mult_checks(const PossibleFloat first_float,
                              const PossibleFloat second_float) {
    std::pair<bool, PossibleFloat> check_nans_result =
        PossibleFloat::check_if_nans(first_float, second_float);
    if (check_nans_result.first) {
        return check_nans_result;
    }
    PossibleFloat ans;
    if ((first_float.check_if_inf() && second_float.check_if_zero()) ||
        (first_float.check_if_zero() && second_float.check_if_inf())) {
        ans = first_float.create_nan();
    }

    else if (first_float.check_if_zero() || second_float.check_if_zero()) {
        ans = first_float.create_zero(first_float.get_bit_for_sign() ^
                                      second_float.get_bit_for_sign());
    }

    else if (first_float.check_if_inf() || second_float.check_if_inf()) {
        ans = first_float.create_inf(first_float.get_bit_for_sign() ^
                                     second_float.get_bit_for_sign());
    }

    if (ans.get_mant_cnt() != 0) {
        return {true, ans};
    }
    return {false, ans};
}

std::pair<bool, PossibleFloat>
ExpressionHolder::plus_checks(const PossibleFloat first_float,
                              const PossibleFloat second_float) {
    std::pair<bool, PossibleFloat> check_nans_result =
        PossibleFloat::check_if_nans(first_float, second_float);
    if (check_nans_result.first) {
        return check_nans_result;
    }
    PossibleFloat ans;
    if (first_float.check_if_inf() && second_float.check_if_inf()) {
        std::int64_t sign =
            first_float.get_bit_for_sign() ^ second_float.get_bit_for_sign();
        if (sign != 0) {
            ans = first_float.create_nan();
        }
        else {
            ans = first_float.create_inf(static_cast<std::uint32_t>(sign));
        }
    }
    else if (first_float.check_if_inf() || second_float.check_if_inf()) {
        if (first_float.check_if_inf()) {
            ans = first_float;
        }
        else {
            ans = second_float;
        }
    }
    else if (first_float.check_if_zero() && second_float.check_if_zero()) {
        if (first_float.get_bit_for_sign() == PossibleFloat::neg_sign_code &&
            second_float.get_bit_for_sign() == PossibleFloat::neg_sign_code) {
            ans = first_float.create_zero(PossibleFloat::neg_sign_code);
        }
        else {
            ans = first_float.create_zero(PossibleFloat::pos_sign_code);
        }
    }
    else if (first_float.check_if_zero() || second_float.check_if_zero()) {
        if (first_float.check_if_zero()) {
            ans = second_float;
        }
        else {
            ans = first_float;
        }
    }
    if (ans.get_mant_cnt() != 0) {
        return {true, ans};
    }
    return {false, ans};
}
