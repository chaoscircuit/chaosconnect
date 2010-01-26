/**
 * \file FFTPlot.cpp
 * \brief Contains the FFTPlot class
 */

#include "FFTPlot.h"

// class constructor
FFTPlot::FFTPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name)
{
    side_gutter_size = 0;
    bottom_gutter_size = 20;
    graph_title = wxT("Fast Fourier transform");
    graph_subtitle = wxT("Power Spectral Density vs. Frequency (Hz)");
}

// class destructor
FFTPlot::~FFTPlot()
{
    // insert your code here
}

void FFTPlot::drawPlot() {
    const int points_to_graph  = 400;
    const int N = 8192;
    
    int x_axis_max = (int)(float(points_to_graph)/(N*1/float(LIBCHAOS_SAMPLE_FREQUENCY)));
    
    startDraw();
    drawXAxis(0.0,x_axis_max,1200);
    
    float a;
    float a_old=0;

    float x_scale = float(graph_width)/points_to_graph;
    float y_scale = graph_height/15.0;

    libchaos_getFFTPlotPoint(&a_old, 1);
    a_old = graph_height + top_gutter_size - (a_old*y_scale);
    
    for(int i = 0; i < points_to_graph; i++) {
        libchaos_getFFTPlotPoint(&a, i+2);
        a = graph_height + top_gutter_size - (a*y_scale);
        if ( a > graph_height + top_gutter_size ) {
            a = graph_height + top_gutter_size;
        }
        
        //Use blue pen
        wxPen bPen(*wxBLUE, 2); // blue pen of width 2
        buffer->SetPen(bPen);
        buffer->DrawLine((int)((i*x_scale)+side_gutter_size), 
                         (int)(a_old), 
                         (int)(((i+1)*x_scale)+side_gutter_size),
                         (int)(a));

        a_old = a;
    }
    
    endDraw();
}
