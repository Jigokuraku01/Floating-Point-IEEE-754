#pragma once
#include <cstdint>
#include <utility>

class NormalFloatNumerHandler {
  public:
    NormalFloatNumerHandler(std::int32_t norm_exp, std::uint32_t norm_mant,
                            bool sign);
    [[nodiscard]]
    std::int32_t get_norm_exp() const;
    [[nodiscard]]
    std::uint32_t get_norm_mant() const;
    [[nodiscard]]
    bool get_sign() const;

  private:
    std::int32_t _norm_exp;
    std::uint32_t _norm_mant;
    bool _sign;
};
class PossibleFloat {
  public:
    explicit PossibleFloat(std::uint32_t exp_cnt = 0,
                           std::uint32_t mant_cnt = 0,
                           std::uint32_t act_number = 0);
    [[nodiscard]]
    PossibleFloat create_nan() const;

    [[nodiscard]]
    PossibleFloat create_inf(std::uint32_t sign) const;
    [[nodiscard]]
    PossibleFloat create_max_number(std::uint32_t sign) const;

    [[nodiscard]]
    PossibleFloat create_zero(std::uint32_t sign) const;

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
    [[nodiscard]]
    std::uint32_t get_exp_bias() const;

    [[nodiscard]]
    std::int32_t get_max_exp() const;
    [[nodiscard]]
    std::int32_t get_min_non_denormalized_exp() const;

    [[nodiscard]]
    NormalFloatNumerHandler get_normal_form() const;

    void set_bit_sign(std::uint32_t inp_sign);
    void set_exp(std::uint32_t inp_exp);
    void set_mant(std::uint32_t inp_mant);
    void set_number(std::uint32_t inp_number);

    [[nodiscard]]
    bool check_if_nan() const;
    [[nodiscard]]
    bool check_if_inf() const;

    static std::pair<bool, PossibleFloat>
    check_if_nans(const PossibleFloat& first_float,
                  const PossibleFloat& second_float);

    [[nodiscard]]
    bool check_if_zero() const;

    static const std::int32_t pos_sign_code = 0;
    static const std::int32_t neg_sign_code = 1;

  private:
    std::uint32_t m_exp_cnt_of_bits;
    std::uint32_t m_mant_cnt_of_bits;
    std::uint32_t m_actual_number;
};
