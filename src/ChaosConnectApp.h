/**
 * \file ChaosConnectApp.h
 * \brief Main Application Header File
 */

#ifndef __CHAOSCONNECTFRMApp_h__
#define __CHAOSCONNECTFRMApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class ChaosConnectFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
