//  parser.cpp (Tessera)
//  Tessera — Math expression parser, evaluator and visualizer.
//
//  Copyright (C) 2026 Alexander Sharzhukov
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <tessera/core/parser.hpp>

namespace tessera {
    namespace core {

        parser::parser() : m_tokensData{}, m_resultMath{} {}

        parser::~parser() {}

        void parser::SetTokensData(const std::vector<std::string>& tokensData) {
            m_tokensData = tokensData;
        }

        bool parser::CheckTokensData() {
            if (m_tokensData.empty()) {
                throw std::length_error("Your tokens data list empty!");
            }

            return true;
        }

        int parser::GetPriority(const std::string& znak) {
            if (znak == "+" || znak == "-") { return 1; }
            if (znak == "*" || znak == "/" || znak == "%") { return 2; }
            if (znak == "^") { return 3; }

            return 0;
        }

        double parser::ExecuteZnak(double a, double b, const std::string& znak) {
            if (znak == "+") { return a + b; }
            if (znak == "-") { return a - b; }
            if (znak == "*") { return a * b; }
            if (znak == "/") {
                if (b == 0.0) { throw std::runtime_error("Math error: division by zero!"); }
                return a / b;
            }
            if (znak == "^") { return std::pow(a, b); }

            return 0.0;
        }

        void parser::ParseTokens() {
            std::vector<double> t_valuesStack;
            std::vector<std::string> t_znakStack;

            for (size_t i = 0; i < m_tokensData.size(); i++) {
                std::string t_token = m_tokensData[i];
                unsigned char t_firstChar = static_cast<unsigned char>(t_token[0]);

                if (std::isdigit(t_firstChar)) {
                    t_valuesStack.push_back(std::stod(t_token));
                }
                else if (t_token == "(") {
                    t_znakStack.push_back(t_token);
                }
                else if (t_token == ")") {
                    while (!t_znakStack.empty() && t_znakStack.back() != "(") {
                        double t_valSecond = t_valuesStack.back(); t_valuesStack.pop_back();
                        double t_valFirst = t_valuesStack.back(); t_valuesStack.pop_back();
                        std::string t_op = t_znakStack.back(); t_znakStack.pop_back();

                        t_valuesStack.push_back(ExecuteZnak(t_valFirst, t_valSecond, t_op));
                    }
                    if (!t_znakStack.empty()) { t_znakStack.pop_back(); }
                }
                else {
                    while (!t_znakStack.empty() && GetPriority(t_znakStack.back()) >= GetPriority(t_token)) {
                        double t_valSecond = t_valuesStack.back(); t_valuesStack.pop_back();
                        double t_valFirst = t_valuesStack.back(); t_valuesStack.pop_back();
                        std::string t_op = t_znakStack.back(); t_znakStack.pop_back();

                        t_valuesStack.push_back(ExecuteZnak(t_valFirst, t_valSecond, t_op));
                    }
                    t_znakStack.push_back(t_token);
                }
            }

            while (!t_znakStack.empty() && t_valuesStack.size() >= 2) {
                double t_valSecond = t_valuesStack.back(); t_valuesStack.pop_back();
                double t_valFirst = t_valuesStack.back(); t_valuesStack.pop_back();
                std::string t_op = t_znakStack.back(); t_znakStack.pop_back();

                t_valuesStack.push_back(ExecuteZnak(t_valFirst, t_valSecond, t_op));
            }

            if (!t_valuesStack.empty()) {
                m_resultMath = t_valuesStack.back();
            }
        }

        bool parser::TransformTokenToResult() {
            try {
                std::ignore = CheckTokensData();

                ParseTokens();

                return true;
            } catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
                return false;
            }

            return true;
        }

        double parser::GetResultMath() const {
            return m_resultMath;
        }

        void parser::PrintResult() {
            try {
                std::ignore = CheckTokensData();

                std::cout << "--------------" << std::endl;
                std::cout << "result: " << m_resultMath << std::endl;

            } catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }

    }
}
