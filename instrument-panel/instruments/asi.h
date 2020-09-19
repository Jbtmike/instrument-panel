#ifndef _ASI_H_
#define _ASI_H_

#include "instrument.h"

class asi : public instrument
{
private:
    const int FastPlaneSpeed = 180;
    const double FastPlaneSizeFactor = 1.075;

    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    int loadedAircraft;
    double airspeedCal;
    double airspeedKnots;
    double airspeedAngle = 0;
    double angle;

    double targetAirspeedAngle;
    double machAngle;
    double prevMachAngle = 248.14444;

    // Hardware knobs
    int calKnob = -1;
    int prevVal = 0;

public:
    asi(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void renderFast();
    void resize();
    void resizeFast();
    void updateFast();
    void addVars();
    void addKnobs();
    void updateKnobs();

};

#endif // _ASI_H
