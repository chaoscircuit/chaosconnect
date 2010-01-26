/**
 * \file Return2Plot.cpp
 * \brief Implements class for plotting second return maps
 */

#include "Return2Plot.h"
#include "ChaosSettings.h"

// class constructor
Return2Plot::Return2Plot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name)
{
    side_gutter_size = 15;
    bottom_gutter_size = 20;
    square = true;
    graph_title = wxT("Second Return Map");
    graph_subtitle = wxT("Peak(n+2) (V) vs. Peak(n) (V)");
}

// class destructor
Return2Plot::~Return2Plot()
{
    // insert your code here
}

void Return2Plot::drawPlot() {
    int x,y;
    
    startDraw();
    
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_ADC) {
        graph_subtitle = wxT("Peak(n+2) (ADC) vs. Peak(n) (ADC)");
    } else{
        graph_subtitle = wxT("Peak(n+2) (V) vs. Peak(n) (V)");
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
        libchaos_getReturnMap2Point(&x,&y, i);
        
        x = (int)(x*x_scale + side_gutter_size);
        y = (int)(graph_height + top_gutter_size - (y * y_scale));

        drawPoint(buffer, x, y);
    }
    
    endDraw();
}
