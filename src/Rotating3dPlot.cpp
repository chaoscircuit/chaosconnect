/**
 * \file Rotating3dPlot.cpp
 * \brief Implements class for a rotating 3d plot
 */

#include "Rotating3dPlot.h"
#include "ChaosSettings.h"

Rotating3dPlot::Rotating3dPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name) {
    /**
    *   Constructor for the rotating 3d plot. Initializes all settings and 
    *   creates a timer to control the rotation.
    */
    side_gutter_size = 20;
    bottom_gutter_size = 0;
    smallest_x_value = -218;
    smallest_y_value = 0;
    largest_x_value = 1024;
    largest_y_value = 1024;
    
    // Create timer
    timer1 = new wxTimer();
    timer1->SetOwner(this, ID_TIMER1);
    timer1->Start(200);
    
    Connect( ID_TIMER1, wxEVT_TIMER,
                    (wxObjectEventFunction) &Rotating3dPlot::timer1Timer );
    graph_title = wxT("Rotating Phase Portrait");
    graph_subtitle = wxT("-X' (V) vs. X (V)");
    zoomable_graph = true;
}

// class destructor
Rotating3dPlot::~Rotating3dPlot() {
    /**
    *   Deconstructor for the Rotating3dPlot class
    */
}

void Rotating3dPlot::drawPlot() {
    /**
    *   Main drawing function for the ChaosPlot classes.
    *
    *   Draws a rotating 3d plot by graphing xdot vs the formula:
    *       x*cos(a*n) + x''*sin(a*n)
    *
    *   In order to get this to look like it is rotating around it's axis
    *   instead of around the outside of a merry-go-round, we must subtract
    *   the bias from our ADC values before we calculate the rotation.
    *   bias = 2V/5V*1024 = 409
    */
    const float a = 2*3.14159/25;
    const int bias = 409;
    int x1,x2,x1_old,x2_old,x3;
    
    startDraw();
    float y_min, y_max;
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
        graph_subtitle = wxT("-X' (V) vs. X (V)");
        y_min = smallest_y_value*3.3/1024;
        y_max = largest_y_value*3.3/1024;
    } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
        graph_subtitle = wxT("-X' (V) vs. X (V)");
        y_min = smallest_y_value*3.3/1024 - 1.2;
        y_max = largest_y_value*3.3/1024 - 1.2;
    } else {
        graph_subtitle = wxT("-X' (ADC) vs. X (ADC)");
        y_min = smallest_y_value;
        y_max = largest_y_value;
    }

    drawYAxis(y_min, y_max, (y_max-y_min)/4.0);

    if(device_connected == false) {
        endDraw();
        return;
    }
    
    //Use red pen
    wxPen pen(*wxRED, 1); // red pen of width 1
    buffer->SetPen(pen);
    
    // Get first plot point
    libchaos_getPlotPoint(&x1,&x2,&x3, 0);
    
    // x1 = x*cos(a*n)+x3*sin(a*n), but we have to subtract the bias from it
    // so that it rotates around the origin, we then add bias so it is visible
    x1 = valueToX(int((x1 - bias)*cos(a*timer_ticks) + (x3-bias)*sin(a*timer_ticks))+bias);
    x2 = valueToY(x2);
    x1_old = x1;
    x2_old = x2;
    
    // Repeat the above math for all the other points and graph them.
    for(int i = 1; i < libchaos_getNumPlotPoints(); i++) {
        libchaos_getPlotPoint(&x1,&x2,&x3, i);
        x1 = valueToX(int((x1 - bias)*cos(a*timer_ticks) + (x3-bias)*sin(a*timer_ticks))+bias);
        x2 = valueToY(x2);
        if(x1 > side_gutter_size && 
           x2 < (graph_height + top_gutter_size) && x2 > top_gutter_size && 
           x1_old > side_gutter_size && 
           x2_old < (graph_height + top_gutter_size) && x2_old > top_gutter_size) {
            buffer->DrawLine(x1_old,x2_old,x1,x2);
        }
        x1_old = x1;
        x2_old = x2;
    }
    
    // Display buffer
    endDraw();
}

int Rotating3dPlot::valueToX(int value) {
    /**
    *   Converts an ADC value to an X coordinate on the graph. 
    *   This function does not attempt to rotate the point, that is handled
    *   in the actual drawing function.
    */
    return (((value - smallest_x_value)*graph_width)/(largest_x_value - smallest_x_value)) + side_gutter_size;
}

int Rotating3dPlot::valueToY(int value) {
    /**
    *   Converts an ADC value to a Y coordinate on the graph.
    */
    return (graph_height-((value-smallest_y_value)*graph_height)/(largest_y_value - smallest_y_value))  + top_gutter_size;
}


int Rotating3dPlot::xToValue(int x) {
    /**
    *   Converts an x point on the graph to an ADC value.
    *   This is a very important function that is used by the parent class
    *   (ChaosPlot) for zooming.
    */
    return ((x - side_gutter_size)*(largest_x_value - smallest_x_value))/graph_width + smallest_x_value;
}

int Rotating3dPlot::yToValue(int y) {
    /**
    *   Converts a y point on the graph to an ADC value.
    *   This is a very important function that is used by the parent class
    *   (ChaosPlot) for zooming.
    */
    return ((graph_height + top_gutter_size - y)*(largest_y_value - smallest_y_value))/graph_height + smallest_y_value;
}

void Rotating3dPlot::zoomDefault() {
    /**
    *   Resets the zoom to the default values.
    */
    largest_x_value = 1024;
    smallest_x_value = -218;
    smallest_y_value = 0;
    largest_y_value = 1024;
}

void Rotating3dPlot::timer1Timer(wxTimerEvent& event) {
    /**
    *   Event handler for the rotation timer.
    *   Timer for rotating the graph. All we need to do here is keep track
    *   of the ticks.
    */
    timer_ticks++;
}

void Rotating3dPlot::setPause(bool paused) {
    /**
    *   Pauses/unpauses the rotation of the graph by starting or stopping
    *   the timer.
    */
    if(paused == true) {
        timer1->Stop();
    } else {
        timer1->Start(200);
    }
}

void Rotating3dPlot::setRotation(int rotation) {
    /**
    *   Sets the rotation of the graph to a specified number. This number
    *   (between 1 and 25) represents the steps through a full rotation
    *   of the graph.
    */
    timer_ticks = rotation;
}
