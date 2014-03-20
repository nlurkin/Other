OBJDIR = obj
ROOTDIR = $(pwd)

xmlproxyDIR = xmlproxy
xmlproxySRC = TestNodeProxy
xmlproxyOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(xmlproxySRC)))

LIBS = -lpthread -ldim -lboost_serialization
LIBSDIR = -L$(DIMDIR)/linux -L$(ROOTDIR)
HDRDIR = -I$(DIMDIR)/dim -I$(ROOTDIR)/xml -I$(ROOTDIR)/xmlproxy

#COMPILER
CFLAGS		= -O -Wall -fPIC -g3
SOFLAGS		= -shared
CC			= g++
CCC			= gcc

all: 
	make -C xmlproxy
	make -C CppServer
	make -C TestClient
	make -C CServer
	
# Other Targets
clean:
	rm -rf obj
	rm -rf receivedfile.xml
	make -C CppServer clean
	make -C TestClient clean
	make -C CServer clean

.PHONY: all clean dependents
.SECONDARY:
