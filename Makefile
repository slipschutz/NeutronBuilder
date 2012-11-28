CXX=`root-config --cxx`
CFLAGS=-c -g -Wall `root-config --cflags` -I./src -I ./include
LDLIBS=`root-config --glibs`
LDFLAGS=`root-config --ldflags`
SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc
OBJECTS=$(SOURCES:.cc=.o) 
MAIN=EvtBuilder_sam.C
MAINO=./src/EvtBuilder_sam.o

EXECUTABLE= EvtBuilder

INCLUDEPATH=include
SRCPATH=src


.PHONY: clean get put all

all: $(EXECUTABLE) 

$(EXECUTABLE) : $(OBJECTS) $(MAINO)
	@echo "Building target" $@ "..." 
	@$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJECTS) $(MAINO)
	@echo
	@echo "Build succeed"


.cc.o:
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $< -o $@ 

$(MAINO): $(MAIN)
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $< -o $@  


get:	
	@echo "In Get"
	@mv ./$(INCLUDEPATH)/*.h .
	@mv ./$(SRCPATH)/*.cc .
	-@mv ./obj/*.o .

put:
	@echo "In put"
	-@mv ./*.h ./$(INCLUDEPATH)/
	-@mv ./*.cc ./$(SRCPATH)/
	-@mv ./*.o ./obj/
	@mv ./$(SRCPATH)/EvtBuilder_sam.cc .

clean:
	-rm ./$(OBJECTS)
	-rm ./$(EXECUTABLE)
	-rm ./$(MAINO)

test:
	@echo $(OBJECTS)
