
# PSYCHO-SYNC ![C++](https://img.shields.io/badge/C++-darkblue?logo=cplusplus&logoColor=white) ![Python](https://img.shields.io/badge/Python-darkgreen?logo=python&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-blue?logo=arduino&logoColor=white)

## Introduction

***Hi I'm Ali*** & This Project is a **Lightweight**, **Portable** Directory **Synchronization** Device **With** External **Memory** for receiving **Backup** that does ***NOT***  **Require** a **Host** With Backup Power.

  

## Installation

### Required hardware

  

- Wemos D1 Mini ![1](https://img.shields.io/badge/1-orange)

- microSD Card Adapter ![2](https://img.shields.io/badge/2-orange)

- microSD Card *(min.16GB max.256GB)*

> The Size Of The microSD card determines the sync memory capacity.

- Charge Module (ex. _TP-4056TYPEC_) ![3](https://img.shields.io/badge/3-orange)

- Rechargeable ‌Battery ![4](https://img.shields.io/badge/4-orange)

> If You Don't want to use Battery and Charge Module Just Remove them *( 3 , 4 )* and Plug The Arduino Board to 5V **BUT** Remember You Don't Have Backup Pqower

- BreadBoard & Jumpers Wires *(Optional)*

___

### Hardware Connections

  

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

### Program Board 

## MIT License

Copyright (c) [2025] [Alamdevs]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.