## Mencari Nilai Ro (Reverse Osmosis)

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

Setelah data Ro didapatkan, maka langkah selanjutnya yaitu memasukkan data Ro tersebut ke bagian #define.

```ino
#define Ro_Value 6.31
```

<br><br>

## Mencari Titik Koordinat Yang Sesuai
  
  Bukalah link berikut: <a href="https://automeris.io/WebPlotDigitizer/">WebPlotDigitizer</a> , lalu klik ``` Launch Now! ```. Kemudian klik ``` Load Image ```. Hal tersebut dapat anda ketahui selengkapnya di bawah ini.

<img src="../documentation/experiment/Load Image.jpg" alt="load-image">
  
  Upload gambar sesuai dengan link yang telah disediakan: <a href="../documentation/experiment/Calibration Graph.jpg" alt="calibration-graph">Klik disini</a>.

<br>
  
  Kemudian memilih tipe plot: ``` 2D (X-Y) Plot ```. Selanjutnya klik ``` Align Axes ``` → ``` Proceed ```.

<img src="../documentation/experiment/Plot Type.jpg" alt="plot-type"><br>
  
  Selanjutnya, anda harus memberikan batas nilai, mulai dari ``` X1 → X2 → Y1 → Y2 ```. Selanjutnya klik ``` Complete! ``` untuk mengatur ``` X-Axis ``` dan ``` Y-Axis ```. Jangan lupa untuk mencentang bagian ``` Log Scale ``` → ``` OK ```.

<img src="../documentation/experiment/Axes Calibration.jpg" alt="axes-calibration"><br>
  
  Langkah berikutnya, klik ``` Add Point (A) ```, lalu pilih kurva yang diinginkan (dalam hal ini hanya menggunakan LPG dan Propane). Jangan lupa cari titik koordinat yang saling berhimpitan satu sama lain seperti yang ditampilkan pada gambar berikut.

<img src="../documentation/experiment/Add Point.jpg" alt="add-point"><br>
  
  Kemudian, klik ``` View Data ``` untuk mengetahui nilai dari titik koordinat yang telah dipilih tadi. Misalnya seperti yang terlihat pada gambar di bawah ini.

<img src="../documentation/experiment/View Data.jpg" alt="view-data"><br>
  
  Langkah terakhir, data koordinat diatas kemudian dimasukkan kedalam kode yang ada di Arduino IDE bagian #define.

```ino
#define x1_Value 199.150007852152
#define x2_Value 797.3322752256328
#define y1_Value 1.664988323698715
#define y2_Value 0.8990240080541785
#define x_Value 497.4177875376839
#define y_Value 1.0876679972710004
```

<br>
  
  Lalu unggah program.&nbsp;&nbsp;&nbsp;<strong>~ SELESAI... , SELAMAT MENCOBA</strong> ~

</div>
