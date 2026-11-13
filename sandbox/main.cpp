//  sandbox/main.cpp (Tessera)
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

#include <cstdlib>
#include <tessera/common/common.hpp>
#include <tessera/core/lexer.hpp>
#include <tessera/core/parser.hpp>

int main() {
    tessera::core::lexer lex;
    tessera::core::parser pars;

    std::string commandReturn = "exit";
    while (true) {
        std::cout << "Tessera $~ ";
        std::string userString2Lexer;
        std::getline(std::cin, userString2Lexer);
        //if (userString2Lexer == commandReturn) { break; }

        lex.SetUserDataString(userString2Lexer);
        if (lex.TransformDataToToken()) {
            lex.PrintToken();

            pars.SetTokensData(lex.GetTokensUserData());
            if (pars.TransformTokenToResult()) {
                pars.PrintResult();
            }
        }

        std::cout << "\nTap \"enter\" for next calc";
        std::cin.get();
        std::system("clear");
    }


    return 0;
}
