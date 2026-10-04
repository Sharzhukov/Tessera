# CMake — Cheatsheet for Tessera

## Three commands that cover 95% of the workflow

```bash
cmake --preset <preset>          # configure (once, or after editing CMakeLists)
cmake --build --preset <preset>  # build
ctest --preset <preset>          # run tests
```

## Presets by operating system

| OS | Debug | Release | ASan |
|---|---|---|---|
| **macOS** | `macos-debug` | `macos-release` | `macos-debug-asan` |
| **Linux** | `linux-debug` | `linux-release` | `linux-debug-asan` |
| **Linux (strict)** | `linux-debug-strict` | — | — |
| **Windows** | `windows-debug` | `windows-release` | — |

List all presets:

```bash
cmake --list-presets
```

## Basic workflow

### First time (or after removing build/)

```bash
cmake --preset macos-debug
cmake --build --preset macos-debug
ctest --preset macos-debug --output-on-failure
```

### Daily (editing .cpp / .hpp files)

```bash
cmake --build --preset macos-debug
ctest --preset macos-debug --output-on-failure
./build/macos-debug/bin/tessera
```

`cmake --preset` is **not** needed — the configuration has not changed.

### After editing CMakeLists.txt or CMakePresets.json

```bash
cmake --preset macos-debug              # reconfigure
cmake --build --preset macos-debug
```

If `CMakePresets.json` changed — remove `build/` first:

```bash
rm -rf build/
cmake --preset macos-debug
cmake --build --preset macos-debug
```

## What changed → what to do

| You changed | Action |
|---|---|
| `.cpp` | `cmake --build --preset <p>` |
| `.hpp` | `cmake --build --preset <p>` |
| `CMakeLists.txt` | `cmake --preset <p>` + build |
| `CMakePresets.json` | `rm -rf build/` + everything again |
| Compiler or CMake version | `rm -rf build/` + everything again |
| Added a new `.cpp` in `src/` | **nothing** — GLOB picks it up (CONFIGURE_DEPENDS) |
| Added a new test in `tests/` | **nothing** — GLOB picks it up |
| Strange linking error | `rm -rf build/` + everything again |

## Build only one target

```bash
cmake --build --preset macos-debug --target TesseraCore        # library
cmake --build --preset macos-debug --target tessera            # app
cmake --build --preset macos-debug --target tessera_sandbox    # sandbox
cmake --build --preset macos-debug --target tessera_core_tests # tests
```

Useful when only editing `app/main.cpp` — no need to rebuild everything.

## CTest — filters and flags

```bash
ctest --preset macos-debug                       # all tests
ctest --preset macos-debug --output-on-failure   # show output of failures
ctest --preset macos-debug -V                    # verbose
ctest --preset macos-debug -R <pattern>          # only tests matching pattern
ctest --preset macos-debug -E <pattern>          # exclude tests matching pattern
ctest --preset macos-debug -N                    # list tests without running
ctest --preset macos-debug --rerun-failed        # only tests that failed last time
```

## Running binaries

```bash
./build/macos-debug/bin/tessera
./build/macos-debug/bin/tessera_sandbox
./build/macos-debug/bin/tessera_core_tests                  # all tests
./build/macos-debug/bin/tessera_core_tests "<pattern>"      # tests matching pattern
./build/macos-debug/bin/tessera_core_tests --list-tests     # list all tests
```

## ASan — memory checks

```bash
cmake --preset macos-debug-asan
cmake --build --preset macos-debug-asan
ctest --preset macos-debug-asan --output-on-failure
```

If ASan complains — that is **your bug**. Read the stack trace, fix it.

## `-Werror` — strict build (for CI)

```bash
cmake --preset linux-debug-strict
cmake --build --preset linux-debug-strict
ctest --preset linux-debug-strict
```

Works only on Linux. On macOS, use regular `macos-debug` — warnings are still enabled.

## Workflow presets (single command)

```bash
cmake --workflow --preset ci-linux
```

Equivalent to:

```bash
cmake --preset linux-debug-strict
cmake --build --preset linux-debug-strict
ctest --preset linux-debug-strict
```

## What the lines in CMakeLists mean

| Line | Purpose |
|---|---|
| `cmake_minimum_required(VERSION 3.25)` | Minimum CMake version |
| `project(Tessera ...)` | Project name, version, language |
| `set(CMAKE_CXX_STANDARD 17)` | C++ standard |
| `add_library(TesseraCore ...)` | Static library |
| `add_executable(tessera ...)` | Executable |
| `target_link_libraries(x PRIVATE y)` | Link targets |
| `target_include_directories(x PUBLIC include/)` | Header search paths |
| `target_compile_features(x PUBLIC cxx_std_17)` | Standard requirement |
| `add_subdirectory(app)` | Include a subdirectory |
| `enable_testing()` | Enable CTest |
| `add_test(...)` | Register a test |

## Where things end up

```
build/macos-debug/
├── bin/
│   ├── tessera                   ← app
│   ├── tessera_sandbox           ← sandbox
│   └── tessera_core_tests        ← tests
├── libTesseraCore.a              ← library
├── compile_commands.json         ← for clangd
└── _deps/catch2-src/             ← FetchContent: Catch2
```

## What NOT to do

| Bad | Good |
|---|---|
| `cd build && cmake ..` | `cmake --preset macos-debug` |
| `cmake .` in the root | `cmake --preset <p>` |
| Forgetting `--preset` | Always specify the preset |
| `rm -rf build/` for every tiny change | Only when CMakeLists or compiler changes |
| Passing `-DCMAKE_BUILD_TYPE=Debug` by hand | The preset already contains it |

## If something breaks

**Step 1.** Read the **first line** of the error. It usually contains the exact cause.

**Step 2.** If unclear — remove build and retry:

```bash
rm -rf build/
cmake --preset macos-debug
cmake --build --preset macos-debug
```

**Step 3.** If it still fails — send the **full output** of configure + build. First 30 lines and last 30.

## Three scenarios to memorize

**1. Quick check that nothing is broken:**

```bash
cmake --build --preset macos-debug && ctest --preset macos-debug
```

**2. Full reset:**

```bash
rm -rf build/ && cmake --preset macos-debug && cmake --build --preset macos-debug
```

**3. Memory check:**

```bash
cmake --preset macos-debug-asan && cmake --build --preset macos-debug-asan && ctest --preset macos-debug-asan
```

**90% of the time — only these three.**

## Useful details

### How long the build took

```bash
time cmake --build --preset macos-debug
```

### Show all presets with details

```bash
cmake --list-presets=all
```

### Show CMake cache (all variables)

```bash
grep -E "^(TESSERA|CMAKE_BUILD)" build/macos-debug/CMakeCache.txt
```

### Check what went into the library archive

```bash
nm -C build/macos-debug/libTesseraCore.a | head -30
```

### See the real compiler commands

```bash
cmake --build --preset macos-debug --verbose
```

## Related notes

- [[TZ-v0.1]]
- [[CMake-Best-Practices]]
- [[Build-Systems]]