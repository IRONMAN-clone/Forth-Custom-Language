# PEARL
Language: PEARL
Operation: Stack
Syntax/Notation: Postfix 
Status: Underdevelopment 
Optimization: 0% 
Turing-Complete: Not-yet
Targetted-Platform: x64_WINDOWS (I didn't try it on linux but atleast i think it wont run on x84_OS)

# Guide

# Section:- keywords
Stack-Operators
```c
dup ( x -- x x )
drop ( x -- )
swap ( x y -- y x )
over ( x y -- x y x )
dupd ( x y -- x x y )
swapd ( x y z -- y x z )
nip ( x y -- y )
rot ( x y z -- y z x )
```

Binary-Operators
```c
2   not print
1 2 band print
1 2 bor  print
1 2 beor print
1 2 shl  print
1 2 shr  print
```

Comparing-Operators
```c
2 1 = print
2 1 != print
2 1 > print
2 1 < print
2 1 >= print
2 1 <= print
```

Conditions and Loops
```c
if 34 35 + 70 = then 69 else 420 end print
if 34 35 + 69 = then 69 else 420 end print
```

Here if is the condition builder and then is the condition controller else is failed status of the condition in then

```c
10 while dup 0 > then
    dup print
    1 -
end
```

Here while is the loop builder and then is as usual used as the loop condition controller end is the keyword which is controlling if 'that' time loop is gonna repeat or not

# Tests
./examples/










