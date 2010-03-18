/**
 * \file XTPlot.h
 * \brief Headers for XTPlot.cpp
 */

#ifndef XTPLOT_H
#define XTPLOT_H

#include "ChaosPlot.h"
#include "libchaos.h"

class XTPlot : public ChaosPlot
{
    public:
        XTPlot(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"));
        ~XTPlot();
        void drawPlot();
        void setX1Visibility(bool visible); 
        void setX2Visibility(bool visible);
        void setX3Visibility(bool visible);
    private:
        bool x1Visible;
        bool x2Visible;
        bool x3Visible;
};

#endif // XTPLOT_H
