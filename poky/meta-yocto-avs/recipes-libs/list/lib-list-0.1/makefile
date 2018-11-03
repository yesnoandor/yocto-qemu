LIB = liblist.a

SRC = $(wildcard *.c)
OBJS := $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIB):  $(OBJS)
	$(AR) rcs $@ $<

all: $(LIB)

clean:
	-rm -f $(LIB) *.elf *.o


