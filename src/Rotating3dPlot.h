/**
 * \file Rotating3dPlot.h
 * \brief Headers for Rotating3dPlot.cpp
 */

#ifndef ROTATING_3DPLOT_H
#define ROTATING_3DPLOT_H

#include "ChaosPlot.h" // inheriting class's header file
#include "libchaos.h"

class Rotating3dPlot : public ChaosPlot
{
    public:
        // class constructor
        Rotating3dPlot(wxWindow* parent, 
               wxWindowID id = wxID_ANY, 
               const wxPoint& pos = wxDefaultPosition, 
               const wxSize& size = wxDefaultSize, 
               long style = wxTAB_TRAVERSAL, 
               const wxString& name = wxT("panel"));
        // class destructor
        ~Rotating3dPlot();
        void drawPlot();
        void setPause(bool paused);
        void setRotation(int rotation);
    
    private:
        int xToValue(int x);
        int yToValue(int y);
        int valueToX(int value);
        int valueToY(int value);
        void zoomDefault();
        void timer1Timer(wxTimerEvent& event);
        
        wxTimer *timer1;
        unsigned int timer_ticks;
        
        enum {
            ID_TIMER1 = 1000,
        };
};

#endif // XYPLOT_H
