//  mathOperation.cpp (Tessera)
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
#ifndef tessera_components_mathSymbol_hpp
#define tessera_components_mathSymbol_hpp

#include <tessera/common/common.hpp>
#include <map>
#include <queue>

namespace tessera {
    namespace components {
        enum class symbolToken : char { 
            e_plus, //~ +
            e_minus, //~    -
            e_multiplication, //    *
            e_division, //~     /
            e_bracketIn, //~    (
            e_bracketOut, //~   )
            e_exponentiation,
            e_square, // square root
            e_equals //~    =
        };

        std::map<unsigned, std::string> priorityOperation;
        std::queue<unsigned> kkk;
        
    }
}

#endif