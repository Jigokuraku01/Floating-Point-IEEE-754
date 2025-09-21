#pragma once
#include <cstdint>
class PossibleFloat {
  public:
    explicit PossibleFloat(std::uint32_t exp_cnt = 0,
                           std::uint32_t mant_cnt = 0,
                           std::uint32_t act_number = 0);
    [[nodiscard]]
    std::uint32_t get_bit_for_sign() const;
    [[nodiscard]]
    std::uint32_t get_exp_cnt() const;
    [[nodiscard]]
    std::uint32_t get_exp() const;
    [[nodiscard]]
    std::uint32_t get_mant_cnt() const;
    [[nodiscard]]
    std::uint32_t get_mant() const;
    [[nodiscard]]
    std::uint32_t get_numb() const;

    void set_bit_sign(std::uint32_t inp_sign);
    void set_exp(std::uint32_t inp_exp);
    void set_mant(std::uint32_t inp_mant);
    void set_number(std::uint32_t inp_number);

  private:
    std::uint32_t _exp_cnt_of_bits;
    std::uint32_t _mant_cnt_of_bits;
    std::uint32_t _actual_number;
};
