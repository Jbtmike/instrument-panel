#include <stdio.h>
#include <stdlib.h>
#include "nav.h"
#include "knobs.h"

nav::nav(int xPos, int yPos, int size) : instrument(xPos, yPos, size)
{
    setName("Nav");
    addVars();
    simVars = &globals.simVars->simVars;

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        addKnobs();
    }
#endif

    resize();
}

/// <summary>
/// Destroy and recreate all bitmaps as instrument has been resized
/// </summary>
void nav::resize()
{
    destroyBitmaps();

    // Create bitmaps scaled to correct size
    scaleFactorX = size / 1600.0f;
    scaleFactorY = size / 2000.0f;

    // 0 = Original (loaded) bitmap
    ALLEGRO_BITMAP* orig = loadBitmap("nav.png");
    addBitmap(orig);

    if (bitmaps[0] == NULL) {
        return;
    }

    // 1 = Destination bitmap (all other bitmaps get assembled to here)
    ALLEGRO_BITMAP* bmp = al_create_bitmap(size, 400 * scaleFactorY);
    addBitmap(bmp);

    // 2 = Main Nav
    bmp = al_create_bitmap(size, 400 * scaleFactorY);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 0, 1600, 400, 0, 0, size, 400 * scaleFactorY, 0);
    addBitmap(bmp);

    // 3 = Main Autopilot
    bmp = al_create_bitmap(size, 400 * scaleFactorY);
    al_set_target_bitmap(bmp);
    al_draw_scaled_bitmap(orig, 0, 400, 1600, 400, 0, 0, size, 400 * scaleFactorY, 0);
    addBitmap(bmp);

    // 4 = Digits
    bmp = al_create_bitmap(380, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 800, 380, 80, 0, 0, 0);
    addBitmap(bmp);

    // 5 = Dot
    bmp = al_create_bitmap(20, 80);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 380, 800, 20, 80, 0, 0, 0);
    addBitmap(bmp);

    // 6 = Switch
    bmp = al_create_bitmap(80, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 400, 800, 80, 34, 0, 0, 0);
    addBitmap(bmp);

    // 7 = Transponder state selected
    bmp = al_create_bitmap(320, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 480, 800, 320, 34, 0, 0, 0);
    addBitmap(bmp);

    // 8 = Transponder state
    bmp = al_create_bitmap(320, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 480, 834, 320, 34, 0, 0, 0);
    addBitmap(bmp);

    // 9 = TX
    bmp = al_create_bitmap(160, 24);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 839, 160, 24, 0, 0, 0);
    addBitmap(bmp);

    // 10 = Autopilot switches
    bmp = al_create_bitmap(640, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 800, 800, 640, 34, 0, 0, 0);
    addBitmap(bmp);

    // 11 = Autopilot display
    bmp = al_create_bitmap(1152, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 0, 884, 1152, 50, 0, 0, 0);
    addBitmap(bmp);

    // 12 = Autopilot vertical speed digits
    bmp = al_create_bitmap(320, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1152, 884, 320, 50, 0, 0, 0);
    addBitmap(bmp);

    // 13 = Autopilot vertical speed 00fpm
    bmp = al_create_bitmap(162, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 966, 834, 162, 50, 0, 0, 0);
    addBitmap(bmp);

    // 14 = Autopilot vertical speed minus
    bmp = al_create_bitmap(23, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1128, 834, 23, 50, 0, 0, 0);
    addBitmap(bmp);

    // 15 = Autopilot ALTS display
    bmp = al_create_bitmap(128, 50);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1151, 834, 128, 50, 0, 0, 0);
    addBitmap(bmp);

    // 16 = Autopilot LOC display
    bmp = al_create_bitmap(66, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1280, 834, 66, 34, 0, 0, 0);
    addBitmap(bmp);

    // 17 = Autopilot G/S display
    bmp = al_create_bitmap(66, 34);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1346, 834, 66, 34, 0, 0, 0);
    addBitmap(bmp);

    // 18 = Autopilot MAN TOGA display
    bmp = al_create_bitmap(172, 32);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1427, 834, 172, 32, 0, 0, 0);
    addBitmap(bmp);

    // 19 = Autopilot MAN MCT display
    bmp = al_create_bitmap(148, 32);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 1451, 800, 148, 32, 0, 0, 0);
    addBitmap(bmp);

    // 20 = Managed or selected indicator
    bmp = al_create_bitmap(21, 21);
    al_set_target_bitmap(bmp);
    al_draw_bitmap_region(orig, 428, 844, 21, 21, 0, 0, 0);
    addBitmap(bmp);

    al_set_target_backbuffer(globals.display);
}

/// <summary>
/// Draw the instrument at the stored position
/// </summary>
void nav::render()
{
    if (bitmaps[0] == NULL) {
        return;
    }

    // Use normal blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw stuff into dest bitmap
    al_set_target_bitmap(bitmaps[1]);

    if ((int)switchSel < (int)Autopilot) {
        renderNav();
    }
    else {
        renderAutopilot();
    }

    // Position dest bitmap on screen
    al_set_target_backbuffer(globals.display);
    al_draw_bitmap(bitmaps[1], xPos, yPos, 0);

    if (!globals.avionics) {
        // Full dim if avionics turned off
        dimInstrument();
    }
    else if (!globals.active) {
        // Semi dim if screensaver activated but avionics turned on
        // allows radio to be tuned when engine not started.
        dimInstrument(false);
    }
}

/// <summary>
/// Draw the Nav Panel at the stored position
/// </summary>
void nav::renderNav()
{
    // Add main nav
    al_draw_bitmap(bitmaps[2], 0, 0, 0);

    // Add panel 1 frequencies
    addFreq3dp(com1Freq, 237, 19);
    addFreq3dp(com1Standby, 523, 19);
    addFreq2dp(nav1Freq, 837, 19);
    addFreq2dp(nav1Standby, 1153, 19);

    // Add panel 2 frequencies
    addFreq3dp(com2Freq, 237, 148);
    addFreq3dp(com2Standby, 523, 148);
    addFreq2dp(nav2Freq, 837, 148);
    addFreq2dp(nav2Standby, 1153, 148);

    // Add panel 3 frequencies
    addNum4(simVars->adfFreq, 273, 278);
    if (hasAdfStandby) {
        addNum4(simVars->adfStandby, 586, 278);
    }

    // Add squawk
    addSquawk(simVars->transponderCode, 968, 278);

    // Add selected switch
    switch (switchSel) {
    case Com1:
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 80, 34, 460 * scaleFactorX, 104 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
        break;
    case Nav1:
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 80, 34, 1064 * scaleFactorX, 104 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
        break;
    case Com2:
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 80, 34, 460 * scaleFactorX, 233 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
        break;
    case Nav2:
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 80, 34, 1064 * scaleFactorX, 233 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
        break;
    case Adf:
        al_draw_scaled_bitmap(bitmaps[6], 0, 0, 80, 34, 460 * scaleFactorX, 363 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
        break;
    }

    // Add transmit1 selected or state
    if (switchSel == Transmit1) {
        al_draw_scaled_bitmap(bitmaps[9], 80, 0, 80, 24, 377 * scaleFactorX, 106 * scaleFactorY, 80 * scaleFactorX, 24 * scaleFactorY, 0);
    }
    else if (simVars->com1Transmit) {
        al_draw_scaled_bitmap(bitmaps[9], 0, 0, 80, 24, 377 * scaleFactorX, 106 * scaleFactorY, 80 * scaleFactorX, 24 * scaleFactorY, 0);
    }

    // Add transmit2 selected or state
    if (switchSel == Transmit2) {
        al_draw_scaled_bitmap(bitmaps[9], 80, 0, 80, 24, 377 * scaleFactorX, 235 * scaleFactorY, 80 * scaleFactorX, 24 * scaleFactorY, 0);
    }
    else if (simVars->com2Transmit) {
        al_draw_scaled_bitmap(bitmaps[9], 0, 0, 80, 24, 377 * scaleFactorX, 235 * scaleFactorY, 80 * scaleFactorX, 24 * scaleFactorY, 0);
    }

    // Add transponder state
    int statePos = 80 * transponderState;
    if (switchSel == Transponder) {
        // Add transponder state selected
        al_draw_scaled_bitmap(bitmaps[7], statePos, 0, 80, 34, 1064 * scaleFactorX, 363 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
    }
    else {
        // Add transponder state
        al_draw_scaled_bitmap(bitmaps[8], statePos, 0, 80, 34, 1064 * scaleFactorX, 363 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);
    }
}

/// <summary>
/// Draw the Autopilot at the stored position
/// </summary>
void nav::renderAutopilot()
{
    // Add main autopilot
    al_draw_bitmap(bitmaps[3], 0, 0, 0);

    // Add autopilot switch selected
    int selPos = 80 * (switchSel - (int)Autopilot);
    int destPos = 203 + 160 * (switchSel - (int)Autopilot);
    al_draw_scaled_bitmap(bitmaps[10], selPos, 0, 80, 34, destPos * scaleFactorX, 339 * scaleFactorY, 80 * scaleFactorX, 34 * scaleFactorY, 0);

    int destSizeX = 128 * scaleFactorX;
    int destSizeY = 50 * scaleFactorY;

    // Add autopilot set values
    if (showSpeed) {
        if (showMach) {
            addNum2dp(machX100, 421, 82);
        }
        else {
            addNum4(airspeed, 403, 82, false);
        }
    }

    if (showHeading) {
        addNum3(heading, 816, 82);
    }

    if (showAltitude) {
        addNum5(altitude, 1188, 82, false);
    }

    if (showVerticalSpeed) {
        addVerticalSpeed(878, 252);
    }

    // Add auto throttle display
    if (simVars->autothrottleActive) {
        al_draw_scaled_bitmap(bitmaps[11], 1024, 0, 128, 50, 212 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
    }

    // Add spd hold display
    if (autopilotSpd == SpdHold) {
        al_draw_scaled_bitmap(bitmaps[11], 896, 0, 128, 50, 331 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
    }

    // Add hdg display
    switch (autopilotHdg) {
    case HdgSet:
        al_draw_scaled_bitmap(bitmaps[11], 0, 0, 128, 50, 461 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        break;
    case LevelFlight:
        al_draw_scaled_bitmap(bitmaps[11], 128, 0, 128, 50, 461 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        break;
    }

    // Add ap display
    if (simVars->autopilotEngaged) {
        al_draw_scaled_bitmap(bitmaps[11], 256, 0, 64, 50, 612 * scaleFactorX, 252 * scaleFactorY, 64 * scaleFactorX, destSizeY, 0);
    }

    // Add fd display
    if (simVars->flightDirectorActive) {
        al_draw_scaled_bitmap(bitmaps[11], 320, 0, 63, 50, 688 * scaleFactorX, 252 * scaleFactorY, 63 * scaleFactorX, destSizeY, 0);
    }

    // Add alt display
    switch (autopilotAlt) {
    case AltHold:
        al_draw_scaled_bitmap(bitmaps[11], 384, 0, 128, 50, 756 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        break;

    case PitchHold:
        al_draw_scaled_bitmap(bitmaps[11], 512, 0, 128, 50, 756 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        break;

    case VerticalSpeedHold:
        al_draw_scaled_bitmap(bitmaps[11], 640, 0, 128, 50, 756 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        // Add white alts display
        al_draw_scaled_bitmap(bitmaps[15], 0, 0, 128, 50, 1160 * scaleFactorX, 252 * scaleFactorY, destSizeX, destSizeY, 0);
        break;
    }

    if (simVars->autopilotApproachHold) {
        // Add cyan LOC display
        al_draw_scaled_bitmap(bitmaps[16], 0, 0, 66, 34, 1317 * scaleFactorX, 227 * scaleFactorY, 66 * scaleFactorX, 34 * scaleFactorY, 0);
    }

    if (simVars->autopilotGlideslopeHold) {
        // Add cyan G/S display
        al_draw_scaled_bitmap(bitmaps[17], 0, 0, 66, 34, 1317 * scaleFactorX, 270 * scaleFactorY, 66 * scaleFactorX, 34 * scaleFactorY, 0);
    }

    if (simVars->autothrottleActive && simVars->throttlePosition > 95) {
        // Add MAN TOGA display
        al_draw_scaled_bitmap(bitmaps[18], 0, 0, 172, 32, 391 * scaleFactorX, 187 * scaleFactorY, 172 * scaleFactorX, 32 * scaleFactorY, 0);
    }
    else if (simVars->autothrottleActive && simVars->throttlePosition > 90) {
        // Add MAN MCT display
        al_draw_scaled_bitmap(bitmaps[19], 0, 0, 148, 32, 403 * scaleFactorX, 187 * scaleFactorY, 148 * scaleFactorX, 32 * scaleFactorY, 0);
    }

    if (managedSpeed) {
        // Add managed speed indicator
        al_draw_scaled_bitmap(bitmaps[20], 0, 0, 21, 21, 384 * scaleFactorX, 220 * scaleFactorY, 21 * scaleFactorX, 21 * scaleFactorY, 0);
    }

    if (managedHeading) {
        // Add managed heading indicator
        al_draw_scaled_bitmap(bitmaps[20], 0, 0, 21, 21, 518 * scaleFactorX, 220 * scaleFactorY, 21 * scaleFactorX, 21 * scaleFactorY, 0);
    }

    if (managedAltitude) {
        // Add managed altitude indicator
        al_draw_scaled_bitmap(bitmaps[20], 0, 0, 21, 21, 806 * scaleFactorX, 220 * scaleFactorY, 21 * scaleFactorX, 21 * scaleFactorY, 0);
    }
}

/// <summary>
/// Displays a 3 digit number
/// </summary>
void nav::addNum3(int val, int x, int y)
{
    int digit1 = (val % 1000) / 100;
    int digit2 = (val % 100) / 10;
    int digit3 = val % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays a 4 digit number
/// </summary>
void nav::addNum4(int val, int x, int y, bool leading)
{
    if (!leading && val == 0) {
        return;
    }

    int digit1 = (val % 10000) / 1000;
    int digit2 = (val % 1000) / 100;
    int digit3 = (val % 100) / 10;
    int digit4 = val % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    if (leading || digit1 != 0) {
        al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    }
    x += 38;

    if (leading || digit1 != 0 || digit2 != 0) {
        al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    }
    x += 38;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit4, 0, 38, 80, (x + 38) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays a 5 digit number
/// </summary>
void nav::addNum5(int val, int x, int y, bool leading)
{
    if (!leading && val == 0) {
        return;
    }

    int digit1 = (val % 100000) / 10000;
    int digit2 = (val % 10000) / 1000;
    int digit3 = (val % 1000) / 100;
    int digit4 = (val % 100) / 10;
    int digit5 = val % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    if (leading || digit1 != 0) {
        al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    }
    x += 38;

    if (leading || digit1 != 0 || digit2 != 0) {
        al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    }
    x += 38;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit4, 0, 38, 80, (x + 38) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit5, 0, 38, 80, (x + 76) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays a value (number * 100) to 2 d.p.
/// </summary>
void nav::addNum2dp(int val, int x, int y)
{
    int digit1 = (val % 1000) / 100;
    int digit2 = (val % 100) / 10;
    int digit3 = val % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[5], 0, 0, 20, 80, (x + 38) * scaleFactorX, yPos, 20 * scaleFactorX, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, (x + 58) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, (x + 96) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays the specified frequency to 2 d.p.
/// </summary>
void nav::addFreq2dp(int freq, int x, int y)
{
    int digit1 = freq / 10000;
    int digit2 = (freq % 10000) / 1000;
    int digit3 = (freq % 1000) / 100;
    int digit4 = (freq % 100) / 10;
    int digit5 = freq % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[5], 0, 0, 20, 80, (x + 114) * scaleFactorX, yPos, 20 * scaleFactorX, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit4, 0, 38, 80, (x + 134) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit5, 0, 38, 80, (x + 172) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays the specified frequency to 3 d.p.
/// </summary>
void nav::addFreq3dp(int freq, int x, int y)
{
    int digit1 = freq / 100000;
    int digit2 = (freq % 100000) / 10000;
    int digit3 = (freq % 10000) / 1000;
    int digit4 = (freq % 1000) / 100;
    int digit5 = (freq % 100) / 10;
    int digit6 = freq % 10;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, (x + 38) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, (x + 76) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[5], 0, 0, 20, 80, (x + 114) * scaleFactorX, yPos, 20 * scaleFactorX, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit4, 0, 38, 80, (x + 134) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit5, 0, 38, 80, (x + 172) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit6, 0, 38, 80, (x + 210) * scaleFactorX, yPos, width, height, 0);
}

/// <summary>
/// Displays the squawk code
/// </summary>
void nav::addSquawk(int code, int x, int y)
{
    // Transponder code is in BCO16
    int digit1 = code / 4096;
    code -= digit1 * 4096;
    int digit2 = code / 256;
    code -= digit2 * 256;
    int digit3 = code / 16;
    int digit4 = code - digit3 * 16;

    int yPos = y * scaleFactorY;
    int width = 38 * scaleFactorX;
    int height = 80 * scaleFactorY;

    al_draw_scaled_bitmap(bitmaps[4], 38 * digit1, 0, 38, 80, x * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit2, 0, 38, 80, (x + 76) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit3, 0, 38, 80, (x + 152) * scaleFactorX, yPos, width, height, 0);
    al_draw_scaled_bitmap(bitmaps[4], 38 * digit4, 0, 38, 80, (x + 228) * scaleFactorX, yPos, width, height, 0);
}

void nav::addVerticalSpeed(int x, int y)
{
    int yPos = y * scaleFactorY;
    int height = 50 * scaleFactorY;

    if (simVars->autopilotVerticalSpeed == 0) {
        // Add 0fpm
        x += 119;
        al_draw_scaled_bitmap(bitmaps[13], 32, 0, 130, 50, x * scaleFactorX, yPos, 130 * scaleFactorX, height, 0);
        return;
    }

    int val = abs(simVars->autopilotVerticalSpeed);
    int digit1 = (val % 10000) / 1000;
    int digit2 = (val % 1000) / 100;

    if (digit1 == 0) {
        x += 32;
    }

    if (simVars->autopilotVerticalSpeed < 0) {
        // Add minus
        al_draw_scaled_bitmap(bitmaps[14], 0, 0, 23, 50, x * scaleFactorX, yPos, 23 * scaleFactorX, height, 0);
    }
    x += 23;

    if (digit1 != 0) {
        al_draw_scaled_bitmap(bitmaps[12], 32 * digit1, 0, 32, 50, x * scaleFactorX, yPos, 32 * scaleFactorX, height, 0);
        x += 32;
    }

    al_draw_scaled_bitmap(bitmaps[12], 32 * digit2, 0, 32, 50, x * scaleFactorX, yPos, 32 * scaleFactorX, height, 0);
    x += 32;

    // Add 00fpm
    al_draw_scaled_bitmap(bitmaps[13], 0, 0, 162, 50, x * scaleFactorX, yPos, 162 * scaleFactorX, height, 0);
}

/// <summary>
/// Fetch flightsim vars and then update all internal variables
/// that affect this instrument.
/// </summary>
void nav::update()
{
    // Check for aircraft change
    bool aircraftChanged = (loadedAircraft != globals.aircraft);
    if (aircraftChanged) {
        loadedAircraft = globals.aircraft;
        showMach = false;
        showHeading = false;
        showSpeed = false;
        showAltitude = false;
        showVerticalSpeed = false;
        prevHeading = simVars->autopilotHeading;
        prevSpeed = simVars->autopilotAirspeed;
        prevAltitude = simVars->autopilotAltitude;
        prevVerticalSpeed = simVars->autopilotVerticalSpeed;
        managedHeading = true;
        managedSpeed = true;
        managedAltitude = true;
    }

    // Check for position or size change
    long *settings = globals.simVars->readSettings(name, xPos, yPos, size);

    xPos = settings[0];
    yPos = settings[1];

    if (size != settings[2]) {
        size = settings[2];
        resize();
    }

#ifndef _WIN32
    // Only have hardware knobs on Raspberry Pi
    if (globals.hardwareKnobs) {
        updateKnobs();
    }
#endif

    // Calculate values - 3 d.p. for comms, 2 d.p. for nav, 0 d.p. for adf
    com1Freq = (simVars->com1Freq + 0.0000001) * 1000.0;
    com1Standby = (simVars->com1Standby + 0.0000001) * 1000.0;
    nav1Freq = (simVars->nav1Freq + 0.0000001) * 100.0;
    nav1Standby = (simVars->nav1Standby + 0.0000001) * 100.0;
    com2Freq = (simVars->com2Freq + 0.0000001) * 1000.0;
    com2Standby = (simVars->com2Standby + 0.0000001) * 1000.0;
    nav2Freq = (simVars->nav2Freq + 0.0000001) * 100.0;
    nav2Standby = (simVars->nav2Standby + 0.0000001) * 100.0;

    if (adfChanged) {
        // See which adf changes
        if (simVars->adfStandby != prevAdfStandby) {
            hasAdfStandby = true;
            adfChanged = false;
        }
        else if (simVars->adfFreq != prevAdf) {
            hasAdfStandby = false;
            adfChanged = false;
        }
    }

    // Show values if they get adjusted in sim
    if (showHeading == false && simVars->autopilotHeading != prevHeading) {
        showHeading = true;
    }

    if (showSpeed == false && simVars->autopilotAirspeed != prevSpeed) {
        showSpeed = true;
    }

    if (showAltitude == false && simVars->autopilotAltitude != prevAltitude) {
        showAltitude = true;
    }

    if (showVerticalSpeed == false && simVars->autopilotVerticalSpeed != prevVerticalSpeed) {
        showVerticalSpeed = true;
    }

    airspeed = simVars->autopilotAirspeed + 0.5;
    machX100 = simVars->autopilotMach * 100 + 0.5;
    heading = simVars->autopilotHeading + 0.5;
    altitude = simVars->autopilotAltitude + 0.5;

    if (simVars->autopilotAirspeedHold == 1) {
        autopilotSpd = SpdHold;
    }
    else {
        autopilotSpd = NoSpd;
    }

    if (simVars->autopilotHeadingLock == 1) {
        autopilotHdg = HdgSet;
    }
    else if (simVars->autopilotLevel == 1) {
        autopilotHdg = LevelFlight;
    }
    else {
        autopilotHdg = NoHdg;
    }

    if (simVars->autopilotAltLock == 1) {
        if (autopilotAlt == VerticalSpeedHold) {
            // Revert to alt hold when within range of target altitude
            int diff = abs(simVars->altAltitude - simVars->autopilotAltitude);
            if (diff < 210) {
                autopilotAlt = AltHold;
            }
        }
        else {
            autopilotAlt = AltHold;
        }
    }
    else if (simVars->autopilotVerticalHold == 1) {
        autopilotAlt = VerticalSpeedHold;
    }
    else if (simVars->autopilotPitchHold == 1) {
        autopilotAlt = PitchHold;
    }
    else {
        autopilotAlt = NoAlt;
    }
}

/// <summary>
/// Add FlightSim variables for this instrument (used for simulation mode)
/// </summary>
void nav::addVars()
{
    globals.simVars->addVar(name, "Com Status:1", true, 1, 0);
    globals.simVars->addVar(name, "Com Transmit:1", true, 1, 1);
    globals.simVars->addVar(name, "Com Active Frequency:1", false, 0.005, 100);
    globals.simVars->addVar(name, "Com Standby Frequency:1", false, 0.005, 100);
    globals.simVars->addVar(name, "Nav Active Frequency:1", false, 0.05, 100);
    globals.simVars->addVar(name, "Nav Standby Frequency:1", false, 0.05, 100);
    globals.simVars->addVar(name, "Com Status:2", true, 1, 0);
    globals.simVars->addVar(name, "Com Transmit:2", true, 1, 0);
    globals.simVars->addVar(name, "Com Active Frequency:2", false, 0.005, 100);
    globals.simVars->addVar(name, "Com Standby Frequency:2", false, 0.005, 100);
    globals.simVars->addVar(name, "Nav Active Frequency:2", false, 0.05, 100);
    globals.simVars->addVar(name, "Nav Standby Frequency:2", false, 0.05, 100);
    globals.simVars->addVar(name, "Adf Active Frequency:1", false, 1, 100);
    globals.simVars->addVar(name, "Adf Standby Frequency:1", false, 1, 100);
    globals.simVars->addVar(name, "Transponder Code:1", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Available", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Master", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Heading Lock Dir", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Heading Lock", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Wing Leveler", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Altitude Lock Var", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Altitude Lock", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Pitch Hold", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Vertical Hold Var", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Vertical Hold", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Airspeed Hold Var", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Mach Hold Var", false, 1, 0);
    globals.simVars->addVar(name, "Autopilot Airspeed Hold", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Approach Hold", true, 1, 0);
    globals.simVars->addVar(name, "Autopilot Glideslope Hold", true, 1, 0);
    globals.simVars->addVar(name, "General Eng Throttle Lever Position:1", false, 1, 0);
    globals.simVars->addVar(name, "Autothrottle Active", true, 1, 0);
}

#ifndef _WIN32

void nav::addKnobs()
{
    // BCM GPIO 8 and 7
    selKnob = globals.hardwareKnobs->add(8, 7, -1, -1, 0);

    // BCM GPIO 12
    selPush = globals.hardwareKnobs->add(12, 0, -1, -1, 0);

    // BCM GPIO 20 and 21
    adjustKnob = globals.hardwareKnobs->add(20, 21, -1, -1, 0);

    // BCM GPIO 16
    adjustPush = globals.hardwareKnobs->add(16, 0, -1, -1, 0);
}

void nav::updateKnobs()
{
    // Read left knob rotate for switch select
    int val = globals.hardwareKnobs->read(selKnob);
    if (val != INT_MIN) {
        // Convert knob value to selection (adjust for desired sensitivity)
        int maxSwitch;
        if (simVars->autopilotAvailable) {
            maxSwitch = (int)ApproachHold;
        }
        else {
            maxSwitch = (int)Transponder;
        }

        int diff = (prevSelVal - val) / 2;
        if (diff > 0) {
            if ((int)switchSel < maxSwitch) {
                switchSel = (Switch)((int)switchSel + 1);
            }
            else {
                switchSel = Transmit1;
            }
            prevSelVal = val;
            adjustSetSel = 0;
        }
        else if (diff < 0) {
            if ((int)switchSel > 0) {
                switchSel = (Switch)((int)switchSel - 1);
            }
            else {
                switchSel = (Switch)maxSwitch;
            }
            prevSelVal = val;
            adjustSetSel = 0;
        }
    }

    // Read left knob push for switch activate
    val = globals.hardwareKnobs->read(selPush);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevSelPush % 2 == 1) {
            if ((int)switchSel < (int)Autopilot) {
                navSwitchPressed();
            }
            else {
                autopilotSwitchPressed();
            }
        }
        prevSelPush = val;
        adjustSetSel = 0;
    }

    // Read right knob rotate for digits set
    val = globals.hardwareKnobs->read(adjustKnob);
    if (val != INT_MIN) {
        int diff = (val - prevAdjustVal) / 2;
        int adjust = 0;
        if (diff > 0) {
            adjust = 1;
        }
        else if (diff < 0) {
            adjust = -1;
        }

        if (adjust != 0) {
            if ((int)switchSel < (int)Autopilot) {
                navAdjustDigits(adjust);
            }
            else {
                autopilotAdjustDigits(adjust);
            }
            prevAdjustVal = val;
        }
        time(&lastAdjust);
    }
    else if (lastAdjust != 0) {
        // Reset digit set selection if more than 5 seconds since last adjustment
        time(&now);
        if (now - lastAdjust > 5) {
            adjustSetSel = 0;
            lastAdjust = 0;
        }
    }

    // Read right knob push for digits set selection
    val = globals.hardwareKnobs->read(adjustPush);
    if (val != INT_MIN) {
        // If previous state was unpressed then must have been pressed
        if (prevAdjustPush % 2 == 1) {
            if (switchSel == Autopilot) {
                // Right knob push on autopilot toggles flight director
                globals.simVars->write(KEY_TOGGLE_FLIGHT_DIRECTOR);
            }
            else {
                // Select which set of digits to adjust
                int digitSets;
                if (switchSel == Com1 || switchSel == Com2 || switchSel == Adf) {
                    digitSets = 3;
                }
                else if (switchSel == Transponder) {
                    digitSets = 4;
                }
                else {
                    digitSets = 2;
                }

                adjustSetSel++;
                if (adjustSetSel >= digitSets) {
                    adjustSetSel = 0;
                }
            }
        }
        prevAdjustPush = val;
    }
}

void nav::navSwitchPressed()
{
    // Swap standby and primary values
    switch (switchSel) {
    case Transmit1:
    {
        globals.simVars->write(KEY_COM1_TRANSMIT_SELECT);
        break;
    }
    case Com1:
    {
        globals.simVars->write(KEY_COM1_RADIO_SWAP);
        break;
    }
    case Nav1:
    {
        globals.simVars->write(KEY_NAV1_RADIO_SWAP);
        break;
    }
    case Transmit2:
    {
        globals.simVars->write(KEY_COM2_TRANSMIT_SELECT);
        break;
    }
    case Com2:
    {
        globals.simVars->write(KEY_COM2_RADIO_SWAP);
        break;
    }
    case Nav2:
    {
        globals.simVars->write(KEY_NAV2_RADIO_SWAP);
        break;
    }
    case Adf:
    {
        if (hasAdfStandby) {
            int freq = adjustAdf(simVars->adfFreq, 0);
            int standby = adjustAdf(simVars->adfStandby, 0);
            globals.simVars->write(KEY_ADF_COMPLETE_SET, freq);
            // This isn't working, looks like an SDK bug!
            globals.simVars->write(KEY_ADF1_PRIMARY_SET, standby);
        }
        break;
    }
    case Transponder:
    {
        if (transponderState == 3) {
            transponderState = 0;
        }
        else {
            transponderState++;
        }
        break;
    }
    }
}

void nav::autopilotSwitchPressed()
{
    switch (switchSel) {
    case Autopilot:
    {
        // Capture current values when autopilot enabled (only if not set)
        captureCurrent();
        globals.simVars->write(KEY_AP_MASTER);
        break;
    }
    case Autothrottle:
    {
        // Toggle auto throttle
        globals.simVars->write(KEY_AUTO_THROTTLE_ARM);
        break;
    }
    case Speed:
    {
        if (autopilotSpd == SpdHold) {
            // Switch between knots and mach display.
            // Sets currently displayed value before switching to
            // set correctly converted value for current altitude.
            if (showMach) {
                // For some weird reason you have to set mach * 100 !
                globals.simVars->write(KEY_AP_MACH_VAR_SET, simVars->autopilotMach * 100);
                showMach = false;
                manSelSpeed();
            }
            else {
                globals.simVars->write(KEY_AP_SPD_VAR_SET, simVars->autopilotAirspeed);
                showMach = true;
            }
        }
        else {
            // Switch to Airspeed hold.
            globals.simVars->write(KEY_AP_AIRSPEED_ON);
            showMach = false;
        }
        break;
    }
    case Heading:
    {
        if (autopilotHdg == HdgSet) {
            autopilotHdg = LevelFlight;
            globals.simVars->write(KEY_AP_HDG_HOLD_OFF);
            manSelHeading();
        }
        else {
            autopilotHdg = HdgSet;
            globals.simVars->write(KEY_AP_HDG_HOLD_ON);
        }
        break;
    }
    case Altitude:
    {
        if (autopilotAlt == AltHold) {
            autopilotAlt = PitchHold;
            globals.simVars->write(KEY_AP_ALT_HOLD_OFF);
            manSelAltitude();
        }
        else {
            autopilotAlt = AltHold;
            globals.simVars->write(KEY_AP_ALT_HOLD_ON);
            manSelAltitude();
        }
        break;
    }
    case VerticalSpeed:
    {
        autopilotAlt = VerticalSpeedHold;
        globals.simVars->write(KEY_AP_ALT_VAR_SET_ENGLISH, simVars->autopilotAltitude);
        globals.simVars->write(KEY_AP_ALT_HOLD_ON);
        captureVerticalSpeed();
        manSelAltitude();
        break;
    }
    case LocatorHold:
    {
        if (simVars->autopilotGlideslopeHold) {
            globals.simVars->write(KEY_AP_APR_HOLD_OFF);
        }
        globals.simVars->write(KEY_AP_LOC_HOLD);
        break;
    }
    case ApproachHold:
    {
        if (simVars->autopilotGlideslopeHold) {
            globals.simVars->write(KEY_AP_APR_HOLD_OFF);
        }
        else {
            globals.simVars->write(KEY_AP_APR_HOLD_ON);
        }
        break;
    }
    }
}

/// <summary>
/// Switch between managed and selected speed.
/// This is undocumented but works for the Airbus A320 neo.
/// </summary>
void nav::manSelSpeed()
{
    managedSpeed = !managedSpeed;
    if (managedSpeed) {
        globals.simVars->write(KEY_SPEED_SLOT_INDEX_SET, 2);
    }
    else {
        globals.simVars->write(KEY_SPEED_SLOT_INDEX_SET, 1);
    }
}

/// <summary>
/// Switch between managed and selected heading.
/// This is undocumented but works for the Airbus A320 neo.
/// </summary>
void nav::manSelHeading()
{
    managedHeading = !managedHeading;
    if (managedHeading) {
        globals.simVars->write(KEY_HEADING_SLOT_INDEX_SET, 2);
    }
    else {
        globals.simVars->write(KEY_HEADING_SLOT_INDEX_SET, 1);
    }
}

/// <summary>
/// Switch between managed and selected altitude.
/// This is undocumented but works for the Airbus A320 neo.
/// </summary>
void nav::manSelAltitude()
{
    managedAltitude = !managedAltitude;
    if (managedAltitude) {
        globals.simVars->write(KEY_ALTITUDE_SLOT_INDEX_SET, 2);
    }
    else {
        globals.simVars->write(KEY_ALTITUDE_SLOT_INDEX_SET, 1);
    }
}

void nav::captureCurrent()
{
    if (!showSpeed) {
        showSpeed = true;

        // Set autopilot speed to within 10 knots of current speed
        int holdSpeed = simVars->asiAirspeed;
        int tens = holdSpeed % 10;
        if (tens < 5) {
            holdSpeed -= tens;
        }
        else {
            holdSpeed += 10 - tens;
        }
        globals.simVars->write(KEY_AP_SPD_VAR_SET, holdSpeed);
    }

    if (!showHeading) {
        showHeading = true;

        // Set autopilot heading to within 10 degrees of current heading
        int holdHeading = simVars->hiHeading;
        int lastDigit = holdHeading % 10;
        if (lastDigit < 5) {
            holdHeading -= lastDigit;
        }
        else {
            holdHeading += 10 - lastDigit;
            if (holdHeading > 359) {
                holdHeading -= 360;
            }
        }
        globals.simVars->write(KEY_HEADING_BUG_SET, holdHeading);
    }

    if (!showAltitude) {
        showAltitude = true;

        // Set autopilot altitude to within 100ft of current altitude
        int holdAlt = simVars->altAltitude;
        int hundreds = holdAlt % 100;
        if (hundreds < 30) {
            holdAlt -= hundreds;
        }
        else {
            holdAlt += 100 - hundreds;
        }
        globals.simVars->write(KEY_AP_ALT_VAR_SET_ENGLISH, holdAlt);
    }
}

void nav::captureVerticalSpeed()
{
    if (!showVerticalSpeed) {
        showVerticalSpeed = true;

        // Make sure VS is in correct direction when first shown
        double setVerticalSpeed = simVars->autopilotVerticalSpeed;

        if (setVerticalSpeed <= 0 && simVars->altAltitude < simVars->autopilotAltitude) {
            setVerticalSpeed = 1000;
        }
        else if (setVerticalSpeed >= 0 && simVars->altAltitude > simVars->autopilotAltitude) {
            setVerticalSpeed = -1000;
        }

        globals.simVars->write(KEY_AP_VS_VAR_SET_ENGLISH, setVerticalSpeed);
    }
}

void nav::navAdjustDigits(int adjust)
{
    switch (switchSel) {
    case Com1:
    {
        double newVal = adjustCom(simVars->com1Standby, adjust);
        globals.simVars->write(KEY_COM1_STBY_RADIO_SET, newVal);
        break;
    }
    case Nav1:
    {
        double newVal = adjustNav(simVars->nav1Standby, adjust);
        globals.simVars->write(KEY_NAV1_STBY_SET, newVal);
        break;
    }
    case Com2:
    {
        double newVal = adjustCom(simVars->com2Standby, adjust);
        globals.simVars->write(KEY_COM2_STBY_RADIO_SET, newVal);
        break;
    }
    case Nav2:
    {
        double newVal = adjustNav(simVars->nav2Standby, adjust);
        globals.simVars->write(KEY_NAV2_STBY_SET, newVal);
        break;
    }
    case Adf:
    {
        int oldVal;
        if (hasAdfStandby) {
            oldVal = simVars->adfStandby;
        }
        else {
            oldVal = simVars->adfFreq;
        }
        int newVal = adjustAdf(oldVal, adjust);
        prevAdf = simVars->adfFreq;
        prevAdfStandby = simVars->adfStandby;
        globals.simVars->write(KEY_ADF_COMPLETE_SET, newVal);
        adfChanged = true;
        break;
    }
    case Transponder:
    {
        int newVal = adjustSquawk(simVars->transponderCode, adjust);
        globals.simVars->write(KEY_XPNDR_SET, newVal);
        break;
    }
    }
}

void nav::autopilotAdjustDigits(int adjust)
{
    switch (switchSel) {
    case Speed:
    {
        showSpeed = true;
        if (showMach) {
            double newVal = adjustMach(simVars->autopilotMach, adjust);
            globals.simVars->write(KEY_AP_MACH_VAR_SET, newVal);
        }
        else {
            double newVal = adjustSpeed(simVars->autopilotAirspeed, adjust);
            globals.simVars->write(KEY_AP_SPD_VAR_SET, newVal);
        }
        break;
    }
    case Heading:
    {
        showHeading = true;
        double newVal = adjustHeading(simVars->autopilotHeading, adjust);
        globals.simVars->write(KEY_HEADING_BUG_SET, newVal);
        break;
    }
    case Altitude:
    {
        showAltitude = true;
        double newVal = adjustAltitude(simVars->autopilotAltitude, adjust);
        globals.simVars->write(KEY_AP_ALT_VAR_SET_ENGLISH, newVal);
        break;
    }
    case VerticalSpeed:
    {
        showVerticalSpeed = true;
        double newVal = adjustVerticalSpeed(simVars->autopilotVerticalSpeed, adjust);
        globals.simVars->write(KEY_AP_VS_VAR_SET_ENGLISH, newVal);
        break;
    }
    }
}

double nav::adjustCom(double val, int adjust)
{
    int whole = val;
    val -= whole;
    int thousandths = (val + 0.0001) * 1000;
    int frac1 = thousandths / 100;
    int frac2 = thousandths % 100;

    if (adjustSetSel == 0) {
        // Adjust whole - Range 118 to 136
        whole += adjust;
        if (whole > 136) {
            whole -= 19;
        }
        else if (whole < 118) {
            whole += 19;
        }
    }
    else if (adjustSetSel == 1) {
        // Adjust 10ths
        frac1 += adjust;
        if (frac1 > 9) {
            frac1 -= 10;
        }
        else if (frac1 < 0) {
            frac1 += 10;
        }
    }
    else {
        // Adjust 100ths and 1000ths
        frac2 += adjust * 5;

        if (frac2 >= 100) {
            frac2 -= 100;
        }
        else if (frac2 < 0) {
            frac2 += 100;
        }

        // Skip .020, .045, .070 and .095
        if (frac2 == 95) {
            frac2 += adjust * 5;
            if (frac2 >= 100) {
                frac2 -= 100;
            }
        }
        else if (frac2 == 20 || frac2 == 45 || frac2 == 70) {
            frac2 += adjust * 5;
        }

        // .020 shows as .025 and .070 shows as .075
        if (frac2 == 25 || frac2 == 75) {
            frac2 -= 5;
        }
    }

    // Convert to BCD
    int digit1 = whole / 100;
    int digit2 = (whole % 100) / 10;
    int digit3 = whole % 10;
    int digit4 = frac1;
    int digit5 = frac2 / 10;
    int digit6 = frac2 % 10;

    // Return digit6 as fraction
    return 65536 * digit1 + 4096 * digit2 + 256 * digit3 + 16 * digit4 + digit5 + digit6 * 0.1;
}

double nav::adjustNav(double val, int adjust)
{
    int whole = val;
    val -= whole;
    int frac = (val + 0.001) * 100;

    if (adjustSetSel == 0) {
        // Adjust whole - Range 108 to 117
        whole += adjust;
        if (whole > 117) {
            whole -= 10;
        }
        else if (whole < 108) {
            whole += 10;
        }
    }
    else {
        // Adjust fraction
        frac += adjust * 5;

        if (frac >= 100) {
            frac -= 100;
        }
        else if (frac < 0) {
            frac += 100;
        }
    }

    // Convert to BCD
    int digit1 = whole / 100;
    int digit2 = (whole % 100) / 10;
    int digit3 = whole % 10;
    int digit4 = frac / 10;
    int digit5 = frac % 10;

    return 65536 * digit1 + 4096 * digit2 + 256 * digit3 + 16 * digit4 + digit5;
}

int nav::adjustAdf(int val, int adjust)
{
    int highDigits = (val % 10000) / 100;
    int digit3 = (val % 100) / 10;
    int digit4 = val % 10;

    if (adjustSetSel == 0) {
        highDigits += adjust;
        if (highDigits >= 18) {
            highDigits -= 17;
        }
        else if (highDigits < 1) {
            highDigits += 17;
        }
    }
    else if (adjustSetSel == 1) {
        // Adjust 3rd digit
        digit3 = adjustDigit(digit3, adjust);
    }
    else {
        // Adjust 4th digit
        digit4 = adjustDigit(digit4, adjust);
    }

    // Convert to BCD
    int digit1 = highDigits / 10;
    int digit2 = highDigits % 10;

    return 268435456.0 * digit1 + 16777216.0 * digit2 + 1048576.0 * digit3 + 65536 * digit4;
}

int nav::adjustSquawk(int val, int adjust)
{
    // Transponder code is in BCD
    int digit1 = val / 4096;
    val -= digit1 * 4096;
    int digit2 = val / 256;
    val -= digit2 * 256;
    int digit3 = val / 16;
    int digit4 = val - digit3 * 16;

    switch (adjustSetSel) {
    case 0:
        digit1 = adjustDigit(digit1, adjust, true);
        break;
    case 1:
        digit2 = adjustDigit(digit2, adjust, true);
        break;
    case 2:
        digit3 = adjustDigit(digit3, adjust, true);
        break;
    case 3:
        digit4 = adjustDigit(digit4, adjust, true);
        break;
    }

    // Convert to BCD
    return digit1 * 4096 + digit2 * 256 + digit3 * 16 + digit4;
}

int nav::adjustSpeed(int val, int adjust)
{
    if (adjustSetSel == 0) {
        // Adjust tens
        val += adjust * 10;
    }
    else {
        // Adjust tens and units
        val += adjust;
    }

    if (val < 0) {
        val = 0;
    }
    else if (val > 990) {
        val = 990;
    }

    return val;
}

double nav::adjustMach(double val, int adjust)
{
    int whole = val;
    val -= whole;
    int frac = val * 100 + 0.5;

    // Default to adjusting fraction first on mach
    if (adjustSetSel == 0) {
        // Adjust fraction
        frac += adjust;

        if (frac >= 100) {
            frac -= 100;
        }
        else if (frac < 0) {
            frac += 100;
        }
    }
    else {
        // Adjust whole
        whole += adjust;
        if (whole > 2) {
            whole -= 3;
        }
        else if (whole < 0) {
            whole += 3;
        }
    }

    // For some weird reason you have to set mach * 100 !
    return whole * 100 + frac;
}

int nav::adjustHeading(int val, int adjust)
{
    if (adjustSetSel == 0) {
        // Adjust tens only
        val += adjust * 10;
    }
    else {
        // Adjust tens and units
        val += adjust;
    }

    if (val > 359) {
        val -= 360;
    }
    else if (val < 0) {
        val += 360;
    }

    return val;
}

int nav::adjustAltitude(int val, int adjust)
{
    int prevVal = val;

    if (adjustSetSel == 0) {
        // Adjust thousands
        val += adjust * 1000;

        if (val < 0) {
            val += 1000;
        }
    }
    else {
        // Adjust thousands and hundreds
        val += adjust * 100;

        if (val < 0) {
            val += 100;
        }
    }

    if (autopilotAlt == VerticalSpeedHold) {
        // Cancel vertical speed hold when target altitude reached
        int diff = abs(val - simVars->altAltitude);
        if (diff < 210 || (val < simVars->altAltitude && prevVal > simVars->altAltitude)
            || (val > simVars->altAltitude && prevVal < simVars->altAltitude)) {
            autopilotAlt = AltHold;
        }
    }

    return val;
}

int nav::adjustVerticalSpeed(int val, int adjust)
{
    // Allow vertical speed to go negative
    val += adjust * 100;

    return val;
}

int nav::adjustDigit(int val, int adjust, bool isSquawk)
{
    int maxDigit;
    if (isSquawk) {
        maxDigit = 7;
    }
    else {
        maxDigit = 9;
    }

    val += adjust;
    if (val > maxDigit) {
        val -= maxDigit + 1;
    }
    else if (val < 0) {
        val += maxDigit + 1;
    }

    return val;
}

#endif // !_WIN32
