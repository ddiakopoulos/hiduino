# HIDUINO

The HIDUINO project aims to provide precompiled firmwares, documentation, and example code for building a class-compliant USB-MIDI device from an Arduino UNO, Mega2560, or Due. These firmwares are built using the [LUFA framework](https://github.com/abcminiuser/lufa-lib) by [Dean Camera](http://www.fourwalledcubicle.com/). HIDUINO was previously developed for robotic instruments and new musical interfaces @ the [California Institute of the Arts](http://mtiid.calarts.edu).

The [NotomotoN](http://vimeo.com/33365051) is a good example of HIDUINO in action!

## Requirements

The full list of requirements is listed on the wiki. At a bare minimum, you'll need:

* Crosspack for OSX or WinAVR for Windows
* Arduino 1.0 - [Arduino MIDI Library v 3.2](http://arduino.cc/playground/Main/MIDILibrary)
* (Recommended) [AVRISPMKII](http://www.atmel.com/tools/AVRISPMKII.aspx)

## Quickstart

The Github wiki contains a host of information on working with HIDUINO. HIDUINO can be flashed onto the ATmega (8u2/16u2) chip on the UNO, Mega2560 (Rev. 2+) and Due. Sketches cannot be bootloaded onto an Arduino while a HIDUINO firmware is loaded on the 8u2, so users can expect to switch between the default usbserial and HIDUINO firmwares regularly during development. The flashing process can be accomplished using an ISP (recommended) or through the DFU bootloader.

Flashing the default arduino usbserial firmware using an avrispmkii:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m


Flashing the HIDUINO_MIDI firmware:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:HIDUINO_MIDI.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m

## License

HIDUINO is released under the MIT license.
