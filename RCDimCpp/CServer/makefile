include ../makefile_common

CServerSRC = mainCServer helper commands decoder
CServerOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(CServerSRC)))


all: RCDimC

RCDimC: ../RCDimC

../RCDimC: $(CServerOBJ)
	$(CCC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: %.c %.h
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

$(OBJDIR)/%CServer.o: main.c
	@mkdir -p $(OBJDIR)
	$(CCC) -o $@ -c $< $(CFLAGS) $(HDRDIR)
	
# Other Targets
clean:
	rm -f ../RCDimC

.PHONY: all clean dependents
.SECONDARY:
