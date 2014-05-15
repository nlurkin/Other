include ../makefile_common

TestClientSRC = mainTestClient TestDimClient
TestClientOBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(TestClientSRC)))

all: testClient

testClient:../testClient

../testClient: $(TestClientOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBSDIR) $(LIBS)

$(OBJDIR)/%.o: %.cpp %.h
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)

$(OBJDIR)/%TestClient.o: main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(HDRDIR)


	
# Other Targets
clean:
	rm -f ../testClient

.PHONY: all clean dependents
.SECONDARY:

