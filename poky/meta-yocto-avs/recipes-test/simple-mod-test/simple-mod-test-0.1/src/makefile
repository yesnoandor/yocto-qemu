TARGET=simple-mod-test

SRC=${wildcard *.c}
OBJS=${patsubst %.c, %.o, $(SRC)}

# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC)  *.o -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
