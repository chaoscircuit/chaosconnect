/**
 * \file XYPlot.cpp
 * \brief Implements class for drawing XY plots
 */

#include "XYPlot.h"
#include "ChaosSettings.h"

// class constructor
XYPlot::XYPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name)
{
    side_gutter_size = 20;
    bottom_gutter_size = 20;
    smallest_x_value = 0;
    smallest_y_value = 0;
    largest_x_value = 1024;
    largest_y_value = 1024;
    graph_title = wxT("Phase Portrait");
    graph_subtitle = wxT("-X' (V) vs. X (V)");
    zoomable_graph = true;
}

// class destructor
XYPlot::~XYPlot()
{
    // insert your code here
}

void XYPlot::drawPlot() {
    static int times_called = 0;
    int x1,x2,x1_old,x2_old,x3;
    
    startDraw();
    float x_min, x_max;
    float y_min, y_max;
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
        graph_subtitle = wxT("-X' (V) vs. X (V)");
        y_min = smallest_y_value*3.3/1024;
        y_max = largest_y_value*3.3/1024;
        x_min = smallest_x_value*3.3/1024;
        x_max = largest_x_value*3.3/1024;
    } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
        graph_subtitle = wxT("-X' (V) vs. X (V)");
        y_min = smallest_y_value*3.3/1024 - 1.2;
        y_max = largest_y_value*3.3/1024 - 1.2;
        x_min = smallest_x_value*3.3/1024 - 1.2;
        x_max = largest_x_value*3.3/1024 - 1.2;
    } else {
        graph_subtitle = wxT("-X' (ADC) vs. X (ADC)");
        y_min = smallest_y_value;
        y_max = largest_y_value;
        x_min = smallest_x_value;
        x_max = largest_x_value;
    }
    
    drawYAxis(y_min, y_max, (y_max-y_min)/4.0);
              
    drawXAxis(x_min, x_max, (x_max-x_min)/4.0);

    if(device_connected == false) {
        endDraw();
        return;
    }
    
    //Use red pen
    wxPen pen(*wxRED, 1); // red pen of width 1
    buffer->SetPen(pen);
    
    // Get first plot point
    libchaos_getPlotPoint(&x1,&x2,&x3, 0);
    x1 = valueToX(x1);
    x2 = valueToY(x2);
    x1_old = x1;
    x2_old = x2;
    
    for(int i = 1; i < libchaos_getNumPlotPoints(); i++) {
        libchaos_getPlotPoint(&x1,&x2,&x3, i);
        x1 = valueToX(x1);
        x2 = valueToY(x2);
        if(x1 > side_gutter_size && 
           x2 < (graph_height+top_gutter_size) && x2 > top_gutter_size &&
           x1_old > side_gutter_size && 
           x2_old < (graph_height+top_gutter_size) && x2_old > top_gutter_size) {
            buffer->DrawLine(x1_old,x2_old,x1,x2);
        }
        x1_old = x1;
        x2_old = x2;
    }
    times_called = 0;
    
    // Display buffer
    endDraw();
}

int XYPlot::xToValue(int x) {
    return ((x - side_gutter_size)*(largest_x_value - smallest_x_value))/graph_width + smallest_x_value;
}

int XYPlot::valueToX(int value) {
    return (((value - smallest_x_value)*graph_width)/(largest_x_value - smallest_x_value)) + side_gutter_size;
}

int XYPlot::yToValue(int y) {
    return (((top_gutter_size + graph_height) - y)*(largest_y_value - smallest_y_value))/graph_height + smallest_y_value;
}

int XYPlot::valueToY(int value) {
    return (graph_height-((value-smallest_y_value)*graph_height)/(largest_y_value - smallest_y_value)) + top_gutter_size;
}

void XYPlot::zoomDefault() {
    largest_x_value = 1024;
    smallest_x_value = 0;
    smallest_y_value = 0;
    largest_y_value = 1024;
}
