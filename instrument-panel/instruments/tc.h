#ifndef _TC_H_
#define _TC_H_

#include "simvarDefs.h"
#include "instrument.h"

class tc : public instrument
{
private:
    SimVars* simVars;
    float scaleFactor;

    // Instrument values (caclulated from variables and needed to draw the instrument)
    double planeAngle = 0;
    double ballAngle = -64;      // Need to turn -90 degrees
    double targetAngle;

public:
    tc(int xPos, int yPos, int size);
    void render();
    void update();

private:
    void resize();
    void addVars();
};

#endif // _TC_H
