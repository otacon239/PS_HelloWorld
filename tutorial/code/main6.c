#include "8bkc-hal.h"      // PS HW Abstraction Layer
#include "8bkc-ugui.h"     // PS uGUI
#include "hsv2rgb.h"       // Header I created for converting HSV to RGB
#include "math.h"          // Needed for fabs(), fmod(), sin()
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc
#include "string.h"        // Needed for str_len()
#include "ugui.h"          // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/

char hello[] = "Hello World!"; // Store string that will be used

const UG_FONT *font = &FONT_6X8; // Define font for more readable code

#define SW KC_SCREEN_W       // Screen width
#define SH KC_SCREEN_H       // Screen height
#define CW font->char_width  // Character width
#define CH font->char_height // Character height

float phase = 0;       // This will be used for the sine wave animation
float color_phase = 0; // This will be used for rotating the base hue for the text

#define SIN_FREQ 0.375 // Multiplier for the sine wave density: smaller = larger wave
#define SIN_DEG 0.2    // Number of degrees per frame the phase for the sine wave moves
#define COL_OFFSET 10  // Number of degrees per character the base hue is offset by
#define COL_DEG 2      // Number of degrees the base hue moves per frame

static void do_powerbtn_menu() {
  int pwr_input = powerbtn_menu_show(kcugui_get_fb()); // Call the powerbutton menu

  if (pwr_input == POWERBTN_MENU_EXIT) { // This handles the input from the power button menu - without it, menu options do nothing
    kchal_exit_to_chooser();
  } else if (pwr_input == POWERBTN_MENU_POWERDOWN) {
    kchal_power_down();
  }
}

void app_main() {
  kchal_init();             // Initialize the PocketSprite SDK.
  kcugui_init();            // Initialize uGUI
  UG_FontSelect(font);      // The default font that is enabled out of the box
  UG_SetForecolor(C_WHITE); // You can find a full list of colors in the µGUI Reference Guide
  UG_SetBackcolor(C_BLACK);

  while (true) {
    kcugui_cls(); // Clear the display
    for (int i = 0; i < strlen(hello); i++) {
      HSV hsv;
      hsv.hue = fmod(color_phase + i * COL_OFFSET, 360);
      hsv.sat = 1;
      hsv.val = 1;
      RGB rgb = hsv2rgb(hsv);

      UG_PutChar(hello[i],                                                                 // Char
                 i * CW + (SW - strlen(hello) * CW) / 2,                                   // X
                 round(sin(i * SIN_FREQ + phase) * (SH / 2 - CH / 2) + (SH / 2 - CH / 2)), // Y
                 kchal_ugui_rgb(rgb.r, rgb.g, rgb.b),                                      // Foreground Color
                 C_BLACK);                                                                 // Background Color
    }
    kcugui_flush(); // Push the buffer to the display

    phase = fmod(phase + 1 / (2 * M_PI) * SIN_DEG, 2 * M_PI); // Move phase forward by SIN_DEG degrees per frame
    color_phase = fmod(color_phase + COL_DEG, 360.0f);        // Move the base hue by COL_DEG per frame

    if (kchal_get_keys() & KC_BTN_POWER) { // Check for power button press
      do_powerbtn_menu();
    }
  }
}