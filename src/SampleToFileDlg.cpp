/**
 * \file SampleToFileDlg.cpp
 * \brief Contains class for the sample to file dialog
 */

#include <usb.h>
#include "SampleToFileDlg.h"
#include "libchaos.h"

BEGIN_EVENT_TABLE(SampleToFileDlg,wxDialog)
	EVT_CLOSE(SampleToFileDlg::OnClose)
	EVT_BUTTON(ID_BTN_SWEEP,SampleToFileDlg::BTN_sweepClick)
END_EVENT_TABLE()

SampleToFileDlg::SampleToFileDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

SampleToFileDlg::~SampleToFileDlg()
{
}

void SampleToFileDlg::CreateGUIControls()
{
	WxBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(220, 0), wxSize(247, 270));
	WxBoxSizer1->Add(WxPanel1,5,wxEXPAND | 0,0);

	WxBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	WxPanel1->SetSizer(WxBoxSizer2);
	WxPanel1->SetAutoLayout(true);

	WxPanel2 = new wxPanel(this, ID_WXPANEL2, wxPoint(467, 35), wxSize(160, 200));
	WxBoxSizer1->Add(WxPanel2,5,wxALIGN_CENTER | wxEXPAND | wxALL,0);

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxPanel2->SetSizer(WxBoxSizer3);
	WxPanel2->SetAutoLayout(true);

	LabelStart = new wxStaticText(WxPanel1, ID_LABELSTART, wxT("Starting Value:"), wxPoint(20, 5), wxDefaultSize, 0, wxT("LabelStart"));
	WxBoxSizer2->Add(LabelStart,0,wxALIGN_CENTER | wxALL,5);

	start = new wxSpinCtrl(WxPanel1, ID_START, wxT("0"), wxPoint(5, 32), wxSize(105, 22), wxSP_ARROW_KEYS, 0, 4095, 0);
	WxBoxSizer2->Add(start,0,wxALIGN_CENTER | wxALL,5);

	LabelEnd = new wxStaticText(WxPanel1, ID_LABELEND, wxT("Ending Value:"), wxPoint(23, 64), wxDefaultSize, 0, wxT("LabelEnd"));
	WxBoxSizer2->Add(LabelEnd,0,wxALIGN_CENTER | wxALL,5);

	end = new wxSpinCtrl(WxPanel1, ID_END, wxT("4095"), wxPoint(5, 91), wxSize(105, 22), wxSP_ARROW_KEYS, 0, 4095, 4095);
	WxBoxSizer2->Add(end,0,wxALIGN_CENTER | wxALL,5);

	LabelStep = new wxStaticText(WxPanel2, ID_LABELSTEP, wxT("Step Size:"), wxPoint(31, 123), wxDefaultSize, 0, wxT("LabelStep"));
	WxBoxSizer3->Add(LabelStep,0,wxALIGN_CENTER | wxALL,5);

	step = new wxSpinCtrl(WxPanel2, ID_STEP, wxT("10"), wxPoint(5, 150), wxSize(105, 22), wxSP_ARROW_KEYS, 1, 100, 10);
	WxBoxSizer3->Add(step,0,wxALIGN_CENTER | wxALL,5);

	LabelPeriods = new wxStaticText(WxPanel2, ID_LABELPERIODS, wxT("Periods Per Tap:"), wxPoint(16, 182), wxDefaultSize, 0, wxT("LabelPeriods"));
	WxBoxSizer3->Add(LabelPeriods,0,wxALIGN_CENTER | wxALL,5);

	periods = new wxSpinCtrl(WxPanel2, ID_PERIODS, wxT("10"), wxPoint(5, 209), wxSize(105, 22), wxSP_ARROW_KEYS, 1, 50000, 10);
	WxBoxSizer3->Add(periods,0,wxALIGN_CENTER | wxALL,5);

	BTN_sweep = new wxButton(WxPanel1, ID_BTN_SWEEP, wxT("Sweep"), wxPoint(25, 241), wxSize(65, 21), 0, wxDefaultValidator, wxT("BTN_sweep"));
	WxBoxSizer2->Add(BTN_sweep,0,wxALIGN_CENTER | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL,5);

	SetTitle(wxT("Sample To File"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	Center();
}

void SampleToFileDlg::OnClose(wxCloseEvent& event) {
	Destroy();
}

void SampleToFileDlg::BTN_sweepClick(wxCommandEvent& event) {
    /** 
     * Perform a sample sweep.
     *
     * This method saves a sweep to a CSV file using the settings 
     * provided by the user.
     *
     * The actual work here is handled by calls to libchaos which save
     * the sampled data to file. A progress bar is used to show how 
     * things are going.
     */
    int cont;

    wxFileDialog dialog(this,
                        _T("Save file"),
                        wxEmptyString,
                        _T("data.csv"),
                        _T("CSV files (*.csv)|*.csv"),
                        wxSAVE|wxOVERWRITE_PROMPT);
    dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {
    wxProgressDialog progress_dialog(_T("Data Collection"),
                            _T("Sample sweep in progress..."),
                            100,    // range
                            this,   // parent
                            wxPD_CAN_ABORT |
                            wxPD_APP_MODAL |
                            wxPD_ELAPSED_TIME |
                            wxPD_ESTIMATED_TIME |
                            wxPD_REMAINING_TIME |
                            wxPD_SMOOTH);

        if(!libchaos_startSampleToCSV((char*)dialog.GetPath().c_str(), 
                             start->GetValue(),
                             end->GetValue(),
                             step->GetValue(),
                             periods->GetValue())) {
            int progress;

            while(progress = libchaos_samplePartToCSV()) {
                cont = progress_dialog.Update(progress);
                if(!cont) {
                  break;
                }
            }
            libchaos_endSampleToCSV();
        }
    }
}
