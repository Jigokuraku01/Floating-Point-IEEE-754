#include "expression_holder.hpp"
#include "input_query.hpp"
#include "my_exception.hpp"
#include "parser.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    try {
        std::vector<std::string> tmp_vec;
        tmp_vec.reserve(static_cast<std::uint64_t>(argc - 1));
        for (std::uint64_t i = 0; i < static_cast<std::uint64_t>(argc - 1);
             ++i) {
            tmp_vec.emplace_back(argv[i + 1]);
        }
        InputQuery inp_query = Parser::parse_input_query(std::move(tmp_vec));
        ExpressionHolder cur_expr_holder = ExpressionHolder{inp_query};
        std::pair<std::string, std::string> ans =
            cur_expr_holder.solve_and_return_ans();
        std::cout << ans.first << " " << ans.second << std::endl;
        return 0;
    }
    catch (const MyException& e) {
        std::cerr << e.get_error();
        return static_cast<std::int32_t>(e.get_code());
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "unknown error";
        return EXIT_FAILURE;
    }
}
