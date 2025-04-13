# Second Challenge (Control LEDs Based on Temperature)

This challenge will be useful for:
- Getting used to using APIs/drivers in the project;
- Implementing the I2C communication protocol.

## Challenge

This challenge introduces a way to switch between the first and second challenges. It also uses LEDs to display the current temperature in comparison to a base temperature. The initial base temperature will be hardcoded (e.g., 24ºC), but it can be updated to the current temperature by pressing the user button.

One green LED will be on when the actual temperature is close to the base temperature. If the actual temperature drops below the base temperature by 1ºC, the green LED will turn off. If the temperature rises above the base temperature:
- At +0.5ºC, a second green LED turns on;
- At +1.0ºC, an orange LED turns on;
- At +1.5ºC, a red LED turns on.

<p align="center">
  <img src="../../Documents/Images/Second_challenge.png" style="background-color: white; padding: 10px;">
</p>

The image above shows what should happen when the temperature deviates from the base temperature.

## Steps

### 1. Add Peripherals and Set Pinout
- Open the folder "[Hardware](../../Hardware/)" in this repository. There, you can find the hardware datasheet and locate the microcontroller pinout.
- For this challenge, identify and configure the user slider (Slide) and the I2C pins (SCL and SDA).
- Add the I2C peripheral to the project.

### 2. Add the SHT31 API
- Go to the "Project" folder and add the SHT31 sensor API to your project.

### 3. Implement the Switch Functionality
- Implement a mechanism to switch between the first and second challenges.

### 4. Use the UI and SHT31 APIs
- Integrate both the UI and SHT31 sensor APIs into your code.

### 5. (Optional) Add `printf` for Debugging
- Enable `printf` functionality to assist with debugging.
