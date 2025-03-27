## Finding Ro (Reverse Osmosis) Value

```ino
#include "MQ2_LPG.h" // library declaration
#define MQ2PIN A0 // mq2 pin declaration

MQ2Sensor mq2(MQ2PIN); // create a new object with the name mq2 to hold the MQ2Sensor class

void setup() {
  Serial.begin(9600); // default baudrate for the Arduino board
  mq2.begin(); // initiate mq2 sensor
}

// gas calibration data
#define RL_Value 10 // 10K ohm
#define x1_Value 0
#define x2_Value 0
#define y1_Value 0
#define y2_Value 0
#define x_Value 0
#define y_Value 0
#define Voltage_Value 5.0
#define bitADC_Value 1023.0 // development board adc resolution

void calibration(){
  mq2.RL(RL_Value); // resistance load setting
  mq2.Volt(Voltage_Value); // voltage sensor setting
  mq2.BitADC(bitADC_Value); // development board adc resolution setting
  mq2.mCurve(x1_Value, x2_Value, y1_Value, y2_Value); // mCurve setting
  mq2.bCurve(x_Value, y_Value); // bCurve setting
  mq2.getCalibrationData(); // get data calibration
  mq2.viewCalibrationData(); // print to serial monitor: data calibration
}

void loop() {
  calibration(); // calls calibration method
  delay(3000); // delay for 3 seconds
}
```

<br>

<div align="justify">
  
<i>After the Ro data is obtained, the next step is to enter the Ro data into the #define section.</i>

```ino
#define Ro_Value 6.31
```

<br><br>

## Finding the Right Coordinate Point

<i>Open the following link: <a href="https://automeris.io/WebPlotDigitizer/">WebPlotDigitizer</a> , then click ``` Launch Now! ```. Then click ``` Load Image ```. You can find out more about this below.</i>

<img src="../documentation/experiment/Load Image.jpg" alt="load-image">

<i>Upload the image according to the link provided: <a href="../documentation/experiment/Calibration Graph.jpg" alt="calibration-graph">Click here</a>.</i>

<br>

<i>Then choose a plot type: ``` 2D (X-Y) Plot ```. Then click ``` Align Axes ``` → ``` Proceed ```.</i>

<img src="../documentation/experiment/Plot Type.jpg" alt="plot-type"><br>

<i>Next, you must provide a value limit, starting from ``` X1 → X2 → Y1 → Y2 ```. Then click ``` Complete! ``` to set the ``` X-Axis ``` and ``` Y-Axis ```. Don't forget to tick the ``` Log Scale ``` → ``` OK ``` section.</i>
  
<img src="../documentation/experiment/Axes Calibration.jpg" alt="axes-calibration"><br>

<i>In the next step, click ``` Add Point (A) ```, then select the desired curve (in this case using only LPG and Propane). Don't forget to look for coordinate points that overlap each other as shown in the following image.</i>

<img src="../documentation/experiment/Add Point.jpg" alt="add-point"><br>

<i>Then, click ``` View Data ``` to find out the value of the coordinate point that was selected earlier. For example, as shown in the image below.</i>

<img src="../documentation/experiment/View Data.jpg" alt="view-data"><br>

<i>In the final step, the coordinate data above is then entered into the code in the #define section of the Arduino IDE.</i>

```ino
#define x1_Value 199.150007852152
#define x2_Value 797.3322752256328
#define y1_Value 1.664988323698715
#define y2_Value 0.8990240080541785
#define x_Value 497.4177875376839
#define y_Value 1.0876679972710004
```

<br>
  
  Then upload the program.&nbsp;&nbsp;&nbsp;<strong>~ DONE... , GOOD LUCK</strong> ~

</div>
