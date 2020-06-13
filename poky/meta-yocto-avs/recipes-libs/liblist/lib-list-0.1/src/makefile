LIB = liblist.a

SRC = $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRC))

%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIB):  $(OBJS)
	$(AR) rcs $@ $<

all: $(LIB)

clean:
	rm -f $(LIB) $(OBJS)


