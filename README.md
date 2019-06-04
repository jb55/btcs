
# btcs

[![Build Status](https://travis-ci.org/jb55/btcs.svg)](https://travis-ci.org/jb55/btcs)

  bitcoin script parser/evaluator/compiler/decompiler
  
  **WARNING**: This is not a consensus-critical implementation of
  bitcoin script. Make sure to actually test your script on testnet or
  with something like [btcdeb](https://github.com/kallewoof/btcdeb).

## Motivation

  I wanted a better way to hack on Bitcoin scripts

## TODOs

- [X] Tokenizer
- [ ] Evaluator (partially complete)
- [X] Compiler
- [ ] Syntax
- [ ] Decompiler

## Usage

  Work in progress. Some OPs are working:

```
$ btcs 1 2 16 3dup 2swap

script      1 2 16 OP_3DUP OP_2SWAP
script_hex  5152606f72
stack       1 2 2 16 16 1
stack_hex   515252606051
```
