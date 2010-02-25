/**
 * \file ChaosConnectFrm.h
 * \brief Main window header file
 */

#ifndef __CHAOSCONNECTFRM_h__
#define __CHAOSCONNECTFRM_h__

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/frame.h>
#else
    #include <wx/wxprec.h>
#endif


#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include "wx/progdlg.h"
#include <wx/dcbuffer.h>

#include "ChaosPanel.h"
#include "SampleToFileDlg.h"
#include "SettingsDlg.h"
#include "AboutDlg.h"


#undef ChaosConnectFrm_STYLE
#define ChaosConnectFrm_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX

class ChaosConnectFrm : public wxFrame
{
    public:
        ChaosConnectFrm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ChaosConnect"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = ChaosConnectFrm_STYLE);
        virtual ~ChaosConnectFrm();

    private:
        // Event handlers
        DECLARE_EVENT_TABLE();
        void OnClose(wxCloseEvent& event);
        void mnuShowLog(wxCommandEvent& event);
        void mnuSampleToFile(wxCommandEvent& event);
        void mnuSettings(wxCommandEvent& event);
        void mnuExit(wxCommandEvent& event);
        void mnuFullscreen(wxCommandEvent& event);
        void mnuSplitScreen(wxCommandEvent& event);
        void mnuQuadScreen(wxCommandEvent& event);
        void mnuAbout(wxCommandEvent& event);
        void timer1Timer(wxTimerEvent& event);
        void OnStartStopBtn(wxCommandEvent& event);
        void OnSettingsApplyBtn(wxCommandEvent& event);
        void OnBifEraseBtn(wxCommandEvent& event);
        void DisplayBifurcationSettings();

        // Functions
        void CreateGUIControls();
        
        // wxWidgets gui objects
        wxTimer *timer1;
        wxMenuBar *menuBar;
        wxStatusBar *statusBar;
        wxPanel *display5;
        ChaosPanel *display4;
        ChaosPanel *display3;
        ChaosPanel *display2;
        ChaosPanel *display1;
        wxFlexGridSizer *flexSizer;
        wxFlexGridSizer *overallSizer;
        wxBoxSizer *bottomRowSizer;
        wxBoxSizer *panel5Sizer;
        wxLogWindow *logger;
        wxHtmlWindow *about;
        wxButton *startStopButton;
        wxStaticText *stepsLabel;
        wxSpinCtrl *stepsSpinner;
        wxStaticText *peaksLabel;
        wxSpinCtrl *peaksSpinner;
        wxButton *settingsApplyButton;
        wxButton *bifEraseButton;

        
    private:
        // Enumeration for GUI controls
        enum {
            ////GUI Enum Control ID Start
            ID_TIMER1 = 1151,
            ID_MNU_FILE,
            ID_MNU_FILE_EXIT,
            ID_MNU_SAMPLE_TO_FILE,
            ID_MNU_SETTINGS,
            ID_MNU_SHOW_LOG,
            ID_MNU_VIEW,
            ID_MNU_VIEW_FULLSCREEN,
            ID_MNU_VIEW_SPLITSCREEN,
            ID_MNU_VIEW_QUADVIEW,
            ID_MNU_ABOUT,
            
            ID_STATUSBAR,
            ID_DISPLAY5,
            ID_DISPLAY4,
            ID_DISPLAY3,
            ID_DISPLAY2,
            ID_DISPLAY1,
            ID_START_STOP_BTN,
            ID_PEAKS_SPINNER,
            ID_STEPS_SPINNER,
            ID_SETTINGS_APPLY_BTN,
            ID_BIF_ERASE_BTN,
            ID_DUMMY_VALUE_
        };
};

#endif
