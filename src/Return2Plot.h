/**
 * \file Return2Plot.h
 * \brief Headers for Return2Plot.cpp
 */
 
#ifndef RETURN2PLOT_H
#define RETURN2PLOT_H

#include "ChaosPlot.h"
#include "libchaos.h"

#define NUM_POINTS 600
class Return2Plot : public ChaosPlot
{
    public:
        // class constructor
        Return2Plot(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"));
        // class destructor
        ~Return2Plot();
        void drawPlot();
    private:
        int points[NUM_POINTS][2];
        int point_count;
        
        void zoomDefault();
};

#endif
