CXX=`root-config --cxx`
CFLAGS=-c -g -Wall `root-config --cflags` -I./src -I ./include
LDLIBS=`root-config --glibs`
LDFLAGS=`root-config --ldflags`
#SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc
SOURCES=$(shell ls ./src/*.cc)
TEMP=$(shell ls ./src/*.cc~)
TEMP2=$(shell ls ./include/*.hh~)
OBJECTS=$(SOURCES:.cc=.o) 
MAIN=EvtBuilder_sam.C
MAINO=./src/EvtBuilder_sam.o

EXECUTABLE= EvtBuilder

INCLUDEPATH=include
SRCPATH=src


.PHONY: clean get put all test sclean

all: $(EXECUTABLE) 

$(EXECUTABLE) : $(OBJECTS) $(MAINO)
	@echo "Building target" $@ "..." 
	@$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(MAINO) $(LDLIBS)
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

sclean:
	-rm ./$(TEMP)
	-rm ./$(TEMP2)
	-rm ./$(OBJECTS)
	-rm ./$(EXECUTABLE)
	-rm ./$(MAINO)

