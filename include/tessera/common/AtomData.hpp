//  AtomData.hpp (Tessera)
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

namespace tessera {
    namespace common {
        enum class AtomDataType {
            // Scalars (Tokens & Values) | Скаляры (Токены и Значения)
            e_int,            // Integer constants (e.g., indices, explicit exponents) | Целочисленные константы (например, индексы, явные показатели степени)
            e_double,         // Standard high-precision float for 2D/3D plotting | Стандартное вещественное число двойной точности для 2D/3D графиков

            // System & Variables | Системные типы и Переменные
            e_char,           // Single-character variables ('x', 'y', 'z', 't') | Одиночные символьные переменные ('x', 'y', 'z', 't')
            e_string,         // Identifiers for custom functions or named variables (e.g., "phi") | Идентификаторы для пользовательских функций или именованных переменных (например, "phi")
            e_bool,           // Logical outcomes for piecewise functions or domain steps | Логические результаты для кусочно-заданных функций или шагов области определения

            // 3D Visualizer & Linear Algebra Essentials | Ключевые типы для 3D-визуализатора и Линейной алгебры
            e_vector3d,       // Native 3D vectors (x, y, z) for geometry operations | Встроенные 3D-векторы (x, y, z) для геометрических операций
            e_matrix,         // Transforms, projections, or coordinate spaces | Матрицы преобразований, проекций или координатных пространств

            // AST / Evaluator Internals | Внутренние типы AST / Вычислителя
            e_function,       // Identified callable built-in math function (sin, cos, etc.) | Распознанная вызываемая встроенная математическая функция (sin, cos и т.д.)
            e_unknown         // Error token / Type checking failure state | Токен ошибки / Состояние сбоя проверки типов
        };

        enum class AtomMark {
            // Basic Arithmetic & Brackets | Базовая арифметика и скобки
            e_plus,           //~ +
            e_minus,          //~ -
            e_multiplication, //~ *
            e_division,       //~ /
            e_bracketOpen,    //~ (
            e_bracketClose,   //~ )
            e_equals,         //~ =

            // Exponentiation & Roots | Возведение в степень и корни
            e_exponentiation, //~ ^
            e_squareRoot,     //~ √ (Square root | Квадратный корень)
            e_cubeRoot,       //~ ∛ (Cube root | Кубический корень)

            // Comparisons | Сравнения
            e_notEquals,      //~ !=
            e_lessThan,       //~ <
            e_greaterThan,    //~ >
            e_lessOrEqual,    //~ <=
            e_greaterOrEqual, //~ >=

            // Set Theory & Logic | Теория множеств и логика
            e_factorial,      //~ !
            e_percent,        //~ %
            e_plusMinus,      //~ ±

            // Functions & Constants | Функции и константы
            e_comma,          //~ , (Argument separator for functions | Для разделения аргументов функций)
            e_absolute        //~ |x| (Absolute value / Magnitude | Модуль / Абсолютная величина)
        };
    }
}
