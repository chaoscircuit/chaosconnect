/**
 * \file ChaosPanel.cpp
 * \brief Contains class for the chaos panels which hold the 4 plots
 */

#include "ChaosPanel.h"
#include "ChaosSettings.h"
#include "../icons/zoom.xpm"
#include "../icons/bullet_red.xpm"
#include "../icons/bullet_green.xpm"
#include "../icons/bullet_blue.xpm"
#include "../icons/control_pause_blue.xpm"

BEGIN_EVENT_TABLE(ChaosPanel, wxPanel)
   EVT_PAINT(ChaosPanel::OnPaint)
   EVT_IDLE(ChaosPanel::OnIdle)
   EVT_CHOICE(ID_CHOICE, ChaosPanel::OnChoice)
   EVT_TOOL(ID_DEFAULT_ZOOM, ChaosPanel::OnZoomDefault)
   EVT_TOOL_RANGE(ID_XT_X1, ID_XT_X3, ChaosPanel::OnShowXTClick)
   EVT_TOOL(ID_BIF_PLAY, ChaosPanel::OnBifPlayPause)
   EVT_CONTEXT_MENU(ChaosPanel::OnListRightClick)
   EVT_MENU(ID_MNU_SAVETOPNG, ChaosPanel::OnMnuSaveToPNG)
   EVT_MENU(ID_MNU_GAME, ChaosPanel::OnMnuGame)
   EVT_MENU(ID_MNU_RECOLLECT, ChaosPanel::OnMnuRecollect)
   EVT_TOOL(ID_3D_PLAY, ChaosPanel::On3DPlayPause)
   EVT_COMMAND_SCROLL_THUMBTRACK(ID_3D_SLIDER, ChaosPanel::On3DSliderChange)
END_EVENT_TABLE()

ChaosPanel::ChaosPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style, const wxString& name, PlotTypes plot) 
                       : wxPanel(parent, id, pos, size, style, name)
{
    /**
    *   Constructor for the Chaos Panel, initializes the user interface
    */
    plotPanel = NULL; 
    plotType = plot;
    wxLogMessage(wxT("Creating panel with plot type: %d"), plotType);
    initGUI();
    this->SetExtraStyle(wxWS_EX_PROCESS_IDLE);
    Hide();
}

// class destructor
ChaosPanel::~ChaosPanel() {
    /**
    *   Destructor for the Chaos Panel
    */
}

void ChaosPanel::initGUI() {
    /**
    *   Initializes the sizers & controls on the Chaos Panel
    */
    // Create toolbar
    initToolbar();

    switch(plotType) {
    case CHAOS_BIFURCATION:
        graphChoice->SetSelection(0);
        break;
    case CHAOS_XY:
        graphChoice->SetSelection(1);
        break;
    case CHAOS_XT:
        graphChoice->SetSelection(2);
        break;
    case CHAOS_RETURN1:
        graphChoice->SetSelection(3);
        break;
    case CHAOS_RETURN2:
        graphChoice->SetSelection(4);
        break;
    default:
        graphChoice->SetSelection(1);
        break;
    }
    
    // Sizers
    panelSizer = new wxFlexGridSizer(1);
    topSizer = new wxBoxSizer(wxHORIZONTAL);
    plotSizer = new wxFlexGridSizer(1);
    
    topSizer->Add(toolbar, 1, wxEXPAND, 2);
    
    plotSizer->AddGrowableRow(0);
    plotSizer->AddGrowableCol(0);    
    
    panelSizer->Add(topSizer, 0, wxEXPAND | wxALL);
    panelSizer->Add(plotSizer, 0, wxEXPAND | wxALL, 2);
    
    panelSizer->AddGrowableRow(1);
    panelSizer->AddGrowableCol(0);
    
    this->SetSizer(panelSizer);
    this->SetAutoLayout(true);
    this->Layout();

    //ChaosPlot panel
    initNewPlot();
}

void ChaosPanel::initToolbar() {
    /** 
    *   Initializes the tool bar for the Chaos Panel. Tools can then be added
    *   and removed from this toolbar based on which graph are being displayed.
    *   The only tool added by this function is the zoom default button.
    */
    toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);

    // Graph Choice box
    wxArrayString choices;
    choices.Add(wxT("Bifurcation"));
    choices.Add(wxT("XY"));
    choices.Add(wxT("XT"));
    choices.Add(wxT("Return Map 1"));
    choices.Add(wxT("Return Map 2"));
    choices.Add(wxT("FFT"));
    choices.Add(wxT("3D Plot"));
    
    graphChoice = new wxChoice(toolbar, ID_CHOICE, wxPoint(25, 5), wxSize(120, 21), choices, 0, wxDefaultValidator, wxT("graphChoice"));
    
    // Set up toolbar
    wxBitmap* toolbarBitmaps[1];
    toolbarBitmaps[0] = new wxBitmap(zoom_xpm);

    toolbar->AddControl(graphChoice);
    toolbar->AddSeparator();
    toolbar->AddTool(ID_DEFAULT_ZOOM, wxT("Default Zoom"), *toolbarBitmaps[0], wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    toolbar->AddSeparator();
    toolbar->Realize();
    
    // Can delete the bitmaps since they're reference counted
    int i;
    for (i = 0; i < 1; i++)
        delete toolbarBitmaps[i];
}

void ChaosPanel::drawPlot() {
    /**
    *   Ensures that we have a valid plotPanel and then passes 
    *   the draw command on to the ChaosPlot object
    */
    if(plotPanel) {
        plotPanel->drawPlot();
    }
}

void ChaosPanel::initNewPlot() {
    /**
    *   Destroys the current plotPanel and creates a new one based
    *   on the choice selected in the dropdown box.
    */
    
    // Get rid of current plot
    if(plotPanel != NULL) {
        plotSizer->Detach(plotPanel);
        plotPanel->Destroy();
        plotPanel = NULL;
    }
    
    if(plotType == CHAOS_BIFURCATION) {
        ChaosSettings::BifVisible = false;
    }
    
    // Create a new plot panel based on selection and set the title
    plotType = PlotTypes(graphChoice->GetSelection());
    clearPlotTools();
    switch(plotType) {
        case CHAOS_BIFURCATION:
            plotPanel = new BifurcationPlot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            addBifurcationTools();
            ChaosSettings::BifVisible = true;
            break;
        case CHAOS_XY:
            plotPanel = new XYPlot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            break;
        case CHAOS_XT:
            plotPanel = new XTPlot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            addXTTools();
            break;
        case CHAOS_RETURN1:
            plotPanel = new Return1Plot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            break;
        case CHAOS_RETURN2:
            plotPanel = new Return2Plot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            break;
        case CHAOS_FFT:
            plotPanel = new FFTPlot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            break;
        case CHAOS_3D:
            plotPanel = new Rotating3dPlot(this, wxID_ANY, wxPoint(5, 30), wxSize(200, 100));
            add3dTools();
            break;
        default:
            break;
    }
    
    // If we created a new panel, add it to the sizer
    if(plotPanel != NULL) {
        plotSizer->Add(plotPanel, 0, wxEXPAND | wxALL, 2);
        plotSizer->Layout();
        plotPanel->setStatusBar(statusBar);
    }
}

void ChaosPanel::OnChoice(wxCommandEvent& evt) {
    /**
    *   Event handler for the dropdown box. Creates a new plot of the
    *   desired type.
    */
    
    if(PlotTypes(graphChoice->GetSelection()) == CHAOS_BIFURCATION && ChaosSettings::BifVisible) {
        wxLogMessage("Cannot display more than one bifurcation graph.");
        graphChoice->SetSelection(plotType);
        return;
    }
    
    initNewPlot();
}

void ChaosPanel::OnIdle(wxIdleEvent& evt) {
    /**
    *   Event handler for idle events.
    *   If we don't have anything else we need to do, draw our plot.
    */
    
    if(plotPanel && isShown) {
        drawPlot();
        if(plotType == CHAOS_BIFURCATION) {
            evt.RequestMore();
        }
    }
}

void ChaosPanel::OnPaint(wxPaintEvent& evt) {
    /**
    *   Event handler for painting the ChaosPanel
    *   Redraws the plot
    */
    wxPaintDC dc(this);
    if(isShown) {
        drawPlot();
    }
}

void ChaosPanel::Show() {
    /**
    *   Informs the panel that it is being shown
    */
    
    if(isShown == false && ChaosSettings::BifVisible == true && plotType == CHAOS_BIFURCATION) {
        graphChoice->SetSelection(2);
        initNewPlot();
        ChaosSettings::BifVisible = true;
    }
    
    isShown = true;
    
    if(plotType == CHAOS_BIFURCATION) {
        ChaosSettings::BifVisible = true;
    }
}

void ChaosPanel::Hide() {
    /**
    *   Informs the panel that it is not being shown
    */
    isShown = false;
    if(plotType == CHAOS_BIFURCATION) {
        ChaosSettings::BifVisible = false;
    }
}

ChaosPlot* ChaosPanel::getChaosPlot() {
    /**
    *   Returns the panel that is being displayed. This is used by the
    *   main window to pass status information to the individual plots.
    */
    return plotPanel;
}

void ChaosPanel::OnZoomDefault(wxCommandEvent& evt) {
    /**
    *   Calls the appropriate zoom function on the chaos plot to reset
    *   the display.  If this was a bifurcation, it sets the redraw flag.
    */
    if(plotPanel)
        plotPanel->zoomDefault();
        
    if(plotType == CHAOS_BIFURCATION) {
        ChaosSettings::BifRedraw = true;
    }
}

void ChaosPanel::clearPlotTools() {
    /**
    *   Removes all of the specialized plot tools from the toolbar so that
    *   we have a clean slate to add the appropriate ones to.
    */
    toolbar->RemoveTool(ID_XT_X1);
    toolbar->RemoveTool(ID_XT_X2);
    toolbar->RemoveTool(ID_XT_X3);
    toolbar->RemoveTool(ID_BIF_PLAY);
    toolbar->RemoveTool(ID_3D_PLAY);
    toolbar->RemoveTool(ID_3D_SLIDER);
}

void ChaosPanel::addXTTools() {
    /**
    *   Adds the toolbar buttons for the XT graph
    *   These consist of toggle buttons for each of the 3 inputs (X, X', X'')
    */
    wxBitmap* toolbarBitmaps[3];
    toolbarBitmaps[0] = new wxBitmap(bullet_red_xpm);
    toolbarBitmaps[1] = new wxBitmap(bullet_blue_xpm);
    toolbarBitmaps[2] = new wxBitmap(bullet_green_xpm);

    toolbar->AddCheckTool(ID_XT_X1, wxT("View X"), *toolbarBitmaps[0], wxNullBitmap, wxT("View X"));
    toolbar->AddCheckTool(ID_XT_X2, wxT("View Xdot"), *toolbarBitmaps[1], wxNullBitmap, wxT("View Xdot"));
    toolbar->AddCheckTool(ID_XT_X3, wxT("View Xdotdot"), *toolbarBitmaps[2], wxNullBitmap, wxT("View Xdotdot"));

    toolbar->ToggleTool(ID_XT_X1, true);
    toolbar->Realize();

    // Can delete the bitmaps since they're reference counted
    int i;
    for (i = 0; i < 3; i++)
        delete toolbarBitmaps[i];
}

void ChaosPanel::addBifurcationTools() {
    /**
    *   Adds the toolbar buttons for the Bifurcation graph
    *   This consists of a play/pause toggle button
    */
    wxBitmap* toolbarBitmaps[1];
    toolbarBitmaps[0] = new wxBitmap(control_pause_blue_xpm);

    toolbar->AddCheckTool(ID_BIF_PLAY, wxT("Play/Pause Bifurcation"), *toolbarBitmaps[0], wxNullBitmap, wxT("Play/Pause Bifurcation"));

    if(ChaosSettings::Paused) {
        toolbar->ToggleTool(ID_BIF_PLAY, true);
    }
    toolbar->Realize();

    // Can delete the bitmaps since they're reference counted
    int i;
    for (i = 0; i < 1; i++)
        delete toolbarBitmaps[i];
}

void ChaosPanel::add3dTools() {
    /**
    *   Adds the toolbar buttons for the 3d graph
    *   These consist of a play/pause toggle button as well as a slider
    *   to manually control the rotation of the graph.
    */
    wxBitmap* toolbarBitmaps[1];
    toolbarBitmaps[0] = new wxBitmap(control_pause_blue_xpm);
    
    toolbar->AddCheckTool(ID_3D_PLAY, wxT("Play/Pause Rotation"), *toolbarBitmaps[0], wxNullBitmap, wxT("Play/Pause Rotation"));
    toolbar->AddControl(new wxSlider(toolbar, ID_3D_SLIDER, 0, 0, 25));
    
    toolbar->Realize();

    // Can delete the bitmaps since they're reference counted
    int i;
    for (i = 0; i < 1; i++)
        delete toolbarBitmaps[i];
}

void ChaosPanel::OnShowXTClick(wxCommandEvent& evt) {
    /**
    *   Event handler for the XT toolbar buttons.
    *   Shows/hides X, X', X'' on the XT graph depending on the state of the buttons.
    */
    ((XTPlot*)plotPanel)->setX1Visibility(toolbar->GetToolState(ID_XT_X1));
    ((XTPlot*)plotPanel)->setX2Visibility(toolbar->GetToolState(ID_XT_X2));
    ((XTPlot*)plotPanel)->setX3Visibility(toolbar->GetToolState(ID_XT_X3));
}

void ChaosPanel::OnBifPlayPause(wxCommandEvent& evt) {
    /**
    *   Event handler for the Bifurcation play/pause button
    *   Pauses/unpauses the bifurcation so that we don't hinder the user as they
    *   attempt to collect other data.
    */
    ((BifurcationPlot*)plotPanel)->setPause(toolbar->GetToolState(ID_BIF_PLAY));
}

void ChaosPanel::OnListRightClick(wxContextMenuEvent& evt)
{
    /**
    *   Event handler for right clicking on a graph.
    *   Among other things, this function pops up a menu that gives the 
    *   user the ability to save the graph to an image file.
    */
    wxMenu mnu;
    mnu.Append(ID_MNU_SAVETOPNG, wxT("Save Graph to PNG..."));
    if ( plotType == CHAOS_RETURN2 && libchaos_getMDACValue() == 1337) {
        mnu.Append(ID_MNU_GAME, wxT("Game..."));
    }
    if (plotType == CHAOS_BIFURCATION) {
        mnu.Append(ID_MNU_RECOLLECT, wxT("Recollect Bifurcation Data"));
    }
    
    PopupMenu(&mnu);
}

void ChaosPanel::OnMnuGame(wxCommandEvent& evt) {
    /**
    *   Event handler for the Game menu click
    *   Lets the user play a game
    */
    srand(time(NULL));
    Game *game = new Game(wxT("Game"));
    game->Centre();
    game->Show(true);
}

void ChaosPanel::OnMnuRecollect(wxCommandEvent& evt) {
    /**
    *   Event handler for the data recollection menu click
    *   Clears the Bifurcation data cache, forcing the data to be recollected.
    */
    libchaos_setPeaksPerMDAC(ChaosSettings::PeaksPerMdac);
    ChaosSettings::BifRedraw = true;
}

void ChaosPanel::OnMnuSaveToPNG(wxCommandEvent& evt) {
    /**
    *   Event handler for the save to image menu click
    *   Prompts the user for a location and then saves the graph as a PNG image
    */
    wxFileDialog dialog(this,
                        _T("Save image file"),
                        wxEmptyString,
                        _T("image.png"),
                        _T("PNG files (*.png)|*.png"),
                        wxSAVE|wxOVERWRITE_PROMPT);
    dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {
        plotPanel->saveToFile(dialog.GetPath());
    }
}

void ChaosPanel::On3DPlayPause(wxCommandEvent& evt) {
    /**
    *   Event handler for the 3d play/pause toolbar button
    *   Pauses/unpauses the rotation of the 3d graph.
    */
    ((Rotating3dPlot*)plotPanel)->setPause(toolbar->GetToolState(ID_3D_PLAY));
}

void ChaosPanel::On3DSliderChange(wxScrollEvent& evt) {
    /**
    *   Event handler for the 3d toolbar slider
    *   The user can control the rotation of the 3d graph using this slider.
    *   The automatic rotation of the 3d graph is automatically halted when this
    *   slider is changed.
    */
    ((Rotating3dPlot*)plotPanel)->setRotation(((wxSlider*)toolbar->FindControl(ID_3D_SLIDER))->GetValue());
    toolbar->ToggleTool(ID_3D_PLAY, true);
    ((Rotating3dPlot*)plotPanel)->setPause(true);
}

void ChaosPanel::setStatusBar(wxStatusBar *s) {
    /**
    *   Gives the panel access to the status bar so it can display cursor
    *   coordinates
    */
    statusBar = s;
    if(plotPanel)
        plotPanel->setStatusBar(statusBar);
}
