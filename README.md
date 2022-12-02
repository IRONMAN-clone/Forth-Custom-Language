# Porth

**EXTREMELY IMPORTANT! THIS LANGUAGE IS A WORK IN PROGRESS AND IT IS A REMAKE OF MY OLDER PITT (WITH LOTS OF ERRORS)! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS LANGUAGE AT YOUR OWN RISK!**

Pitt is a [Concatenative](https://en.wikipedia.org/wiki/Concatenative_programming_language) [Stack-Oriented](https://en.wikipedia.org/wiki/Stack-oriented_programming) [Programming Language](https://en.wikipedia.org/wiki/Programming_language) for [Computers](https://en.wikipedia.org/wiki/Computer)

# Help
```console
./pitt 
```
``` pascal
pitt: fatal error: no input files or operation type provided!
PITT:
pitt (command) <filename> [subcmd]
COMMAND:
       run     simulates the file
       dump    dumps all the tokens
SUBCMD:
       timeit  Times exection time
```

# Simulation
``` Pascal
./hello.pitt
25 5 * print
```
```console
./pitt run ./hello.pitt
..... Simulation Infos ......
125
```
#### Stack Operations

| Keyword   | Operation        | Description                                                                                  |
| ---       | ---              | ---                                                                                          |
| `dup`     | `a -- a a`       | duplicate the element on top of the stack.                                                   |
| `swap`    | `a b -- b a`     | swap 2 elements on the top of the stack.                                                     |
| `drop`    | `a b -- a`       | drops the top element of the stack.                                                          |
| `over`    | `a b -- a b a`   | copy the element below the top of the stack                                                  | 
| `rot`     | `a b c -- b c a` | rotate the top three stack   elements.                                                       |      
| `-rot`    | `a b c -- c a b` | rotate the top three element of the stack                                                    |         
| `nip`     | `a b -- b `      | drops the element before the top element                                                     |                                                                                               

# Examples
*Located at ./examples/*
