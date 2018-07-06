# PocketSprite Hello World Tutorial

Before jumping straight into the tutorial, I wanted to give a heads-up that this is my first time working with compiled C. Before this, I worked with the Arduino IDE and Python for the most part, so I'm not entirely used to working with environment variables and the `make` command all that much.

I should also note that any time there is a link within an explanation of a step, it's expected that you're looking at that page when I'm referring to that page. In other words, if you see the link, click it!

That being said, this was certainly quite the adventure, and many thanks go out to @Spritetm and my close friend  ***Mike*** for all of their support on me learning this project. Without further ado, let's get started!

## Reference Materials

If you're going to be working with the PocketSprite, (I'll be referring to it as the PS from now on), you're going to be referencing *a lot* of documentation. Here are some of the documents and resources I found useful throughout the project:

* PS SDK’s documentation: http://pocketsprite-sdk.readthedocs.io/en/latest/
* PS Unofficial Discord: https://discord.gg/ZFka8Qa
* PS Subreddit: https://reddit.com/r/PocketSprite/
* PS Github: https://github.com/PocketSprite
* µGUI Github: https://github.com/achimdoebler/UGUI
* µGUI Reference Guide: http://embeddedlightning.com/download/reference-guide/ ([mirror just in case](https://drive.google.com/open?id=18mFoiasivllBQAZpeXbYaeohxYOpiYLc))

And this isn't everything that I referenced for this simple little project. Undoubtedly, the most powerful resource, as any programmer will confirm, was my trusty Google search. I'll explain along the way where I struggled and how I came to my solutions.

## Setting up the C Environment

I should mention now that I do all of my work on this device in Linux. Mint to be specific. If you're doing this in Windows or Mac, I'm not much help. That being said, let's follow our guides in order.

The first thing mentioned in the Getting Started section of the [PS SDK documentation](http://pocketsprite-sdk.readthedocs.io/en/latest/gettingstarted/index.html#software) for software is that we need to [setup ESP-IDF](https://esp-idf.readthedocs.io/en/latest/get-started/index.html), which is the compiler for the ESP32; the microcontroller the PS runs on. This page looks daunting at first, but just scroll down until you find the nice large, friendly icons and click the Linux icon.

Follow the instructions step-by-step. *Slowly.* I had a lot of trouble here since it was my first go around with this, but just read everything on the page and follow each step carefully. Something I should mention briefly, although it will apply to few, I happen to use an alternative shell that's not `bash`, so I would occasionally run into weirdness. If you do that yourself, just save yourself the hassle and use `bash`.

Once you've followed the steps on the Toolchain Setup, follow the [Get ESP-IDF](https://esp-idf.readthedocs.io/en/latest/get-started/index.html#get-started-get-esp-idf) link. I recommend following the steps outlined in the `hello_world` project just to make sure everything compiles. Once it comes back without any errors, you can safely delete your `hello_world` folder. However, you can skip the steps talking about enabling permissions for `/dev/tty0` and the parts about flashing since we're going to be uploading the files for our code. You can simply do the following to test that creating the project folder worked:

```bash
cd ~/esp/hello_world
make
```

***Note: Need to check with Mike on how to make this compile***

You should get an output similar to this:

```
[Imagine good output here]
```

Once that's done, it's time to...

### Setup the PocketSprite Environment

Again, just slowly follow the steps provided in the [PS SDK Getting Started](http://pocketsprite-sdk.readthedocs.io/en/latest/gettingstarted/index.html#software) page. Once you have your environment variables setup and ready to go, it's time to get started on our actual program!

## Build Your First Project

The absolute minimum required for getting your project started is the following:

* ***List of things that we need***

Once you have that, we can open up our `main.c` file and add the following code:

```C
#include "8bkc-hal.h"      // PS HW Abstraction Layer

void app_main()
{
    kchal_exit_to_chooser(); // Exit the application
}
```

I'll give you 3 guesses as to what this program does, but chances are, you only need one. This will simply exit the application and bring you back to the Wifi connection screen. Nothing too exciting, but it will prove that we can make an app.

Next, we'll need to get this code running on the device. You can simply run the following:

```bash
cd ~/esp/hello-world
make
```

The first time you run this, it can take some time. If you want to speed up the process, you can use `make -j X` with X being the number of cores/threads your computer has available.

This step will create a new directory, `build`, which contain all of the compiled code. The one we're interested in is `hello-world.bin`. Technically you can change this to the .spp extension if you want, but the device recognizes the .bin extension without any issue, so I personally just use the .bin.

If you haven't already figured out how to upload files to the device, it's fairly simple. Power on the device and leave it on the
```
WIFI AP
 pkspr
GO TO:
HTTP://192.
168.4.1/
```
screen. Then, connect your computer to that WiFi access point. There will be no internet connection unless you also have an ethernet connection. Once connected, open the http://192.168.4.1 URL the browser of your choice and click the "Upload!" button. Then navigate to the `build` folder and choose our `hello-world.bin`. Once uploaded, press any button on the PS and choose hello-world from the list.

If we did everything right, it should pause for just a brief moment, then return to the same WiFi screen we saw earlier. Success! (unless not, in which case, good luck and use your resources!)

From this point forward, any time we have code that *should* run, I'll put a full code link at the end of a section for comparison. Whenever you see the full code link, also assume that you can compile the code and upload to the device and it should work.

Note: If at any point, you find that my full code *does not* work, feel free to reach out to me on [the Discord](https://discord.gg/ZFka8Qa) \@Otacon#6446.

## Getting Text on the Screen

Now that we have a base for our program, let's get started with getting some text on the screen.

We'll need to bring in a few things for this. Add the following lines next to our other `#include` statement:

```C
#include "8bkc-ugui.h"     // PS uGUI
#include "ugui.h"          // Full uGUI library - Full uGUI reference guide: http://embeddedlightning.com/download/reference-guide/
```

Then add this to the `app_main()` function:

```C
UG_FontSelect(&FONT_6X8); // The default font that is enabled out of the box
UG_SetForecolor(C_WHITE); // You can find a full list of colors in the µGUI Reference Guide
UG_SetBackcolor(C_BLACK);

UG_PutString(0, 0, "Hello World!"); // UG S16 x , UG S16 y , char* str
kcugui_flush(); // Send buffer to display
```

You can remove the `kchal_exit_to_chooser();` line we added earlier for now. Otherwise, it won't stay on the screen for very long.

[Code Checkpoint 1](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/code/main1.c)

The important thing to note here is that we didn't give ourselves a way to exit the app. none of the buttons will respond and it appears we're stuck. At any point (no matter how stuck it appears), you can press the Start and Power buttons at the same time to force a reboot. This will come in quite handy while we're writing for the PS and something goes wrong. I speak from experience when I say that it will happen often.

At this point, you should be able to see just the text "Hello World!" on the display. It's a bit small so the '!' gets put on the next line.

## Configuring the Power Button Menu

Time to include more libraries. Add the following line where the other libraries are:

```C
#include "powerbtn_menu.h"   // Power Button menu stuff powerbtn_menu_show, constants, etc
```

Then add the following above `app_main()`:

```C
static void do_powerbtn_menu() {
	int i = powerbtn_menu_show(kcugui_get_fb()); // Call the powerbutton menu
	
	if (i == POWERBTN_MENU_EXIT) { // This handles the input from the power button menu - without it, menu options do nothing
		kchal_exit_to_chooser();
	}
	if (i == POWERBTN_MENU_POWERDOWN) {
		kchal_power_down();
	}
}
```

Okay. What did we just add? First off, the line `int i = powerbtn_menu_show(kcugui_get_fb());` calls the power button menu from `powerbtn_menu.h` and since the function outputs to an `int` type variable, we need to store that output to something. In this case, the variable `i` that we initialized.

Now that `i` has a value, we need to do something with it. So we check it against the constants, which you can read about ***here***

We then need to add a section to our `app_main()` to tell it to check the hardware buttons for any input:

```C
while (true) {
    if (kchal_get_keys() & KC_BTN_POWER)
    { // Check for power button press
        do_powerbtn_menu();
    }
}
```

Don't be tempted to remove the brackets to shorten things. We'll be adding more here later on.

[Code Checkpoint 2](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/code/main2.c)

Now when you press the power button, you should see the menu that shows for the built-in emulators, including the brightness and volume controls.

## Let's Make it Move!

If you're not a fan of math, this next section is not going to be the easiest to read, but I've done my best to explain how everything works. If we want to replicate what I've done in the video where the text moves along a sin wave, we'll need some more libraries, one of which, is not part of the SDK or C itself, because I had to write my own.

You'll need to download the [header file](https://github.com/otacon239/PS_HelloWorld/blob/master/main/hsv2rgb.h) and [C file](https://github.com/otacon239/PS_HelloWorld/blob/master/main/hsv2rgb.c) and place them in the `main` folder. This handles the conversion from Hue, Saturation, Value to Red, Green, Blue. If you're feeling adventerous, you can read about it [here](https://www.rapidtables.com/convert/color/hsv-to-rgb.html).

Let's add the following:

```C
#include <stdint.h> // uint8_t, etc.
#include "string.h" // Needed for str_len()
#include "math.h" // Needed for fabs(), fmod(), sin()
#include "hsv2rgb.h" // Header I created for converting HSV to RGB
```

We first need to split our "Hello World" string into multiple parts and make it into a variable we can use. We'll be taking out the

```C
UG_PutString(0, 0, "Hello World!");
kcugui_flush(); // Send buffer to display
```

lines for now. Let's start by defining the string using the following line:

```C
char hello[] = "Hello World!"; // Store strings that will be used
```

This will make the `"Hello World!"` string into an array of characters that we can iterate through in a loop so that each of them can get unique X and Y values.

Before we get into anything too complex, let's define some values to make our lives easier later on:

```C
const UG_FONT *font = &FONT_6X8; // Define font for more readable code

#define SW KC_SCREEN_W       // Screen width
#define SH KC_SCREEN_H       // Screen height
#define CW font->char_width  // Character width
#define CH font->char_height // Character height
```

Let's also swap out the `UG_FontSelect(&FONT_6X8);` line for `UG_FontSelect(font);` now that we have that as a variable.

Now that we have some simple variable to rely on, let's restore the functionality we started with by making our own version of the `UG_PutString()` function. Add the following lines to the `while()` loop:

```C
kgucui_cls(); // Clear the display
for (int i = 0; i < strlen(hello); i++) {
	UG_PutChar(hello[i], // Char
		i * CW, // X
		0, // Y
		C_WHITE, // FG Color
		C_BLACK); // BG Color
}
kcugui_flush(); // Push the buffer to the display
```

It will look a bit odd this early on to put these on separate lines, but as these values become more complex, it will be very useful having them split up like this.

If you run the code at this point, you should have almost the same result as before, except that the text is a bit more condensed since the `UG_PutString()` command adds an extra pixel of space in between each letter, which isn't necessary with this font. So it now fits on one line.

The first thing we'll do is center the text. This is where the math comes in. See the below picture:

`![Audacity opening screen](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/images/ti1.png)`

We're trying to get the value of `X` here. We figure out the width of the text by taking the length of the string and multiplying it by the number of characters in the string with `CW*strlen(hello)`.

Next, we need to figure out to total of the blank space left over by subtracting that from `SW` so we get `SW-CW*strlen(hello)`. Then we just need to get half of that value: `(SW-CW*strlen(hello))/2`. Now we just add that offset to each character.

Your `UG_PutChar` function should look like this now:
```C
UG_PutChar(hello[i],                        // Char
	i * CW + (SW - strlen(hello) * CW) / 2, // X
	0,                                      // Y
	C_WHITE,                                // FG Color
	C_BLACK);                               // BG Color
```

# 

# 

## Making Some Noise

Time for us to make use of that built-in speaker. Some important information about the speaker and some challenges I faced. First off, it's important to know that the PS processes audio best when it's an 8-bit audio file with a mono channel. It can technically handle a high sample rate, but since we're using such a small speaker and trying to save on space, I found myself processing the audio at 8 khz.

It can be a bit of a challenge making sure the audio meets those requirements. If you don't need this or don't want to do this yourself, you can skip to the "Adding our code" section and just use [this file](https://github.com/otacon239/PS_HelloWorld/blob/master/main/sound/gameboy.wav).

If you want to do it yourself, read on!

#### Converting the audio

I personally used Audacity for this since it's free and fairly simple to do what we're doing in it. To start, open any file that you want to convert. I recommend a short sound so that it doesn't take up much space.

You'll be presented with the following screen:

`![Audacity opening screen](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/aud1.png)`

First, we'll want to trim off the extra silence at the beginning and end. You can do this by selecting a section like text, then just pressing the Delete key:

`![Deleting audio](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/aud2.png)`

Next, adjust the sample rate by choosing `Tracks > Resample...` and resample it to 8000 Hz. To make it a Mono track, choose `Tracks > Stereo to Mono`. 

Finally, we can export it. Choose `File > Export Audio`. For filetype, choose "Other uncompressed formats". Set the header to WAV, encoding to Unsigned 8-Bit PCM. Make sure to change the file extension to .wav.

`![File Settings](https://github.com/otacon239/PS_HelloWorld/tree/master/main/tutorial/aud3.png)`

Keeping the file name short will save you some hassle later on, so keep that in mind.

#### Adding our code

Again, we'll need to add another library:

```C
#include "sndmixer.h"		 // Sound Mixer library
```

Now we need to do something a bit different. We need to tell the compiler