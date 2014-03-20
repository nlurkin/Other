include ../makefile_common

CppLibSRC    = NA62DimCommands NA62DimServer
CppLibOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppLibSRC)))
CppServerSRC = ConfigDecoder TestServer mainCppServer
CppServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CppServerSRC)))

all: libcppserver.so RCDimCpp

RCDimCpp: ../RCDimCpp
libcppserver.so: ../libcppserver.so

# Tool invocations
../RCDimCpp: $(CppServerOBJ) $(xmlproxyDIR)/xmlconfig_TestNode.h $(xmlproxyOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS) -lcppserver 

$(OBJDIR)/%.o: %.cpp %.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)
	
$(OBJDIR)/%Proxy.o: $(xmlproxyDIR)/%Proxy.cpp $(xmlproxyDIR)/xmlconfig_%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)


../libcppserver.so: $(CppLibOBJ)
	$(CC) $(SOFLAGS) -Wl,-soname,libcppserver.so -o $@ $^ $(CFLAGS)

$(OBJDIR)/%CppServer.o: main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)


# Other Targets
clean:
	rm -f ../RCDimCpp
	rm -rf ../*.so

.PHONY: all clean dependents
.SECONDARY:
