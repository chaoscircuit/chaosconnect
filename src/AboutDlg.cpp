/**
 * \file AboutDlg.cpp
 * \brief About Dialog Class
 */

#include <usb.h>
#include "AboutDlg.h"
#include "libchaos.h"
#include "ChaosSettings.h"

using namespace ChaosSettings;

BEGIN_EVENT_TABLE(AboutDlg,wxDialog)
    EVT_CLOSE(AboutDlg::OnClose)
    EVT_BUTTON(ID_BUTTONOK, AboutDlg::OnOk)
END_EVENT_TABLE()

AboutDlg::AboutDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    CreateGUIControls();
}

AboutDlg::~AboutDlg()
{
}

void AboutDlg::CreateGUIControls()
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

    htmlPanel = new wxHtmlWindow(this,
                             ID_HTMLPANEL,
                             wxDefaultPosition, wxSize(200,300),
                             wxHW_SCROLLBAR_NEVER,
                             wxString(wxT("HTML_WINDOW")));
    
    wxString html = wxString::Format(wxT(""
        "<h3>About</h3>"
        "Version: %.3f <br>"
        "libchaos Version: %.3f <br>"
        "Firmware Version: %.3f <br>"
        ""),
        ChaosSettings::Version,
        (float)libchaos_getVersion()/1000.0,
        (float)libchaos_getFirmwareVersion()/1000.0);
    
    htmlPanel->SetPage(html);
    
    panelVertSizer->Add(htmlPanel,0,wxALIGN_LEFT | wxALL,0);
    
    // Buttons (OK)
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    panelVertSizer->Add(buttonSizer,0,wxALIGN_RIGHT | wxALL,5);

    buttonOk = new wxButton(WxPanel1, ID_BUTTONOK, wxT("Ok"));
    buttonSizer->Add(buttonOk,0,wxALIGN_LEFT | wxALL,5);

    SetTitle(wxT("About"));
    SetIcon(wxNullIcon);
    
    GetSizer()->Layout();
    GetSizer()->Fit(this);
    Center();
}

void AboutDlg::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void AboutDlg::OnOk(wxCommandEvent& event) {
    Destroy();
}
