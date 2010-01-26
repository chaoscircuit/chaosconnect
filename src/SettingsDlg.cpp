/**
 * \file SettingsDlg.cpp
 * \brief Implements class for the settings dialog window
 */

#include <usb.h>
#include "SettingsDlg.h"
#include "libchaos.h"
#include "ChaosSettings.h"

using namespace ChaosSettings;

BEGIN_EVENT_TABLE(SettingsDlg,wxDialog)
    EVT_CLOSE(SettingsDlg::OnClose)
    EVT_BUTTON(ID_BUTTONAPPLY, SettingsDlg::OnApply)
    EVT_BUTTON(ID_BUTTONOK, SettingsDlg::OnOk)
    EVT_BUTTON(ID_BUTTONCANCEL, SettingsDlg::OnCancel)
END_EVENT_TABLE()

SettingsDlg::SettingsDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
    loadSettings();
}

SettingsDlg::~SettingsDlg()
{
}

void SettingsDlg::CreateGUIControls()
{
    mainVertSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainVertSizer);
    this->SetAutoLayout(true);

    WxPanel1 = new wxPanel(this, ID_WXPANEL1, 
                           wxDefaultPosition, wxDefaultSize);
    mainVertSizer->Add(WxPanel1,5,wxEXPAND | 0,0);

    panelVertSizer = new wxBoxSizer(wxVERTICAL);
    WxPanel1->SetSizer(panelVertSizer);
    WxPanel1->SetAutoLayout(true);

    // MDAC values
    wxString mdac_radio_list[2] = { 
        wxT("Raw Value"), 
        wxT("Calculate Resistance")
        }; 
    mdacRadio = new wxRadioBox(WxPanel1, ID_MDACRADIO, 
                               wxT("MDAC Value Conversion"), 
                               wxDefaultPosition, wxDefaultSize, 
                               2,mdac_radio_list);
    panelVertSizer->Add(mdacRadio,0,wxALIGN_LEFT | wxALL,5);

    // ADC values
    wxString adc_radio_list[3] = { 
        wxT("Raw Value"), 
        wxT("Calculate Voltage"), 
        wxT("Calculate Biased Voltage")
        }; 
    adcRadio = new wxRadioBox(WxPanel1, ID_ADCRADIO, 
                               wxT("ADC Value Conversion"), 
                               wxDefaultPosition, wxDefaultSize, 
                               3,adc_radio_list);
    panelVertSizer->Add(adcRadio,0,wxALIGN_LEFT | wxALL,5);

    // Point Size
    wxString point_size_radio_list[3] = { 
        wxT("Small"), 
        wxT("Medium"), 
        wxT("Large")
        }; 
    pointSizeRadio = new wxRadioBox(WxPanel1, ID_POINTSIZERADIO, 
                               wxT("Point Size"), 
                               wxDefaultPosition, wxDefaultSize, 
                               3,point_size_radio_list);
    panelVertSizer->Add(pointSizeRadio,0,wxALIGN_LEFT | wxALL,5);

    // Steps
    stepsSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(stepsSizer,0,wxALIGN_LEFT | wxALL,5);
    
    stepsLabel = new wxStaticText(WxPanel1, ID_STEPSLABEL, 
                                  wxT("Bifurcation Steps"),
                                  wxDefaultPosition, wxDefaultSize, 
                                  0);
    stepsSizer->Add(stepsLabel,0,wxALIGN_LEFT | wxALL,5);
    
    stepsSpinner = new wxSpinCtrl(WxPanel1, ID_STEPSSPINNER, 
                                  wxT("100"), 
                                  wxDefaultPosition, wxDefaultSize, 
                                  wxSP_ARROW_KEYS, 0, 4095, 100);
    stepsSizer->Add(stepsSpinner,0,wxALIGN_LEFT | wxALL,5);
    
    // Peaks
    peaksSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(peaksSizer,0,wxALIGN_LEFT | wxALL,5);
    
    peaksLabel = new wxStaticText(WxPanel1, ID_PEAKSLABEL, 
                                  wxT("Bifurcation Peaks"),
                                  wxDefaultPosition, wxDefaultSize, 
                                  0);
    peaksSizer->Add(peaksLabel,0,wxALIGN_LEFT | wxALL,5);
    
    peaksSpinner = new wxSpinCtrl(WxPanel1, ID_PEAKSSPINNER, 
                                  wxT("10"), 
                                  wxDefaultPosition, wxDefaultSize, 
                                  wxSP_ARROW_KEYS, 0, 100, 10);
    peaksSizer->Add(peaksSpinner,0,wxALIGN_LEFT | wxALL,5);
    
    // Amount of data per sample
    amountSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(amountSizer,0,wxALIGN_LEFT | wxALL,5);
    
    amountLabel = new wxStaticText(WxPanel1, ID_AMOUNTLABEL, 
                                  wxT("Data Points Per Sample (thousands)"),
                                  wxDefaultPosition, wxDefaultSize, 
                                  0);
    amountSizer->Add(amountLabel,0,wxALIGN_LEFT | wxALL,5);
    
    amountSpinner = new wxSpinCtrl(WxPanel1, ID_AMOUNTSPINNER, 
                                  wxT("2"), 
                                  wxDefaultPosition, wxDefaultSize, 
                                  wxSP_ARROW_KEYS, 1, 8, 2);
    amountSizer->Add(amountSpinner,0,wxALIGN_LEFT | wxALL,5);
    
    // Transient Data to drop
    transientSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(transientSizer,0,wxALIGN_LEFT | wxALL,5);
    
    transientLabel = new wxStaticText(WxPanel1, ID_TRANSIENTLABEL, 
                                  wxT("Transient Points to Drop (thousands)"),
                                  wxDefaultPosition, wxDefaultSize, 
                                  0);
    transientSizer->Add(transientLabel,0,wxALIGN_LEFT | wxALL,5);
    
    transientSpinner = new wxSpinCtrl(WxPanel1, ID_TRANSIENTSPINNER, 
                                  wxT("4"), 
                                  wxDefaultPosition, wxDefaultSize, 
                                  wxSP_ARROW_KEYS, 0, 24, 4);
    transientSizer->Add(transientSpinner,0,wxALIGN_LEFT | wxALL,5);
    
    // GUI Refresh Time
    refreshSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(refreshSizer,0,wxALIGN_LEFT | wxALL,5);
    
    refreshLabel = new wxStaticText(WxPanel1, ID_REFRESHLABEL, 
                                  wxT("GUI Refresh Time (ms)"),
                                  wxDefaultPosition, wxDefaultSize, 
                                  0);
    refreshSizer->Add(refreshLabel,0,wxALIGN_LEFT | wxALL,5);
    
    refreshSpinner = new wxSpinCtrl(WxPanel1, ID_REFRESHSPINNER, 
                                  wxT("300"), 
                                  wxDefaultPosition, wxDefaultSize, 
                                  wxSP_ARROW_KEYS, 100, 2000, 300);
    refreshSizer->Add(refreshSpinner,0,wxALIGN_LEFT | wxALL,5);
    
    // Buttons (OK|CANCEL|APPLY)
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(buttonSizer,0,wxALIGN_RIGHT | wxALL,5);

    buttonOk = new wxButton(WxPanel1, ID_BUTTONOK, wxT("Ok"));
    buttonSizer->Add(buttonOk,0,wxALIGN_LEFT | wxALL,5);

    buttonCancel = new wxButton(WxPanel1, ID_BUTTONCANCEL, wxT("Cancel"));
    buttonSizer->Add(buttonCancel,0,wxALIGN_LEFT | wxALL,5);

    buttonApply = new wxButton(WxPanel1, ID_BUTTONAPPLY, wxT("Apply"));
    buttonSizer->Add(buttonApply,0,wxALIGN_LEFT | wxALL,5);

    SetTitle(wxT("Settings"));
    SetIcon(wxNullIcon);
    
    GetSizer()->Layout();
    GetSizer()->Fit(this);
    Center();
}

void SettingsDlg::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void SettingsDlg::OnApply(wxCommandEvent& event) {
    updateSettings();
}

void SettingsDlg::OnOk(wxCommandEvent& event) {
    updateSettings();
    Destroy();
}

void SettingsDlg::OnCancel(wxCommandEvent& event) {
    Destroy();
}

void SettingsDlg::updateSettings() {
    BifXAxis = mdacRadio->GetSelection();
    YAxisLabels = adcRadio->GetSelection();
    
    switch(pointSizeRadio->GetSelection()) {
        case 0:
            PointSize = SMALL_PT;
            break;
        case 1:
            PointSize = MEDIUM_PT;
            break;
        case 2:
            PointSize = LARGE_PT;
            break;
    }
    
    BifStepsPerWindow = stepsSpinner->GetValue();
    
    if(PeaksPerMdac != peaksSpinner->GetValue()) {
        PeaksPerMdac = peaksSpinner->GetValue();
        libchaos_setPeaksPerMDAC(PeaksPerMdac);
    }

    PointsPerSample = amountSpinner->GetValue()*1020;
    libchaos_setNumPlotPoints(PointsPerSample);

    TransientPoints = transientSpinner->GetValue();
    libchaos_setTransientData(TransientPoints);

    UpdatePeriod = refreshSpinner->GetValue();
    ChaosSettings::BifRedraw = true;
}

void SettingsDlg::loadSettings() {
    mdacRadio->SetSelection(BifXAxis);
    adcRadio->SetSelection(YAxisLabels);
    
    switch(PointSize) {
        case SMALL_PT:
            pointSizeRadio->SetSelection(0);
            break;
        case MEDIUM_PT:
            pointSizeRadio->SetSelection(1);
            break;
        case LARGE_PT:
            pointSizeRadio->SetSelection(2);
            break;
        default:
            pointSizeRadio->SetSelection(1);
            break;
    }

    stepsSpinner->SetValue(BifStepsPerWindow);
    peaksSpinner->SetValue(PeaksPerMdac);
    amountSpinner->SetValue(PointsPerSample/1020);
    transientSpinner->SetValue(TransientPoints);
    refreshSpinner->SetValue(UpdatePeriod);
}
