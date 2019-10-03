CC = gcc
CFLAGS = -lpthread

src = ./source
header = ./header
media  = ./media
c_src = $(wildcard src/*.c)
o_src = $(c_src:.c=.o)
h_head = $(wildcard head/*.h)
Client: mul_c.o
        $(CC) $(CFLAGS) $@ $^

mul_c.o: mul_c.c

Server: $(o_src)
        echo $(o_src)
        $(CC) $(OUTPUT_OPTION) $@ $^

$(o_src): $(h_head)
