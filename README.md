# Crafting Interpreters

This is a repo working through the book [Crafting Intertreters](https://craftinginterpreters.com/contents.html). Which aims to make a basic tree-walking interpreter and a byte code machine.

Current Position is: (https://craftinginterpreters.com/evaluating-expressions.html)

## Building & Running

### Building

```bash
mkdir build
cd build
cmake ..
make
```

Produces:
- `jlox` — main interpreter
- `expr_util` — AST struct generator (dev util)

### Running Lox Programs
1. Place `.lox` file in `build/`
2. Run:
    ```bash
    ./jlox yourfile.lox
    ```
   Or launch the REPL:
    ```bash
    ./jlox
    ```
   Then type Lox expressions interactively.

#### Example:
```bash
cp ../*.lox .
./jlox tester.lox
./jlox tester_sum.lox
```

---

## Example: Running a Lox File

Given `tester_sum.lox`:

```lox
(10 - 4) * 2 <= 2 * 3 + 4 / (1 + 1)
```

Run:
```bash
./jlox tester_sum.lox
```

Output:
```
Boolean: False

(10 - 4) * 2 <= 2 * 3 + 4 / (1 + 1)

[0] (1) LEFT_PAREN -- (
[1] (1) NUMBER -- 10 --- 10.000000
[2] (1) MINUS -- -
[3] (1) NUMBER -- 4 --- 4.000000
[4] (1) RIGHT_PAREN -- )
[5] (1) STAR -- *
[6] (1) NUMBER -- 2 --- 2.000000
[7] (1) LESS_EQUAL -- <=
[8] (1) NUMBER -- 2 --- 2.000000
[9] (1) STAR -- *
[10] (1) NUMBER -- 3 --- 3.000000
[11] (1) PLUS -- +
[12] (1) NUMBER -- 4 --- 4.000000
[13] (1) SLASH -- /
[14] (1) LEFT_PAREN -- (
[15] (1) NUMBER -- 1 --- 1.000000
[16] (1) PLUS -- +
[17] (1) NUMBER -- 1 --- 1.000000
[18] (1) RIGHT_PAREN -- )
[19] (2) EOF -- (null)

(0) Binary: (1) LESS_EQUAL -- <=
| (1) Binary: (1) STAR -- *
| | (2) Grouping:
| | | (3) Binary: (1) MINUS -- -
| | | | (4) Literal: (1) NUMBER -- 10 --- 10.000000
| | | | (4) Literal: (1) NUMBER -- 4 --- 4.000000
| | (2) Literal: (1) NUMBER -- 2 --- 2.000000
| (1) Binary: (1) PLUS -- +
| | (2) Binary: (1) STAR -- *
| | | (3) Literal: (1) NUMBER -- 2 --- 2.000000
| | | (3) Literal: (1) NUMBER -- 3 --- 3.000000
| | (2) Binary: (1) SLASH -- /
| | | (3) Literal: (1) NUMBER -- 4 --- 4.000000
| | | (3) Grouping:
| | | | (4) Binary: (1) PLUS -- +
| | | | | (5) Literal: (1) NUMBER -- 1 --- 1.000000
| | | | | (5) Literal: (1) NUMBER -- 1 --- 1.000000
```

---

## Project Status / What’s Accomplished
- Project implements chapters from [Crafting Interpreters](https://craftinginterpreters.com/contents.html) in C.
- AST for literals, grouping, unary, binary expressions in place
- Lexer, token stream, parser, and tree-walk interpreter end-to-end
- Runs single-file input or REPL, outputs evaluation + debug info
---

## Detailed TODO (by Book Chapter)

### Ch. 7: Evaluating Expressions
- [x] Eval for literal, grouping, unary, binary (arithmetic, comparison, equality)
- [x] Truthiness, equality, error on type mismatch
- [ ] Division by zero detection (runtime error if `/ 0`)
- [ ] String concatenation if either arg string (loose mode for `+`)
- [ ] Improve runtime error reporting (token/position)
- [ ] Support/disable mixed-type comparisons (chapter challenge)

### Next Chapters Prep
- [ ] Statement syntax: variable assignment, blocks, control flow
- [ ] Variable environment, assignment evaluation
- [ ] Functions, returns, closure capture (later)
- [ ] Class/object syntax, field/method/runtime support
- [ ] Pretty-print Lox results (numbers as `1` not `1.0`)
- [ ] Error handling: handler context, show real token/source info
- [ ] Test harness: run all `.lox` files, check output

---



