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

#include <cstddef>
#include <iostream>
#include <string>
#include <tessera/common/common.hpp>
#include <tessera/common/AtomData.hpp>
//#include <tessera/core/lexer.hpp>
using std::cout;
using std::endl;
using std::cin;
#include <cctype>



int main() {
    std::string AppName = "Tessera visual calcs";
    for (size_t j = 0; j < 3; j++) {
        if (j == 1) { cout << "# " << AppName << " #"; }
        else { for (size_t i = 0; i < AppName.size() + 4; cout << "#", i++); }
        cout << endl;
    }


    lexer lex;

    // || std::isalpha(t_currentSymbol)
    cout << "\nTessera $~ ";
    std::string userString2Lexer;
    std::getline(cin, userString2Lexer);
    lex.SetUserDataString(userString2Lexer);

    bool resTest = lex.TransformDataToToken();
    cout << "Result transform: " << resTest << endl;

    lex.PrintToken();

    std::cin.get();
    return 0;
}
