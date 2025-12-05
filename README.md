# MAX7219 Zephyr Driver

This driver allows users to program a MAX7219 chip to be used with an 8x8 LED matrix.
This repository contains:
- Zephyr device driver for the MAX7219 for use with an LED matrix
- Devicetree overlays for the MAX32655FTHR and APARD32690
- Kconfig prj.conf enabling the driver as well as SPI
- Sample application code which flashes the display, then outputs the Analog Devices logo

This application requires basic Zephyr knowledge of workspaces, etc.
- For this example, the MAX32655FTHR will be used, but the overlay and build command can be changed to use a different board

# Hardware Setup
Hardware required:
- MAX32655FTHR board
- MAX7219 with 8x8 LED Matrix (Can be found on Amazon)
- Sparkfun Logic level converter (4 channel, bidirectional)
- Micro USB cable (with data transfer capabilities)
- Jumper wires
  
Hardware Setup:
- Connect the SPI pins on the MAX32655FTHR to the level shifter's LV side (LV1, LV2...).  MISO is not needed.
- Connect the appropriate pins on the level shifter's HV side (HV1, HV2...) to the SPI pins on the MAX7219 board.
- Connenct the 1.8V pin on the MAX32655FTHR to the LV reference pin (LV). Connect GND pin of the MAX32655FTHR to the ground reference on the LV side.
- Connenct the VBUS pin (+5V) on the MAX32655FTHR to the HV reference pin (LV). Connect GND pin of the MAX7219 board to the ground reference on the HV side. 
- Also connect the VBUS pin (+5V) on the MAX32655FTHR to the VCC pin 

# Software Setup:
Initialize a Zephyr workspace with this repo as the manifest repo
```
west init -m https://github.com/Tche-Caver/max7219-zephyr-driver max7219-wkspc
```

Move into the workspace topdir and update the workspace with modules specified in the manifest file
```
cd max7219-wkspc
west update
```

Move into the manifest repo and build the application
```
cd max7219-zephyr-driver
west build -p always -b max32655fthr/max32655/m4 app
```
In this case the MAX32655 is being used, so the board specifier is max32655fthr/max32655/m4.  This can be changed for different boards.

Flash the application
```
west flash
```

