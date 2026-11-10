//  token.hpp (Tessera)
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
#include <tessera/common/common.hpp>
#include <tessera/common/AtomData.hpp>
#include <tessera/core/lexer.hpp>

namespace tessera {
    namespace core {
        class token
        {
        private:
            std::vector<Atom> m_memoryAtomList;
        public:
            token();
            ~token();

            void SetAtomList(const std::vector<Atom>& atomList);
            std::vector<Atom> GetAtomList() const;
            void PrintMathToken();
        };
    }
}
