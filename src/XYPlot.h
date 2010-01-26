/**
 * \file XYPlot.h
 * \brief Headers for XYPlot.cpp
 */

#ifndef XYPLOT_H
#define XYPLOT_H

#include "ChaosPlot.h"
#include "libchaos.h"

class XYPlot : public ChaosPlot
{
    public:
        // class constructor
        XYPlot(wxWindow* parent, 
               wxWindowID id = wxID_ANY, 
               const wxPoint& pos = wxDefaultPosition, 
               const wxSize& size = wxDefaultSize, 
               long style = wxTAB_TRAVERSAL, 
               const wxString& name = wxT("panel"));
        // class destructor
        ~XYPlot();
        void drawPlot();
    
    private:
        int xToValue(int x);
        int yToValue(int y);
        int valueToX(int value);
        int valueToY(int value);
        void zoomDefault();
};

#endif // XYPLOT_H
