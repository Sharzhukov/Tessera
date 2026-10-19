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
            //TODO: Started lexer
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
