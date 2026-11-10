//  context.cpp (Tessera)
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

#include <tessera/core/context.hpp>

namespace tessera {
    namespace core {
        context::context() : m_variables{}, m_constants{}, m_lastResult{} {
            InitConstants();
        }

        void context::InitConstants() {
            m_constants[ComAtomConstant::e_pi] = 3.141592653589793;
            m_constants[ComAtomConstant::e_euler] = 2.718281828459045;
            m_constants[ComAtomConstant::e_goldenRatio] = 1.618033988749895;
            m_constants[ComAtomConstant::e_g0] = 9.80665;
        }

        void context::SetVariable(std::string varName, double varValue) {
            m_variables[varName] = varValue;
        }

        double context::GetVariable(std::string varName) const {
            auto t_it = m_variables.find(varName);
            if (t_it != m_variables.end()) {
                return t_it->second;
            }

            return 0.0;
        }
    }
}
