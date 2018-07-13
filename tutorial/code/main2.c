#include "8bkc-hal.h"  // PS HW Abstraction Layer
#include "8bkc-ugui.h" // PS uGUI
#include "ugui.h"      // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/

void app_main() {
  kchal_init();             // Initialize the PocketSprite SDK.
  kcugui_init();            // Initialize uGUI
  UG_FontSelect(&FONT_6X8); // The default font that is enabled out of the box
  UG_SetForecolor(C_WHITE); // You can find a full list of colors in the µGUI Reference Guide
  UG_SetBackcolor(C_BLACK);

  kcugui_cls();                       // Clear the display
  UG_PutString(0, 0, "Hello World!"); // UG S16 x , UG S16 y , char* str
  kcugui_flush();                     // Send buffer to display
}