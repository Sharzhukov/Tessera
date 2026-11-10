//  commandRegister.cpp (Tessera)
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

#include <tessera/core/commandRegister.hpp>

namespace tessera {
    namespace core {
        commandRegister::commandRegister() : m_commandList{} {
            RegisterCommand(command("help", "Show all Tessera commands"));
            RegisterCommand(command("clear", "Clear console session"));
            RegisterCommand(command("exit", "Exit Tessera calculator"));
        }

        commandRegister::~commandRegister() {}

        void commandRegister::RegisterCommand(const command& cmd) {
            m_commandList.push_back(cmd);
        }

        void commandRegister::PrintCommandList() {
            for (size_t i = 0; i < m_commandList.size(); i++) {
                std::cout << m_commandList[i].GetCommandName() << " - " << m_commandList[i].GetCommandDescription() << std::endl;
            }
        }
    }
}
