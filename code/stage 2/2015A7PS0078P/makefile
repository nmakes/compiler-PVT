#all:
#	gcc -g -o stage1exe lexer.c parser.c driver.c

driver: parser.c lexer.c driver.c
	$(CC) -g $(CFLAGS) parser.c lexer.c driver.c $(LDFLAGS) -o stage1exe
