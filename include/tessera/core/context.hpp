//  context.hpp (Tessera)
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

using ComAtomConstant = tessera::common::AtomConstant;

namespace tessera {
    namespace core {
        struct context
        {
            std::map<std::string, double> m_variables;
            std::map<ComAtomConstant, double> m_constants;
            double m_lastResult;

            context();
            void InitConstants();
            void SetVariable(std::string varName, double varValue);
            double GetVariable(std::string varName) const;
        };
    }
}
