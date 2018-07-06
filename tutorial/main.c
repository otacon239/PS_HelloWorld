#include "8bkc-hal.h"      // PS HW Abstraction Layer
#include "8bkc-ugui.h"     // PS uGUI
#include "ugui.h"          // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc

#include <stdint.h>  // uint8_t, etc.
#include "string.h"  // Needed for str_len()
#include "math.h"    // Needed for fabs(), fmod(), sin()
#include "hsv2rgb.h" // Header I created for converting HSV to RGB

const UG_FONT *font = &FONT_6X8; // Define font for more readable code

#define SW KC_SCREEN_W       // Screen width
#define SH KC_SCREEN_H       // Screen height
#define CW font->char_width  // Character width
#define CH font->char_height // Character height

static void do_powerbtn_menu()
{
    int i = powerbtn_menu_show(kcugui_get_fb()); // Call the powerbutton menu

    if (i == POWERBTN_MENU_EXIT)
    { // This checks the input from the power button menu - without it, menu options do nothing
        kchal_exit_to_chooser();
    }
    if (i == POWERBTN_MENU_POWERDOWN)
    {
        kchal_power_down();
    }
}

void app_main()
{
    UG_FontSelect(font);
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);

    while (true) {
        kgucui_cls(); // Clear the display
        for (int i = 0; i < strlen(hello); i++) {
            UG_PutChar(hello[i],                        // Char
                i * CW + (SW - strlen(hello) * CW) / 2, // X
                0,                                      // Y
                C_WHITE,                                // FG Color
                C_BLACK);                               // BG Color
        }
        kcugui_flush(); // Push the buffer to the display

        if (kchal_get_keys() & KC_BTN_POWER)
        { // Check for power button press
            do_powerbtn_menu();
        }
    }
}