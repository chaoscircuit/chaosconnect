/**
 * \file FFTPlot.h
 * \brief Header file for FFTPlot.cpp
 */

#ifndef FFTPLOT_H
#define FFTPLOT_H

#include <wx/wx.h>
#include "ChaosPlot.h" // inheriting class's header file
#include "libchaos.h"

class FFTPlot : public ChaosPlot
{
    public:
        // class constructor
        FFTPlot(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"));
        // class destructor
        ~FFTPlot();
        void drawPlot();
};

#endif // XTPLOT_H
