# int-computer-2019
Advent of code solution for the Int Computer

```
IntComputer : simple machine code interpreter
Usage :
    main.exe program
        program : comma separated program memory
    main.exe -f path
        path : path to file with one line of comma separated program memory

Arguments :
    --help, -h shows this help
    -f input program file
    --dump show dump before and after execution
    --debug, -d run in debug mode
    --input input string used inplace of cin

Debug mode :
    (s)top
    (n)ext
    (d)ump [start] [end]
    (r)un all
    (m)emory [address1] [address2] ...
    (i)nstruction pointer address
```

## Examples :

 - Quine program
```
> .\main.exe 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99
109 1 204 -1 1001 100 1 100 1008 100 16 101 1006 101 0 99
```
