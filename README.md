# HIDUINO

The HIDUINO project aims to provide precompiled firmwares, documentation, and example code for building a class-compliant USB-MIDI device from an Arduino UNO, Mega2560, or Due. These firmwares are built using the [LUFA framework](https://github.com/abcminiuser/lufa-lib) by [Dean Camera](http://www.fourwalledcubicle.com/). HIDUINO was previously developed for robotic instruments and new musical interfaces @ the [California Institute of the Arts](http://mtiid.calarts.edu). HIDUINO relies on Arduino boards where a second AVR chip is used as the USB controller, so it won't work with single chip boards (e.g. Leonardo), nor older models that used an FTDI chip as USB controller (e.g. Duemilanove). 

Some good examples of HIDUINO:

[mHzKontrol](http://www.youtube.com/watch?v=f4GCczAaD8A)

[NotomotoN](http://vimeo.com/33365051)

## Requirements

The full list of requirements is listed on the wiki. At a bare minimum, you'll need:

* Crosspack for OSX or WinAVR for Windows
* Latest Arduino IDE + [Latest Arduino MIDI Library](http://arduino.cc/playground/Main/MIDILibrary)
* (Recommended) [AVRISPMKII](http://www.atmel.com/tools/AVRISPMKII.aspx)

## Quickstart

The Github wiki contains a host of information on working with HIDUINO. HIDUINO can be flashed onto the ATmega (8u2/16u2) chip on the UNO, Mega2560 and Due. Sketches cannot be bootloaded onto an Arduino while a HIDUINO firmware is loaded on the 8u2, so users can expect to switch between the default usbserial and HIDUINO firmwares regularly during development. The flashing process can be accomplished using an ISP (recommended) or through the DFU bootloader.

If you are using the ISP method, an easier way of development is to flash the USB controller chip with HIDUINO just once. Then, in the Arduino IDE, you can select "Upload Using Programmer" and connect the ISP to the header block near the main chip. This means you can flash your new Arduino sketch from the IDE and never have to switch firmwares on the commandline. A major caveat of this method is that you can't debug values over serial. 

Flashing the HIDUINO_MIDI firmware located in the Compiled Firmwares directory:

```Shell
avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:HIDUINO_MIDI.hex \
-U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
```

The latest revisions (3+) of the Mega and UNO (and Due) use the Atmega 16u2 chip, so note the name changes of the default usbserial firmware below. On older revisions, the 8u2 chip is used. 8u2 firmwares are also included in the Compiled Firmware directory. The only important distinction between them is the product string that appears when an Arduino connects to a computer. avrdude will give a generic error if you try to flash an 8u2 chip with a 16u2 firmware and vice-versa. The at90usb82 chip profile is used for the following commands as it resembles both 16u2 and 8u2 -- one less thing to remember to change.

Flashing usbserial onto an UNO using an avrispmkii:

```Shell
avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_uno_16u2.hex \
-U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
```

Flashing usbserial onto a Mega2560 using an avrispmkii:

```Shell
avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_mega_16u2.hex \
-U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
```

 Flashing usbserial onto a Due using an avrispmkii:

 ```Shell
avrdude -p at90usb82 -F -P usb -c avrispmkii -U flash:w:usbserial_due_16u2.hex \
-U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
```

## A Note About Compiling
Compiling the firmware from scratch lets you change the name of the USB device (default HIDUINO). Right now, the latest LUFA doesn't work with the the src in this repository, although the v1.0 tag will work with an older version of LUFA noted in the 1.0 tag description. 

## License

HIDUINO is released under the MIT license.
