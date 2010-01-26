/**
 * \file ChaosPlot.h
 * \brief Headers for ChaosPlot.cpp
 */

#ifndef CHAOSPLOT_H
#define CHAOSPLOT_H
#include <wx/dcbuffer.h>
#include <wx/panel.h>
#include <wx/wx.h>

class ChaosPlot : public wxPanel
{
    public:
        // class constructor
        ChaosPlot(wxWindow* parent, 
                   wxWindowID id = wxID_ANY, 
                   const wxPoint& pos = wxDefaultPosition, 
                   const wxSize& size = wxDefaultSize, 
                   long style = wxTAB_TRAVERSAL, 
                   const wxString& name = wxT("panel"));
        // class destructor
        virtual ~ChaosPlot();
        virtual void drawPlot() = 0;
        void startDraw();
        void endDraw();
        void drawYAxis(float bottom, float top, float interval);
        void drawXAxis(float bottom, float top, float interval);
        void setDeviceStatus(bool connected, int value);
        virtual void zoomDefault();
        void saveToFile(wxString filename);
        
        protected:
        virtual int xToValue(int x);
        virtual int yToValue(int y);
        virtual int valueToX(int value);
        virtual int valueToY(int value);
        void drawPoint(wxDC* buffer, int x, int y);
        bool save_to_file;
        wxString save_filename;
        bool zoomable_graph;
        
        // Variables used for zooming
        int largest_x_value;
        int smallest_x_value;
        int smallest_y_value;
        int largest_y_value;
        bool mouse_dragging;

        int device_mdac_value;
        bool device_connected;
        int height;
        int width;
        int graph_height;
        int graph_width;
        int side_gutter_size;
        int bottom_gutter_size;
        int top_gutter_size;
        int* mdac_value;
        wxString graph_title;
        wxString graph_subtitle;
        wxBufferedDC* buffer;
        wxBitmap* bmp;
        wxClientDC* dc;
        
        bool square;
        
        // Mouse location variables 
        wxPoint drag_start;
        wxPoint current_position;
    
        // Event handlers
        DECLARE_EVENT_TABLE();
        void OnMouseDown(wxMouseEvent& evt);
        virtual void OnMouseUp(wxMouseEvent& evt);
        void OnMouseMove(wxMouseEvent& evt);
};

#endif // CHAOSPLOT_H
