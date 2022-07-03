# Arquivos fonte

## ```llist```
```llist.c``` e ```llist.h``` são a implementação de uma lista encadeada
simples, muito usada para montar a representação de árvore do código após e
durante a análise sintática.

## ```provolone```
```provolone.c``` é o ponto de entrada do compilador, aqui as opções de linha
de comando são parseadas e validadas.

O código é lido então do ```stdin``` pela ```yyparse()```. Caso a árvore
sintática resultante estiver não marcada como ```bootstrapped```, quer dizer
que mais passadas são necessárias. A árvore é então traduzida para provolone
novamente e salvo em um arquivo temporário que então serve como input para
yyparse() novamente. Esse processo se repete até o código estiver marcado como
```bootstrapped```.

Com todas as passadas concluídas, o código é passado para uma função que compila para C.

## ```provolone_compiler```
```provolone_compiler.c``` e ```provolone_compiler.h``` implementam as funções
que transformam árvores abstratas em código fonte, seja C ou provolone (com a
possibilidade de implementar outros targets). Porém, apenas código provolone
"puro", ou totalmente ```bootstrapped``` pode ser compilado para uma linguagem
final como C. Representações intermediárias devem ser re-compiladas para
provolone para serem novamente analisadas.

## ```provolone_program```
```provolone_program.c``` e ```provolone_program.h``` implementam as estruturas
para montar a árvore abstrata do programa provolone. Aqui as estruturas do
programa são representadas por estruturas de dados que juntas montam uma
representação hierárquica do programa. Tal estrutura pode ser apresentada como
uma árvore com a opção ```--tree``` para o compilador.

## ```provolone_lexer.l```
É o arquivo flex que gera o lexer da linguagem. Espaços em branco ignorados, os
tokens finais como PROGRAM e ENTRADA são representados as is. IDs são
representados por strings de números e letras começados por letras.

## ```provolone_parser.y```
É o arquivo bison que gera o parser da linguagem. Estruturas que precisarão de outra passada mudam a flag ```is_bootstrapped``` de p para 0.

O estado do parser, onde a árvore sintática é montada fica no parâmetro p.

