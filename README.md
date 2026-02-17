# ⏰ DS1307 Time and Date Display on LCD

This project demonstrates how to interface the **DS1307 Real-Time Clock (RTC)** module with a **16x2 LCD display** to show the current time and date.

The system reads real-time data from the DS1307 using I2C communication and displays it on the LCD in a readable format.

---

## 📌 Features

- Displays real-time **Time (HH:MM:SS)**
- Displays **Date (DD/MM/YYYY)**
- Automatic update every second
- Battery backup support (RTC continues during power failure)
- Simple and beginner-friendly project

---

## 🧩 Components Required

- DS1307 RTC Module
- 16x2 LCD Display
- PIC16F877A
- 10k RESISTOR (for LCD contrast)
- Connecting Wires
- Breadboard
- 3V Coin Cell Battery (CR2032)

---

## 🔌 Circuit Connections

### DS1307 to Arduino
| DS1307 | Arduino |
|--------|----------|
| VCC    | 5V       |
| GND    | GND      |
| SDA    | C4       |
| SCL    | C5       |

### LCD to Arduino (4-bit mode example)
| LCD | Arduino |
|-----|----------|
| RS  | B0       |
| EN  | C2       |
| D4  | D5       |
| D5  | D4       |
| D6  | D3       |
| D7  | D2       |

---

## 📂 Libraries Required

Install the following libraries from Arduino Library Manager:

- `RTClib`
- `LiquidCrystal`

---

## 🧠 Working Principle

1. The DS1307 keeps track of time and date.
2. The Arduino reads RTC data using I2C.
3. The data is converted from BCD to decimal.
4. The formatted time and date are displayed on the LCD.
5. The display refreshes every second.

---

## 🖥️ Example Output

```
Time: 10:45:32
Date: 17/02/2026
```

---

## 🚀 Applications

- Digital Clock
- Calendar Display
- Data Logging Systems
- Embedded System Projects
- Automation Systems

---

