#include "8bkc-hal.h" // PS HW Abstraction Layer

void app_main() {
  kchal_init();            // Initialize the PocketSprite SDK.
  kchal_exit_to_chooser(); // Exit the application
}