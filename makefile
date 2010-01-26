COMPILERPATH = C:/Program Files/Dev-Cpp/

BUILD     = build
SRC       = src

WXLIBNAME = wxmsw28
CPP       = g++.exe
LINK      = g++.exe
WINDRES   = windres.exe
BIN       = ChaosConnect.exe

OBJ       = $(BUILD)/ChaosConnectApp.o \
            $(BUILD)/ChaosConnectFrm.o \
            $(BUILD)/ChaosPanel.o \
            $(BUILD)/ChaosPlot.o \
            $(BUILD)/BifurcationPlot.o \
            $(BUILD)/XYPlot.o \
            $(BUILD)/XTPlot.o \
            $(BUILD)/FFTPlot.o \
            $(BUILD)/Return1Plot.o \
            $(BUILD)/Return2Plot.o \
            $(BUILD)/Rotating3dPlot.o \
            $(BUILD)/SampleToFileDlg.o \
            $(BUILD)/AboutDlg.o \
            $(BUILD)/SettingsDlg.o \
            $(BUILD)/ChaosSettings.o \
            $(BUILD)/Game.o \
            $(BUILD)/GameBoard.o \
            $(BUILD)/GameShape.o \
            $(BUILD)/ChaosConnectResource.o
            
LIBS      = -L"$(COMPILERPATH)lib" \
            -mwindows \
            -l$(WXLIBNAME) \
            -lwxpng \
            -lwxtiff \
            -lwxjpeg \
            -lwxzlib \
            -lkernel32 \
            -luser32 \
            -lgdi32 \
            -lcomdlg32 \
            -lwinmm \
            -lshell32 \
            -lcomctl32 \
            -lole32 \
            -loleaut32 \
            -luuid \
            -lrpcrt4 \
            -ladvapi32 \
            -lodbc32 \
            lib/libchaos.a \
            lib/libusb.a

CXXINCS   = -I"$(COMPILERPATH)include" \
            -I"$(COMPILERPATH)" \
            -I"$(COMPILERPATH)include/common/wx/msw" \
            -I"$(COMPILERPATH)include/common/wx/generic" \
            -I"$(COMPILERPATH)include/common/wx/html" \
            -I"$(COMPILERPATH)include/common/wx/xml" \
            -I"$(COMPILERPATH)include/common/wx/xrc" \
            -I"$(COMPILERPATH)include/common/wx" \
            -I"$(COMPILERPATH)include/common"

CXXFLAGS  = $(CXXINCS) \
            -ffunction-sections \
            -fdata-sections \
            -fno-exceptions \
            -fno-pcc-struct-return \
            -fstrict-aliasing \
            -Wall \
            -D__WXMSW__ \
            -D__GNUWIN32__ \
            -D__WIN95__ \
            -O2 \
            -s
LDFLAGS   = -s -Wl,--gc-sections -Os
RM        = rm -f

.PHONY: all all-before all-after clean clean-custom
all: all-before $(BIN) all-after

clean: clean-custom
	$(RM) $(OBJ) "$(BIN)"

all-before:
	test -d $(BUILD) || mkdir $(BUILD)

$(BIN): $(OBJ) $(SRC)/libchaos.h
	$(LINK) $(OBJ) -o "$(BIN)" $(LIBS) $(LDFLAGS)

$(BUILD)/ChaosConnectApp.o: $(SRC)/ChaosConnectApp.cpp $(SRC)/ChaosConnectApp.h $(SRC)/ChaosConnectFrm.h $(SRC)/ChaosPanel.h $(SRC)/ChaosPlot.h $(SRC)/BifurcationPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XTPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XYPlot.h $(SRC)/ChaosPlot.h  $(SRC)/Return1Plot.h $(SRC)/ChaosPlot.h  $(SRC)/Return2Plot.h $(SRC)/ChaosPlot.h  $(SRC)/FFTPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/ChaosConnectApp.cpp -o $(BUILD)/ChaosConnectApp.o $(CXXFLAGS)

$(BUILD)/ChaosConnectResource.o: $(SRC)/ChaosConnectResource.rc icons/main.ico
	$(WINDRES) $(SRC)/ChaosConnectResource.rc $(BUILD)/ChaosConnectResource.o

$(BUILD)/ChaosConnectFrm.o: $(SRC)/ChaosConnectFrm.cpp $(SRC)/ChaosConnectFrm.h $(SRC)/ChaosPanel.h $(SRC)/ChaosPlot.h $(SRC)/BifurcationPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XTPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XYPlot.h $(SRC)/ChaosPlot.h  $(SRC)/Return1Plot.h $(SRC)/ChaosPlot.h  $(SRC)/Return2Plot.h $(SRC)/ChaosPlot.h  $(SRC)/FFTPlot.h $(SRC)/ChaosPlot.h  
	$(CPP) -c $(SRC)/ChaosConnectFrm.cpp -o $(BUILD)/ChaosConnectFrm.o $(CXXFLAGS)

$(BUILD)/ChaosPanel.o: $(SRC)/ChaosPanel.cpp $(SRC)/ChaosPanel.h $(SRC)/ChaosPlot.h $(SRC)/BifurcationPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XTPlot.h $(SRC)/ChaosPlot.h  $(SRC)/XYPlot.h $(SRC)/ChaosPlot.h  $(SRC)/Return1Plot.h $(SRC)/ChaosPlot.h  $(SRC)/Return2Plot.h $(SRC)/ChaosPlot.h  $(SRC)/FFTPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/ChaosPanel.cpp -o $(BUILD)/ChaosPanel.o $(CXXFLAGS)

$(BUILD)/ChaosPlot.o: $(SRC)/ChaosPlot.cpp $(SRC)/ChaosPlot.h
	$(CPP) -c $(SRC)/ChaosPlot.cpp -o $(BUILD)/ChaosPlot.o $(CXXFLAGS)

$(BUILD)/BifurcationPlot.o: $(SRC)/BifurcationPlot.cpp $(SRC)/BifurcationPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/BifurcationPlot.cpp -o $(BUILD)/BifurcationPlot.o $(CXXFLAGS)

$(BUILD)/XYPlot.o: $(SRC)/XYPlot.cpp $(SRC)/XYPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/XYPlot.cpp -o $(BUILD)/XYPlot.o $(CXXFLAGS)

$(BUILD)/XTPlot.o: $(SRC)/XTPlot.cpp $(SRC)/XTPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/XTPlot.cpp -o $(BUILD)/XTPlot.o $(CXXFLAGS)

$(BUILD)/FFTPlot.o: $(SRC)/FFTPlot.cpp $(SRC)/FFTPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/FFTPlot.cpp -o $(BUILD)/FFTPlot.o $(CXXFLAGS)

$(BUILD)/Return1Plot.o: $(SRC)/Return1Plot.cpp $(SRC)/Return1Plot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/Return1Plot.cpp -o $(BUILD)/Return1Plot.o $(CXXFLAGS)

$(BUILD)/Return2Plot.o: $(SRC)/Return2Plot.cpp $(SRC)/Return2Plot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/Return2Plot.cpp -o $(BUILD)/Return2Plot.o $(CXXFLAGS)

$(BUILD)/Rotating3dPlot.o: $(SRC)/Rotating3dPlot.cpp $(SRC)/Rotating3dPlot.h $(SRC)/ChaosPlot.h 
	$(CPP) -c $(SRC)/Rotating3dPlot.cpp -o $(BUILD)/Rotating3dPlot.o $(CXXFLAGS)

$(BUILD)/SampleToFileDlg.o: $(SRC)/SampleToFileDlg.cpp $(SRC)/SampleToFileDlg.h
	$(CPP) -c $(SRC)/SampleToFileDlg.cpp -o $(BUILD)/SampleToFileDlg.o $(CXXFLAGS)

$(BUILD)/SettingsDlg.o: $(SRC)/SettingsDlg.cpp $(SRC)/SettingsDlg.h
	$(CPP) -c $(SRC)/SettingsDlg.cpp -o $(BUILD)/SettingsDlg.o $(CXXFLAGS)

$(BUILD)/AboutDlg.o: $(SRC)/AboutDlg.cpp $(SRC)/AboutDlg.h
	$(CPP) -c $(SRC)/AboutDlg.cpp -o $(BUILD)/AboutDlg.o $(CXXFLAGS)

$(BUILD)/ChaosSettings.o: $(SRC)/ChaosSettings.cpp $(SRC)/ChaosSettings.h
	$(CPP) -c $(SRC)/ChaosSettings.cpp -o $(BUILD)/ChaosSettings.o $(CXXFLAGS)

$(BUILD)/Game.o: $(SRC)/Game.cpp $(SRC)/Game.h
	$(CPP) -c $(SRC)/Game.cpp -o $(BUILD)/Game.o $(CXXFLAGS)

$(BUILD)/GameBoard.o: $(SRC)/GameBoard.cpp $(SRC)/GameBoard.h
	$(CPP) -c $(SRC)/GameBoard.cpp -o $(BUILD)/GameBoard.o $(CXXFLAGS)

$(BUILD)/GameShape.o: $(SRC)/GameShape.cpp $(SRC)/GameShape.h
	$(CPP) -c $(SRC)/GameShape.cpp -o $(BUILD)/GameShape.o $(CXXFLAGS)
