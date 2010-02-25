/**
 * \file ChaosSettings.h
 * \brief Headers for ChaosSettings.cpp
 */

#ifndef CHAOSSETTINGS_H
#define CHAOSSETTINGS_H

namespace ChaosSettings {
    /**
    *   Namespace for global settings in the GUI.
    *   These global settings are used by various components in the GUI
    *   for graphing and displaying information.
    */
    
    // Determines if the x axis of the bifurcation diagram should be mdac values or resistances (MDAC_VALUES, RESISTANCE_VALUES)
    extern int BifXAxis; 
    
    // Sets the number of MDAC steps that are plotted per window on the bifurcation diagram
    extern int BifStepsPerWindow;
    
    // Sets the number of peaks to collect per mdac value
    extern int PeaksPerMdac;
    
    // Sets the size of the points (SMALL_PT, MEDIUM_PT, LARGE_PT)
    extern int PointSize;
    
    // Sets the units for the Y axis on most graphs (Y_AXIS_VBIAS, Y_AXIS_VGND, Y_AXIS_ADC)
    extern int YAxisLabels;
    
    // Determines how many points should be collected when updating the GUI
    extern int PointsPerSample;
    
    // Determines how many points should dropped per sample
    extern int TransientPoints;
    
    // Determines how fast the GUI updates (measured in milliseconds)
    extern int UpdatePeriod;
    
    // Set to true if the user clicks the pause button to halt data collection
    extern bool Paused;
    
    // Determines if we need to completely redraw the bifurcation or if we can used the cached bitmap
    extern bool BifRedraw;
    
    // Version number for the PC software
    extern float Version;
    
    // Set to true if there is a bifurcation graph displayed, used to show/hide bifurcation controls
    extern bool BifVisible;
    
    // Initializes all variables to a default value
    extern void initSettings();
    
    // Enumerations used for setting values
    enum {
        MDAC_VALUES = 0,
        RESISTANCE_VALUES
    };
    
    enum {
        SMALL_PT = 0,
        MEDIUM_PT = 2,
        LARGE_PT = 3
    };
    
    enum {
        Y_AXIS_ADC = 0,
        Y_AXIS_VBIAS,
        Y_AXIS_VGND
    };
}

#endif
