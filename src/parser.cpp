#include "parser.hpp"

#include "i_possible_float_numbers.hpp"
#include "input_query.hpp"
#include "my_exception.hpp"
#include <cstdlib>
#include <string>
#include <vector>

InputQuery Parser::parse_input_query(const std::vector<std::string>&& argv_x) {
    auto argv = argv_x;
    if (argv.size() != 3 && argv.size() != 5 && argv.size() != 6) {
        throw MyException(EXIT_FAILURE, "Invalid input format");
    }

    std::uint32_t cur_cnt_for_exp;
    std::uint32_t cur_cnt_for_mant;
    if (argv[0].size() != 1) {
        throw MyException(EXIT_FAILURE, "invalid input format: " + argv[0]);
    }
    switch (argv[0][0]) {
        case ('h'): {
            cur_cnt_for_exp = 5;
            cur_cnt_for_mant = 10;
            break;
        }
        case ('s'): {
            cur_cnt_for_exp = 8;
            cur_cnt_for_mant = 23;
            break;
        }
        default: {
            throw MyException(EXIT_FAILURE, "invalid input format: " + argv[0]);
        }
    }

    PossibleFloat first_number(cur_cnt_for_exp, cur_cnt_for_mant);
    PossibleFloat second_number(cur_cnt_for_exp, cur_cnt_for_mant);
    PossibleFloat third_number(cur_cnt_for_exp, cur_cnt_for_mant);
    PossibleRounding cur_rounding;
    PossibleOperations cur_operation = PossibleOperations::NO_OPER;
    if (argv[1].size() != 1) {
        throw MyException(EXIT_FAILURE,
                          "Invalid input operation format:" + argv[1]);
    }
    switch (argv[1][0]) {
        case ('0'): {
            cur_rounding = PossibleRounding::TOWARD_ZERO;
            break;
        }
        case ('1'): {
            cur_rounding = PossibleRounding::TOWARD_NEAREST_EVEN;
            break;
        }
        case ('2'): {
            cur_rounding = PossibleRounding::TOWARD_POS_INFINITY;
            break;
        }
        case ('3'): {
            cur_rounding = PossibleRounding::TOWARD_NEG_INFINITY;
            break;
        }
        default: {
            throw MyException(EXIT_FAILURE,
                              "unknown rounding code: " + argv[1]);
        }
    }

    if (argv.size() == 3) {
        first_number.set_number(parse_to_int(argv[2], 16));
    }
    else if (argv.size() == 5) {
        //std::swap(argv[2], argv[3]);
        if (argv[2].size() != 1) {
            throw MyException(EXIT_FAILURE, "unknown operation: " + argv[2]);
        }
        switch (argv[2][0]) {
            case ('+'): {
                cur_operation = PossibleOperations::PLUS;
                break;
            }
            case ('-'): {
                cur_operation = PossibleOperations::MINUS;
                break;
            }
            case ('*'): {
                cur_operation = PossibleOperations::MUL;
                break;
            }
            case ('/'): {
                cur_operation = PossibleOperations::DIV;
                break;
            }
            default: {
                throw MyException(EXIT_FAILURE,
                                  "unknown operation: " + argv[2]);
            }
        }
        first_number.set_number(parse_to_int(argv[3], 16));
        second_number.set_number(parse_to_int(argv[4], 16));
    }
    else {
        if (argv[2].size() != 3) {
            throw MyException(EXIT_FAILURE, "unknown operation: " + argv[2]);
        }
        if (argv[2] == "mad") {
            cur_operation = PossibleOperations::MAD;
        }
        else if (argv[2] == "fma") {
            cur_operation = PossibleOperations::FMA;
        }
        else {
            throw MyException(EXIT_FAILURE, "unknown operation: " + argv[2]);
        }
        first_number.set_number(parse_to_int(argv[3], 16));
        second_number.set_number(parse_to_int(argv[4], 16));
        third_number.set_number(parse_to_int(argv[5], 10));
    }
    InputQuery ans = {first_number, second_number, third_number, cur_rounding,
                      cur_operation};
    return ans;
}

std::uint64_t Parser::parse_to_int(const std::string& inpStr,
                                   std::int64_t base) {
    std::uint64_t pos = 0;
    std::uint64_t result =
        std::stoul(inpStr, &pos, static_cast<std::int32_t>(base));

    if (pos != inpStr.size()) {
        throw MyException(EXIT_FAILURE,
                          "Invalid input number format: " + inpStr.substr(pos));
    }

    return result;
}
