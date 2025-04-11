# First challenge (Control LEDs based on button press time)
This challenge will be useful for:
- Learn how to create a project using the STM32CubeIDE;
- Learn how to interpret an hardware datasheet;
- Get used to use APIs/drivers in the project.


## Challenge
This challenge will be to write a code that will turn on/off each LED depending on the amount of time the user button is being pressed. For each second that passes while the button is being pressed, one LED should turn on. When the button is released, for each half of a second passes, one LED should turn off.

<p align="center">
  <img src="../../Documents/Images/First_challenge.png" style="background-color: white; padding: 10px;">
</p>

The image above show what should happen when the button is being pressed or released.


## Steps
### 1. Create a project
- Open the STM32CubeIDE and create a project for the microcontroller STM32F401RBT6.

### 2. Add peripherals and set pinout
- For this step you will need to open the folder "[Hardware](../../Hardware/)" in this repository. There you will have a pdf file which is the hardware datasheet. Search for the sheet with the microcontroller pinout to check which pins are in use.
- For this challenge you will need to search for the LEDs pins (LED1, LED2, LED3, LED4), and the button pin (Button1).
- You will need no peripheral for this challenge.

### 3. Add the folder "Project" to the project
- Get the folder "Project" and add to the your STM32 project.

### 4. Call the manager functions
- To avoid adding to much code to the main.c file, our code will be written in the manager.c file.
- For that, just include the manager.h in the main.c and call the functions Manager_Intialize() and Manager_Loop().

### 5. Write the manager code
- Use the manager.c file to write the code for this challenge.

### 6. Use the UI API present in the Project/UI folder
- To use this API you will need to adapt you code in the manager to be able to use the UI API. You cannot change the UI API.

### 7. (Extra) Add printf to allow debug
- Enable the functionality of doing printf for a better debug.
