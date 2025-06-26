
# PSYCHO-SYNC ![C++](https://img.shields.io/badge/C++-darkblue?logo=cplusplus&logoColor=white) ![Python](https://img.shields.io/badge/Python-darkgreen?logo=python&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-blue?logo=arduino&logoColor=white)

## Introduction

***Hi I'm Ali*** & This Project is a **Lightweight**, **Portable** Directory **Synchronization** Device **With** External **Memory** for receiving **Backup** that does ***NOT***  **Require** a **Host** With Backup Power.

  

## Installation

### 1. Required hardware

  

- Wemos D1 Mini ![1](https://img.shields.io/badge/1-orange)

- microSD Card Adapter ![2](https://img.shields.io/badge/2-orange)

- microSD Card *(min.16GB max.256GB)*

> The Size Of The microSD card determines the sync memory capacity.

- Charge Module (ex. _TP-4056TYPEC_) ![3](https://img.shields.io/badge/3-orange)

- Rechargeable ‌Battery ![4](https://img.shields.io/badge/4-orange)

> If You Don't want to use Battery and Charge Module Just Remove them *( 3 , 4 )* and Plug The Arduino Board to 5V **BUT** Remember You Don't Have Backup Pqower

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

### 3. Program Board 


## License

[MIT](https://choosealicense.com/licenses/mit/)
