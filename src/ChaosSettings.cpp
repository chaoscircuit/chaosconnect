/**
 * \file ChaosSettings.cpp
 * \brief Settings namespace
 */

#include "ChaosSettings.h"

namespace ChaosSettings {
    /**
    *   Global settings for the chaos application.  This namespace allows all
    *   parts of the program to access the user settings for drawing.
    */
    int BifXAxis;
    int BifStepsPerWindow;
    int PeaksPerMdac;
    int PointSize;
    int YAxisLabels;
    int PointsPerSample;
    int TransientPoints;
    int UpdatePeriod;
    bool Paused;
    float Version;
    bool BifRedraw;
    
    void initSettings() {
        /**
        *   Initializes the settings to their default value.  It might be
        *   nice in the future for this function to read from a configuration
        *   file.
        */
        BifXAxis = MDAC_VALUES;
        BifStepsPerWindow = 50;
        PeaksPerMdac = 10;
        PointSize = MEDIUM_PT;
        YAxisLabels = Y_AXIS_VBIAS;
        PointsPerSample = 2040;
        TransientPoints = 4;
        UpdatePeriod = 300;
        Paused = false;
        BifRedraw = true;
        Version = 1.003;
    }
}
