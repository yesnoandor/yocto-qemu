#${CC} helloYocto.o -o helloYocto
#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

TARGET=helloYocto

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}



# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

all: $(TARGET)

$(TARGET): $(CUR_OBJS)
	$(CC) $(LDFLAGS) *.o -o $@  

#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(OBJS) $(TARGET)
