# gcc -g -c lexer.c -o lexer.o
# gcc -g -c parser.c -o parser.o
# gcc -g -c driver.c -o driver.o
# gcc -o ncc -g lexer.o parser.o driver.o
# gdb ./ncc
gcc -g -o ncc lexer.c parser.c driver.c