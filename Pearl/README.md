# PEARL
Language: PEARL
Operation: Stack
Syntax/Notation: Postfix 
Status: Underdevelopment 
Optimization: 0% 
Cross-Platform: Not-yet
Targetted-Platform: x64_WINDOWS (I didn't try it on linux but atleast i think it wont run on x84_OS)

# Guide
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

# Tests
./examples/










