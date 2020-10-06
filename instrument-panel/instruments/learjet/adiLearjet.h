#ifndef _ADI_LEARJET_H_
#define _ADI_LEARJET_H_

#include "simvars.h"
#include "instrument.h"

class adiLearjet : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (calculated from variables and needed to draw the instrument)
    double bankAngle = 0;
    double pitchAngle = 0;
    int adiCal = 0;
    int currentAdiCal = 0;
    int gyroSpinTime = 0;
    int failCount = 201;
    time_t lastPowerTime;

    // Hardware knobs
    int calKnob = -1;

public:
    adiLearjet(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
    void addKnobs();
    void updateKnobs();
};

#endif // _ADI_LEARJET_H
