TARGET=libmdp.a

HEARDER=$(wildcard *.hxx)
SOURCE=$(wildcard *.cxx)
OBJ=$(patsubst %.cxx,%.o,$(SOURCE))
OBJS+=$(OBJ)

INCL_PATH+=./
INCL_PATH+=/home/yixiong/xsde_complie/xsde-3.2.0-1+dep/xsde/libxsde/xsde
INCL_PATH+=/home/yixiong/xsde_complie/xsde-3.2.0-1+dep/xsde/libxsde
INCL_PATH_FLAG=$(patsubst %,-I%,$(INCL_PATH))

LIB_PATH+=/home/yixiong/xsde_complie/xsde-3.2.0-1+dep/xsde/libxsde/xsde
LIB_PATH+=/home/yixiong/xsde_complie/xsde-3.2.0-1+dep/xsde/libxsde
LIB_PATH+=/usr/lib64/
LIB_PATH+=/usr/local/lib
LIB_PATH_FLAG=$(patsubst %,-L%,$(LIB_PATH))

LIB_STATIC+=xsde
LIB_STATIC_FLAG=$(patsubst %,-l%,$(LIB_STATIC))

CFLAGS+= -g -Wall $(INCL_PATH_FLAG) $(LIB_PATH_FLAG)
CPPFLAGS+= -g -Wall $(INCL_PATH_FLAG) $(LIB_PATH_FLAG)
CC=g++

all:$(TARGET)

$(TARGET):$(OBJS)
	ar rv $(TARGET) $(OBJS)

$(OBJS):$(SOURCE) $(HEARDER)
	$(CC) $(CPPFLAGS) -c $(SOURCE)

rebuild:clean all

clean:
	rm -f *.o
	rm -f $(TARGET)
