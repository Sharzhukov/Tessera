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
#include <map>
#include <tessera/common/common.hpp>
#include <tessera/common/AtomData.hpp>

using ComAtomDataType = tessera::common::AtomDataType;
using ComAtomMark = tessera::common::AtomMark;
using ComAtomConstant = tessera::common::AtomConstant;

namespace tessera {
    namespace core {

        class Atom
        {
        private:
            ComAtomDataType m_ComAtomDataType;
            ComAtomMark m_ComAtomMark;
            unsigned int m_atomPosition;
            std::string m_atomValue;
        public:
            Atom();
            Atom(ComAtomDataType& ComAtomDataType, ComAtomMark& ComAtomMark, unsigned int& atomPosition);
            Atom(ComAtomDataType ComAtomDataType, ComAtomMark ComAtomMark, unsigned int atomPosition, std::string atomValue);

            void SetComAtomDataType(ComAtomDataType& ComAtomDataType);
            void SetComAtomMark(ComAtomMark& ComAtomMark);
            void SetAtomPosition(unsigned int& atomPosition);
            void SetAtomValue(std::string& atomValue);

            ComAtomDataType GetComAtomDataType() const;
            ComAtomMark GetComAtomMark() const;
            unsigned int GetAtomPosition() const;
            std::string GetAtomValue() const;

            ~Atom();
        };

        class lexer {
        private:
            size_t m_countSpace;
            std::string m_inputUserDataString;
            std::vector<std::string> m_tokensUserData;
            std::vector<Atom> m_atomsUserData;

            //TODO: tokenConst enum, prioriti, 3 map, number-symbol-znak + key priority position
            std::map<unsigned int, std::string> m_mapNumber;
            std::map<unsigned int, std::string> m_mapSymbol;
            std::map<unsigned int, std::string> m_mapZnak;

            void LexerToken();
            ComAtomMark ConvertZnakToMark(char ch);
        public:
            lexer();
            ~lexer();

            void SetUserDataString(std::string InputUserDataString);
            bool CheckUserDataString();
            bool TransformDataToToken();

            bool ismath(char ch);
            void PrintToken();

            std::vector<std::string> GetTokensUserData() const;
            std::vector<Atom> GetAtomsUserData() const;
        };

    }
}
