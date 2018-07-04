#include <stdint.h> // uint8_t, etc.
#include "string.h"
#include "math.h"
#include "hsv2rgb.h"

#include "8bkc-hal.h"        // PS HW Abstraction Layer
#include "powerbtn_menu.h"   // Power Button menu stuff powerbtn_menu_show, constants, etc
#include "8bkc-ugui.h"		 // PS uGUI
#include "ugui.h"			 // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/
#include "sndmixer.h"		 // Sound Mixer library

#include "freertos/FreeRTOS.h" // These libraries are required for the vTaskDelay function
#include "freertos/task.h"

#define DEG 0.2
#define PI 3.14159

// UG_FONT font = FONT_6X8; // need to find a way to implement setting a font variable to be used later in code

char hello[] = "Hello World!"; // Store strings that will be used
char goodbye[] = "Goodbye!";
float phase = 0; // This will be used for the sin wave animation
float color_phase = 0; // This will be used for showing color in the text
float adj_height = (KC_SCREEN_H / 2) - 4; // This will be used for offsetting the sin wave to match screen height

extern const uint8_t gameboy_wav_start[] asm("_binary_gameboy_wav_start"); // Utilize generated binary pointers
extern const uint8_t gameboy_wav_end[] asm("_binary_gameboy_wav_end");

void exit_anim() { // What to show when exiting
	kcugui_cls();
	UG_PutString((KC_SCREEN_W - strlen(goodbye) * FONT_6X8.char_width) / 2, (KC_SCREEN_H - FONT_6X8.char_height) / 2, goodbye);
	kcugui_flush();
	vTaskDelay(pdMS_TO_TICKS(2000)); // PS version of sleep(ms)
}

static void do_powerbtn_menu() {
	int i = powerbtn_menu_show(kcugui_get_fb()); // Call the powerbutton menu
	
	if (i == POWERBTN_MENU_EXIT) { // This checks the input from the power button menu - without it, menu options do nothing
		exit_anim();
		kchal_exit_to_chooser();
	}
	if (i == POWERBTN_MENU_POWERDOWN) {
		exit_anim();
		kchal_power_down();
	}
}

void play_sound() {
	int id = sndmixer_queue_wav(gameboy_wav_start, gameboy_wav_end, 0);
	sndmixer_play(id);
}

void app_main() {
	kchal_init(); // Initialize the PocketSprite SDK.
	kcugui_init(); // Initialize uGUI
	sndmixer_init(1, 8000); // (no. of channels, sample rate in khz)
	UG_FontSelect(&FONT_6X8); // The default font that is enabled out of the box
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
	while (true) {
		kcugui_cls();
		
		for (int i = 0; i < strlen(hello); i++) {
			HSV hsv;
			hsv.hue = fmod(color_phase+i*5, 360);
			hsv.sat = 1;
			hsv.val = 1;
			RGB rgb = hsv2rgb(hsv);
			UG_PutChar(hello[i], // Char
				i * FONT_6X8.char_width + (KC_SCREEN_W-strlen(hello)*FONT_6X8.char_width)/2, // X
				round((sin(i * .375 + phase) * adj_height + adj_height)), // Y
				kchal_ugui_rgb(rgb.r, rgb.g, rgb.b), // FG Color
				C_BLACK); // BG Color
		}
		
		phase = fmod(phase + 1 / (2 * PI) * DEG, 2*PI); // Move phase forward by DEG degrees per frame
		color_phase = fmod(color_phase + 1, 360.0f);
		kcugui_flush(); // Send buffer to display

		if (kchal_get_keys() == KC_BTN_POWER) { // Check for power button press
			do_powerbtn_menu();
		} else if (kchal_get_keys() == KC_BTN_A) {
			play_sound();
		}
	}
}