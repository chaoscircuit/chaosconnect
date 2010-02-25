/**
 * \file ChaosPlot.cpp
 * \brief Contains class for plotting
 */
 
#include "ChaosPlot.h"
#include "ChaosSettings.h"

BEGIN_EVENT_TABLE(ChaosPlot, wxPanel)
   EVT_LEFT_DOWN(ChaosPlot::OnMouseDown)
   EVT_LEFT_UP(ChaosPlot::OnMouseUp)
   EVT_MOTION(ChaosPlot::OnMouseMove)
END_EVENT_TABLE()

// class constructor
ChaosPlot::ChaosPlot(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name) 
                       : wxPanel(parent, id, pos, size, style, name) {
    /**
    *   Constructor for the ChaosPlot class
    *   Sets up a default graph with generic sizes/titles
    */
    dc = NULL;
    device_connected = false;
    largest_x_value = 1000;
    smallest_x_value = 0;
    smallest_y_value = 0;
    largest_y_value = 1000;
    mouse_dragging = false;
    square = false;
    top_gutter_size = 40;
    graph_title = wxT("Graph Title");
    graph_subtitle = wxT("Graph Subtitle");
    save_to_file = false;
    zoomable_graph = false;
}

// class destructor
ChaosPlot::~ChaosPlot() {
    /**
    *   Deconstructor for the ChaosPlot class
    *   All wxWidgets objects are deleted by the class using the Destory method
    */

}

void ChaosPlot::startDraw() {
    /**
    *   Begins the drawing of a graph by setting up the buffers, drawing 
    *   the graphing rectangle, and placing a title on the graph.
    */
    // update the sizes
    this->GetSize(&width, &height);
    graph_width = width - side_gutter_size;
    graph_height = height - bottom_gutter_size - top_gutter_size;
    
    if(dc) delete dc;
    dc = new wxClientDC(this);

    // Initialize the buffers
    bmp = new wxBitmap();

    bmp->SetHeight(height);
    bmp->SetWidth(width);

    buffer = new wxBufferedDC(dc, *bmp);

    // Update drawing area
    int txt_width, txt_height;
    buffer->GetTextExtent(graph_title, &txt_width, &txt_height);
    top_gutter_size = 2*txt_height + 5;

    // Return plots are graphed in a square window
    if( square ) {
        if ( graph_width > graph_height ) {
            graph_width = graph_height;
        } else {
            graph_height = graph_width;
        }
        side_gutter_size = width/2 - graph_width/2;
    }
    
    // Clear data and draw our rectangle
    wxPen axisPen(*wxLIGHT_GREY, 1);
    buffer->SetPen(axisPen);
    buffer->SetBackground(dc->GetBackground());

    buffer->Clear();
    buffer->DrawRectangle(side_gutter_size, 
                          top_gutter_size, 
                          graph_width, 
                          graph_height + 1);
                          
    // Draw our graph title
    buffer->DrawText(graph_title, (width/2) - (txt_width/2), 2);
    buffer->GetTextExtent(graph_subtitle, &txt_width, &txt_height);
    buffer->DrawText(graph_subtitle, (width/2) - (txt_width/2), 4+txt_height);
    
}

void ChaosPlot::endDraw() {
    /**
    *   Ends a drawing by drawing the zooming rectangle if necessary,
    *   flushing the buffer, and saving the graph to a file if requested.
    */    
    if(zoomable_graph == true && mouse_dragging == true) {
        buffer->SetPen(wxPen(*wxGREEN, 1));
        buffer->SetBrush(wxBrush(*wxGREEN, wxTRANSPARENT));
        buffer->DrawRectangle(drag_start.x, drag_start.y,
                              current_position.x - drag_start.x,
                              current_position.y - drag_start.y);
    }
    
    if(save_to_file == true) {
        // Create bitmap and DC
        wxBitmap* pictureBmp = new wxBitmap(width, height);
        wxMemoryDC* pictureDC = new wxMemoryDC(*pictureBmp);
        save_to_file = false;
        
        // Copy the picture to the memory DC
        pictureDC->Blit(0, 0, width, height, buffer, 0, 0);
        
        // Save file
        wxInitAllImageHandlers();
        pictureBmp->SaveFile(save_filename, wxBITMAP_TYPE_PNG);
        
        wxLogMessage(wxT("Saving image file: %s"), save_filename.c_str());
        delete pictureDC;
        delete pictureBmp;
    }

    delete buffer;
    delete bmp;
}

void ChaosPlot::drawYAxis(float bottom, float top, float interval) {
    /**
    *   Draws the Y-Axis on the graph using the specified minimum and maximum
    *   values as well as the interval.
    *
    *   Uses integers as axis labels unless the interval is small, then uses
    *   floats out to 1 decimal place.
    *
    *   Also draws lines across the graph at the label values
    */
    wxString buf;
    
    wxPen axisPen(*wxLIGHT_GREY, 1);
    buffer->SetPen(axisPen);
    
    float scale = graph_height/fabs(top - bottom);
    
    int num_labels = (int)(fabs(top - bottom)/fabs(interval));

    for(int i = 0; i <= num_labels; i++ ) {
        if(fabs(int(interval) - interval) > .05 && abs(int(interval)) < 2.0) {
            buf = wxString::Format(wxT("%.1f"),(bottom + i*interval));
        } else {
            buf = wxString::Format(wxT("%d"),int(bottom + i*interval));
        }
       
        int x = 0;
        int y = (int)(graph_height-scale*i*fabs(interval) + top_gutter_size);
        
        buffer->DrawText(buf,
                         side_gutter_size-15+x,
                         y);
        buffer->DrawLine(side_gutter_size-5,
                         y,
                         x+graph_width+side_gutter_size,
                         y);
    }
}

void ChaosPlot::drawXAxis(float bottom, float top, float interval) {
    /**
    *   Draws the X-Axis on the graph using the specified minimum and maximum
    *   values as well as the interval.
    *
    *   Uses integers as axis labels unless the interval is small, then uses
    *   floats out to 1 decimal place.
    *
    *   Also draws lines across the graph at the label values
    */
    wxString buf;
    
    wxPen axisPen(*wxLIGHT_GREY, 1);
    buffer->SetPen(axisPen);
    
    float scale = graph_width/fabs(top - bottom);
    
    int num_labels = (int)(fabs(top - bottom)/fabs(interval));
    
    for(int i = 0; i <= num_labels; i++ ) {
        if(fabs(int(interval) - interval) > .05 && abs(int(interval)) < 2.0) {
            buf = wxString::Format(wxT("%.1f"),(bottom + i*interval));
        } else {
            buf = wxString::Format(wxT("%d"),int(bottom + i*interval));
        }
        
        int x = (int)(scale*i*fabs(interval) + side_gutter_size);
        int y = (int)(graph_height + top_gutter_size);
        
        buffer->DrawText(buf,
                         x,
                         y+5);
        buffer->DrawLine(x,
                         y+5,
                         x,
                         top_gutter_size);
    }
}

void ChaosPlot::setDeviceStatus(bool connected, int value) {
    /**
    *   Recieves information about the device and stores it locally.  
    *   This is used by graphs to determine if they should draw data or not.
    *   and gives them the ability to read the current mdac value.
    */
    device_connected = connected;
    device_mdac_value = value;
}

void ChaosPlot::OnMouseDown(wxMouseEvent& evt) {
    /**
    *   Event handler for the mouse down on a plot.
    *   This event sets the start of a zooming rectangle that is drawn as
    *   the user holds down the mouse and drags.
    */
    evt.Skip();
    mouse_dragging = true;
    current_position = drag_start = evt.GetPosition();
}

void ChaosPlot::OnMouseUp(wxMouseEvent& evt) {
    /**
    *   Event handler for the mouse up on a plot.
    *   This event sets the zoom level for a graph based on the zooming
    *   rectangle drawn by the user.  This function will only zoom if the
    *   rectangle is greater than min_delta and if zooming will not give
    *   a higher difference between the minimum and maximum values than
    *   MAX_ZOOM.
    */
    const int min_delta = 10;
    const int MAX_ZOOM = 25;
    if(mouse_dragging == true) {
        int x_position = evt.m_x;
        int y_position = evt.m_y;
        
        // Initialize tmp1 and tmp2
        int tmp1 = smallest_x_value;
        int tmp2 = largest_x_value;

        // Calculate the new X boundaries based on if we dragged a box left or right
        if(drag_start.x < x_position && x_position - drag_start.x > min_delta) {
            tmp1 = xToValue(drag_start.x);
            tmp2 = xToValue(x_position);
        } else if(drag_start.x > x_position && drag_start.x - x_position > min_delta) {
            tmp1 = xToValue(x_position);
            tmp2 = xToValue(drag_start.x);
        }
        
        // Update the largest and smallest values
        if(tmp2 - tmp1 > MAX_ZOOM) {
            smallest_x_value = tmp1;
            largest_x_value = tmp2;
        }
        
        // Calculate the new Y boundaries based on if we dragged a box up or down
        if(drag_start.y < y_position && y_position - drag_start.y > min_delta) {
            tmp1 = yToValue(drag_start.y);
            tmp2 = yToValue(y_position);
        } else if(drag_start.y > y_position && drag_start.y - y_position > min_delta) {
            tmp1 = yToValue(y_position);
            tmp2 = yToValue(drag_start.y);
        }
        
        // Update the largest and smallest values
        if(tmp1 - tmp2 > MAX_ZOOM) {
            largest_y_value = tmp1;
            smallest_y_value = tmp2;
        }
        
        // Write zoom to log
        wxLogMessage(wxT("y_min: %d\ny_max: %d"), smallest_y_value, largest_y_value);
        wxLogMessage(wxT("x_min: %d\nx_max: %d"), smallest_x_value, largest_x_value);
    }
    
    // We aren't dragging the mouse anymore
    mouse_dragging = false;
}

void ChaosPlot::OnMouseMove(wxMouseEvent& evt) {
    /**
    *   Event handler for when the mouse is moving over the plot.
    *   This updates the position so we can draw our zooming rectangle.
    */
    if(mouse_dragging == true) {
        current_position = evt.GetPosition();
    }
    if(statusBar) {
        statusBar->SetStatusText(wxString::Format(wxT("(%d,%d)"), 
                                        xToValue(evt.m_x),
                                        yToValue(evt.m_y)), 3);
    }
}

int ChaosPlot::xToValue(int x) {
    /**
    *   Dummy function to convert from an on screen X coordinate to a 
    *   meaningful value. Overloaded in classes inherited from this one.
    */
    return x;
}

int ChaosPlot::yToValue(int y) {
    /**
    *   Dummy function to convert from an on screen Y coordinate to a 
    *   meaningful value. Overloaded in classes inherited from this one.
    */
    return y;
}

int ChaosPlot::valueToX(int value) {
    /**
    *   Dummy function to convert from a meaningful value to an on screen 
    *   X coordinate. Overloaded in classes inherited from this one.
    */
    return value;
}

int ChaosPlot::valueToY(int value) {
    /**
    *   Dummy function to convert from a meaningful value to an on screen 
    *   Y coordinate. Overloaded in classes inherited from this one.
    */
    return value;
}

void ChaosPlot::zoomDefault() {
    /**
    *   Dummy function to return graph to it's default zooming level.
    *   Overloaded in classes inherited from this one.
    */
    return;
}

void ChaosPlot::drawPoint(wxDC* buffer, int x, int y) {
    /**
    *   Draws a point at the given (X,Y) coordinate on the given DC using
    *   the point size specified in the settings.  Use this so you don't
    *   have to check the point size yourself when drawing a point.
    */
    if(ChaosSettings::PointSize == ChaosSettings::SMALL_PT) {
        buffer->DrawPoint(x, y);
    } else {
        buffer->DrawCircle(x, y, ChaosSettings::PointSize);
    }
}

void ChaosPlot::saveToFile(wxString filename) {
    /**
    *   Sets the flags required to save the graph to a file.  The graph 
    *   will be saved next time it is drawn.
    */
    save_to_file = true;
    save_filename = filename;
}

void ChaosPlot::setStatusBar(wxStatusBar *s) {
    /**
    *   Gives the plot access to the status bar so it can display cursor
    *   coordinates
    */
    statusBar = s;
}
