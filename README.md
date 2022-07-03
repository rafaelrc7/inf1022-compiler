# INF1022 Final Assigment
Bootstrapping compiler for a simple defined language.

## Bulding
Instructions at [INSTALL](/INSTALL.md).

## Provol-One
### Mudanças da linguagem base
- Eu inclui uma instrução ``DEC(X)`` que decrementa uma variável
- Eu adicionei a opção de definir o nome do módulo, opcionalmente escrevendo um
  ``ID`` ao lado de ``PROGRAM``

### Adições de bootstrapping
- IF
- IF/ELSE
- REPITA n VEZES cmds FIM

#### Assigment de expressões simples
- ID = ID + ID
- ID = ID - ID
- ID = ID * ID

### Exemplos
Eu fiz alguns exemplos que estão na pasta ``samples/`` a partir da raíz do projeto.

Nesta pasta existem pares de código em provol e funções main em C que podem ser compilados juntos.

Ex.
- ``samples/fib.provol``
- ``samples/fib.c``

Para compilar, assumindo que o compilador já esteja compilado:
```sh
cd samples
../build/provolone -o fibf.c < ./fib.provol
gcc -o fib.c fibf.c
```

#### fib
Rodando o código passando um número como argumento, o programa retorna o
enésimo número da sequência de fibonacci.
```sh
$ ./fib 0
0nth fib: 0
$ ./fib 1
1nth fib: 1
$ ./fib 2
2nth fib: 1
$ ./fib 3
3nth fib: 2
$ ./fib 4
4nth fib: 3
$ ./fib 5
5nth fib: 5
```

#### pow2
Rodando o código passando um número como argumento, o programa retorna 2^n.
```sh
$ ./pow2 0
2^0: 1
$ ./pow2 1
2^1: 2
$ ./pow2 2
2^2: 4
$ ./pow2 3
2^3: 8
$ ./pow2 4
2^4: 16
$ ./pow2 5
2^5: 32
$ ./pow2 6
2^6: 64
$ ./pow2 7
2^7: 128
$ ./pow2 8
2^8: 256
```

#### is_even
Rodando o código passando um número como argumento, o programa retorna se é par
ou não.
```sh
$ ./iseven 0
is 0 even: true
$ ./iseven 1
is 1 even: false
$ ./iseven 2
is 2 even: true
$ ./iseven 3
is 3 even: false
$ ./iseven 55
is 55 even: false
$ ./iseven 56
is 56 even: true
$ ./iseven 100
is 100 even: true
$ ./iseven 101
is 101 even: false
```

## License
Licensed under the [MIT License](/COPYING).

