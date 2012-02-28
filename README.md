# HIDUINO

The HIDUINO project aims to provide ready-to-download firmwares, documentation, and example code for turning a Rev. 2+ Arduino UNO or Mega2560 into a class-compliant, driverless MIDI/HID device. These firmwares are directly based off of the [LUFA framework](https://github.com/abcminiuser/lufa-lib) by [Dean Camera](http://www.fourwalledcubicle.com/), a generic USB/HID library for Atmel's line of ATmega USB-compatible microcontrollers. HIDUINO is actively developed by [Dimitri Diakopoulos](http://www.dimitridiakopoulos.com) and sponsored by the [California Institute of the Arts](http://calarts.edu). 

The first major revision of stable firmwares (with additional functionality and bug fixes) was released in October 2011.

Interested in seeing what HIDUINO can do? Check out the [NotomotoN, a robotic percussionist](http://vimeo.com/33365051). 

## Requirements 

The full list of requirements is listed on the wiki. At a bare minimum, you'll need: 

* Crosspack for OSX or WinAVR for Windows
* Arduino 1.0+ - [Arduino MIDI Library v 3.2](http://arduino.cc/playground/Main/MIDILibrary)
* Arduino < 1.0 - [Arduino MIDI Library v3.1.1](http://arduino.cc/playground/Main/MIDILibrary)
* (Recommended) [AVRISPMKII](http://www.atmel.com/tools/AVRISPMKII.aspx)

## Quickstart
 
The Github wiki has a host of information about using HIDUINO. HIDUINO can be flashed onto the ATmega 8u2 chip on Revision 2 (and newer) Arduino UNO and Mega 2560s. Sketches cannot be bootloaded onto an Arduino while a HIDUINO firmware is loaded on the 8u2, so users can expect to switch between the default usbserial and HIDUINO regularly during development. The flashing process can be accomplished via an external ISP (recommended) or DFU bootloader. 


Flashing the default arduino usbserial firmware using an avrispmkii: 

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m


Flashing the HIDUINO_MIDI firmware: 

    avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:HIDUINO_MIDI.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
