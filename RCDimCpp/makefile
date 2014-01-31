OBJDIR = obj

CppServerDIR = CppServer
CppServerSRC = mainCppServer ConfigDecoder NA62DimCommands NA62DimServer
CppServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppServerSRC)))

CServerDIR = CServer
CServerSRC = mainCServer helper commands decoder
CServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CServerSRC)))

TestClientDIR = TestClient
TestClientSRC = mainTestClient TestDimClient
TestClientOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(TestClientSRC)))

LIBS = -lpthread -ldim
LIBSDIR = /home/ncl/Software/Install/dim_v20r7/linux
HDRDIR = /home/ncl/Software/Install/dim_v20r7/dim

#COMPILER
CFLAGS		= -O -Wall -fPIC -g3
SOFLAGS		= -shared
CC			= g++
CCC			= gcc

all: RCDimCpp testClient RCDimC

# Tool invocations
RCDimCpp: $(CppServerOBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: $(CppServerDIR)/%.cpp $(CppServerDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)

$(OBJDIR)/%CppServer.o: $(CppServerDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)

RCDimC: $(CServerOBJ)
	$(CCC) -o $@ $^ $(CFLAGS) -L$(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: $(CServerDIR)/%.c $(CServerDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)

$(OBJDIR)/%CServer.o: $(CServerDIR)/main.c
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)
	
testClient: $(TestClientOBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: $(TestClientDIR)/%.cpp $(TestClientDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)

$(OBJDIR)/%TestClient.o: $(TestClientDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HDRDIR)


	
# Other Targets
clean:
	rm -f RCDimCpp
	rm -f RCDimC
	rm -f testClient
	rm -rf obj

.PHONY: all clean dependents
.SECONDARY:
