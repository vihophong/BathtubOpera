CONFIG += c++11

TARGET = aida
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

SOURCES += \
    LuckyDoll.cpp \
    BuildEvents.cpp \
    Beam.cpp \
    BeamDictionary.cpp \
    CommandLineInterface.cpp \
    AIDA.cpp \
    AIDADictionary.cpp \
    EURICA.cpp \
    EURICADictionary.cpp \
    aida_treeinfo.cpp

HEADERS += \
    BuildEvents.h \
    Beam.h \
    BeamLinkDef.h \
    BeamDictionary.h \
    CommandLineInterface.h \
    AIDA.h \
    AIDAdefs.h \
    AIDALinkDef.h \
    AIDADictionary.h \
    EURICA.h \
    EURICAdefs.h \
    EURICALinkDef.h \
    EURICADictionary.h \
    aida_treeinfo.h



#---------------------ROOT include----------------------------
LIBS += $$system(root-config --cflags --glibs)  -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lpthread -lm -ldl

INCLUDEPATH += "$(ROOTSYS)/include"
DEPENDPATH += "$(ROOTSYS)/include"

#---------------------ANAROOT include----------------------------
LIBS +=-L$(TARTSYS)/lib -lanaeurica -lananadeko -lanacore -lanabrips -lXMLParser
INCLUDEPATH +=$(TARTSYS)/include

#---lib local
LIBS +=-L/home/phong/lib -lEURICA -lBeam -lAIDA

