/**
 * \file SettingsDlg.h
 * \brief Headers for SettingsDlg.cpp
 */

#ifndef __SETTINGSDLG_H__
#define __SETTINGSDLG_H__

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
#include <wx/statusbr.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include "wx/progdlg.h"
#include "wx/arrstr.h"

#undef SettingsDlg_STYLE
#define SettingsDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX

class SettingsDlg : public wxDialog
{
    private:
        DECLARE_EVENT_TABLE();
        
    public:
        SettingsDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ChaosConnect"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = SettingsDlg_STYLE);
        virtual ~SettingsDlg();
    private:
        void OnApply(wxCommandEvent& event);
        void OnOk(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        
        void updateSettings();
        void loadSettings();
        
        wxBoxSizer *mainVertSizer;
        wxPanel *WxPanel1;
        wxBoxSizer *panelVertSizer;

        wxRadioBox *mdacRadio;
        
        wxRadioBox *adcRadio;
        
        wxRadioBox *pointSizeRadio;
        
        wxBoxSizer *stepsSizer;
        wxStaticText *stepsLabel;
        wxSpinCtrl *stepsSpinner;

        wxBoxSizer *peaksSizer;
        wxStaticText *peaksLabel;
        wxSpinCtrl *peaksSpinner;

        wxBoxSizer *amountSizer;
        wxStaticText *amountLabel;
        wxSpinCtrl *amountSpinner;

        wxBoxSizer *transientSizer;
        wxStaticText *transientLabel;
        wxSpinCtrl *transientSpinner;

        wxBoxSizer *refreshSizer;
        wxStaticText *refreshLabel;
        wxSpinCtrl *refreshSpinner;

        wxBoxSizer *buttonSizer;
        wxButton *buttonOk;
        wxButton *buttonCancel;
        wxButton *buttonApply;

        wxWindow* parent;
        
    private:
        enum
        {
            ID_WXPANEL1,
            ID_MDACLABEL,
            ID_MDACRADIO,
            ID_ADCRADIO,
            ID_POINTSIZERADIO,
            ID_STEPSLABEL,
            ID_STEPSSPINNER,
            ID_PEAKSLABEL,
            ID_PEAKSSPINNER,
            ID_AMOUNTLABEL,
            ID_AMOUNTSPINNER,
            ID_TRANSIENTLABEL,
            ID_TRANSIENTSPINNER,
            ID_REFRESHLABEL,
            ID_REFRESHSPINNER,
            ID_BUTTONOK,
            ID_BUTTONCANCEL,
            ID_BUTTONAPPLY,
            ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
        };
        
    private:
        void OnClose(wxCloseEvent& event);
        void CreateGUIControls();
};

#endif
