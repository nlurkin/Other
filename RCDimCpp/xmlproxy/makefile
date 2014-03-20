include ../makefile_common

xmlproxySRC = TestNodeProxy
xmlproxyOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(xmlproxySRC)))

all: $(OBJDIR)/TestNodeProxy.o

$(OBJDIR)/%Proxy.o: %Proxy.cpp xmlconfig_%.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)


# Other Targets
clean:

.PHONY: all clean dependents
.SECONDARY:
