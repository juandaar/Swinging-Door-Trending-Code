// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Swing_door.ino
    Created:	09-Aug-18 6:32:07 PM
    Author:     Juan David Arias Correa
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts


#include "Settings.h"
#include "SwingDoor_manager.h"


SwingDoor_manager *SD_manager = new SwingDoor_manager();


void setup()
{
	SD_manager->setup();
}

// Add the main program code into the continuous loop() function
void loop()
{
	SD_manager->loop();

}
