/**
 * \file ChaosConnectApp.cpp
 * \brief Main Application Code
 */

#include "ChaosConnectApp.h"
#include "ChaosConnectFrm.h"

IMPLEMENT_APP(ChaosConnectFrmApp)

bool ChaosConnectFrmApp::OnInit()
{
    /**
    *   Creates the main form and shows it
    */
    ChaosConnectFrm* frame = new ChaosConnectFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int ChaosConnectFrmApp::OnExit()
{
    /**
    *   Closes the application and shuts down the USB connection established
    *   by libchaos.
    */
    libchaos_close();
    return 0;
}
