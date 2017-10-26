
# btcs

  bitcoin script parser/evaluator/compiler/decompiler

## Motivation

  I wanted a better way to hack on Bitcoin scripts

## TODOs

- [X] Tokenizer
- [ ] Evaluator
- [ ] Compiler
- [ ] Decompiler

## Usage

  Work in progress. Some OPs are working:

```
./btcs <<< '1 2 16 3dup 2swap'
script: 1 2 16 3DUP 2SWAP 
stack:  1 2 2 16 16 1
```

  It should tokenize everything at least, even BIP examples! syntax might change ;)

```
./btcs <<EOS

IF
  <now + 3 months> CHECKLOCKTIMEVERIFY DROP
  <Lenny's pubkey> CHECKSIGVERIFY
  1
ELSE
  2
ENDIF
<Alice's pubkey> <Bob's pubkey> 2 CHECKMULTISIG

EOS
```

  spits out

```
IF
ex:<now + 3 months>
CHECKLOCKTIMEVERIFY
DROP
ex:<Lenny's pubkey>
CHECKSIGVERIFY
1
ELSE
2
ENDIF
ex:<Alice's pubkey>
ex:<Bob's pubkey>
2
CHECKMULTISIG
```
