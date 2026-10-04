# Tessera — Technical Specification v0.1

## 1. Project goal

Build a program that:

- accepts a mathematical expression as text,
- reads it, computes the result,
- reports syntax and semantic errors in a clear way.

The first version is a **console REPL**. Graphics (function plotting, matrices, physics) will come in later versions.

**Name:** Tessera.
**Style:** C++17, cross-platform (macOS / Linux / Windows).

---

## 2. What the user sees

The program starts, prints a `> ` prompt, reads a line, prints the result or an error, and shows the prompt again. The loop continues until the user exits.

**Sample session:**

```
Tessera v0.1.0
Type 'exit' or press Ctrl+D to quit.

> 2 + 2 * 3
= 8

> (2 + 2) * 3
= 12

> sqrt(16)
= 4

> sin(pi / 2)
= 1

> 2 ^ 3 ^ 2
= 512

> 1 / 0
Error: Division by zero

> 2 +
Error: Unexpected end of input

> exit
Bye.
```

**Interface requirements:**

- Prompt `> ` before every line.
- Result printed as `= <number>`.
- Error printed as `Error: <message>`.
- An error does **not** terminate the program — the loop continues.
- Empty line — ignored.
- `exit` or EOF — terminates the program.

---

## 3. Three conceptual layers

The program **must** be split into three independent layers.

### Layer 1 — Reading the text

Takes a **string**. Returns a **list of atomic units** (called *tokens* in compiler terminology). This layer does not know about the structure of the expression and does not compute anything.

### Layer 2 — Building the structure

Takes a **list of atomic units**. Returns a **tree** that describes the structure of the expression. This layer does not compute anything, but knows about operator precedence and syntax.

### Layer 3 — Computing the result

Takes a **tree** and **variable values**. Returns a **number**. This layer does not know about the text.

**Rule:** each layer must not reach into the internals of its neighbors. Only through a public interface.

---

## 4. Layer 1 — reading the text

### 4.1. What is an atomic unit

An atomic unit is the smallest meaningful piece of input. For example, `2 + sin(x)` produces:

- "number 2"
- "plus"
- "identifier `sin`"
- "opening parenthesis"
- "identifier `x`"
- "closing parenthesis"
- "end of input"

### 4.2. Categories

The minimum set to recognize:

| Category | Example input |
|---|---|
| Number | `42`, `3.14`, `0.5` |
| Identifier | `x`, `sin`, `pi`, `my_var` |
| Operator `+` | `+` |
| Operator `-` | `-` |
| Operator `*` | `*` |
| Operator `/` | `/` |
| Operator `^` | `^` |
| Opening parenthesis | `(` |
| Closing parenthesis | `)` |
| Comma | `,` |
| End of input | (service) |
| Unrecognized | `@`, `#`, `$`, ... |

**Every atomic unit** carries:

- its category,
- the original text,
- its position in the source string (for error messages),
- if it is a number — its numeric value.

### 4.3. Behavior requirements

1. **Spaces, tabs, line breaks** between units are ignored.
2. **A number** starts with a digit. It may contain one dot and digits after it. Examples: `42`, `3.14`. **Not supported** in v0.1: `1e5`, `0x1F`, `.5`, `1.`.
3. **An identifier** starts with a letter or underscore and continues with letters, digits, or underscores.
4. **Operators and parentheses** are single characters. Not supported in v0.1: `**`, `//`, `==`, `<=`, `>=`.
5. **Unrecognized characters** become units of the "unrecognized" category with the character itself in the text field. **No exception is thrown** — the next layer decides what to do.
6. **At the end of the list**, a service unit of category "end of input" is always added. This simplifies the next layer.

---

## 5. Layer 2 — building the tree

### 5.1. What is an expression tree

The tree describes the **structure** of the expression, taking precedence into account. Example: `2 + 3 * 4` is parsed so that `3 * 4` is the **right subtree** of `+`, because multiplication binds tighter.

Graphically:

```
       (+)
      /   \
     2    (*)
         /   \
        3     4
```

### 5.2. Operator precedence (from lowest to highest)

1. Addition `+`, subtraction `-`
2. Multiplication `*`, division `/`
3. Unary minus `-`, unary plus `+`
4. Power `^` — **right-associative**
5. Function call, parentheses, numbers, variables

### 5.3. Associativity

- `+`, `-`, `*`, `/` are **left-associative**: `2 - 3 - 4` parses as `(2 - 3) - 4`.
- `^` is **right-associative**: `2 ^ 3 ^ 2` parses as `2 ^ (3 ^ 2) = 512`.

### 5.4. Grammar (formal description)

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

**Read it like this:**

- `expression` — a `term`, followed by zero or more pairs of `(+ or -) term`.
- `term` — a `unary`, followed by zero or more pairs of `(* or /) unary`.
- `unary` — a sign followed by another `unary`, or a `power`.
- `power` — either a `primary`, or `primary ^ unary` (note: the **right side** is `unary`, not `power`).
- `primary` — a number, an identifier with optional parentheses (function call), an identifier, or `( expression )`.

### 5.5. Recommended implementation

**Recursive descent:** one function per precedence level. Each function calls the next one in the chain. `primary` is the "leaf" — that is where recursion ends.

### 5.6. Error handling

**Must throw an exception** with a message that includes the **position** in the source string:

- `Unexpected token at position N: '<lexeme>'`
- `Expected ')' at position N`
- `Unexpected end of input`

**Situations that must be errors:**

- `2 +` — no right operand
- `(2 + 2` — missing closing parenthesis
- `2 2` — two numbers in a row without an operator
- `2 @ 2` — unrecognized character
- (empty string) — no expression
- `sin(` — no argument and no closing parenthesis
- `sin(1, 2, 3)` — too many arguments (optional: only if you want to check it during parsing; otherwise — during evaluation)

---

## 6. Layer 3 — computing the result

### 6.1. Purpose

Walks the tree, returns a number. Keeps a map of variables.

### 6.2. Behavior

- **Number** — returned as is.
- **Variable** — looked up in the map. Found → its value. Not found → exception `Unknown variable: <name>`.
- **Unary minus** — negation.
- **Unary plus** — value unchanged.
- **`+`, `-`, `*`** — arithmetic.
- **`/`** — if the right operand is zero → exception `Division by zero`.
- **`^`** — power (can use `std::pow`).

### 6.3. Built-in functions

Minimum set for v0.1:

| Name | Arguments | Purpose |
|---|---|---|
| `sin` | 1 | sine |
| `cos` | 1 | cosine |
| `tan` | 1 | tangent |
| `sqrt` | 1 | square root |
| `abs` | 1 | absolute value |
| `log` | 1 | natural logarithm |
| `exp` | 1 | exponential |
| `pow` | 2 | power |

**Requirements:**

- Unknown function → exception `Unknown function: <name>`.
- Wrong number of arguments → exception `<name> expects N argument(s)`.

### 6.4. Variables

- Stored in a map `name → value`.
- Added **from outside** (through a public method). The parser **does not create** variables.
- When the REPL starts, two constants are set: `pi` and `e`.

---

## 7. Testing requirements

**Tests are part of the specification.** Code is not considered done without them.

### 7.1. Coverage for Layer 1

- an integer number, a number with a dot;
- each operator separately;
- parentheses and comma;
- an identifier;
- a full expression (several unit categories);
- spaces are ignored;
- an unrecognized character → a unit of category "unrecognized";
- "end of input" is appended at the end;
- the position of a unit in the source string is correct.

### 7.2. Coverage for Layers 2 + 3

- simple addition;
- precedence of `*` over `+`;
- parentheses change the order;
- unary minus;
- **right-associativity of `^`** (`2^3^2 = 512`);
- precedence of `^` over unary minus (`-2^2 = -4`, not `4`);
- function calls: 1 argument, 2 arguments;
- nested calls (`sqrt(sqrt(16)) = 2`);
- unknown function → error;
- unknown variable → error;
- division by zero → error;
- syntax errors: `2 +`, `(2 + 2`, `2 2`, `2 @ 2`, empty string, `sin(`.

### 7.3. Tools

- Framework — **Catch2 v3** (already wired through `FetchContent` in `tests/CMakeLists.txt`).
- Tests are run through `ctest`.
- Recommended: write tests **as you go** — first failing, then green (**TDD**).

### 7.4. Coverage targets

Minimum — **80 %** for Layers 1 and 2. Layer 3 — **90 %** (its coverage is easier to reach).

---

## 8. What is NOT in v0.1

Explicitly **not** implemented now:

- graphics, windows, visualization;
- matrices, vectors, complex numbers;
- physics formulas;
- scientific notation `1e5`;
- hex `0x1F`;
- factorial `!`, modulo `%`, bitwise operators;
- comparison operators `<`, `>`, `==`, `!=`;
- variables **in the parser** (only through the `set_variable` API);
- command history;
- multi-line input;
- user-defined functions;
- derivatives, integrals, limits.

All of that — **separate versions** (v0.2, v0.3, ...).

---

## 9. Naming (non-binding recommendations)

You are free to choose your own names. For reference, common terms in the C++ community:

| Concept | Common term |
|---|---|
| Layer 1 | Lexer, Tokenizer |
| Layer 2 | Parser |
| Layer 3 | Evaluator, Interpreter |
| Atomic unit | Token |
| Tree | AST, Expression tree |
| Unit category | TokenType, TokenKind |
| Tree node | Expr, AstNode |

**Code style:** PascalCase for types, snake_case or camelCase for methods — choose **one** and stick with it.

---

## 10. Definition of Done for v0.1

A version is considered done when:

- [ ] All mandatory tests are green (`ctest --preset macos-debug`).
- [ ] Coverage ≥ 80 % for Layers 1–3 (measured with `llvm-cov` / `lcov`).
- [ ] The REPL handles `2+2*3`, `sqrt(16)`, `sin(pi/2)`, `2^3^2`.
- [ ] The REPL does not crash on bad input.
- [ ] Build is clean on `linux-debug-strict` (no warnings).
- [ ] Build is clean under ASan (`macos-debug-asan` or `linux-debug-asan`).
- [ ] README updated: what it does, how to build, how to run, example.
- [ ] CI (GitHub Actions) is green.
- [ ] Tag `v0.1.0` created, GitHub Release published.

---

## 11. How to work with the mentor

Rules:

- **You do not ask for code.** You write it yourself.
- **You send attempts** — with the question "I don't understand why the test fails with X".
- **You ask about concepts**, not ready-made solutions.
- **The mentor reviews, explains, guides** — but does not write on your behalf.
- **If stuck** — send your attempt and a precise question. The mentor gives a hint.

---

## 12. Order of work

1. Read this spec **twice**.
2. Open `tests/` — empty at first, add tests **yourself**. Section 7 lists what to cover.
3. Start with **Layer 1**. One test → implementation → green → next.
4. **Do not write everything at once.** One function at a time.
5. Only when all Layer 1 tests are green — move to Layer 2.
6. Only when all Layer 2 tests are green — move to Layer 3.
7. Only when all three layers are green — write the REPL.
8. Commit **every day**, even if it is a single test.

**Go. You write. The mentor stands by.**