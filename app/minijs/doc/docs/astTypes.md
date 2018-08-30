AST Types
=========

AstFn
-----
```c
ScanResult (*) (void *context, Scanner *scanner)
```

AstTp
-----
```c
Tp<void *context, AstFn *f>
```

AstMsg
-----
```c
Tp3<void *context, AstFn *f, char *msg>
```

AstArr
------
```c
Arr<AstTp *tp>
```
