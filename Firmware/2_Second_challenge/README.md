# Second Challenge (Control LEDs Based on Temperature)

This challenge will be useful for:
- Getting used to using APIs/drivers in the project;
- Implementing the I2C communication protocol.

## Challenge

This challenge introduces a way to switch between the first and second challenges. It also uses LEDs to display the current temperature in comparison to a base temperature. The initial base temperature will be hardcoded (e.g., 24ºC), but it can be updated to the current temperature by pressing the user button.

One green LED will be ON when the actual temperature is close to the base temperature. If the actual temperature drops below the base temperature by 1 ºC, the green LED will turn off. If the temperature rises above the base temperature:
- At +0.5 ºC, a second green LED turns on;
- At +1.0 ºC, an orange LED turns on;
- At +1.5 ºC, a red LED turns on.

<p align="center">
  <img src="../../Documents/Images/Second_challenge.png" style="background-color: white; padding: 10px;">
</p>

The image above shows what should happen when the temperature deviates from the base temperature.

## Steps

### 1. Add Peripherals and Set Pinout
- Go to the Altium (or PDF) file used in the First challenge.
- For this challenge, identify and configure the user slider (Slide) and the I2C pins (SCL and SDA).
- Add the I2C peripheral to the project.

### 2. Add the SHT31 API
- Go to the "Project" folder and add the SHT31 sensor API to your project.

### 3. Implement the Switch Functionality
- Implement a mechanism to switch between the first and second challenges.

### 4. Use the UI and SHT31 APIs
- Integrate both the UI and SHT31 sensor APIs into your code.

### 5. Enable `printf` for Debugging and checking the measured temperature and humidity (if not configured in the previous challenge)
- Enable `printf` functionality to assist with debugging, by configuring the UART peripheral and defining the following function in your main.c file.
```c
/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/* USER CODE END 4 */
```

> [!WARNING]  
> Don't forget to enable the use of float values in printf.