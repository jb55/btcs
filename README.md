
# btcs

  bitcoin script toolbox

## Motivation

  I wanted a better way to hack on Bitcoin scripts

## TODOs

- [ ] Script evaluator
- [ ] Script compiler
- [ ] Script decompiler

## Usage

  Just started this project. You can already tokenize BIP examples though!

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
