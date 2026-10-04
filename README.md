# Tessera

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.25%2B-green.svg)
![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-GPL--3.0-yellow.svg)

> **Math expression parser, evaluator and visualizer.**

Tessera turns mathematical expressions from plain text into numbers, and (eventually) into pictures. Type `2 + 2 * 3` — get `8`. Type `sin(pi / 2)` — get `1`. Type `y = x^2` — see the curve.

The project grows step by step: from a text-based console to a full visualizer with a GUI.

**Status:** early development. See the [Roadmap](#-roadmap).

---

## ✨ Features

### Working now

- ✅ **CMake build system** — presets for macOS / Linux / Windows
- ✅ **Test infrastructure** — Catch2 + CTest, ready to use
- ✅ **Cross-platform** — one codebase, three operating systems

### In progress (v0.1)

- 🚧 **Read math expressions** — turn text into a structured form
- 🚧 **Compute results** — support for `+ - * / ^`, parentheses, and common functions
- 🚧 **Interactive console** — type an expression, get an answer
- 🚧 **Error reporting** — clear messages when something is wrong

### Planned

- **v0.2** — Matrices and vectors
- **v0.3** — Graphical interface (window, menu, input field)
- **v0.4** — Function plotting (`y = f(x)`)
- **v0.5** — Physics module (kinematics, dynamics)
- **v0.6** — Complex numbers
- **v1.0** — Stable public release

---

## 🛠 Requirements

| Tool | Minimum version |
|---|---|
| CMake | 3.25 |
| C++ compiler | MSVC 2019+, Clang 12+, GCC 9+ |
| Ninja | 1.11 *(recommended)* |
| Git | any |

---

## 🚀 Build

The project uses **CMake presets** for reproducible builds on all platforms.

### macOS

```bash
cmake --preset macos-debug
cmake --build --preset macos-debug
ctest --preset macos-debug --output-on-failure
```

### Linux

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
ctest --preset linux-debug --output-on-failure
```

### Windows

Open **Developer PowerShell for VS** and run:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --preset windows-debug -C Debug --output-on-failure
```

### All available presets

```bash
cmake --list-presets
```

---

## ▶️ Usage

```bash
./build/macos-debug/bin/tessera
```

```
Tessera v0.1.0
Type 'exit' or press Ctrl+D to quit.

> 2 + 2 * 3
= 8

> sqrt(16)
= 4

> sin(pi / 2)
= 1

> 2 ^ 3 ^ 2
= 512

> 1 / 0
Error: Division by zero

> exit
Bye.
```

---

## 🧪 Tests

```bash
ctest --preset macos-debug --output-on-failure
```

Run a specific test binary directly:

```bash
./build/macos-debug/bin/tessera_core_tests
```

---

## 🗺 Roadmap

- [x] Project skeleton (CMake + presets + CI-ready)
- [ ] **v0.1** — Read, compute, and print math expressions
- [ ] **v0.2** — Matrices and vectors
- [ ] **v0.3** — Graphical interface
- [ ] **v0.4** — Function plotting
- [ ] **v0.5** — Physics module
- [ ] **v0.6** — Complex numbers
- [ ] **v1.0** — Stable public release

---

## 📚 Documentation

- [Technical specification v0.1](docs/TZ-v0.1.md) — what the project does and why
- [CMake cheatsheet](docs/CMake-Cheatsheet.md) — build commands reference

---

## 🤝 Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) and follow our [Code of Conduct](CODE_OF_CONDUCT.md).

For security issues, see [SECURITY.md](SECURITY.md).

---

## 📄 License

**GNU General Public License v3.0 or later** — see [LICENSE](LICENSE) or <https://www.gnu.org/licenses/gpl-3.0.html>.

```
Copyright (C) 2026 Alexander Sharzhukov
```

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

---

## 👤 Author

**Alexander Sharzhukov**

- Website: [sharzhukov.com](https://sharzhukov.com)
- GitHub: [@Sharzhukov](https://github.com/Sharzhukov)
- Email: [admin@sharzhukov.com](mailto:admin@sharzhukov.com)