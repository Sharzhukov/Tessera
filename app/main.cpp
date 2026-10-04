//  app/main.cpp (Tessera)
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

#include <iostream>
#include <string>

int main() {
    std::cout << "Tessera v0.1.0\n";
    std::cout << "Type 'exit' or press Ctrl+D to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> " << std::flush;
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        if (line.empty()) continue;

        // TODO: read, compute, print
        std::cout << "= (not implemented yet)\n";
    }

    std::cout << "\nBye.\n";
    return 0;
}