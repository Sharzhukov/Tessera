//  parser.hpp (Tessera)
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
#include <tessera/core/context.hpp>

namespace tessera {
    namespace core {
        class parser
        {
        private:
            std::vector<std::string> m_tokensData;
            double m_resultMath;

            //TODO: prioriti znak and brackets
            int GetPriority(const std::string& znak);
            double ExecuteZnak(double a, double b, const std::string& znak);
            void ParseTokens();
        public:
            parser();
            ~parser();

            void SetTokensData(const std::vector<std::string>& tokensData);
            bool CheckTokensData();
            bool TransformTokenToResult();

            double GetResultMath() const;
            void PrintResult();
        };
    }
}
