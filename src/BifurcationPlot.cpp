/**
 * \file BifurcationPlot.cpp
 * \brief Bifurcation Plot Class
 */
  
#include "BifurcationPlot.h"

// class constructor
BifurcationPlot::BifurcationPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : ChaosPlot(parent, id, pos, size, style, name) {
    /**
    *   Constructor for the Bifurcation class
    *   This class inherits from the ChaosPlot class and redefines various
    *   settings and drawing functions to create a Bifurcation diagram of
    *   data collected from the Chaos Unit.
    */
    side_gutter_size = 20;
    bottom_gutter_size = 30;
    mouse_dragging = false;
    zoomDefault();
    Connect( wxID_ANY, wxEVT_LEFT_DCLICK,
                    (wxObjectEventFunction) &BifurcationPlot::OnDblClick );
    paused = ChaosSettings::Paused;
    graph_title = wxT("Bifurcation Plot");
    graph_subtitle = wxT("Peaks (V) vs. Mdac value");
    zoomable_graph = true;
    ChaosSettings::BifRedraw = true;
    bifBmp = NULL;
}

// class destructor
BifurcationPlot::~BifurcationPlot() {
    /**
    *   Deconstructor for the Bifurcation class
    */
}

void BifurcationPlot::drawPlot() {
    /**
    *   Main drawing function for the plot classes.
    *
    *   The bifurcation plot is stored in a wxBitmap object so that we don't
    *   have to recalculate and draw every point every time we refresh the
    *   screen.  Since all of the ChaosPlots are buffered, this means that
    *   the Bifurcation drawing functions actually go through two buffers.
    *   This is not the most efficient way to do this, but it does allow us
    *   to use the functionality of the ChaosPlot subclass.
    *
    *   Steps for drawing:
    *
    *   Calculate X & Y axis values and size.
    *
    *   Store our current size.
    *
    *   Calls startDraw() to initalize the drawing DC. (Updates size)
    *
    *   Compare old size with current size to determine if it needs to redraw everything.
    *
    *   If so, draw axis on BufferedDC and copy them over to the MemoryDC,
    *   then redraw all points.
    *
    *   If not, collect points for up to 2 MDAC values and only draw 
    *   the new points on the MemoryDC.
    *
    *   Draw the MDAC reference line on the graph.
    *
    *   Finally, copy the MemoryDC over to the BufferedDC and draw it to the DC.
    *
    */
    float x_min, x_max;
    float y_min, y_max;
    wxString xaxis_title;
    wxMemoryDC bifMemDC;
    
    // Get information for the X axis
    if(ChaosSettings::BifXAxis == ChaosSettings::MDAC_VALUES) {
        xaxis_title = wxString(wxT("Mdac values"));
    } else {
        xaxis_title = wxString(wxT("Resistance (Ohms)"));
    }

    // Get scaling/label information for the Y axis
    if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
        graph_subtitle = wxString::Format(wxT("Peaks (V) vs. %s"), xaxis_title.c_str());
        y_min = smallest_y_value*3.3/1024;
        y_max = largest_y_value*3.3/1024;
        x_min = smallest_x_value*3.3/1024;
        x_max = largest_x_value*3.3/1024;
    } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
        graph_subtitle = wxString::Format(wxT("Peaks (V) vs. %s"), xaxis_title.c_str());
        y_min = smallest_y_value*3.3/1024 - 1.2;
        y_max = largest_y_value*3.3/1024 - 1.2;
        x_min = smallest_x_value*3.3/1024 - 1.2;
        x_max = largest_x_value*3.3/1024 - 1.2;
    } else {
        graph_subtitle = wxString::Format(wxT("Peaks (ADC) vs. %s"), xaxis_title.c_str());
        y_min = smallest_y_value;
        y_max = largest_y_value;
        x_min = smallest_x_value;
        x_max = largest_x_value;
    }
    
    // Store old size information for future comparison
    int old_width = width;
    int old_height = height;
    
    // Update size and draw background & titles
    startDraw();
    
    // Check and see if the size of the graph has changed
    // If it has, we need to redraw our cached image
    if(old_width != width || old_height != height) {
        ChaosSettings::BifRedraw = true;
    }
    
    // Do we need to redraw everything that we have cached?
    if(ChaosSettings::BifRedraw == true) {
        // Draw the Y axis on the buffered DC
        drawYAxis(y_min, y_max, (y_max-y_min)/4.0);
        
        // Draw the X axis on the buffered DC
        if(ChaosSettings::BifXAxis == ChaosSettings::MDAC_VALUES) { 
            drawXAxis(float(largest_x_value),
                  float(smallest_x_value),
                  -1*((largest_x_value-smallest_x_value)/4));
        } else {
            float min = libchaos_mdacToResistance(largest_x_value);
            float max = libchaos_mdacToResistance(smallest_x_value);
            drawXAxis(min, max, ((max-min)/4));
        }
        
        // If we aren't connected, then we're done
        if(device_connected == false) {
            endDraw();
            return;
        }

        // if we already have a cached image, delete it
        if(bifBmp)
            delete bifBmp;
            
        // Create a bitmap to cache our bifurcation drawing to
        bifBmp = new wxBitmap(width, height);
        
        // Select the bitmap to a memory DC so we can draw on it and initialize it with a background
        bifMemDC.SelectObject(*bifBmp);
        bifMemDC.SetBrush(dc->GetBackground());
        bifMemDC.SetPen(*wxTRANSPARENT_PEN);
        bifMemDC.DrawRectangle(0,0,width,height);
        
        // Copy what we have on the buffer so far (title, axis, labels) to the cache
        bifMemDC.Blit(0, 0, width, height, buffer, 0, 0);
    } else {
        // No need to redraw everything, just select the cache so we can draw more onto it
        bifMemDC.SelectObject(*bifBmp);
    }

    //Use blue pen
    wxPen bluePen(*wxBLUE, 1); // blue pen of width 1
    wxBrush blueBrush(*wxBLUE_BRUSH);
    bifMemDC.SetPen(bluePen);
    bifMemDC.SetBrush(blueBrush);

    int* peaks;
    int new_points;
    int step = (int)(float(graph_width) / float(ChaosSettings::BifStepsPerWindow));
    
    if(step == 0) step = 1;
    
    // If we are paused, don't collect new data points
    if(paused || ChaosSettings::Paused) {
        new_points = 0;
    } else {
        new_points = 2;
        libchaos_disableFFT();
    }


    // Draw points, collecting new data if necessary.
    int miss_mdac = -1;
    for(int i = 1; i < graph_width; i+=step) {
        int mdac_value = xToMdac(i);
        
        /* Since the user can technically zoom into areas slightly beyond
        our mdac limits, we have to ensure we have a correct value. */
        if (mdac_value > 4095) {
            mdac_value = 4095;
        } else if (mdac_value < 0) {
            mdac_value = 0;
        }
        
        bool cacheHit = libchaos_peaksCacheHit(mdac_value);

        if(!cacheHit) new_points--;
        if((ChaosSettings::BifRedraw && cacheHit) || (!cacheHit && new_points > 0)) {

            peaks = libchaos_getPeaks(mdac_value);
            if(cacheHit == false) {
                miss_mdac = mdac_value;
            }
            
            bifMemDC.SetPen(bluePen);
            for(int j = 0; j < ChaosSettings::PeaksPerMdac; j++) {
                int y = valueToY(peaks[j]);
                if(y < graph_height + top_gutter_size && y > top_gutter_size) {
                    drawPoint(&bifMemDC, valueToX(mdac_value), y);
                }
            }
        }
        
    }
    
    if( new_points > 0 ) libchaos_enableFFT();
    
    if(miss_mdac != -1) {
        device_mdac_value = miss_mdac;
    }
    
    // We're finished redrawing everything, so don't do it again unless we need to
    if(ChaosSettings::BifRedraw == true) {
        ChaosSettings::BifRedraw = false;
    }

    // Copy our cache onto the buffered DC
    // (It is redundant to use a MemoryDC and a BufferedDC, it may be more efficient
    // to not use the BufferedDC for the bifurcation)
    buffer->Blit(0,0, width, height, &bifMemDC, 0, 0);
    
    // Draw the line for the MDAC
    drawMdacLine(buffer);
    
    // Flush the buffer and output to the screen.
    endDraw();

}

void BifurcationPlot::OnMouseUp(wxMouseEvent& evt) {
    /**
    *   Event handler for the mouse up on a plot.
    *   This event sets the zoom level for a graph based on the zooming
    *   rectangle drawn by the user.  This function will only zoom if the
    *   rectangle is greater than min_delta and if zooming will not give
    *   a higher difference between the minimum and maximum values than
    *   MAX_ZOOM.  
    *
    *   Overrides the MouseUp function in ChaosPlot because the
    *   Bifurcation plots the largest X value on the left.
    */
    const int min_delta = 10;
    const int MAX_ZOOM = 50;
    if(mouse_dragging == true) {
        int x_position = evt.m_x;
        int y_position = evt.m_y;
        if(drag_start.x < x_position && x_position - drag_start.x > min_delta) {
            int tmp2 = xToValue(drag_start.x);
            int tmp1 = xToValue(x_position);
            if(tmp2 - tmp1 > MAX_ZOOM) {
                smallest_x_value = tmp1;
                largest_x_value = tmp2;
                ChaosSettings::BifRedraw = true;
            }
        } else if(drag_start.x > x_position && drag_start.x - x_position > min_delta) {
            int tmp2 = xToValue(x_position);
            int tmp1 = xToValue(drag_start.x);
            if(tmp2 - tmp1 > MAX_ZOOM) {
                smallest_x_value = tmp1;
                largest_x_value = tmp2;
                ChaosSettings::BifRedraw = true;
            }
        }
        
        if(drag_start.y < y_position && y_position - drag_start.y > min_delta) {
            int tmp1 = yToValue(drag_start.y);
            int tmp2 = yToValue(y_position);
            if(tmp1 - tmp2 > MAX_ZOOM) {
                largest_y_value = tmp1;
                smallest_y_value = tmp2;
                ChaosSettings::BifRedraw = true;
            }
        } else if(drag_start.y > y_position && drag_start.y - y_position > min_delta) {
            int tmp1 = yToValue(y_position);
            int tmp2 = yToValue(drag_start.y);
            if(tmp1 - tmp2 > MAX_ZOOM) {
                largest_y_value = tmp1;
                smallest_y_value = tmp2;
                ChaosSettings::BifRedraw = true;
            }
        }
        
        wxLogMessage(wxT("y_min: %d\ny_max: %d"), smallest_y_value, largest_y_value);
        wxLogMessage(wxT("x_min: %d\nx_max: %d"), smallest_x_value, largest_x_value);
    }
    mouse_dragging = false;
}

void BifurcationPlot::drawMdacLine(wxDC* dc) {
    /**
    *   Draws the line for the MDAC on the graph so the user can see where
    *   they are on the bifurcation diagram.
    */
    //Use red pen
    wxPen redPen(*wxRED, 1); // red pen of width 1
    
    dc->SetPen(redPen);
    int x = valueToX(device_mdac_value);
    dc->DrawLine(x, top_gutter_size, x, graph_height+ top_gutter_size);
}

void BifurcationPlot::OnDblClick(wxMouseEvent& evt) {
    /**
    *   Event Handler for double clicking on the graph.
    *   Sets the MDAC value to the location click on the graph.
    */
    int value = xToValue(evt.m_x);
    libchaos_setMDACValue(value);
}

int BifurcationPlot::valueToX(int mdac_value) {
    /**
    *   Converts a mdac value to an X coordinate on the graph.
    */
    if(ChaosSettings::BifXAxis == ChaosSettings::MDAC_VALUES) {  
        return side_gutter_size + (int)(((largest_x_value - mdac_value)/float(largest_x_value-smallest_x_value))*graph_width);
    } else {
        float min = libchaos_mdacToResistance(smallest_x_value);
        float max = libchaos_mdacToResistance(largest_x_value);
        float mdac = libchaos_mdacToResistance(mdac_value);
        return side_gutter_size + (int)(((max - mdac)/float(max-min))*graph_width);
    }
}

int BifurcationPlot::xToValue(int x) {  
    /**
    *   Converts an X coordinate to either an MDAC value or a resistance 
    *   depending on the user settings.
    */
    if(ChaosSettings::BifXAxis == ChaosSettings::MDAC_VALUES) { 
        return largest_x_value - (int)((float(x - side_gutter_size)/graph_width)*float(largest_x_value-smallest_x_value));
    } else {
        float min = libchaos_mdacToResistance(largest_x_value);
        float max = libchaos_mdacToResistance(smallest_x_value);
        int resistance = libchaos_mdacToResistance(largest_x_value) + int((float(x - side_gutter_size)/graph_width)*float(max-min));
        int mdac = libchaos_resistanceToMdac(resistance);
        wxLogMessage(wxT("Resistance: %d MDAC: %d"), resistance, mdac);
        return mdac;
    }
}

int BifurcationPlot::xToMdac(int x) {
    /**
    *   Converts an X coordinate to an MDAC value regardless of the user settings.
    */
    int mdac_value;
    int mdac_step = (int)(float(largest_x_value-smallest_x_value) / float(ChaosSettings::BifStepsPerWindow));
    
    mdac_value = largest_x_value - (int)(float(largest_x_value-smallest_x_value)*(float(x)/(float)graph_width));
    
    if(mdac_step == 0) {
        mdac_step = 1;
    }
    
    mdac_value = mdac_value - (mdac_value % mdac_step);
    
    if(mdac_value < 0) mdac_value = 0;

    return mdac_value;
}

int BifurcationPlot::valueToY(int value) {
    /**
    *   Converts an ADC value to a  Y coordinate.
    */
    return (graph_height-((value-smallest_y_value)*graph_height)/(largest_y_value - smallest_y_value))  + top_gutter_size;
}

int BifurcationPlot::yToValue(int y) {
    /**
    *   Converts a Y coordinate to an ADC value.
    */
    if ( graph_height == 0) {
        return 1;
    }
    return ((graph_height + top_gutter_size - y)*(largest_y_value - smallest_y_value))/graph_height + smallest_y_value;
}

void BifurcationPlot::zoomDefault() {
    /**
    *   Resets the zooming on the graph to the default level.
    */
    largest_x_value = 4095;
    smallest_x_value = 0;
    smallest_y_value = 0;
    largest_y_value = 1024;
}

void BifurcationPlot::setPause(bool pause) {
    /**
    *   Pauses or unpauses the bifurcation based on the flag passed in.
    */
    paused = pause;
}

void BifurcationPlot::UpdateStatusBar(int m_x, int m_y) {
    /**
    *   Updates the cursor information in the status bar
    */
    float x, y;
    if(statusBar) {
        x = xToValue(m_x);
        y = yToValue(m_y);
        
        if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VBIAS) {
            y = y*3.3/1024 - 1.2;
        } else if(ChaosSettings::YAxisLabels == ChaosSettings::Y_AXIS_VGND) {
            y = y*3.3/1024;
        }
        
        if(ChaosSettings::BifXAxis == ChaosSettings::RESISTANCE_VALUES) {
            x = libchaos_mdacToResistance(x)/1000;
            statusBar->SetStatusText(wxString::Format(wxT("(%.3fk,%.3f)"),
                                        x,
                                        y), 3);
        } else {
            statusBar->SetStatusText(wxString::Format(wxT("(%d,%.3f)"),
                                        (int)x,
                                        y), 3);
        }
    }
}
