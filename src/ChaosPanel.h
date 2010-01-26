/**
 * \file ChaosPanel.h
 * \brief Headers for ChaosPanel.cpp
 */

#ifndef CHAOSPANEL_H
#define CHAOSPANEL_H

#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/listctrl.h>
#include "wx/toolbar.h"

#include "ChaosPlot.h"
#include "BifurcationPlot.h"
#include "XTPlot.h"
#include "XYPlot.h"
#include "Return1Plot.h"
#include "Return2Plot.h"
#include "FFTPlot.h"
#include "Rotating3dPlot.h"
#include "Game.h"

class ChaosPanel : public wxPanel
{
    public:
        enum PlotTypes {
            CHAOS_BIFURCATION = 0,
            CHAOS_XY,
            CHAOS_XT,
            CHAOS_RETURN1,
            CHAOS_RETURN2,
            CHAOS_FFT,
            CHAOS_3D
        };
        
        // class constructor
        ChaosPanel(wxWindow* parent, 
                       wxWindowID id = wxID_ANY, 
                       const wxPoint& pos = wxDefaultPosition, 
                       const wxSize& size = wxDefaultSize, 
                       long style = wxTAB_TRAVERSAL, 
                       const wxString& name = wxT("panel"),
                       PlotTypes plot = CHAOS_XT);
        // class destructor
        ~ChaosPanel();
        void drawPlot();
        void Show();
        void Hide();
        ChaosPlot* getChaosPlot();
        
    private:
        void initGUI();
        void initNewPlot();
        void initToolbar();
        void clearPlotTools();
        void addXTTools();
        void addBifurcationTools();
        void add3dTools();
        
        //Event Handlers
        void OnChoice(wxCommandEvent& evt);
        void On3DPlayPause(wxCommandEvent& evt);
        void On3DSliderChange(wxScrollEvent& evt);
        void OnListRightClick(wxContextMenuEvent &evt);
        void OnMnuSaveToPNG(wxCommandEvent& evt);
        void OnMnuGame(wxCommandEvent& evt);
        void OnIdle(wxIdleEvent& evt);
        void OnPaint(wxPaintEvent& evt);
        void OnZoomDefault(wxCommandEvent& evt);
        void OnShowXTClick(wxCommandEvent& evt);
        void OnBifPlayPause(wxCommandEvent& evt);
        void OnMnuRecollect(wxCommandEvent& evt);
        
        // wxWidgets components
        wxChoice* graphChoice;
        wxFlexGridSizer* panelSizer;
        wxFlexGridSizer* plotSizer;
        wxBoxSizer* topSizer;
        ChaosPlot* plotPanel;
        wxToolBar* toolbar;
        int* mdac_value;
        
        // wxWidgets IDs
        enum {
            ID_CHOICE = 1000,
            ID_DEFAULT_ZOOM = 1001,
            ID_XT_X1,
            ID_XT_X2,
            ID_XT_X3,
            ID_BIF_PLAY,
            ID_MNU_SAVETOPNG,
            ID_MNU_GAME,
            ID_3D_SLIDER,
            ID_3D_PLAY,
            ID_MNU_RECOLLECT
        };
    protected:
        DECLARE_EVENT_TABLE()
        
        // class variables
        bool isShown;
        PlotTypes plotType;
};

#endif // CHAOSPANEL_H
