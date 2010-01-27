/**
 * \file FFTPlot.cpp
 * \brief Contains the FFTPlot class
 */

#include "FFTPlot.h"

FFTPlot::FFTPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name) {
   /**
    *   Constructor for the FFTPlot class.
    *
    *   This class inherits basic plot functionality from ChaosPlot
    */

    side_gutter_size = 0;
    bottom_gutter_size = 20;
    graph_title = wxT("Fast Fourier transform");
    graph_subtitle = wxT("Power Spectral Density vs. Frequency (Hz)");
}

FFTPlot::~FFTPlot() {
    /**
     * Destructor for the FFTPlot
     */
}

void FFTPlot::drawPlot() {
    /** 
     * Draw the FFT Plot
     *
     * The FFT data is not collected by this function but is simply 
     * draw by it. The main timer call to libchaos_readPlot() handles 
     * the data collection and FFT calculation and then stores that 
     * data. Calls to libchaos_getFFTPlotPoint() pull in the data from
     * libchaos. From here, it's just plotting the points where we want 
     * them.
     */
    // number of points to use in the graph
    const int points_to_graph  = 400;
    // number of data points used for FFT
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
