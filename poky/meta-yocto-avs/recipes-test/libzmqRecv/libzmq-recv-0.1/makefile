#${CC} helloYocto.o -o helloYocto
#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

TARGET=zeromqRecv

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

#CFLAGS=-D_REENTRANT  -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/alsa-lib
#CFLAGS1=-D_REENTRANT  -I$(PKG_CONFIG_SYSROOT_DIR)/usr/include/libmad
#LDFLAGS= -libxml

# make all .c or .cpp 
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	$(CC) $(CFLAGS) -I=/usr/include/libxml2 -c $< -o $@

#%.o: %.cpp
#	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
#	@$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

all: $(TARGET)

$(TARGET): $(CUR_OBJS)
	$(CC) $(LDFLAGS) *.o -o $@ -L=/usr/lib -lzmq

#helloYocto: helloYocto.o
#	${CC} ${LDFLAGS} $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(OBJS) $(TARGET)
