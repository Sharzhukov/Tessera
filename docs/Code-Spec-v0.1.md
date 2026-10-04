# Tessera — Code Specification v0.1

> This document describes **how** to implement Tessera v0.1.
> It is a companion to `TZ-v0.1.md`, which describes **what** the program does.
>
> The reader is assumed to be the author of the code. No code snippets are
> provided here on purpose — the implementation is the reader's own work.

---

## 1. Purpose

Tessera reads a mathematical expression as text, computes its value, and prints
the result. This spec covers:

- the three-layer architecture,
- the responsibilities of each layer,
- the interfaces between layers,
- error handling,
- testing strategy,
- the build system.

---

## 2. Big picture

The program is split into **three layers** plus an **application**.

```
        ┌───────────────────────┐
        │   Application (REPL)  │
        └───────────┬───────────┘
                    │ uses
        ┌───────────┼───────────┬───────────────┐
        │           │           │               │
        ▼           ▼           ▼               │
   ┌────────┐  ┌────────┐  ┌──────────┐         │
   │Layer 1 │→ │Layer 2 │→ │ Layer 3  │         │
   └────────┘  └────────┘  └──────────┘         │
   text→units  units→tree  tree→number          │
```

Rules:

- Each layer is a **separate module** (its own header + source).
- Layers are **independent**: Layer 2 does not know about Layer 1's internals,
  Layer 3 does not know about Layer 2's internals.
- Only the public interface crosses layer boundaries.
- The application wires them together.

---

## 3. Suggested file organization

Names are up to you. A reasonable layout:

```
include/tessera/
    <unit_type>.hpp        # atomic-unit type and its category
    <layer1>.hpp           # text → list of atomic units
    <tree_node>.hpp        # tree node types
    <layer2>.hpp           # list of units → tree
    <layer3>.hpp           # tree + variables → number

src/
    <layer1>.cpp
    <layer2>.cpp
    <layer3>.cpp

app/
    main.cpp               # REPL

tests/
    test_<layer1>.cpp
    test_<layer2>.cpp
    test_<layer3>.cpp
```

Note: the CMake build uses `GLOB_RECURSE` — you do not need to edit any
`CMakeLists.txt` when adding new `.cpp` files in `src/` or `tests/`.

---

## 4. Layer 1 — text to atomic units

### 4.1. Responsibility

Given a `std::string` (or `std::string_view`), produce a **list of atomic
units**. Each unit describes one meaningful piece of the input.

### 4.2. Interface (conceptual)

- A type representing **a single atomic unit**.
- A type representing **the category** of a unit.
- A class or free function that takes text and returns the list of units.

The exact shape is your choice. Options:

- Class with a constructor and a `Tokenize`-like method.
- Free function `tokenize(text) -> vector<unit>`.
- Class + a static helper.

### 4.3. Contents of a unit

At minimum:

- **category** — one of an enumeration,
- **text** — the original substring,
- **position** — index in the source string (0-based),
- **value** — numeric value, if the category is "number".

If category is not a number, `value` is unused (0 or unspecified).

### 4.4. Categories

Required for v0.1:

| Category | Examples |
|---|---|
| Number | `42`, `3.14`, `0.5` |
| Identifier | `x`, `sin`, `pi`, `my_var` |
| Plus | `+` |
| Minus | `-` |
| Star | `*` |
| Slash | `/` |
| Caret | `^` |
| LeftParen | `(` |
| RightParen | `)` |
| Comma | `,` |
| End | (never appears in input) |
| Invalid | any unrecognized char |

### 4.5. Algorithm

Walk the input **once**, left to right.

At each step:

1. If the current character is whitespace — skip it.
2. If it is a digit — start collecting a number.
3. If it is a letter or underscore — start collecting an identifier.
4. If it is one of the single-character operators or parentheses — emit a
   corresponding unit.
5. Otherwise — emit an "invalid" unit for that one character.

At the end, append an "end of input" unit.

### 4.6. Number rules

- Starts with a digit.
- May contain **one** dot, followed by digits.
- Not supported in v0.1: `1e5`, `0x1F`, `.5`, `1.`.

### 4.7. Identifier rules

- Starts with a letter or underscore.
- Continues with letters, digits, or underscores.

### 4.8. Error handling

**Layer 1 does not throw.** Invalid characters become "invalid" units. The
decision about whether that is a fatal error belongs to Layer 2.

### 4.9. Why always append "end of input"

Layer 2 needs a sentinel to detect "we reached the end". Without it, every
check would be `if (pos >= tokens.size())` — error-prone. With `End` — cleaner.

### 4.10. Test scenarios

Write tests **before** the implementation. At minimum:

- `"42"` → one Number unit with value 42, plus End.
- `"3.14"` → one Number unit with value 3.14.
- `"+-*/^"` → five operator units, plus End.
- `"(,)"` → three units: LeftParen, Comma, RightParen, plus End.
- `"sin"` → one Identifier unit with text `"sin"`.
- `"2 + x * 3"` → Number, Plus, Identifier, Star, Number, End.
- `"  1  +  2  "` → Number, Plus, Number, End (whitespace ignored).
- `"2 @ 2"` → Number, Invalid, Number, End.
- Positions: `"  2+3"` — unit "2" has position 2, not 0.

---

## 5. Layer 2 — units to tree

### 5.1. Responsibility

Given a list of atomic units, produce a **tree** that describes the structure
of the expression.

### 5.2. Interface (conceptual)

- A set of **tree node types**.
- A class or free function that takes units and returns a pointer to the root.

Since C++ does not have built-in sum types (in C++17), you have several options:

- **Inheritance**: base class + subclasses per node kind. Recursive descent
  produces subclasses and returns `unique_ptr<Base>`.
- **Variant**: `std::variant` of structs. Requires C++17; the parent struct
  holds `unique_ptr<Variant>`.
- **Tag + fields**: a single struct with a kind tag and a `union` — not
  recommended for beginners.

For v0.1, **inheritance** is the most straightforward.

### 5.3. Node kinds

Required for v0.1:

| Kind | Fields |
|---|---|
| Number | `double value` |
| Variable | `std::string name` |
| Unary | `char op`, `ptr operand` |
| Binary | `char op`, `ptr left`, `ptr right` |
| Call | `std::string callee`, `vector<ptr> args` |

### 5.4. Grammar recap

```
expression  →  term (("+" | "-") term)*
term        →  unary (("*" | "/") unary)*
unary       →  ("-" | "+") unary | power
power       →  primary ("^" unary)?
primary     →  NUMBER
            |  IDENTIFIER "(" args? ")"
            |  IDENTIFIER
            |  "(" expression ")"
args        →  expression ("," expression)*
```

### 5.5. Algorithm — recursive descent

One **private method per level**:

1. `ParseExpression` — handles `+` and `-`
2. `ParseTerm` — handles `*` and `/`
3. `ParseUnary` — handles unary `-` and `+`
4. `ParsePower` — handles `^` (right-associative!)
5. `ParsePrimary` — number, identifier, function call, parenthesized expression

Each method calls the next one in the chain. `ParsePrimary` is the "leaf".

### 5.6. The right-associative `^`

This is the trickiest part. In `2 ^ 3 ^ 2`:

- **Left-associative** would give `(2 ^ 3) ^ 2 = 64` — **wrong**.
- **Right-associative** gives `2 ^ (3 ^ 2) = 512` — **correct**.

The trick: in `ParsePower`, after seeing `^`, call `ParseUnary` for the right
operand, **not** `ParsePower`. This allows chaining `^` to the right.

### 5.7. Precedence of `-2 ^ 2`

Because unary minus sits **below** power in the grammar, `-2 ^ 2` parses as
`-(2 ^ 2) = -4`. **Not** `(-2) ^ 2 = 4`. This is important — write a test for it.

### 5.8. Error handling

**Throw `std::runtime_error`** with a message that includes the position. The
following situations must be errors:

| Input | Reason |
|---|---|
| `2 +` | No right operand |
| `(2 + 2` | Missing `)` |
| `2 2` | Two numbers in a row |
| `2 @ 2` | Invalid unit |
| (empty) | Nothing to parse |
| `sin(` | Missing argument and `)` |

Example messages:

```
Unexpected token at position 4: '<lexeme>'
Expected ')' at position 7
Unexpected end of input
```

### 5.9. Test scenarios

- `"2 + 2"` → parses to a Binary `+`.
- `"2 + 2 * 3"` → `+` on top, right child is `*`.
- `"(2 + 2) * 3"` → `*` on top.
- `"-2 + 5"` → unary minus on left.
- `"2 ^ 3 ^ 2"` → top `^`, right child is `^` → evaluates to 512.
- `"-2 ^ 2"` → evaluates to `-4`.
- `"sqrt(16)"` → call with 1 argument.
- `"pow(2, 10)"` → call with 2 arguments.
- Errors listed in §5.8.

---

## 6. Layer 3 — tree to number

### 6.1. Responsibility

Given a tree and a map of variable values, return the numeric result.

### 6.2. Interface (conceptual)

- A class that holds **variables** (a map from name to `double`).
- A method that takes a tree node and returns a `double`.
- Methods to **set** and **clear** variables.

### 6.3. Traversal strategy

You have several options:

- **Virtual method on the node** (node knows how to evaluate itself).
- **Visitor pattern** (separate visitor class).
- **`dynamic_cast` + if-chain** (simplest for small projects).
- **Variant + `std::visit`** (if you used `std::variant` for the tree).

For v0.1, **any is fine**. `dynamic_cast` is the fastest to write, but
virtual methods age better. Pick one and be consistent.

### 6.4. Behavior

| Node | Result |
|---|---|
| Number | `value` |
| Variable | value from map; if not found — throw |
| Unary `-` | negate |
| Unary `+` | same value |
| Binary `+ - *` | arithmetic |
| Binary `/` | if right is 0 — throw |
| Binary `^` | `std::pow` |
| Call | look up function name, check arg count, apply |

### 6.5. Built-in functions

Minimum set:

| Name | Args | Implementation |
|---|---|---|
| `sin` | 1 | `std::sin` |
| `cos` | 1 | `std::cos` |
| `tan` | 1 | `std::tan` |
| `sqrt` | 1 | `std::sqrt` |
| `abs` | 1 | `std::abs` |
| `log` | 1 | `std::log` |
| `exp` | 1 | `std::exp` |
| `pow` | 2 | `std::pow` |

### 6.6. Error messages

- Unknown variable → `Unknown variable: <name>`
- Division by zero → `Division by zero`
- Unknown function → `Unknown function: <name>`
- Wrong arg count → `<name> expects N argument(s)`

### 6.7. Test scenarios

- `"2 + 2"` → `4.0`
- `"2 + 2 * 3"` → `8.0`
- `"(2 + 2) * 3"` → `12.0`
- `"-2 + 5"` → `3.0`
- `"2 ^ 3 ^ 2"` → `512.0`
- `"-2 ^ 2"` → `-4.0`
- `"sqrt(16)"` → `4.0`
- `"sqrt(sqrt(16))"` → `2.0`
- `"pow(2, 10)"` → `1024.0`
- `"sin(0)"` → `0.0` (approximate)
- `"1 / 0"` → throws
- `"foo(1)"` → throws
- `"x + 1"` (with no `x` set) → throws
- `"x + 1"` (with `x = 2`) → `3.0`

---

## 7. Application — REPL

### 7.1. Responsibility

Reads a line, parses, evaluates, prints. Loops until `exit` or EOF.

### 7.2. Behavior

1. Print banner.
2. Print `> ` and read a line.
3. On EOF or `exit` — terminate.
4. On empty line — continue.
5. Parse → evaluate → print `= <result>`.
6. On any exception — print `Error: <what>` and continue.
7. Between runs, variables `pi` and `e` are preset.

### 7.3. Error handling

**Never crash.** Catch `std::exception` (and its subclasses), print, continue.
The REPL must survive any input.

### 7.4. Constants

At startup, add:

- `pi` = 3.141592653589793
- `e`  = 2.718281828459045

---

## 8. Testing strategy

### 8.1. Order

Write tests **layer by layer**, and within each layer — **test by test**.

1. Start with **Layer 1**. Write the first test. Run it. It fails.
2. Write the **minimum** code to make it pass.
3. Commit. Move to the next test.
4. Repeat until Layer 1's tests are all green.
5. Only then move to Layer 2.
6. Only then Layer 3.
7. Only then the REPL.

### 8.2. Test file organization

One test file per layer, in `tests/`:

- `test_<layer1>.cpp`
- `test_<layer2>.cpp`
- `test_<layer3>.cpp`

Each file uses Catch2, no `main()` — Catch2 provides one.

### 8.3. Test naming

Test case names should describe the behavior:

```
"Layer1: integer number"
"Layer1: decimal number"
"Layer2: unary minus applies to the value on the right"
"Layer3: power is right-associative"
```

### 8.4. Assertions

Catch2 macros you will use:

| Macro | Use |
|---|---|
| `REQUIRE(cond)` | Hard assertion — stops the test |
| `CHECK(cond)` | Soft assertion — continues |
| `REQUIRE_THROWS_AS(expr, Type)` | Must throw a specific type |
| `REQUIRE_NOTHROW(expr)` | Must not throw |

### 8.5. Running tests

```bash
cmake --build --preset macos-debug
ctest --preset macos-debug --output-on-failure
```

Or run the test binary directly:

```bash
./build/macos-debug/bin/tessera_core_tests
```

### 8.6. Coverage (optional)

For CI, measure coverage with `llvm-cov` (macOS) or `lcov` (Linux). Target:
≥ 80 % for Layers 1–2, ≥ 90 % for Layer 3.

---

## 9. Common pitfalls

1. **Right-associativity of `^`.** If you write `ParsePower` calling itself
   recursively, you will get left-associativity. Call `ParseUnary` on the right.

2. **Unary minus precedence.** `-2 ^ 2` is `-4`, not `4`. The grammar has
   unary below power.

3. **Missing `End` unit.** Without it, Layer 2 will have to bounds-check
   `pos < tokens.size()` everywhere. Add `End` at the end of Layer 1's output.

4. **Position tracking.** If your error messages say `position N`, N must be
   the index in the **original** string, not the index in the token list.

5. **Whitespace in the middle of numbers.** `1 2` should be two numbers, not
   twelve. Your number scanner must stop at non-digits.

6. **Empty input.** Empty string → Layer 1 returns `[End]` → Layer 2 should
   throw `Unexpected end of input`.

7. **Trailing garbage.** `2 2` → Layer 2 must throw on the second `2`.

8. **Function arg count.** Check it in Layer 3, not Layer 2 — unless you have
   a strong reason.

9. **Division by zero.** Check the right operand, not the left.

10. **`std::abs` vs `std::fabs`.** For `double`, both work. Prefer `std::fabs`
    when you want to be explicit that the argument is floating-point.

---

## 10. Definition of done for v0.1

A version is done when **all** of these are true:

- [ ] All tests are green (`ctest --preset macos-debug`).
- [ ] The REPL handles `2+2*3`, `sqrt(16)`, `sin(pi/2)`, `2^3^2`.
- [ ] The REPL does not crash on any input.
- [ ] `-Werror` clean (`cmake --preset linux-debug-strict`).
- [ ] ASan clean (`cmake --preset macos-debug-asan`).
- [ ] README updated.
- [ ] CI (GitHub Actions) green on all three platforms.
- [ ] Tag `v0.1.0` created, GitHub Release published.

---

## 11. Build system recap

The project is built with CMake and produces **four artifacts** from one
source tree:

| Artifact | Type | Purpose |
|---|---|---|
| `TesseraCore` | library (static by default, shared via `-DBUILD_SHARED_LIBS=ON`) | reusable core |
| `tessera` | executable | the REPL, links against `TesseraCore` |
| `tessera_sandbox` | executable | your private experiments |
| `tessera_core_tests` | executable | tests, links against `TesseraCore` |

To consume `TesseraCore` from another CMake project:

```cmake
find_package(Tessera REQUIRED)
target_link_libraries(myapp PRIVATE Tessera::Core)
```

Nothing about the build needs to change while you work — `GLOB_RECURSE` picks
up new `.cpp` files automatically.

---

## 12. How to work with the mentor

- You write the code. The mentor reviews it.
- You send **attempts** with **precise questions** — not "give me the code".
- You ask about **concepts** — not ready-made solutions.
- When stuck: send the current attempt, the failing test output, and one
  sentence explaining what you expected. The mentor gives a hint.