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
    zoomable_graph = true;
    point_count = 0;
    zoomDefault();
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
    static int old_mdac = 0;
    static int old_x_range = 0;
    static int old_y_range = 0;
    // Clear cache if necessary
    if(old_mdac != device_mdac_value ||
        old_x_range != (largest_x_value - smallest_x_value) ||
        old_y_range != (largest_y_value - smallest_y_value)) {
        
        // Reset cache
        point_count = 0;
        
        // Update old values
        old_mdac = device_mdac_value;
        old_x_range = (largest_x_value - smallest_x_value);
        old_y_range = (largest_y_value - smallest_y_value);
    }
    
    startDraw();
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_ADC) {
        graph_subtitle = wxT("Peak(n+1) (ADC) vs. Peak(n) (ADC)");
    } else{
        graph_subtitle = wxT("Peak(n+1) (V) vs. Peak(n) (V)");
    }
    
    float x_min, x_max;
    float y_min, y_max;
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
        y_min = smallest_y_value*3.3/1024;
        y_max = largest_y_value*3.3/1024;
        x_min = smallest_x_value*3.3/1024;
        x_max = largest_x_value*3.3/1024;
    } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
        y_min = smallest_y_value*3.3/1024 - 1.2;
        y_max = largest_y_value*3.3/1024 - 1.2;
        x_min = smallest_x_value*3.3/1024 - 1.2;
        x_max = largest_x_value*3.3/1024 - 1.2;
    } else {
        y_min = smallest_y_value;
        y_max = largest_y_value;
        x_min = smallest_x_value;
        x_max = largest_x_value;
    }
    
    drawYAxis(y_min, y_max, (y_max-y_min)/3.0);
              
    drawXAxis(x_min, x_max, (x_max-x_min)/3.0);
    
    buffer->DrawLine(side_gutter_size,graph_height+top_gutter_size,
                     side_gutter_size+graph_width,top_gutter_size);

    //Use blue pen
    wxPen bluePen(*wxBLUE, 1); // blue pen of width 1
    wxBrush blueBrush(*wxBLUE_BRUSH);
    
    // Use blue brush
    buffer->SetPen(bluePen);
    buffer->SetBrush(blueBrush);
    
    // Get points from library
    for(int i = 0; i < libchaos_getNumReturnMapPoints(); i++) {
        libchaos_getReturnMap1Point(&x,&y, i);
        
        // Check that we can store more points and that the point found is inside the window
        if( point_count < NUM_POINTS && 
            x > smallest_x_value && x < largest_x_value &&
            y > smallest_y_value && y < largest_y_value) {
            
            // Make sure we don't already have this point stored
            bool unique = true;
            for(int j = 0; j < point_count; j++)
            {
                if(points[j][0] == x && points[j][1] == y) {
                    unique = false;
                    break;
                }
            }
            
            // Add point to array
            if(unique) {
                points[point_count][0] = x;
                points[point_count][1] = y;
                point_count++;
            }
        }
    }
    
    if(libchaos_getNumReturnMapPoints() > 500) {
        libchaos_refreshReturnMapPoints();
    }

    for(int i = 0; i < point_count; i++) {
        drawPoint(buffer, 
                valueToX(points[i][0]), 
                valueToY(points[i][1]));
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

int Return1Plot::xToValue(int x) {
    /**
    *   Converts an x point on the graph to an ADC value.
    *   This is a very important function that is used by the parent class
    *   (ChaosPlot) for zooming.
    */
    if ( graph_width == 0) {
        return 1;
    }
    return ((x - side_gutter_size)*(largest_x_value - smallest_x_value))/graph_width + smallest_x_value;
}

int Return1Plot::valueToX(int value) {
    /**
    *   Converts an ADC value to an X coordinate on the graph.
    */
    return (((value - smallest_x_value)*graph_width)/(largest_x_value - smallest_x_value)) + side_gutter_size;
}

int Return1Plot::yToValue(int y) {
    /**
    *   Converts a y point on the graph to an ADC value.
    *   This is a very important function that is used by the parent class
    *   (ChaosPlot) for zooming.
    */
    if ( graph_height == 0) {
        return 1;
    }
    return (((top_gutter_size + graph_height) - y)*(largest_y_value - smallest_y_value))/graph_height + smallest_y_value;
}

int Return1Plot::valueToY(int value) {
    /**
    *   Converts an ADC value to a Y coordinate on the graph.
    */
    return (graph_height-((value-smallest_y_value)*graph_height)/(largest_y_value - smallest_y_value)) + top_gutter_size;
}

void Return1Plot::zoomDefault() {
    /**
    *   Resets the zoom to the default values.
    */
    largest_x_value = 1024;
    smallest_x_value = 0;
    smallest_y_value = 0;
    largest_y_value = 1024;
}
