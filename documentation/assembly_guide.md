# Saturn Rotating Gift - Assembly Guide

## Komponen (contoh)
- Arduino Uno / Nano
- Motor DC 3-6V (atau continuous rotation servo)
- Motor driver L298N / TB6612 atau MOSFET + transistor sesuai kebutuhan
- WS2812B LED ring (8-24 LED)
- Power supply 5V (cukup kuat untuk LED + motor)
- Kabel jumper, breadboard / pcb
- Model Saturnus 3D-print (ring terpisah agar bisa berputar)

## Wiring (contoh)
- Motor driver IN1/IN2 ke pin 4 dan 7 (direction), EN ke pin 5 (PWM)
- NeoPixel data ke pin 6
- NeoPixel +5V and GND ke power supply (bukan langsung dari Arduino jika banyak LED)
- Common ground antara power supply dan Arduino
- Serial USB antara Arduino dan komputer (untuk backend)

Skematik sederhana:
- Motor + -> Motor driver out A
- Motor - -> Motor driver out B
- Motor driver VCC -> 5V supply
- Motor driver GND -> GND
- NeoPixel VCC -> 5V supply
- NeoPixel GND -> GND
- NeoPixel DIN -> Arduino pin 6

## Setup
1. Upload firmware `/firmware/saturn_motor_controller.ino` ke Arduino
2. Jalankan backend (opsional): `cd web-app && npm install && node server.js`
3. Buka `http://localhost:3000` lalu hubungkan ke port serial yang sesuai (di server config)

## Safety
- Jangan menyambungkan NeoPixel langsung ke 5V dari Arduino jika LED banyak, gunakan supply terpisah.
- Pastikan power supply memiliki arus cukup untuk motor + LED.
