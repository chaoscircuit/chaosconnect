/**
 * \file XTPlot.cpp
 * \brief Implements class for drawing the XT plots
 */

#include "XTPlot.h"
#include "ChaosSettings.h"

XTPlot::XTPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name) {
    /**
    *   Constructor for the XT plot. Defaults to only showing X.
    */
    side_gutter_size = 20;
    bottom_gutter_size = 20;
    x1Visible = true;
    x2Visible = false;
    x3Visible = false;
    graph_title = wxT("Waveform as a function of time");
    graph_subtitle = wxT("X (V) vs. T(ms)");
}

XTPlot::~XTPlot() {
    /**
    *   Deconstructor for the Rotating3dPlot class
    */
}

void XTPlot::drawPlot() {
    /**
    *   Main drawing function for the XTPlot class.
    *
    *   Calculates the units for the axis and then draws them
    *   Draws an XT graph using X, X', and X'' depending on which ones
    *   the user has selected.
    *
    */
    const int xt_points = 300;
    
    // max time on the graph in ms
    float max_time = xt_points*(1/72000.0)*1000;
    
    static int times_called = 0;
    
    int x1,x2,x1_old,x2_old, x3, x3_old;
    int start;
    
    startDraw();
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
        graph_subtitle = wxT("X (V) vs. T(ms)");
        drawYAxis(0.0,3.3,1);
    } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
        graph_subtitle = wxT("X (V) vs. T(ms)");
        drawYAxis(-1.2,2.1,.5);
    } else {
        graph_subtitle = wxT("X (ADC) vs. T(ms)");
        drawYAxis(0,1024,341);
    }
    drawXAxis(0,max_time,max_time/5.0);

    if(device_connected == false) {
        endDraw();
        return;
    }
    
    int plot_width = width-side_gutter_size-2;
    int plot_points;
    float x_scale;
    float y_scale = float(graph_height)/1024.0;
    
    plot_points = xt_points;
    x_scale = float(plot_width)/xt_points;
    
    start = libchaos_getTriggerIndex();

    // Get first plot point
    libchaos_getPlotPoint(&x1,&x2,&x3, start);
    x3 = graph_height + top_gutter_size - int(x3*y_scale);
    x2 = graph_height + top_gutter_size - int(x2*y_scale);
    x1 = graph_height + top_gutter_size - int(x1*y_scale);
    x1_old = x1;
    x2_old = x2;
    x3_old = x3;
        
    for(int i = 1; i < plot_points; i++) {
        libchaos_getPlotPoint(&x1,&x2,&x3, i+start);
        x3 = graph_height + top_gutter_size - int(x3*y_scale);
        x2 = graph_height + top_gutter_size - int(x2*y_scale);
        x1 = graph_height + top_gutter_size - int(x1*y_scale);
        
        if(x1Visible == true) {
            //Use red pen
            wxPen rPen(*wxRED, 2); // red pen of width 2
            buffer->SetPen(rPen);
            buffer->DrawLine((int)(x_scale*(i-1)+side_gutter_size+1), x1_old, (int)(x_scale*i+side_gutter_size+1), x1);
        }
        
        if(x2Visible == true) {
            //Use blue pen
            wxPen bPen(*wxBLUE, 2); // blue pen of width 2
            buffer->SetPen(bPen);
            buffer->DrawLine((int)(x_scale*(i-1)+side_gutter_size+1), x2_old, (int)(x_scale*i+side_gutter_size+1), x2);
        }
        
        if(x3Visible == true) {
            //Use green pen
            wxPen gPen(*wxGREEN, 2); // blue pen of width 2
            buffer->SetPen(gPen);
            buffer->DrawLine((int)(x_scale*(i-1)+side_gutter_size+1), x3_old, (int)(x_scale*i+side_gutter_size+1), x3);
        }
        
        x1_old = x1;
        x2_old = x2;
        x3_old = x3;
    }
    times_called = 0;
    
    // Display buffer
    endDraw();
}

void XTPlot::setX1Visibility(bool visible) {
    /**
    *   Enables or disables the visibility of X on the graph
    */
    x1Visible = visible;
}

void XTPlot::setX2Visibility(bool visible) {
    /**
    *   Enables or disables the visibility of X' on the graph
    */
    x2Visible = visible;
}

void XTPlot::setX3Visibility(bool visible) {
    /**
    *   Enables or disables the visibility of X'' on the graph
    */
    x3Visible = visible;
}
