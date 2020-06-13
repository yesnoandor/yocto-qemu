TARGET=libzmq-send-test

SRC=${wildcard *.c}
OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) *.o -o $@ -L=/usr/lib -lzmq

clean:
	rm -f $(OBJS) $(TARGET)

