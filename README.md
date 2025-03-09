# Syntax_Analyser for C++

This project implements a basic lexer (also known as a lexical analyzer or a syntax analyser) for C++ source code. The lexer tokenizes source code into various token types like keywords, identifiers, operators, literals, numbers, and more. The program processes a provided C++ file and categorizes each part of the code.

## Features

- **Token Types:** 
  - **Identifiers**: Variables, function names, etc.
  - **Keywords**: C++ reserved words like `if`, `else`, `int`, `float`, etc.
  - **Operators**: Arithmetic and logical operators like `+`, `-`, `*`, `=`, etc.
  - **Separators**: Symbols like parentheses, braces, and commas.
  - **Numbers**: Integer and floating-point numbers.
  - **Literals**: String literals enclosed in quotes.
  - **Comments**: Both single-line (`//`) and multi-line (`/* ... */`) comments.
  - **Format Specifiers**: Handles format specifiers within function calls like `printf` and `scanf`.
  - **Function Calls**: Specifically detects calls to `printf` and `scanf`.

- **C++ Features Supported:**
  - Detection of comments, operators, and literals.
  - Special handling for `printf` and `scanf` function calls to capture format specifiers.
  - Regular expression support to identify format specifiers like `%d`, `%f`, etc.
  - Line-by-line parsing and output of token information, including the type and length of each token.

## How It Works

1. **Reading Code:** The program reads the source code from an input file (`input.in`).
2. **Tokenization:** It processes the code character by character, detecting and categorizing tokens based on predefined rules.
3. **Output:** The lexer outputs the tokens, their types, their line numbers, and their lengths.
4. **Special Case Handling:**
   - Strings and comments are handled specially to correctly capture multi-line comments and string literals.
   - Format specifiers inside `printf` and `scanf` calls are extracted and categorized.

## Installation & Usage

1. **Compile the Program:**
   Ensure you have a C++ compiler (e.g., `g++`) installed.

   ```sh
   g++ -o mn main.cpp -std=c++11
2. **Run the Program:**
   Provide and input C++ file (input.in).
   
   ```sh
   ./mn
