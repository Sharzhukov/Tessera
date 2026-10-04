# Contributing to Tessera

First off — thank you for considering contributing to Tessera!
Your help makes this project better.

By participating, you agree to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

---

## Table of Contents

- [How Can I Contribute?](#how-can-i-contribute)
- [Code Style](#code-style)
- [Development Setup](#development-setup)
- [Commit Messages](#commit-messages)
- [License](#license)
- [Contact](#contact)

---

## How Can I Contribute?

### Reporting Bugs

Open an issue on GitHub with:

- A clear title and a short description
- Steps to reproduce the problem
- Expected behavior vs. actual behavior
- Your operating system (Windows / macOS / Linux) and compiler version
- The output of `cmake --version`
- A minimal code snippet or input expression that triggers the problem, if possible

### Suggesting Features

Feature requests are welcome. Open an issue and describe:

- What you want to add
- Why it is useful for the project
- How it should behave (if you already have ideas)

### Pull Requests

The `main` branch is protected. All changes go through a pull request.

1. Fork the repository (or create a branch if you have write access):

   ```bash
   git checkout -b feature/your-feature-name
   ```

2. Make your changes.

3. Build and test locally on your operating system
   (see [Development Setup](#development-setup)).

4. Commit with a clear message (see [Commit Messages](#commit-messages)).

5. Push your branch:

   ```bash
   git push origin feature/your-feature-name
   ```

6. Open a Pull Request against `main`.

The maintainer **squash-merges** approved pull requests. Merge commits are
not allowed — `main` keeps a linear history.

---

## Code Style

### Language and standard

- **C++17**, standard library only — no third-party dependencies in the core.
- Headers use the `.hpp` extension. Sources use `.cpp`.
- Every header starts with `#pragma once` (no include guards).
- Do not use `using namespace std;` in headers or sources.
- Do not use raw `new` / `delete` in user code. Prefer `std::unique_ptr`,
  `std::make_unique`, or containers.
- Prefer value semantics over pointers where possible.

### Naming conventions

| Element | Style | Example |
|---|---|---|
| Namespace | lowercase, nested with `::` | `myproject::core` |
| Class / struct / enum | `PascalCase` | `MyType`, `MyEnum` |
| Public function / method | `PascalCase` | `DoSomething()`, `GetValue()` |
| Class member | pick one style and stay consistent | `value_` **or** `m_value` — not both |
| Local variable / parameter | `snake_case` | `start_pos`, `max_size` |
| Constant | `kPascalCase` | `kMaxDepth`, `kBufferSize` |
| Enum value | `PascalCase` | `MyEnum::FirstValue` |

> **Important:** pick one member style per class and stay consistent. Mixing
> `value_` and `m_value` inside the same class is not allowed.

### Includes

Order of includes in every `.cpp` file:

1. Own header (if any)
2. Other project headers
3. C++ standard library headers

Example:

```cpp
// some_module.cpp
#include <myproject/core/some_module.hpp>  // 1. own header
#include <myproject/core/other_module.hpp> // 2. project headers
#include <string>                          // 3. standard library
#include <vector>
```

Project headers are included with angle brackets, using the path relative to
the `include/` directory.

### License header

Every source and header file starts with:

```cpp
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Alexander Sharzhukov
```

The full text of the license lives in the [LICENSE](LICENSE) file at the
repository root.

### Formatting

- Indentation: 4 spaces. No tabs.
- Line length: aim for 100 characters or fewer.
- One statement per line.
- Braces: K&R style (opening brace on the same line).

---

## Development Setup

### Prerequisites

- **CMake** ≥ 3.25
- **Ninja** ≥ 1.11 (recommended)
- A **C++17 compiler**:
  - Windows — MSVC 2019 or newer (from Visual Studio)
  - macOS — AppleClang (from Xcode Command Line Tools)
  - Linux — GCC 9 or newer, or Clang 12 or newer
- **Git**

### Clone and build

```bash
git clone https://github.com/Sharzhukov/Tessera.git
cd Tessera
cmake --preset macos-debug        # or linux-debug / windows-debug
cmake --build --preset macos-debug
ctest --preset macos-debug --output-on-failure
```

### Testing

Run all tests:

```bash
ctest --preset macos-debug --output-on-failure
```

Run the test binary directly:

```bash
./build/macos-debug/bin/tessera_core_tests
```

---

## Commit Messages

This project follows [Conventional Commits](https://www.conventionalcommits.org/).

Format:

```
<type>(<scope>): <short description>
```

Common types:

| Type | Use for |
|---|---|
| `feat` | A new feature |
| `fix` | A bug fix |
| `refactor` | A code change without behavior change |
| `test` | Adding or fixing tests |
| `docs` | Documentation only |
| `style` | Formatting, no code change |
| `chore` | Build, CI, tooling |
| `perf` | Performance improvement |

Examples:

```
feat: read numbers and operators from input
fix: handle right-associative exponent
test: cover division by zero
docs: add build instructions
```

The `<scope>` is optional. Use it when it adds clarity, for example
`feat(parser): ...` or `fix(ui): ...`.

---

## License

By contributing, you agree that your contributions are licensed under the
**GNU General Public License v3.0 or later** — see the [LICENSE](LICENSE) file
for the full text.

---

## Contact

- **Website:** [sharzhukov.com](https://sharzhukov.com)
- **GitHub:** [@Sharzhukov](https://github.com/Sharzhukov)
- **Email:** [admin@sharzhukov.com](mailto:admin@sharzhukov.com)