# Pitt

**EXTREMELY IMPORTANT! THIS LANGUAGE IS A WORK IN PROGRESS AND IT IS A REMAKE OF MY OLDER PITT (WITH LOTS OF ERRORS)! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS LANGUAGE AT YOUR OWN RISK!**

Pitt is a [Concatenative](https://en.wikipedia.org/wiki/Concatenative_programming_language) [Stack-Oriented](https://en.wikipedia.org/wiki/Stack-oriented_programming) [Programming Language](https://en.wikipedia.org/wiki/Programming_language) for [Computers](https://en.wikipedia.org/wiki/Computer)

# File-System
##### Special NOTE: File-System is dependant on C-String so Native-Pitt-Strings don't work please cast them to C-String for these to work but opening-file works with normal pitt-strings 
**Added File-System which is controlled by seperate functions rather than the syscalls themselves but will try to add support for syscalls to do the same stuff**
##### Added opening file , reading and closing
##### WRITE is added 

# Milestones 
- [x]  Turing Complete 
- [x]  Stack Based
- [x]  Simulated
- [x]  Native String and C-String system
- [x]  C++ File-System
- [ ]  C/Syscall File-System
- [ ]  All syscalls 
- [ ]  Compiled
- [ ]  Optimized
**NOTE: Milestones are development logs not selling points**

  
# Info
**NOTE: PITT'S PARSING METHODE HAS BEEN CHANGED**

# Simulation
``` Pascal
./hello.pitt
"Hello World" puts
```
```console
./pitt run ./hello.pitt
..... Simulation Infos ......
Hello World
```

#### DataTypes
```pascal
INT - 5 , 10 , 232 
BOOL - 0 , 1 
STR - "Hello" , "Bye" 
PTR - pointer is also INT but it is specialised type
```

#### ARGC & ARGV
**ARGC = pushes the argument count onto the stack and ARGV is a keyword that takes 1 argument from stack (location) and tries to read it from provided-arguments**

#### Stack Operations

| Keyword    | Signature        | Description                                                                                  |
| ---        | ---              | ---                                                                                          |
| `dup`      | `a -- a a`       | duplicate the element on top of the stack.                                                                                       |
| `swap`     | `a b -- b a`     | swap 2 elements on the top of the stack.                                                                                       |
| `drop`     | `a b -- a`       | drops the top element of the stack.                                                                                       |
| `over`     | `a b -- a b a`   | copy the element below the top of the stack                                                                                        | 
| `rot`      | `a b c -- b c a` | rotate the top three stack   elements.                                                                                    |      
| `-rot`     | `a b c -- c a b` | rotate the top three element of the stack                                                                                        |       
| `nip`      | `a b -- b `      | drops the element before the top  element |                                                                                               

# Memory 
| Keyword   | Signature        | Operation                                                   |
| ----      | ----             | ----                                                        |
| `!i8`       | `a b c -- a`     | Pops two items from the stack (data and location) and stores the data at the given location of byte stack|
| `@i8`       | `a b -- a`       | Pops location from the stack and reads the location from  byte stack|
| `addr(i8)`  | ` -- a`          | Pushes the beginning of the byte stack usable location intothe stack|
| `!i16`       | `a b c -- a`     | Pops two items from the stack (data and location) and stores the data at the given location of byte stack|
| `@i16`       | `a b -- a`       | Pops location from the stack and reads the location from  byte stack|
| `addr(i16)`  | ` -- a`          | Pushes the beginning of the byte stack usable location intothe stack|
| `!i32`       | `a b c -- a`     | Pops two items from the stack (data and location) and stores the data at the given location of byte stack        |
| `@i32`       | `a b -- a`       | Pops location from the stack and reads the location from  byte stack|
| `addr(i32)`  | ` -- a`          | Pushes the beginning of the byte stack usable location intothe stack |

# Examples
*Located at ./examples/*

# NOTE
*Removed the newline printing from `print` function & Modified the parser a bit & Added upto 32bit memory support*


# Def
**NOTE: DEF is goto keyword from C/C++**


# Const 
**NOTE: Const is same as constants they can be initialized with 2 types (for now) int or str and they cast `str` to `cstr` which could be recasted later**

# Memory
**NOTE: Memory just reserves a memory of size (given) and gives it a name which on being called generates its beginning and size-offset onto the stack**

# Assertion
##### Assertion takes 1 C-String as output string and 1 condition to make an assert 

# SYSCALLS
##### write   [writes bytes into fd from memory]
##### exit    [exits current program with an error-code]
##### pause   [pauses the terminal]
##### read    [reads from a fd and dumps it into memory]
##### getpid  [gets id of this process]


# TYPECHECK
**Our basic typecheck and error-reporting is done in RUNTIME but a special kind of type-check will be introduced to make sure that the code follows some rules => **

To-Be-Done
```py
conditions =>  
    PTR  op PTR = INVALID 
    PTR  op INT = VALID 
    PTR  op BOOL = INVALID
    INT  op INT = VALID 
    INT  op BOOL =  VALID 
    BOOL op BOOL = INVALID 
    
    make-sure functions follows these same rules  

finally => 
    stack.empty() == true (else throw error)
```
