#${CC} helloYocto.o -o helloYocto
#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

TARGET=libdebugTest

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

#CFLAGS=-D_REENTRANT  -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/alsa-lib
#CFLAGS1=-D_REENTRANT  -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/libmad
#LDFLAGS= -llist
#-L=/usr/lib

# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

all: $(TARGET)

$(TARGET): $(CUR_OBJS)
	$(CC)  *.o -o $@ -L=/usr/lib -ldebug

#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(OBJS) $(TARGET)
