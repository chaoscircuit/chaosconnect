/**
 * \file Return1Plot.cpp
 * \brief Implements class for plotting a first return map
 */

#include "Return1Plot.h"
#include "ChaosSettings.h"

Return1Plot::Return1Plot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name) {
    /**
    *   Constructor for the first return map.
    *   This class inherits from the ChaosPlot class and redefines various
    *   settings and drawing functions to create a first return map of the
    *   data collected from the Chaos Unit.
    */
    side_gutter_size = 15;
    bottom_gutter_size = 20;
    square = true;
    graph_title = wxT("First Return Map");
    graph_subtitle = wxT("Peak(n+1) (V) vs. Peak(n) (V)");
    Connect( wxID_ANY, wxEVT_LEFT_DCLICK,
                    (wxObjectEventFunction) &Return1Plot::OnDblClick );
                    
    // Create timer
    timer1 = new wxTimer();
    timer1->SetOwner(this, ID_TIMER1);
    timer1->Stop();
    timer_ticks = 0;
    
    Connect( ID_TIMER1, wxEVT_TIMER,
                    (wxObjectEventFunction) &Return1Plot::timer1Timer );
}

Return1Plot::~Return1Plot() {
    /**
    *   Deconstructor for the Return1Plot class
    */
}

void Return1Plot::drawPlot() {
    /**
    *   Main drawing function for the Return1Plot class.
    *
    *   Draws the axis on the graph.
    *   Draws the y=x line across the graph (useful for analyzing return maps)
    *   Plots the points using consecutive peaks for x and y values
    */
    int x,y;
    
    startDraw();
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_ADC) {
        graph_subtitle = wxT("Peak(n+1) (ADC) vs. Peak(n) (ADC)");
    } else{
        graph_subtitle = wxT("Peak(n+1) (V) vs. Peak(n) (V)");
    }
    
    drawYAxis(0.0,3.3,1);
    drawXAxis(0.0,3.3,1);
    
    buffer->DrawLine(side_gutter_size,graph_height+top_gutter_size,
                     side_gutter_size+graph_width,top_gutter_size);

    //Use blue pen
    wxPen bluePen(*wxBLUE, 1); // blue pen of width 1
    wxBrush blueBrush(*wxBLUE_BRUSH);
    
    // Use blue brush
    buffer->SetPen(bluePen);
    buffer->SetBrush(blueBrush);
    
    float x_scale = graph_width/1024.0;
    float y_scale = graph_height/1024.0;

    // Get first plot point
    for(int i = 0; i < libchaos_getNumReturnMapPoints(); i++) {
        libchaos_getReturnMap1Point(&x,&y, i);
        
        x = (int)(x*x_scale + side_gutter_size);
        y =  (int)(graph_height + top_gutter_size - (y * y_scale));

        drawPoint(buffer, x, y);
    }
    
    followReturnPlot(0);
    
    endDraw();
}

void Return1Plot::OnDblClick(wxMouseEvent& evt) {
    /**
    *   Event handler for the graph double click.
    *   Enables or disables the return map following
    */
    if(timer1->IsRunning()) {
        timer1->Stop();
        timer_ticks = 0;
    } else {
        timer1->Start(ChaosSettings::UpdatePeriod);
    }
}

void Return1Plot::followReturnPlot(int index) {
    /**
    *   Draws lines to follow the return plot and show if it is behaving
    *   periodically or chaotically.  This is done by following a point to
    *   the y=x line, mirroring it and drawing a line between the 3 points.
    */
    int line_count = timer_ticks % 20;
    int x,y;
    float x_scale = graph_width/1024.0;
    float y_scale = graph_height/1024.0;
    
    //Use green pen
    wxPen greenPen(*wxGREEN, 1);
    buffer->SetPen(greenPen);
    
    for(int i = index; i < libchaos_getNumReturnMapPoints() && i < line_count; i++) {
        libchaos_getReturnMap1Point(&x,&y, i);
        x = (int)(x*x_scale);
        y =  (int)((y * y_scale));
        buffer->DrawLine(x + side_gutter_size, graph_height + top_gutter_size - x, x + side_gutter_size, graph_height + top_gutter_size - y);
        buffer->DrawLine(x + side_gutter_size, graph_height + top_gutter_size - y, y + side_gutter_size, graph_height + top_gutter_size - y);
    }
}

void Return1Plot::timer1Timer(wxTimerEvent& event) {
    /**
    *   Event handler for timer1.
    *   Increments timer_ticks so that we know how many return map points to follow.
    */
    timer_ticks++;
}
