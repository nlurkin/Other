ROOTCONFIG   := $(ROOTSYS)/bin/root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags) -DUSE_ROOT
ROOTINCDIR   := $(shell $(ROOTCONFIG) --incdir)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs) 

CC=g++ -g
CFLAGS= -W -Wall -I$(ROOTINCDIR)
CXXFLAGS= -O -Wall -fPIC $(ROOTCFLAGS) $(CPPFLAGS)
LDFLAGS= $(ROOTLIBS) $(LIBS)
EXEC=check_complete


SRC= check_complete.c 

all:	$(EXEC)

#Compilation du programme
check_complete: $(SRC)
		$(CC) -o $@ $(SRC) $(CXXFLAGS) $(LDFLAGS)

#Clean
.PHONY: clean cleanall

cleanall:	clean

clean: 
		rm -rf *.o
		rm -rf $(EXEC)
