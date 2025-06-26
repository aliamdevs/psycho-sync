
# PSYCHO-SYNC ![C++](https://img.shields.io/badge/C++-darkblue?logo=cplusplus&logoColor=white) ![Python](https://img.shields.io/badge/Python-darkgreen?logo=python&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-blue?logo=arduino&logoColor=white)

  

## Introduction

  

***Hi I'm Ali*** & This Project is a **Lightweight**, **Portable** Directory **Synchronization** Device **With** External **Memory** for receiving **Backup** that does ***NOT***  **Require** a **Host** With Backup Power.

  

 ## Dependency
 

 - Arduino Board  & And Other Module (Hardware)
 - [Psyncer Repository.](https://github.com/aliamdevs/psyncer/)

## Installation

  

### 1. Required hardware

  

  

- Wemos D1 Mini ![1](https://img.shields.io/badge/1-orange)

  

- microSD Card Adapter ![2](https://img.shields.io/badge/2-orange)

  

- microSD Card *(min.16GB max.256GB)*

  

> The Size Of The microSD card determines the sync memory capacity.

  

- Charge Module (ex. _TP-4056TYPEC_) ![3](https://img.shields.io/badge/3-orange)

  

- Rechargeable ‌Battery ![4](https://img.shields.io/badge/4-orange)

  

> If You Don't want to use Battery and Charge Module Just Remove them *( 3 , 4 )* and Plug The Arduino Board to 5V **BUT** Remember You Don't Have Backup Power

  

- BreadBoard & Jumpers Wires *(Optional)*

  

___

  

### 2. Hardware Connections

  

  

![Hardware Connections](/arduino/circuit/CONNECTIONS.jpg)

  

**Wemos D1 Mini & SD Card Adapter Pins in common:**

  

| D1 Mini | SD Adapter |
|--|--|
| GND | GND|
| 3v3 | 3v3|
| D8| CS|
| D7 | MOSI|
| D6 | MISO|
| D5 | CLK|

  

  

**Wemos D1 Mini & TP4056 Pins in common:**

  

  

| D1 Mini | TP4056 |
|--|--|
| GND | OUT -|
| 5V | OUT +|

  

  

**TP4056 & Battery :**

  

  

| TP4056 | Battery |
|--|--|
| B - | Negative OF Battery|
| B + | Positive OF Battery|

  

___

  

### 3. Program The Board
	
*Clone* or *Download* This Repository 
& *Choose* **One** of the **Methods** below.

 - Using Arduino IDE
	1.  Install Arduino IDE :
	From the Arduino website,  [Download](https://www.arduino.cc/en/Main/OldSoftwareReleases)  & install the latest Arduino IDE (v1.8 or later). Available for Windows, Mac, Linux & ARM.
	
	2. Add the ESP8266 module libraries:
	From  _**File > Preferences**_, add a  _Boards Manager URL_  `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
	From _**Tools > Board > Board Manager**_, search for _D1 mini_ and install (+/- 150MB)
	
	3. Connect The Board :
	From  _**Tools > Port**_  select the correct COM port# as assigned to the device.
	
	4. Install Libraries From  _**Tools> Manage Libraries**_ Search And Install 
	. ESP8266.h Library
	. SD.h Library
	
	5. Open the Arduino Skech from ` ./arduino/arduino_skech/arduino_skech.ino`
	
	6. Upload Code On the Module With Clicking on Upload Icon like below Image .

	![Sketch](/arduino/circuit/Sketch.jpg)

 - Using esptool/py
    1. Dependencies :
    . Install [Python](https://www.python.org/) .
    . Install [esptool.py](https://github.com/espressif/esptool?tab=readme-ov-file) .
    2. Find The COM You use And Run The Script In `./bin` Directory :
    ```bash
    cd ./bin
    esptool --port COM4 write-flash 0x0000 ./esptool.bin
    ```
	
> If You Have Unkown Error *or* For More Information About Esptool Refer to 		[Esptool.py Docs](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/basic-commands.html) .

---
### 4. Setup microSD Card
- **Simple Way :**
	1. Connect To `psycho-sync`  WiFi with `12345678` Password.
	2. Open `http:/40.0.0.4/sd/template/simple/` url.
	3. Done SD Card is Set With Simple Functions.
- **Advance Way :**
    1. Access to microSD Card.
    2. Format it *(Earese All Data)*.
    3. Copy & Paste The Content of `./sd` Directory into microSD Card.
    4. Done.
---
### 5. Install Psyncer
Go To [**Psyncer Repository**](https://github.com/aliamdevs/psyncer/) & Install it .

## Usage

 - [Psyncer Repository.](https://github.com/aliamdevs/psyncer/)
		A Debug Enviromant that Sync a Directory Between Devices Connected to Psycho-Syncer & Module itself.
 - [Manual Psycho-Syncer Docs.](./docs/README.md)
 With Some *POST & GET* Requests **Send** *or* **Get** *and* **Manage** Files in microSD Card.

## License

[MIT](./LICENSE.txt)