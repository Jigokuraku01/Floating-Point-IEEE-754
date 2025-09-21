#include "i_possible_float_numbers.hpp"

#include "expression_holder.hpp"
#include "my_exception.hpp"
#include <cstdlib>

PossibleFloat::PossibleFloat(std::uint32_t exp_cnt, std::uint32_t mant_cnt,
                             std::uint32_t act_number)
    : _exp_cnt_of_bits(exp_cnt), _mant_cnt_of_bits(mant_cnt),
      _actual_number(act_number) {
}
[[nodiscard]]
std::uint32_t PossibleFloat::get_bit_for_sign() const {
    return _actual_number >> (_exp_cnt_of_bits + _mant_cnt_of_bits);
}

[[nodiscard]]
std::uint32_t PossibleFloat::get_exp_cnt() const {
    return _exp_cnt_of_bits;
}

[[nodiscard]]
std::uint32_t PossibleFloat::get_numb() const {
    return _actual_number;
}

[[nodiscard]]
std::uint32_t PossibleFloat::get_exp() const {
    return (_actual_number >> _mant_cnt_of_bits) &
           get_all_ones_at_inp_bit_cnt(_exp_cnt_of_bits);
}

[[nodiscard]]
std::uint32_t PossibleFloat::get_mant_cnt() const {
    return _mant_cnt_of_bits;
}

[[nodiscard]]
std::uint32_t PossibleFloat::get_mant() const {
    return _actual_number & get_all_ones_at_inp_bit_cnt(_mant_cnt_of_bits);
}

void PossibleFloat::set_bit_sign(std::uint32_t inp_sign) {
    if (inp_sign >= 2) {
        throw MyException(EXIT_FAILURE, "invalid sign bit");
    }
    _actual_number &= (get_all_ones_at_inp_bit_cnt(sizeof(_actual_number) - 1));
    _actual_number |= (inp_sign) << (_exp_cnt_of_bits + _mant_cnt_of_bits);
}
void PossibleFloat::set_exp(std::uint32_t inp_exp) {
    inp_exp &= get_all_ones_at_inp_bit_cnt(_exp_cnt_of_bits);
    _actual_number &= (get_all_ones_at_inp_bit_cnt(_mant_cnt_of_bits) +
                       (1 << (_mant_cnt_of_bits + _exp_cnt_of_bits)));
    _actual_number |= (inp_exp << _mant_cnt_of_bits);
}
void PossibleFloat::set_mant(std::uint32_t inp_mant) {
    inp_mant &= get_all_ones_at_inp_bit_cnt(_mant_cnt_of_bits);
    _actual_number &= (get_all_ones_at_inp_bit_cnt(sizeof(_actual_number)) -
                       get_all_ones_at_inp_bit_cnt(_mant_cnt_of_bits));
    _actual_number |= inp_mant;
}

void PossibleFloat::set_number(std::uint32_t inp_number) {
    _actual_number = inp_number;
}