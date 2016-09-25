.EXPORT_ALL_VARIABLES:

.PHONY: clean all

BIN_DIR = $(HOME)/bin
LIB_DIR = $(HOME)/lib
COMMON_DIR = $(HOME)/common/
TARTSYS=/usr/local/anaroot5
EURICASYS=$(HOME)/ribf140/Go4EURICA
SALVADORSYS=$(HOME)/progs/salvador/inc


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINC      := -I$(shell root-config --incdir)

CPP             = g++
CFLAGS		= -Wall -Wno-long-long -g -O3 $(ROOTCFLAGS) -fPIC

INCLUDES        = -I./inc -I$(COMMON_DIR) -I$(TARTSYS)/include -I$(EURICASYS) -I$(SALVADORSYS)
BASELIBS 	= -lm $(ROOTLIBS) $(ROOTGLIBS) -L$(LIB_DIR) -L$(TARTSYS)/lib -lSpectrum -lXMLParser
ALLIBS  	=  $(BASELIBS) -lCommandLineInterface -lanaroot -lananadeko -lanacore -lanabrips -lanaloop -lanadali -lSalvador
LIBS 		= $(ALLIBS)
LFLAGS		= -g -fPIC -shared
CFLAGS += -Wl,--no-as-needed
LFLAGS += -Wl,--no-as-needed
CFLAGS += -Wno-unused-variable -Wno-write-strings

LIB_O_FILES = build/EURICA.o build/EURICADictionary.o 

O_FILES = build/BuildEvents.o

all: MergeEURICA IsomerHistos

MergeEURICA: MergeEURICA.cc $(LIB_DIR)/libSalvador.so $(LIB_DIR)/libEURICA.so $(O_FILES)
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) -lEURICA -lGo4EURICA $(O_FILES) -o $(BIN_DIR)/$@ 

IsomerHistos: IsomerHistos.cc $(LIB_DIR)/libSalvator.so $(LIB_DIR)/libEURICA.so 
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) -lEURICA -lGo4EURICA -o $(BIN_DIR)/$@ 

$(LIB_DIR)/libEURICA.so: $(LIB_O_FILES)
	@echo "Making $@"
	@$(CPP) $(LFLAGS) -o $@ $^ -lc

build/%.o: src/%.cc inc/%.hh
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

build/%Dictionary.o: build/%Dictionary.cc
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -fPIC -c $< -o $@

build/%Dictionary.cc: inc/%.hh inc/%LinkDef.h
	@echo "Building $@"
	@mkdir -p build
	@rootcint -f $@ -c $(INCLUDES) $(ROOTCFLAGS) $(notdir $^)

doc:	doxyconf
	doxygen doxyconf


clean:
	@echo "Cleaning up"
	@rm -rf build doc
	@rm -f inc/*~ src/*~ *~
