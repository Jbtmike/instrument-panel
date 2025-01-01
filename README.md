I tested with MSFS 2024 but there is a problem with the display: the instruments work fine but they remain dark as if the batteries were switched off.
I had a quick look at the code and the variables in FS2020 and FS2024.
It seems that MS has changed the names of some of them, especially in the electrical part. That's why dcVolts and batteryLoad variables always return 0.
In instrument-panel.cpp, I changed the test at the beginning of "updateCommon()", leaving only "globals.electrics = globals.connected", recompiled, and it's usable with FS2024: the screen no longer darkens.
The ammeter needle doesn't move anymore and the annunciator doesn't seem to work anymore.
I only fly single engine airplanes and haven't done extensive testing. 
For users who want to test FS2024, it may be possible to wait for a more complete revision of the tool by scott-vincent (instrument-data-link and instrument-panel).
I wish you all a happy new year 2025!
