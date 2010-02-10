/**
 * \file ChaosConnectFrm.cpp
 * \brief Main Windows Class
 */
 
#include <usb.h>
#include "ChaosSettings.h"
#include "ChaosConnectFrm.h"
#include "libchaos.h"
#include "Game.h"

#define BORDER_SIZE 5
#define TOP_BORDER_SIZE 30
#define SIDE_GUTTER_SIZE 15

BEGIN_EVENT_TABLE(ChaosConnectFrm,wxFrame)
    EVT_CLOSE(ChaosConnectFrm::OnClose)
    EVT_TIMER(ID_TIMER1,ChaosConnectFrm::timer1Timer)
    EVT_MENU(ID_MNU_FILE_EXIT, ChaosConnectFrm::mnuExit)
    EVT_MENU(ID_MNU_SHOW_LOG, ChaosConnectFrm::mnuShowLog)
    EVT_MENU(ID_MNU_SAMPLE_TO_FILE, ChaosConnectFrm::mnuSampleToFile)
    EVT_MENU(ID_MNU_SETTINGS, ChaosConnectFrm::mnuSettings)
    EVT_MENU(ID_MNU_VIEW_FULLSCREEN, ChaosConnectFrm::mnuFullscreen)
    EVT_MENU(ID_MNU_VIEW_SPLITSCREEN, ChaosConnectFrm::mnuSplitScreen)
    EVT_MENU(ID_MNU_VIEW_QUADVIEW, ChaosConnectFrm::mnuQuadScreen)    
    EVT_MENU(ID_MNU_ABOUT, ChaosConnectFrm::mnuAbout)    
    EVT_BUTTON(ID_START_STOP_BTN, ChaosConnectFrm::OnStartStopBtn)
    EVT_BUTTON(ID_SETTINGS_APPLY_BTN, ChaosConnectFrm::OnSettingsApplyBtn)
END_EVENT_TABLE()

ChaosConnectFrm::ChaosConnectFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style) {
    /**
    *   Constructor for the Main form. Creates the GUI and initializes the
    *   chaos library.
    */
    // Set up logger
    logger = new wxLogWindow(this, wxT("Chaos Connect log"), false, false);
    wxLog::SetActiveTarget(logger);
    wxLogMessage(wxT("Init ChaosConnect log"));
    
    // Create GUI
    ChaosSettings::initSettings();
    CreateGUIControls();
    
    // Set up libchaos
    libchaos_init();
    libchaos_setPeaksPerMDAC(ChaosSettings::PeaksPerMdac);
    
    wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);
    
    stepsSpinner->SetValue(ChaosSettings::BifStepsPerWindow);
    peaksSpinner->SetValue(ChaosSettings::PeaksPerMdac);
}

ChaosConnectFrm::~ChaosConnectFrm() {
    /** 
    *   Destructor for the Main form.
    */
    timer1->Stop();
}

void ChaosConnectFrm::CreateGUIControls() {
    /**
    *   Creates all of the GUI controls on the main form.
    *
    *   GUI layout consists of a menu bar and status bar on the main form
    *   A wxFlexGrid sizer with one column is created to hold the controls
    *   Four plots are then layed out in another wxFlexGridSizer so that 
    *   in the first row of the overall sizer.  A horizontal wxBoxSizer is
    *   then created for the bottom row so that we can add a stretchable
    *   panel down there for additional controls.
    */
    
    // Create sizer
    overallSizer = new wxFlexGridSizer(1);
    flexSizer = new wxFlexGridSizer(2, 2, 0, 0);
    bottomRowSizer = new wxBoxSizer(wxHORIZONTAL);
    panel5Sizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Overall Sizer
    overallSizer->Add(flexSizer, 0, wxEXPAND | wxALL);
    overallSizer->AddGrowableRow(0);
    overallSizer->AddGrowableCol(0);
    
    // Make the Flex Sizer grow
    flexSizer->AddGrowableRow(0);
    flexSizer->AddGrowableRow(1);
    flexSizer->AddGrowableCol(0);
    flexSizer->AddGrowableCol(1);
    
    // Create 4 ChaosPanel displays
    display1 = new ChaosPanel(this, ID_DISPLAY1, wxPoint(0, 0), wxSize(200, 100), wxTAB_TRAVERSAL, wxT("display1"), ChaosPanel::CHAOS_XT);
    flexSizer->Add(display1,0,wxEXPAND | wxALL,0);

    display2 = new ChaosPanel(this, ID_DISPLAY2, wxPoint(200, 0), wxSize(200, 100), wxTAB_TRAVERSAL, wxT("display2"), ChaosPanel::CHAOS_XY);
    flexSizer->Add(display2,0,wxEXPAND | wxALL,0);

    display3 = new ChaosPanel(this, ID_DISPLAY3, wxPoint(0, 100), wxSize(200, 100), wxTAB_TRAVERSAL, wxT("display3"), ChaosPanel::CHAOS_BIFURCATION);
    flexSizer->Add(display3,0,wxEXPAND | wxALL,0);

    display4 = new ChaosPanel(this, ID_DISPLAY4, wxPoint(200, 100), wxSize(200, 100), wxTAB_TRAVERSAL, wxT("display4"), ChaosPanel::CHAOS_RETURN1);
    flexSizer->Add(display4,0,wxEXPAND | wxALL,0);

    // Sizer for the controls along the bottom
    display5 = new wxPanel(this, ID_DISPLAY5, wxPoint(200, 100), wxSize(200, 100), wxTAB_TRAVERSAL, wxT("display5"));
    overallSizer->Add(panel5Sizer, 0, wxEXPAND | wxALL);
    panel5Sizer->Add(display5, 1, wxEXPAND | wxALL);
    display5->SetSizer(bottomRowSizer);
    
    // Create pause button
    startStopButton = new wxButton(display5, ID_START_STOP_BTN, wxT("Pause"));
    
    // Settings to adjust how many bifurcation steps are taken
    stepsLabel = new wxStaticText(display5, wxID_ANY, wxT("Bifurcation Steps: "));
    stepsSpinner = new wxSpinCtrl(display5, ID_STEPS_SPINNER, wxT("100"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 4095, 100);
    
    // Settings to adjust how many peaks are taken at each step
    peaksLabel = new wxStaticText(display5, wxID_ANY, wxT("Bifurcation Peaks: "));
    peaksSpinner = new wxSpinCtrl(display5, ID_PEAKS_SPINNER, wxT("10"), wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 100, 10);
    
    // Buttons
    settingsApplyButton = new wxButton(display5, ID_SETTINGS_APPLY_BTN, wxT("Apply"));
    
    bottomRowSizer->Add(stepsLabel, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    bottomRowSizer->Add(stepsSpinner, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    bottomRowSizer->Add(peaksLabel, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    bottomRowSizer->Add(peaksSpinner, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    bottomRowSizer->Add(settingsApplyButton, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    bottomRowSizer->AddStretchSpacer(1);
    bottomRowSizer->Add(startStopButton, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    // Create status bar
    /* 
      ------------------------------------------------------
      | Connected: ???   | MDAC VALUE: ????                 |
      ------------------------------------------------------ 
    */
    // Create fields
    statusBar = new wxStatusBar(this, ID_STATUSBAR);
    statusBar->SetFieldsCount(3);
    statusBar->SetStatusText(wxT("Connected: ?"),0);
    statusBar->SetStatusText(wxT("MDAC Value: ????"),1);
    statusBar->SetStatusText(wxT("Resistance: ????"),2);
    
    // Set field sizes
    int statusBar_Widths[3];
    statusBar_Widths[0] = 80;
    statusBar_Widths[1] = 100;
    statusBar_Widths[2] = -1; // Auto scale to the rest of the size
    statusBar->SetStatusWidths(3,statusBar_Widths);
    
    // Set status bar
    SetStatusBar(statusBar);
    
    // Create menu bar
    menuBar = new wxMenuBar();
    
    // File menu
    wxMenu *fileMenu = new wxMenu(0);
    fileMenu->Append(ID_MNU_SAMPLE_TO_FILE, wxT("Sample To File..."), wxT(""), wxITEM_NORMAL);
    fileMenu->Append(ID_MNU_SETTINGS, wxT("Settings..."), wxT(""), wxITEM_NORMAL);
    fileMenu->Append(ID_MNU_SHOW_LOG, wxT("Show Log"), wxT(""), wxITEM_NORMAL);
    fileMenu->Append(ID_MNU_FILE_EXIT, wxT("Exit"), wxT(""), wxITEM_NORMAL);
    menuBar->Append(fileMenu, wxT("File"));
    
    // View menu
    wxMenu *viewMenu = new wxMenu(0);
    viewMenu->Append(ID_MNU_VIEW_FULLSCREEN, wxT("Fullscreen"), wxT(""), wxITEM_CHECK);
    viewMenu->Append(ID_MNU_VIEW_SPLITSCREEN, wxT("Split Screen"), wxT(""), wxITEM_CHECK);
    viewMenu->Append(ID_MNU_VIEW_QUADVIEW, wxT("Quad View"), wxT(""), wxITEM_CHECK);
    menuBar->Append(viewMenu, wxT("View"));
    menuBar->Check(ID_MNU_VIEW_FULLSCREEN, true);

    // Help menu
    wxMenu *helpMenu = new wxMenu(0);
    helpMenu->Append(ID_MNU_ABOUT, wxT("About"), wxT(""));
    menuBar->Append(helpMenu, wxT("Help"));

    SetMenuBar(menuBar);

    // Create timer
    timer1 = new wxTimer();
    timer1->SetOwner(this, ID_TIMER1);
    timer1->Start(ChaosSettings::UpdatePeriod);

    // Set window properties and title bar
    SetTitle(wxT("ChaosConnect 1.0"));
    SetIcon(wxNullIcon);
    SetMinSize(wxSize(600,400));
    
    // Set up the sizer
    this->SetSizer(overallSizer);
    this->SetAutoLayout(true);
    
    // Set fullscreen mode
    flexSizer->Show(display2, false, true);
    flexSizer->Show(display3, false, true);
    flexSizer->Show(display4, false, true);
    display1->Show();
    flexSizer->Fit(this); // Adjust window size to min
    flexSizer->Layout(); // Layout items in the sizer
    Center();

}

void ChaosConnectFrm::OnClose(wxCloseEvent& event) {
    /**
    *   Event handler for the form closing event
    *   Exit the ChaosConnect Program
    */
    timer1->Stop();
    Destroy();
}

void ChaosConnectFrm::mnuShowLog(wxCommandEvent& event) {
    /**
    *   Shows the log window
    */
    logger->Show();
}

void ChaosConnectFrm::mnuAbout(wxCommandEvent& event) {
    /**
    *   Shows the about dialog which displays version information
    */
    AboutDlg* aboutFrame = new AboutDlg(this);
    aboutFrame->Show();
}

void ChaosConnectFrm::mnuSampleToFile(wxCommandEvent& event) {
    /**
    *   Shows the SampleToFile Dialog which gives the user the ability to
    *   sweep through the MDAC values and save the data to a file.
    */
    SampleToFileDlg* frame = new SampleToFileDlg(NULL);
    timer1->Stop();
    frame->ShowModal();
    timer1->Start();
}

void ChaosConnectFrm::mnuSettings(wxCommandEvent& event) {
    /**
    *   Shows the settings window which allows the user to change various
    *   UI settings in the program.
    */
    SettingsDlg* settingsFrame = new SettingsDlg(this);
    settingsFrame->Show();
}

void ChaosConnectFrm::mnuExit(wxCommandEvent& event) {
    /**
    *   Exit the ChaosConnect Program
    */ 
    timer1->Stop();
    Destroy();
}

void ChaosConnectFrm::mnuFullscreen(wxCommandEvent& event) { 
    /**
    *   Sets up the view so that one ChaosPanel is displayed fullscreen
    */
    // enable/disable painting on the panels
    display1->Show();
    display2->Hide(); 
    display3->Hide();
    display4->Hide();
    
    // Show the panels in the sizer
    flexSizer->Show(display2, false, true);
    flexSizer->Show(display3, false, true);
    flexSizer->Show(display4, false, true);
    flexSizer->Layout();
    
    menuBar->Check(ID_MNU_VIEW_FULLSCREEN, true);
    menuBar->Check(ID_MNU_VIEW_SPLITSCREEN, false);
    menuBar->Check(ID_MNU_VIEW_QUADVIEW, false);
}

void ChaosConnectFrm::mnuSplitScreen(wxCommandEvent& event) {
    /**
    *   Sets up the view so that two ChaosPanels are displayed in a horizontal split
    */
    // enable/disable painting on the panels
    display1->Show();
    display2->Hide();
    display3->Show();
    display4->Hide();
    
    // Show the panels in the sizer
    flexSizer->Show(display2, false, true);
    flexSizer->Show(display3, true, true);
    flexSizer->Show(display4, false, true);
    flexSizer->Layout();
    
    menuBar->Check(ID_MNU_VIEW_FULLSCREEN, false);
    menuBar->Check(ID_MNU_VIEW_SPLITSCREEN, true);
    menuBar->Check(ID_MNU_VIEW_QUADVIEW, false);
}

void ChaosConnectFrm::mnuQuadScreen(wxCommandEvent& event) {
    /**
    *   Sets up the view so that four ChaosPanels are displayed, one in each quadrant of the window.
    */
    // enable painting on the panels
    display1->Show();
    display2->Show();
    display3->Show();
    display4->Show();
    
    // Show the panels in the sizer
    flexSizer->Show(display2, true, true);
    flexSizer->Show(display3, true, true);
    flexSizer->Show(display4, true, true);
    flexSizer->Layout();
    
    menuBar->Check(ID_MNU_VIEW_FULLSCREEN, false);
    menuBar->Check(ID_MNU_VIEW_SPLITSCREEN, false);
    menuBar->Check(ID_MNU_VIEW_QUADVIEW, true);
}

void ChaosConnectFrm::timer1Timer(wxTimerEvent& event) {
    /**
    *   Event handler for the timer. 
    *   Determines if a chaos unit is connected and updates the status bar
    *   Sends the connection information to the plots.
    *   Collects more data if data collection is not paused.
    */
     if(libchaos_isConnected() == true) {  
        // Update the status bar
        statusBar->SetStatusText(wxT("Connected: Yes"),0);
        int value = libchaos_getMDACValue();
        statusBar->SetStatusText(wxString::Format(wxT("MDAC Value: %d"), value), 1);
        statusBar->SetStatusText(wxString::Format(wxT("Resistance: %.2fk"), libchaos_mdacToResistance(value)/1000.0), 2);
        display1->getChaosPlot()->setDeviceStatus(true, value);
        display2->getChaosPlot()->setDeviceStatus(true, value);
        display3->getChaosPlot()->setDeviceStatus(true, value);
        display4->getChaosPlot()->setDeviceStatus(true, value);
        // Read data from the device
        if(ChaosSettings::Paused == false) {
            libchaos_readPlot(-1);
        }
    } else {
        statusBar->SetStatusText(wxT("Connected: No"),0);
        display1->getChaosPlot()->setDeviceStatus(false, 4095);
        display2->getChaosPlot()->setDeviceStatus(false, 4095);
        display3->getChaosPlot()->setDeviceStatus(false, 4095);
        display4->getChaosPlot()->setDeviceStatus(false, 4095);
    }
    
    // Reset the interval if we need to
    if(timer1->GetInterval() != ChaosSettings::UpdatePeriod) {
        timer1->Start(ChaosSettings::UpdatePeriod);
    }
    
    wxWakeUpIdle();
}

void ChaosConnectFrm::OnStartStopBtn(wxCommandEvent& event) {
    /**
    *   Event handler for the start/stop button. 
    *   Pauses/unpauses the collection of new data.
    */
    if(ChaosSettings::Paused == true) {
        ChaosSettings::Paused = false;
        startStopButton->SetLabel(wxT("Pause"));
    } else {
        ChaosSettings::Paused = true;
        startStopButton->SetLabel(wxT("Resume"));
    }
}

void ChaosConnectFrm::OnSettingsApplyBtn(wxCommandEvent& event) {
    /**
    *   Event handler for the apply button
    *   Applys the bifurcation settings to the plot. If PeaksPerMadc has
    *   been changed, the bifurcation will be redrawn.
    */
    ChaosSettings::BifStepsPerWindow = stepsSpinner->GetValue();
    if(ChaosSettings::PeaksPerMdac != peaksSpinner->GetValue()) {
        ChaosSettings::PeaksPerMdac = peaksSpinner->GetValue();
        libchaos_setPeaksPerMDAC(ChaosSettings::PeaksPerMdac);
        ChaosSettings::BifRedraw = true;
    }
}
