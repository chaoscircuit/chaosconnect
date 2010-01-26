/**
 * \file SampleToFileDlg.h
 * \brief Headers for SampleToFileDlg.cpp
 */

#ifndef __SAMPLETOFILEDLG_H__
#define __SAMPLETOFILEDLG_H__

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


#undef SampleToFileDlg_STYLE
#define SampleToFileDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX

class SampleToFileDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		SampleToFileDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ChaosConnect"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = SampleToFileDlg_STYLE);
		virtual ~SampleToFileDlg();
		void BTN_sweepClick(wxCommandEvent& event);
		void SampleToFileDlgActivate(wxActivateEvent& event);
	private:
		wxTimer *WxTimer1;
		wxStatusBar *WxStatusBar1;
		wxButton *OpenBifurcation;
		wxSpinCtrl *mdacValue;
		wxStaticText *LabelSetMDAC;
		wxBoxSizer *WxBoxSizer3;
		wxPanel *WxPanel2;
		wxButton *BTN_sweep;
		wxSpinCtrl *periods;
		wxStaticText *LabelPeriods;
		wxSpinCtrl *step;
		wxStaticText *LabelStep;
		wxSpinCtrl *end;
		wxStaticText *LabelEnd;
		wxSpinCtrl *start;
		wxStaticText *LabelStart;
		wxBoxSizer *WxBoxSizer2;
		wxPanel *WxPanel1;
		wxBoxSizer *WxBoxSizer1;
		wxWindow* parent;
		
	private:
		enum
		{
			ID_WXTIMER1 = 1077,
			ID_WXSTATUSBAR1 = 1078,
			ID_OPENBIFURCATION = 1093,
			ID_MDACVALUE = 1082,
			ID_LABELSETMDAC = 1090,
			ID_WXPANEL2 = 1079,
			ID_BTN_SWEEP = 1070,
			ID_PERIODS = 1076,
			ID_LABELPERIODS = 1087,
			ID_STEP = 1075,
			ID_LABELSTEP = 1086,
			ID_END = 1074,
			ID_LABELEND = 1085,
			ID_START = 1073,
			ID_LABELSTART = 1084,
			ID_WXPANEL1 = 1067,
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
