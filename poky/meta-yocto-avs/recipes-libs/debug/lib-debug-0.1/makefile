

LIB = libdebug.so

SRC = $(wildcard *.c)
OBJS := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -fPIC $< -o $@

$(LIB):  $(OBJS)
	$(CC) -fpic -shared $< -o $@ $(LDFLAGS)

all: $(LIB)

clean:
	-rm -f $(LIB) *.elf *.o


