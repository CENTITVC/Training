# Second challenge (Control LEDs based on temperature)
This challenge will be usefull for:
- Get used to use APIs/drivers in the project;
- Implement I2C communication protocol.

## Challenge
This challenge will allow to switch between the first and the second challenge. Also will display, using the LEDs, the actual temperature in comparison to a base temperature. The initial base temperature will be hardcoded (24ºC for example), but it can be changed by the press of the user button to the actual temperature. The LEDs will have one green LED on, indicating that the actual temperature is close to the base temperature. If the actual temperature falls below the base temperature by 1ºC the green LED will turn off. If the actual temperature goes above the base temperature by 0.5ºC a new LED will turn on (+0.5ºC -> second green, +1ºC -> orange, +1.5ºC red).

<p align="center">
  <img src="../../Documents/Images/Second_challenge.png" style="background-color: white; padding: 10px;">
</p>

The image above show what should happen when the temperature deviates from the base temperature.


## Steps
### 1. Add peripherals and set pinout
- For this step you will need to open the folder "[Hardware](../../Hardware/)" in this repository. There you can open the hardware datasheet and search for the microcontroller sheet.
- For this challenge, you will need to add the user slider (Slide), and the I2C pins (SCL, and SDA).
- You will need to add the I2C peripheral.

### 2. Add the SHT31 API
- Go to the folder "Project" and add the SHT31 sensor API.

### 3. Implement the switch functionality
- Implement the switch functionality to allow swap between the first and the new second challenge.

### 4. Use the UI and SHT31 APIs

### 5. (Extra) Add printf to allow debug
