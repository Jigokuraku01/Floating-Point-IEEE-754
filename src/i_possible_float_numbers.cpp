#include "i_possible_float_numbers.hpp"

#include "expression_holder.hpp"
#include "my_exception.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>

PossibleFloat::PossibleFloat(std::uint64_t exp_cnt, std::uint64_t mant_cnt,
                             std::uint64_t act_number)
    : m_exp_cnt_of_bits(exp_cnt), m_mant_cnt_of_bits(mant_cnt),
      m_actual_number(act_number) {
}
[[nodiscard]]
std::uint64_t PossibleFloat::get_bit_for_sign() const {
    return m_actual_number >> (m_exp_cnt_of_bits + m_mant_cnt_of_bits);
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_exp_cnt() const {
    return m_exp_cnt_of_bits;
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_numb() const {
    return m_actual_number;
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_exp() const {
    return (m_actual_number >> m_mant_cnt_of_bits) &
           get_all_ones_at_inp_bit_cnt(m_exp_cnt_of_bits);
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_mant_cnt() const {
    return m_mant_cnt_of_bits;
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_mant() const {
    return m_actual_number & get_all_ones_at_inp_bit_cnt(m_mant_cnt_of_bits);
}

[[nodiscard]]
std::int64_t PossibleFloat::get_max_exp() const {
    return static_cast<std::int64_t>(
        get_all_ones_at_inp_bit_cnt(get_exp_cnt()) - 1 - get_exp_bias());
}

[[nodiscard]]
std::int64_t PossibleFloat::get_min_non_denormalized_exp() const {
    return static_cast<std::int64_t>(1 - get_exp_bias());
}

[[nodiscard]]
std::uint64_t PossibleFloat::get_exp_bias() const {
    return (1 << (get_exp_cnt() - 1)) - 1;
}

void PossibleFloat::set_bit_sign(std::uint64_t inp_sign) {
    if (inp_sign != pos_sign_code && inp_sign != neg_sign_code) {
        throw MyException(EXIT_FAILURE, "invalid sign bit");
    }

    m_actual_number &=
        (get_all_ones_at_inp_bit_cnt(get_exp_cnt() + get_mant_cnt()));
    m_actual_number |= (inp_sign) << (m_exp_cnt_of_bits + m_mant_cnt_of_bits);
}

void PossibleFloat::set_exp(std::uint64_t inp_exp) {
    inp_exp &= get_all_ones_at_inp_bit_cnt(m_exp_cnt_of_bits);
    m_actual_number &= (get_all_ones_at_inp_bit_cnt(m_mant_cnt_of_bits) +
                        (1 << (get_mant_cnt() + get_exp_cnt())));
    m_actual_number |= (inp_exp << m_mant_cnt_of_bits);
}
void PossibleFloat::set_mant(std::uint64_t inp_mant) {
    inp_mant &= get_all_ones_at_inp_bit_cnt(m_mant_cnt_of_bits);
    m_actual_number &=
        (get_all_ones_at_inp_bit_cnt(get_mant_cnt() + get_exp_cnt() + 1) -
         get_all_ones_at_inp_bit_cnt(m_mant_cnt_of_bits));
    m_actual_number |= inp_mant;
}

void PossibleFloat::set_number(std::uint64_t inp_number) {
    m_actual_number = inp_number & get_all_ones_at_inp_bit_cnt(
                                       get_exp_cnt() + get_mant_cnt() + 1);
}

PossibleFloat PossibleFloat::create_nan() const {
    PossibleFloat new_float(get_exp_cnt(), get_mant_cnt());
    new_float.set_exp(get_all_ones_at_inp_bit_cnt(get_exp_cnt()));
    new_float.set_mant(1 << (get_mant_cnt() - 1));
    new_float.set_bit_sign(PossibleFloat::neg_sign_code);
    return new_float;
}

PossibleFloat PossibleFloat::create_inf(std::uint64_t sign) const {
    if (sign >= 2) {
        throw MyException(EXIT_FAILURE, "invalid sign" + std::to_string(sign));
    }
    PossibleFloat new_float(get_exp_cnt(), get_mant_cnt());
    new_float.set_exp(get_all_ones_at_inp_bit_cnt(get_exp_cnt()));
    new_float.set_bit_sign(sign);
    return new_float;
}

PossibleFloat PossibleFloat::create_max_number(std::uint64_t sign) const {
    if (sign >= 2) {
        throw MyException(EXIT_FAILURE, "invalid sign" + std::to_string(sign));
    }
    PossibleFloat new_float(get_exp_cnt(), get_mant_cnt());
    new_float.set_exp(get_all_ones_at_inp_bit_cnt(new_float.get_exp_cnt()) - 1);
    new_float.set_mant(get_all_ones_at_inp_bit_cnt(new_float.get_mant_cnt()));
    new_float.set_bit_sign(get_bit_for_sign());
    return new_float;
}

PossibleFloat PossibleFloat::create_zero(std::uint64_t sign) const {
    if (sign >= 2) {
        throw MyException(EXIT_FAILURE, "invalid sign" + std::to_string(sign));
    }
    PossibleFloat new_float(get_exp_cnt(), get_mant_cnt());
    new_float.set_bit_sign(sign);
    return new_float;
}

NormalFloatNumerHandler PossibleFloat::get_normal_form() const {
    //do not work with zeros
    if (get_exp() != 0) {
        return {static_cast<std::int64_t>(get_exp()) -
                    static_cast<std::int64_t>(get_exp_bias()),
                static_cast<uint64_t>(
                    get_mant() & get_all_ones_at_inp_bit_cnt(get_mant_cnt())),
                get_bit_for_sign()};
    }
    std::int64_t act_exp = static_cast<std::int64_t>(get_exp()) -
                           static_cast<std::int64_t>(get_exp_bias());
    std::uint64_t act_mant = get_mant();
    std::uint64_t index_of_last_one = get_mant_cnt() + 1;
    for (std::uint64_t i = 0; i < get_mant_cnt(); ++i) {
        if ((act_mant & (1 << i)) != 0) {
            index_of_last_one = i;
        }
    }
    act_mant <<= (get_mant_cnt() - index_of_last_one);
    act_exp -= static_cast<std::int64_t>((get_mant_cnt() - index_of_last_one));
    return {act_exp + 1,
            static_cast<uint64_t>(act_mant &
                                  get_all_ones_at_inp_bit_cnt(get_mant_cnt())),
            get_bit_for_sign()};
}

std::pair<bool, PossibleFloat>
PossibleFloat::check_if_nans(const PossibleFloat& first_float,
                             const PossibleFloat& second_float) {
    PossibleFloat ans;
    std::pair<bool, PossibleFloat> first_pos_nan = first_float.check_if_nan();
    std::pair<bool, PossibleFloat> sec_pos_nan = second_float.check_if_nan();
    if (first_pos_nan.first) {
        ans = first_pos_nan.second;
    }
    else if (sec_pos_nan.first) {
        ans = sec_pos_nan.second;
    }
    if (ans.get_mant_cnt() != 0) {
        if ((ans.get_mant() >> (ans.get_mant_cnt() - 1)) == 0) {
            ans.set_number(ans.get_numb() + (1 << (ans.get_mant_cnt() - 1)));
        }
        return {true, ans};
    }

    return {false, ans};
}

std::pair<bool, PossibleFloat>
PossibleFloat::check_if_nans3(const PossibleFloat& first_float,
                              const PossibleFloat& second_float,
                              const PossibleFloat& third_float) {
    PossibleFloat ans;
    std::pair<bool, PossibleFloat> first_pos_nan = first_float.check_if_nan();
    std::pair<bool, PossibleFloat> sec_pos_nan = second_float.check_if_nan();
    std::pair<bool, PossibleFloat> third_pos_nan = third_float.check_if_nan();
    if (first_pos_nan.first) {
        ans = first_pos_nan.second;
    }
    else if (sec_pos_nan.first) {
        ans = sec_pos_nan.second;
    }
    else if (third_pos_nan.first) {
        ans = third_pos_nan.second;
    }
    if (ans.get_mant_cnt() != 0) {
        if ((ans.get_mant() >> (ans.get_mant_cnt() - 1)) == 0) {
            ans.set_number(ans.get_numb() + (1 << (ans.get_mant_cnt() - 1)));
        }
        return {true, ans};
    }

    return {false, ans};
}

bool PossibleFloat::check_if_zero() const {
    return get_exp() == 0 && get_mant() == 0;
}

bool PossibleFloat::check_if_inf() const {
    return get_exp() == get_all_ones_at_inp_bit_cnt(get_exp_cnt()) &&
           get_mant() == 0;
}

std::pair<bool, PossibleFloat> PossibleFloat::check_if_nan() const {
    if (get_exp() != get_all_ones_at_inp_bit_cnt(get_exp_cnt())) {
        return {false, *this};
    }
    if (get_mant() == 0) {
        return {false, *this};
    }
    PossibleFloat new_float = *this;
    if ((get_mant() & (1 << get_mant_cnt())) == 0) {
        new_float.set_mant(new_float.get_mant() + (1 << (get_mant_cnt() - 1)));
    }
    return {true, new_float};
}

NormalFloatNumerHandler::NormalFloatNumerHandler(std::int64_t norm_exp,
                                                 std::uint64_t norm_mant,
                                                 std::uint64_t sign)
    : _norm_exp(norm_exp), _norm_mant(norm_mant), _sign(sign) {
}

std::int64_t NormalFloatNumerHandler::get_norm_exp() const {
    return _norm_exp;
}

std::uint64_t NormalFloatNumerHandler::get_norm_mant() const {
    return _norm_mant;
}

std::uint64_t NormalFloatNumerHandler::get_sign() const {
    return _sign;
}