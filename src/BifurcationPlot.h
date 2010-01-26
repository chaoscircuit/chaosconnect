/**
 * \file BifurcationPlot.h
 * \brief Bifurcation Plot Header File
 */

#ifndef BIFURCATIONPLOT_H
#define BIFURCATIONPLOT_H

#include "ChaosPlot.h"
#include <wx/wx.h>
#include "libchaos.h"
#include "ChaosSettings.h"

class BifurcationPlot : public ChaosPlot
{
    public:
        // class constructor
        BifurcationPlot(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"));
        // class destructor
        ~BifurcationPlot();
        void drawPlot();
        void setPause(bool pause);
    private: 
        // Functions
        void drawMdacLine(wxDC* dc);
        int valueToX(int mdac_value);
        int xToValue(int x);
        int valueToY(int y);
        int yToValue(int y);
        void zoomDefault();
        int xToMdac(int x);
    
        wxBitmap* bifBmp;
        
        // Event handlers
        void OnDblClick(wxMouseEvent& evt);
        void OnMouseUp(wxMouseEvent& evt);
        
        bool paused;
};

#endif // BIFURCATIONPLOT_H
