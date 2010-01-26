/**
 * \file Return1Plot.h
 * \brief Headers for Return1Plot.cpp
 */

#ifndef RETURN1PLOT_H
#define RETURN1PLOT_H

#include "ChaosPlot.h"
#include "libchaos.h"

class Return1Plot : public ChaosPlot
{
    public:
        // class constructor
        Return1Plot(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"));
        // class destructor
        ~Return1Plot();
        void drawPlot();
    private:
        void OnDblClick(wxMouseEvent& evt);
        void followReturnPlot(int index);
        void timer1Timer(wxTimerEvent& event);
        
        wxTimer *timer1;
        int timer_ticks;
        
        enum {
            ID_TIMER1 = 1000,
        };
};

#endif
