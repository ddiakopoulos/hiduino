# HIDUINO

The HIDUINO project aims to provide precompiled firmwares, documentation, and example code for building a class-compliant USB-MIDI device from an Arduino UNO, Mega2560, or Due. These firmwares are built using the [LUFA framework](https://github.com/abcminiuser/lufa-lib) by [Dean Camera](http://www.fourwalledcubicle.com/). HIDUINO was previously developed for robotic instruments and new musical interfaces @ the [California Institute of the Arts](http://mtiid.calarts.edu).

The [NotomotoN](http://vimeo.com/33365051) is a good example of HIDUINO in action!

## Requirements

The full list of requirements is listed on the wiki. At a bare minimum, you'll need:

* Crosspack for OSX or WinAVR for Windows
* Arduino 1.0 - [Arduino MIDI Library v 3.2](http://arduino.cc/playground/Main/MIDILibrary)
* (Recommended) [AVRISPMKII](http://www.atmel.com/tools/AVRISPMKII.aspx)

## Quickstart

The Github wiki contains a host of information on working with HIDUINO. HIDUINO can be flashed onto the ATmega (8u2/16u2) chip on the UNO, Mega2560 and Due. Sketches cannot be bootloaded onto an Arduino while a HIDUINO firmware is loaded on the 8u2, so users can expect to switch between the default usbserial and HIDUINO firmwares regularly during development. The flashing process can be accomplished using an ISP (recommended) or through the DFU bootloader.

Flashing the HIDUINO_MIDI firmware located in the Compiled Firmwares directory:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:HIDUINO_MIDI.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m

The latest revisions (3+) of the Mega and UNO (and Due) use the Atmega 16u2 chip, so note the name changes of the default usbserial firmware below. On older revisions, the 8u2 chip is used. 8u2 firmwares are also included in the Compiled Firmware directory. The only important distinction between them is the product string that appears when an Arduino connects to a computer. avrdude will give a generic error if you try to flash an 8u2 chip with a 16u2 firmware and vice-versa. The at90usb82 chip profile is used for the following commands as it resembles both 16u2 and 8u2 -- one less thing to remember to change. 

Flashing usbserial onto an UNO using an avrispmkii:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_uno_16u2.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m

Flashing usbserial onto a Mega2560 using an avrispmkii:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_mega_16u2.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m

 Flashing usbserial onto a Due using an avrispmkii:

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_due_16u2.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m

## License

HIDUINO is released under the MIT license.
