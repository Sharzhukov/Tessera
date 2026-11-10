//  lexer.cpp (Tessera)
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

#include <tessera/core/lexer.hpp>

namespace tessera {
    namespace core {

        // ---- Atom ----
        Atom::Atom() : m_ComAtomDataType{ComAtomDataType::e_unknown}, m_ComAtomMark{ComAtomMark::e_plus}, m_atomPosition{}, m_atomValue{} {}

        Atom::Atom(ComAtomDataType& ComAtomDataType, ComAtomMark& ComAtomMark, unsigned int& atomPosition)
            : m_ComAtomDataType{ComAtomDataType}, m_ComAtomMark{ComAtomMark}, m_atomPosition{atomPosition}, m_atomValue{} {}

        Atom::Atom(ComAtomDataType ComAtomDataType, ComAtomMark ComAtomMark, unsigned int atomPosition, std::string atomValue)
            : m_ComAtomDataType{ComAtomDataType}, m_ComAtomMark{ComAtomMark}, m_atomPosition{atomPosition}, m_atomValue{atomValue} {}

        void Atom::SetComAtomDataType(ComAtomDataType& ComAtomDataType) {
            m_ComAtomDataType = ComAtomDataType;
        }

        void Atom::SetComAtomMark(ComAtomMark& ComAtomMark) {
            m_ComAtomMark = ComAtomMark;
        }

        void Atom::SetAtomPosition(unsigned int& atomPosition) {
            m_atomPosition = atomPosition;
        }

        void Atom::SetAtomValue(std::string& atomValue) {
            m_atomValue = atomValue;
        }

        ComAtomDataType Atom::GetComAtomDataType() const {
            return m_ComAtomDataType;
        }

        ComAtomMark Atom::GetComAtomMark() const {
            return m_ComAtomMark;
        }

        unsigned int Atom::GetAtomPosition() const {
            return m_atomPosition;
        }

        std::string Atom::GetAtomValue() const {
            return m_atomValue;
        }

        Atom::~Atom() {}


        // ---- lexer ----
        lexer::lexer() : m_countSpace{}, m_inputUserDataString{}, m_tokensUserData{}, m_atomsUserData{}, m_mapNumber{}, m_mapSymbol{}, m_mapZnak{} {}

        lexer::~lexer() {}

        void lexer::SetUserDataString(std::string InputUserDataString) {
            m_inputUserDataString = InputUserDataString;
        }

        bool lexer::CheckUserDataString() {
            if (m_inputUserDataString.empty()) {
                throw std::length_error("Your user data/math line empty!");
            }

            return true;
        }

        ComAtomMark lexer::ConvertZnakToMark(char ch) {
            if (ch == '+') { return ComAtomMark::e_plus; }
            else if (ch == '-') { return ComAtomMark::e_minus; }
            else if (ch == '*') { return ComAtomMark::e_multiplication; }
            else if (ch == '/') { return ComAtomMark::e_division; }
            else if (ch == '(') { return ComAtomMark::e_bracketOpen; }
            else if (ch == ')') { return ComAtomMark::e_bracketClose; }
            else if (ch == '=') { return ComAtomMark::e_equals; }
            else if (ch == '^') { return ComAtomMark::e_exponentiation; }
            else if (ch == '!') { return ComAtomMark::e_factorial; }
            else if (ch == '%') { return ComAtomMark::e_percent; }
            else if (ch == '<') { return ComAtomMark::e_lessThan; }
            else if (ch == '>') { return ComAtomMark::e_greaterThan; }
            else if (ch == ',') { return ComAtomMark::e_comma; }

            return ComAtomMark::e_plus;
        }

        void lexer::LexerToken() {
            size_t t_cursorLine = 0;
            unsigned int t_tokenPosition = 0;
            std::string t_tmpBuffer;

            m_tokensUserData.clear();
            m_atomsUserData.clear();
            m_mapNumber.clear();
            m_mapSymbol.clear();
            m_mapZnak.clear();
            m_countSpace = 0;

            while (true) {
                unsigned char t_currentSymbol = static_cast<unsigned char>(m_inputUserDataString[t_cursorLine]);
                unsigned char t_nextSymbol;
                if (t_cursorLine + 1 < m_inputUserDataString.size() && m_inputUserDataString[t_cursorLine + 1] != '\0') {
                    t_nextSymbol = static_cast<unsigned char>(m_inputUserDataString[t_cursorLine + 1]);
                }
                else { t_nextSymbol = char(); }

                if (std::isdigit(t_currentSymbol) || t_currentSymbol == '.') {
                    t_tmpBuffer += t_currentSymbol;
                    if (!std::isdigit(t_nextSymbol) && t_nextSymbol != '.') {
                        m_tokensUserData.push_back(t_tmpBuffer);
                        m_mapNumber[t_tokenPosition] = t_tmpBuffer;

                        ComAtomDataType t_type = (t_tmpBuffer.find('.') != std::string::npos) ? ComAtomDataType::e_double : ComAtomDataType::e_int;
                        m_atomsUserData.push_back(Atom(t_type, ComAtomMark::e_plus, t_tokenPosition, t_tmpBuffer));

                        t_tokenPosition++;
                        t_tmpBuffer.clear();
                    }
                }
                else if (std::isalpha(t_currentSymbol)) {
                    t_tmpBuffer += t_currentSymbol;
                    if (!std::isalpha(t_nextSymbol)) {
                        m_tokensUserData.push_back(t_tmpBuffer);
                        m_mapSymbol[t_tokenPosition] = t_tmpBuffer;

                        ComAtomDataType t_type = (t_tmpBuffer.size() == 1) ? ComAtomDataType::e_char : ComAtomDataType::e_string;
                        m_atomsUserData.push_back(Atom(t_type, ComAtomMark::e_plus, t_tokenPosition, t_tmpBuffer));

                        t_tokenPosition++;
                        t_tmpBuffer.clear();
                    }
                }
                else {
                    if (std::isspace(t_currentSymbol)) { m_countSpace++; }
                    if (ismath(t_currentSymbol)) {
                        t_tmpBuffer = t_currentSymbol;
                        m_tokensUserData.push_back(t_tmpBuffer);
                        m_mapZnak[t_tokenPosition] = t_tmpBuffer;

                        ComAtomMark t_mark = ConvertZnakToMark(t_currentSymbol);
                        m_atomsUserData.push_back(Atom(ComAtomDataType::e_unknown, t_mark, t_tokenPosition, t_tmpBuffer));

                        t_tokenPosition++;
                        t_tmpBuffer.clear();
                    }
                }

                t_cursorLine++;
                if (t_cursorLine == m_inputUserDataString.size()) { break; }
            }
        }

        bool lexer::TransformDataToToken() {
            try {
                std::ignore = CheckUserDataString();

                LexerToken();

                return true;
            } catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
                return false;
            }

            return true;
        }

        bool lexer::ismath(char ch) {
            const std::string mathsym = "+-*/=()*^%!<>,";
            if (mathsym.find(ch) != std::string::npos) {
                return true;
            }

            return false;
        }

        void lexer::PrintToken() {
            try {
                std::ignore = CheckUserDataString();

                for (size_t i = 0; i < m_tokensUserData.size(); i++) {
                    std::cout << "token " << i << ": " << m_tokensUserData[i] << std::endl;
                }

                std::cout << "--------------" << std::endl;
                std::cout << "space: " << m_countSpace << std::endl;

            } catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }

        std::vector<std::string> lexer::GetTokensUserData() const {
            return m_tokensUserData;
        }

        std::vector<Atom> lexer::GetAtomsUserData() const {
            return m_atomsUserData;
        }

    }
}
