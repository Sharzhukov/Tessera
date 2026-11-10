//  command.cpp (Tessera)
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

#include <tessera/core/command.hpp>

namespace tessera {
    namespace core {
        command::command() : m_commandName{}, m_commandDescription{} {}

        command::command(std::string commandName, std::string commandDescription)
            : m_commandName{commandName}, m_commandDescription{commandDescription} {}

        command::~command() {}

        void command::SetCommandName(std::string commandName) {
            m_commandName = commandName;
        }

        void command::SetCommandDescription(std::string commandDescription) {
            m_commandDescription = commandDescription;
        }

        std::string command::GetCommandName() const {
            return m_commandName;
        }

        std::string command::GetCommandDescription() const {
            return m_commandDescription;
        }
    }
}
