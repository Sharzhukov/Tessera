//  lexer.hpp (Tessera)
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

#pragma once
#include <array>
#include <vector>
#include <tessera/common/common.hpp>
#include <tessera/common/AtomData.hpp>

using ComAtomDataType = tessera::common::AtomDataType;
using ComAtomMark = tessera::common::AtomMark;

namespace tessera {
    namespace core {
        class lexer {
        private:
            size_t m_countSpace;
            std::string m_inputUserDataString;
            std::vector<std::string> m_tokensUserData;

            //TODO: tokenConst enum, prioriti, 3 map, number-symbol-znak + key priority position
            void LexerToken() {
                size_t t_cursorLine = 0;
                std::string t_tmpBuffer;

                while (true) {
                    unsigned char t_currentSymbol = static_cast<unsigned char>(m_inputUserDataString[t_cursorLine]);
                    unsigned char t_nextSymbol;
                    if (m_inputUserDataString[t_cursorLine + 1] != '\0') {
                        t_nextSymbol = static_cast<unsigned char>(m_inputUserDataString[t_cursorLine + 1]);
                    }
                    else { t_nextSymbol = char(); }

                    if (std::isdigit(t_currentSymbol)) {
                        t_tmpBuffer += t_currentSymbol;
                        if (!std::isdigit(t_nextSymbol)) {
                            m_tokensUserData.push_back(t_tmpBuffer);
                            t_tmpBuffer.clear();
                        }
                    }
                    else if (std::isalpha(t_currentSymbol)) {
                        t_tmpBuffer += t_currentSymbol;
                        if (!std::isalpha(t_nextSymbol)) {
                            m_tokensUserData.push_back(t_tmpBuffer);
                            t_tmpBuffer.clear();
                        }
                    }
                    else {
                        if (std::isspace(t_currentSymbol)) { m_countSpace++; }
                        if (ismath(t_currentSymbol)) {
                            t_tmpBuffer = t_currentSymbol;
                            m_tokensUserData.push_back(t_tmpBuffer);
                            t_tmpBuffer.clear();
                        }
                    }

                    t_cursorLine++;
                    if (t_cursorLine == m_inputUserDataString.size()) { break; }
                }
            }
        public:
            lexer() : m_countSpace{}, m_inputUserDataString{}, m_tokensUserData{} {}

            void SetUserDataString(std::string InputUserDataString) {
                m_inputUserDataString = InputUserDataString;
            }

            bool CheckUserDataString() {
                if (m_inputUserDataString.empty()) {
                    throw std::length_error("Your user data/math line empty!");
                }

                return true;
            }

            bool TransformDataToToken() {
                try {
                    std::ignore = CheckUserDataString();

                    LexerToken();

                    return true;
                } catch (const std::exception& ex) {
                    cout << ex.what() << endl;
                    return false;
                }

                return true;
            }

            bool ismath(char ch) {
                const std::string mathsym = "+-=()*^%!.<>,";
                if (mathsym.find(ch) != std::string::npos) {
                    return true;
                }

                return false;
            }

            void PrintToken() {
                try {
                    std::ignore = CheckUserDataString();

                    for (size_t i = 0; i < m_tokensUserData.size(); i++) {
                        cout << "token " << i << ": " << m_tokensUserData[i] << endl;
                    }

                    cout << "--------------" << endl;
                    cout << "space: " << m_countSpace << endl;

                } catch (const std::exception& ex) {
                    cout << ex.what() << endl;
                }
            }

        };


        class Atom
        {
        private:
            ComAtomDataType m_ComAtomDataType;
            ComAtomMark m_ComAtomMark;
            unsigned int m_atomPosition;
        public:
            Atom(ComAtomDataType& ComAtomDataType, ComAtomMark& ComAtomMark, unsigned int& atomPosition);
            void SetComAtomDataType(ComAtomDataType& ComAtomDataType);
            void SetComAtomMark(ComAtomMark& ComAtomMark);
            ComAtomDataType GetComAtomDataType();
            ComAtomMark GetComAtomMark();
            ~Atom();
        };
    }
}
