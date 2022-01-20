# switchtest
stm32 usb hid Nintendo switch stuff

This is based on the Hori Fight Stick.

Some findings:

 * Vendor and Product IDs are crucial.
 
 * The Switch seems to completely ignore everything in the report description after 
   the initial Usage Page (Generic Desktop) and Usage (Joystick). Everything else can
   be gibberish, and the controller will still be recognized and work, as long as the
   report is in the order the Switch expects. This means that we cannot change the 
   controller's functionality (e.g., by making the joysticks more than 8 bits), or
   insert a report ID. 
   
 * The description I am currently using (taken from something I found on the net) has 
   joysticks that don't seem to work with Windows.