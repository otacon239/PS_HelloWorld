#include "hsv2rgb.h" // Header I created for converting HSV to RGB
#include "math.h"    // Needed for fabs(), fmod(), sin()
#include "string.h"  // Needed for str_len()

#include "8bkc-hal.h"      // PS HW Abstraction Layer
#include "8bkc-ugui.h"     // PS uGUI
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc
#include "sndmixer.h"      // Sound Mixer library
#include "ugui.h"          // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/

#include "freertos/FreeRTOS.h" // These libraries are required for the vTaskDelay function
#include "freertos/task.h"

const UG_FONT *font = &FONT_6X8; // Define font for more readable code

#define SIN_DEG 0.2    // Number of degrees per frame the phase for the sin wave moves
#define COL_DEG 2      // Number of degrees the base hue moves per frame
#define SIN_FREQ 0.375 // Multiplier for the sin wave density - smaller = larger wave
#define COL_OFFSET 30  // Number of degrees per character the base hue is offset by
#define SW KC_SCREEN_W       // Screen width
#define SH KC_SCREEN_H       // Screen height
#define CW font->char_width  // Character width
#define CH font->char_height // Character height

char hello[] = "Hello World!"; // Store strings that will be used
char goodbye[] = "Goodbye!";
float phase = 0;       // This will be used for the sin wave animation
float color_phase = 0; // This will be used for showing color in the text

extern const uint8_t gameboy_wav_start[] asm("_binary_gameboy_wav_start"); // Utilize generated binary pointers
extern const uint8_t gameboy_wav_end[] asm("_binary_gameboy_wav_end");

void exit_anim() { // What to show when exiting
  kcugui_cls();
  UG_PutString((SW - ((CW + 1) * strlen(goodbye) - 1)) / 2, (SH - CH) / 2, goodbye);
  /* CW + 1 because UG_PutSring() adds an extra pixel of
    	space between each character and take 1 back for the
    	last character that does not need the space */
  kcugui_flush();
  vTaskDelay(pdMS_TO_TICKS(2000)); // PS version of sleep(ms)
}

static void do_powerbtn_menu() {
  int i = powerbtn_menu_show(kcugui_get_fb()); // Call the powerbutton menu

  exit_anim();
  if (i == POWERBTN_MENU_EXIT) { // This checks the input from the power button menu - without it, menu options do nothing
    kchal_exit_to_chooser();
  } else if (i == POWERBTN_MENU_POWERDOWN) {
    kchal_power_down();
  }
}

void play_sound() {
  int id = sndmixer_queue_wav(gameboy_wav_start, gameboy_wav_end, 0);
  sndmixer_play(id);
}

void app_main() {
  kchal_init();           // Initialize the PocketSprite SDK.
  kcugui_init();          // Initialize uGUI
  sndmixer_init(1, 8000); // (no. of channels, sample rate in khz)
  UG_FontSelect(font);    // The default font that is enabled out of the box
  UG_SetForecolor(C_WHITE);
  UG_SetBackcolor(C_BLACK);
  while (true) {
    kcugui_cls();

    for (int i = 0; i < strlen(hello); i++) {
      HSV hsv;
      hsv.hue = fmod(color_phase + i * COL_OFFSET, 360);
      hsv.sat = 1;
      hsv.val = 1;
      RGB rgb = hsv2rgb(hsv);

      UG_PutChar(hello[i],                                                                 // Char
                 i * CW + (SW - strlen(hello) * CW) / 2,                                   // X
                 round(sin(i * SIN_FREQ + phase) * (SH / 2 - CH / 2) + (SH / 2 - CH / 2)), // Y
                 kchal_ugui_rgb(rgb.r, rgb.g, rgb.b),                                      // FG Color
                 C_BLACK);                                                                 // BG Color
    }

    phase = fmod(phase + 1 / (2 * M_PI) * SIN_DEG, 2 * M_PI); // Move phase forward by SIN_DEG degrees per frame
    color_phase = fmod(color_phase + COL_DEG, 360.0f);    // Move the base hue by COL_DEG per frame
    kcugui_flush();                                       // Send buffer to display

    if (kchal_get_keys() & KC_BTN_POWER) { // Check for power button press
      do_powerbtn_menu();
    } else if (kchal_get_keys() & KC_BTN_A) { // Check for a button press
      play_sound();
    }
  }
}