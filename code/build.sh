gcc -g -c lexer.c -o lexer.o
gcc -g -c driver.c -o driver.o
gcc -o ncc -g lexer.o driver.o
# gdb ./ncc
