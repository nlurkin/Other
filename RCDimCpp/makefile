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

all: xmlproxy libcppserver.so RCDimCpp testClient RCDimC

# Tool invocations
RCDimCpp:
	make -C CppServer RCDimCpp

libcppserver.so:
	make -C CppServer libcppserver.so
	
xmlproxy:
	make -C xmlproxy

testClient:
	make -C TestClient

RCDimC:
	make -C CServer
	
# Other Targets
clean:
	rm -rf obj
	make -C CppServer clean

.PHONY: all clean dependents
.SECONDARY:
