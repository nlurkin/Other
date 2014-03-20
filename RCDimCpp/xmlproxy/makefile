include ../makefile_common

xmlproxySRC = TestNodeProxy
xmlproxyOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(xmlproxySRC)))

all: xml $(OBJDIR)/TestNodeProxy.o demo

xml:
	../xml/xmlpp TestNode.h

$(OBJDIR)/%Proxy.o: %Proxy.cpp xmlconfig_%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

demo: demoout.c xmlconfig_TestNode.h $(OBJDIR)/TestNodeProxy.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS) $(HDRDIR)

# Other Targets
clean:
	rm -rf demo
	rm -rf TestNodeProxy.cpp
	rm -rf xmlconfig_TestNode.h
	rm -rf TestNode.hpp
	rm -rf ColdConfig.xml
	rm -rf WarmConfig.xml

.PHONY: all clean dependents
.SECONDARY:
