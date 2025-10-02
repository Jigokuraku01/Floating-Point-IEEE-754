#pragma once
#include "expression_holder.hpp"
#include "i_possible_float_numbers.hpp"
#include "input_query.hpp"
#include "my_exception.hpp"
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <utility>

//Designed only for non neg numbers
template <std::uint64_t N>
class BigInt {
  public:
    BigInt() = default;

    explicit BigInt(std::uint64_t value) {
        m_cur_bitset = std::bitset<N>(value);
    }

    BigInt operator+(const BigInt& other) const {
        BigInt result;
        std::uint64_t carry = 0;

        for (std::uint64_t i = 0; i < N; ++i) {
            std::uint64_t first = m_cur_bitset[i];
            std::uint64_t second = other.m_cur_bitset[i];

            std::uint64_t sum = first ^ second ^ carry;
            carry = (first & second) | (first & carry) | (second & carry);

            result.m_cur_bitset[i] = sum;
        }
        if (carry == 1 && N > 0) {
            throw MyException(EXIT_FAILURE, "Overflow handled");
        }
        return result;
    }

    BigInt& operator+=(BigInt<N> second_float) {
        *this = *this + second_float;
        return *this;
    }

    BigInt& operator-=(BigInt<N> second_float) {
        *this = *this - second_float;
        return *this;
    }

    BigInt operator-(const BigInt& other) const {

        BigInt result;

        std::uint64_t borrow = 0;

        for (std::uint64_t i = 0; i < N; ++i) {
            std::uint64_t first = m_cur_bitset[i];
            std::uint64_t second = other.m_cur_bitset[i];

            std::uint64_t diff = first ^ second ^ borrow;

            borrow = (~first & second) | (~first & borrow) | (second & borrow);

            result.m_cur_bitset[i] = diff;
        }

        return result;
    }
    BigInt operator*(const BigInt& other) const {
        BigInt result;
        BigInt temp;

        for (std::uint64_t i = 0; i < N; ++i) {
            if (other.m_cur_bitset[i] == 1) {
                temp = *this;
                temp <<= i;
                result = result + temp;
            }
        }

        return result;
    }

    BigInt& operator*=(const BigInt& other) {
        *this = *this * other;
        return *this;
    }
    BigInt<N> get_big_numb(std::uint64_t start = 0, std::uint64_t end = N) {
        auto ans = BigInt<N>(0);
        for (std::uint64_t i = start; i < end; ++i) {
            ans.set_bit(i, m_cur_bitset[i]);
        }
        return ans;
    }
    std::uint64_t get_numb(std::uint64_t start = 0, std::uint64_t end = N) {
        std::uint64_t ans = 0;
        std::uint64_t mult = 1;
        for (std::uint64_t i = start; i < end; ++i) {
            ans += m_cur_bitset[i] * mult;
            mult *= 2;
        }
        return ans;
    }

    void format_to_float_with_different_base(PossibleFloat& inp_float,
                                             PossibleRounding cur_rounding,
                                             std::uint64_t extra_base) {
        std::uint64_t index_of_last_one = UINT64_MAX;
        for (std::uint64_t i = 0; i < N; ++i) {
            if (get_bit(i) == 1) {
                index_of_last_one = i;
            }
        }
        if (index_of_last_one == UINT64_MAX) {
            inp_float.set_mant(0);
            inp_float.set_exp(0);
            return;
        }
        BigInt<N> tmp_fract_slice;
        tmp_fract_slice = get_big_numb(0, extra_base);

        m_cur_bitset >>= extra_base;
        index_of_last_one = UINT64_MAX;
        for (std::uint64_t i = 0; i < N; ++i) {
            if (get_bit(i) == 1) {
                index_of_last_one = i;
            }
        }
        if (index_of_last_one < inp_float.get_mant_cnt()) {
            std::uint64_t act_numb = get_numb();
            if (tmp_fract_slice == BigInt<N>(0)) {
                inp_float.set_exp(0);
                inp_float.set_mant(act_numb);
                return;
            }
            switch (cur_rounding) {
                case (PossibleRounding::TOWARD_ZERO): {
                    break;
                }
                case (PossibleRounding::TOWARD_POS_INFINITY): {
                    if (inp_float.get_bit_for_sign() ==
                        PossibleFloat::pos_sign_code) {
                        act_numb += 1;
                    }
                    break;
                }
                case (PossibleRounding::TOWARD_NEG_INFINITY): {
                    if (inp_float.get_bit_for_sign() ==
                        PossibleFloat::neg_sign_code) {
                        act_numb += 1;
                    }
                    break;
                }
                case (PossibleRounding::TOWARD_NEAREST_EVEN): {
                    if (tmp_fract_slice > (BigInt<N>(1) << (extra_base - 1))) {
                        act_numb += 1;
                    }
                    if (tmp_fract_slice == (BigInt<N>(1) << (extra_base - 1))) {
                        if (act_numb % 2 == 1) {
                            act_numb += 1;
                        }
                    }
                    break;
                }
            }

            if (act_numb >= (1ULL << inp_float.get_mant_cnt())) {
                inp_float.set_mant(0);
                inp_float.set_exp(1);
            }
            else {
                inp_float.set_exp(0);
                inp_float.set_mant(act_numb);
            }
            return;
        }
        BigInt<N> fract_slice =
            get_big_numb(0, index_of_last_one - inp_float.get_mant_cnt());
        fract_slice <<= extra_base;
        fract_slice += tmp_fract_slice;

        std::uint64_t act_exp =
            index_of_last_one - inp_float.get_mant_cnt() + 1;
        std::uint64_t act_mant =
            get_numb(index_of_last_one - inp_float.get_mant_cnt(),
                     index_of_last_one + 1);

        if (index_of_last_one == inp_float.get_mant_cnt()) {
            inp_float.set_exp(act_exp);
            inp_float.set_mant(act_mant);
            return;
        }
        if (fract_slice != BigInt<N>(0)) {
            switch (cur_rounding) {
                case (PossibleRounding::TOWARD_ZERO): {
                    break;
                }
                case (PossibleRounding::TOWARD_POS_INFINITY): {
                    if (inp_float.get_bit_for_sign() == 0 &&
                        fract_slice != BigInt<N>(0)) {
                        act_mant += 1;
                    }
                    break;
                }
                case (PossibleRounding::TOWARD_NEG_INFINITY): {
                    if (inp_float.get_bit_for_sign() == 1 &&
                        fract_slice != BigInt<N>(0)) {
                        act_mant += 1;
                    }
                    break;
                }
                case (PossibleRounding::TOWARD_NEAREST_EVEN): {
                    std::uint64_t end_of_slice =
                        index_of_last_one - inp_float.get_mant_cnt() - 1;
                    BigInt<N> shifted_one =
                        (BigInt<N>(1) << (end_of_slice + extra_base));
                    if (fract_slice > shifted_one) {
                        act_mant += 1;
                    }
                    if (fract_slice == shifted_one) {
                        if ((act_mant & 1) == 1) {
                            act_mant += 1;
                        }
                    }
                    break;
                }
            }
        }
        if (act_mant == (1LL << (inp_float.get_mant_cnt() + 1))) {
            act_mant >>= 1;
            act_exp += 1;
        }

        if (act_exp >
            (get_all_ones_at_inp_bit_cnt(inp_float.get_exp_cnt()) - 1)) {
            std::pair<bool, PossibleFloat> ans_pair =
                ExpressionHolder::inf_max_checks(inp_float, cur_rounding);
            if (ans_pair.first) {
                inp_float = ans_pair.second;
            }
            return;
        }

        inp_float.set_exp(act_exp);
        inp_float.set_mant(act_mant);
    }

    void format_to_float(PossibleFloat& inp_float,
                         PossibleRounding cur_rounding) {
        format_to_float_with_different_base(inp_float, cur_rounding, 0);
    }

    BigInt operator<<(std::uint64_t shift) const {
        BigInt<N> result;

        if (shift >= N) {
            return BigInt(0);
        }

        for (std::int64_t i = N - 1; i >= static_cast<std::int64_t>(shift);
             --i) {
            result.m_cur_bitset[static_cast<std::uint64_t>(i)] =
                m_cur_bitset[static_cast<std::uint64_t>(i) - shift];
        }

        for (std::uint64_t i = 0; i < shift && i < N; ++i) {
            result.m_cur_bitset[i] = 0;
        }

        return result;
    }
    BigInt& operator<<=(std::uint64_t shift) {
        *this = *this << shift;
        return *this;
    }
    BigInt operator>>(std::uint64_t shift) const {
        BigInt result;

        if (shift >= N) {
            return BigInt(0);
        }

        for (std::uint64_t i = 0; i < N - shift; ++i) {
            result.set_bit(i, m_cur_bitset[i + shift]);
        }

        for (std::uint64_t i = N - shift; i < N; ++i) {
            result.set_bit(i, 0);
        }

        return result;
    }
    BigInt& operator>>=(std::uint64_t shift) {
        *this = *this >> shift;
        return *this;
    }

    bool operator==(const BigInt& other) const {
        return m_cur_bitset == other.m_cur_bitset;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool operator<(const BigInt& other) const {

        for (std::int64_t i = N - 1; i >= 0; --i) {
            auto cur_index = static_cast<std::uint64_t>(i);
            if (m_cur_bitset[cur_index] != other.m_cur_bitset[cur_index]) {
                return m_cur_bitset[cur_index] < other.m_cur_bitset[cur_index];
            }
        }

        return false;
    }

    bool operator>(const BigInt& other) const {
        return other < *this;
    }

    bool operator<=(const BigInt& other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    [[nodiscard]]
    std::uint64_t operator[](std::uint64_t pos) const {
        return m_cur_bitset[pos];
    }
    [[nodiscard]]
    std::uint64_t get_bit(std::uint64_t pos) const {
        if (pos >= N) {
            return 0;
        }
        return m_cur_bitset[pos];
    }

    void set_bit(std::uint64_t pos, std::uint64_t value) {
        if (pos < N) {
            m_cur_bitset[pos] = value;
        }
        else {
            throw MyException(EXIT_FAILURE, "invalid pos in bitset");
        }
    }

    static constexpr std::uint64_t size() {
        return N;
    }

  private:
    std::bitset<N> m_cur_bitset;
};