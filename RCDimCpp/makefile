OBJDIR = obj

CppServerDIR = CppServer
CppLibSRC    = NA62DimCommands NA62DimServer
CppLibOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppLibSRC)))
CppServerSRC = ConfigDecoder TestServer mainCppServer
CppServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppServerSRC)))

CServerDIR = CServer
CServerSRC = mainCServer helper commands decoder
CServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CServerSRC)))

TestClientDIR = TestClient
TestClientSRC = mainTestClient TestDimClient
TestClientOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(TestClientSRC)))

LIBS = -lpthread -ldim
LIBSDIR = -L$(DIMDIR)/linux -L.
HDRDIR = -I$(DIMDIR)/dim

#COMPILER
CFLAGS		= -O -Wall -fPIC -g3
SOFLAGS		= -shared
CC			= g++
CCC			= gcc

all: libcppserver.so RCDimCpp testClient RCDimC

# Tool invocations
RCDimCpp: $(CppServerOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS) -lcppserver

$(OBJDIR)/%.o: $(CppServerDIR)/%.cpp $(CppServerDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

libcppserver.so: $(CppLibOBJ)
	$(CC) $(SOFLAGS) -Wl,-soname,$@ -o $@ $^ $(CFLAGS)

$(OBJDIR)/%CppServer.o: $(CppServerDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

RCDimC: $(CServerOBJ)
	$(CCC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: $(CServerDIR)/%.c $(CServerDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

$(OBJDIR)/%CServer.o: $(CServerDIR)/main.c
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)
	
testClient: $(TestClientOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: $(TestClientDIR)/%.cpp $(TestClientDIR)/%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

$(OBJDIR)/%TestClient.o: $(TestClientDIR)/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)


	
# Other Targets
clean:
	rm -f RCDimCpp
	rm -f RCDimC
	rm -f testClient
	rm -rf obj
	rm -rf *.so

.PHONY: all clean dependents
.SECONDARY:
