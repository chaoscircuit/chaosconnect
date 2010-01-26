/**
 * \file AboutDlg.h
 * \brief About Dialog Class Headers
 */

#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

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
////Header Include End
#include "wx/progdlg.h"
#include "wx/arrstr.h"
#include <wx/html/htmlwin.h>

#undef AboutDlg_STYLE
#define AboutDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX

class AboutDlg : public wxDialog
{
    private:
        DECLARE_EVENT_TABLE();
        
    public:
        AboutDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ChaosConnect"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = AboutDlg_STYLE);
        virtual ~AboutDlg();
    private:
        void OnOk(wxCommandEvent& event);
        
        wxBoxSizer *mainVertSizer;
        wxPanel *WxPanel1;
        wxBoxSizer *panelVertSizer;

        wxHtmlWindow* htmlPanel;
        
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
            ID_HTMLPANEL,
            ID_BUTTONOK,
            ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
        };
        
    private:
        void OnClose(wxCloseEvent& event);
        void CreateGUIControls();
};

#endif
