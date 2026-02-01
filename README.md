# 📞 Call-Based Home Automation System  
*(Works With and Without Internet)*

---

## 🏆 Project Overview

This project is a **Call-Based Home Automation System** that allows users to control home electrical appliances such as lights using **normal phone calls**, **mobile applications**, **AI voice assistants**, and **manual switches**.

The most important feature of this system is that it **continues to work even when internet connectivity is unavailable**, using **telecom voice calls and DTMF input**.  
This makes the system reliable, inclusive, and suitable for real-world environments like rural areas and emergency situations.

🏅 **Winner – Google TechSprint Hackathon**

---

## ✨ Key Features

- 📞 Appliance control using normal phone calls (IVR)
- 🌐 Works with and without internet
- 🔢 DTMF keypad-based control (Press 1 / Press 2)
- 🔌 Direct GPIO-based control using Raspberry Pi
- 🤖 Optional AI Assistant & mobile app support
- 🔘 Manual physical switch support
- 💡 Low-cost and scalable architecture

---

## 🧠 System Architecture

```
User Phone (Keypad / Smartphone)
         |
         | Voice Call
         |
Telecom Network (GSM / SIP)
         |
Asterisk Server (Raspberry Pi)
         |
Python Script (DTMF Trigger)
         |
   GPIO (Relay Module)
         |
  Home Appliance (Light)
```

---

## 🛠 Hardware Requirements

- Raspberry Pi (3 / 4 / Zero 2 W)
- Relay Module (5V)
- Electrical Appliance (Light)
- Jumper Wires
- Power Supply
- GSM Dongle or SIP Trunk (for call connectivity)

---

## 💻 Software Requirements

- Raspberry Pi OS (Linux)
- Asterisk PBX
- Python 3
- RPi.GPIO Library

---

## ⚙️ Asterisk Installation (Step-by-Step)

### Step 1: Update System
```bash
sudo apt update && sudo apt upgrade -y
```

### Step 2: Install Asterisk
```bash
sudo apt install asterisk -y
```

### Step 3: Verify Installation
```bash
asterisk -rvvv
```

**Expected output:**
```
Asterisk Ready
```

**Exit console:**
```bash
exit
```

---

## 📁 Important Asterisk Configuration Paths

```
/etc/asterisk/extensions.conf   -> Dial plan
/etc/asterisk/sip.conf          -> SIP configuration
/var/lib/asterisk/sounds/       -> Audio prompts
```

---

## ☎️ IVR Dial Plan Configuration

**Open dial plan file:**
```bash
sudo nano /etc/asterisk/extensions.conf
```

**Add the following code:**
```ini
[ivr-menu]
exten => s,1,Answer()
 same => n,Playback(welcome)
 same => n,Playback(press-1-for-on)
 same => n,Playback(press-2-for-off)
 same => n,WaitExten(10)

exten => 1,1,System(python3 /home/pi/light_on.py)
 same => n,Playback(light-turned-on)
 same => n,Hangup()

exten => 2,1,System(python3 /home/pi/light_off.py)
 same => n,Playback(light-turned-off)
 same => n,Hangup()
```

**Reload dial plan:**
```bash
sudo asterisk -rx "dialplan reload"
```

---

## 🐍 Python Code for Light Control

### File: `/home/pi/light_on.py`
```python
import RPi.GPIO as GPIO
import time

RELAY_PIN = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(RELAY_PIN, GPIO.OUT)

GPIO.output(RELAY_PIN, GPIO.LOW)  # Relay ON
time.sleep(1)

GPIO.cleanup()
```

### File: `/home/pi/light_off.py`
```python
import RPi.GPIO as GPIO
import time

RELAY_PIN = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(RELAY_PIN, GPIO.OUT)

GPIO.output(RELAY_PIN, GPIO.HIGH)  # Relay OFF
time.sleep(1)

GPIO.cleanup()
```

**Make scripts executable:**
```bash
chmod +x /home/pi/light_on.py
chmod +x /home/pi/light_off.py
```

---

## 🔌 GPIO Wiring

| Raspberry Pi GPIO | Relay Module |
|-------------------|--------------|
| GPIO17 (Pin 11)   | IN           |
| 5V                | VCC          |
| GND               | GND          |

---

## 📞 How Call-Based Control Works

1. **User calls the system number**
2. **Asterisk answers the call**
3. **IVR plays instructions**
4. **User presses:**
   - `1` → Light ON
   - `2` → Light OFF
5. **Python script executes**
6. **Relay toggles appliance state**
7. ✅ **Internet is not required**

---

## 🧪 Testing

1. Call the configured number
2. Press `1` → Light turns ON
3. Press `2` → Light turns OFF
4. Works with keypad phones and smartphones

---

## 🎯 Target Audience

- Homes in low-connectivity areas
- Elderly and non-smartphone users
- Rural and semi-urban households
- Emergency backup automation use cases

---

## 🏆 Recognition

🏅 **Winner – Google TechSprint Hackathon**  
Recognized for building an inclusive and internet-independent home automation solution.

---

## 🔮 Future Enhancements

- Voice feedback using text-to-speech
- SMS-based appliance status updates
- Multi-appliance control
- Secure authentication
- Mobile dashboard integration

---

## 📌 Conclusion

This project demonstrates that smart home automation does not require continuous internet access.  
By combining call-based control using Asterisk and Python, the system ensures reliability, accessibility, and real-world usability.

**Built with ❤️ for inclusive and reliable technology**
- LDR-based automatic street light operation
- Real-time appliance state synchronization
- Low-cost, scalable, and reliable system

---

## 🎥 Project Demo (Animated View)

<p align="center">
  <img src="/image.png" width="500"/>
</p>

---

## 🏗️ System Architecture

- User
├── Phone Call (IVR)
├── Mobile App
├── AI Assistant
└── Manual Switch
↓
Raspberry Pi (Controller)
↓
Relay Module
↓
Electrical Appliances / Street Light


---

## 🔧 Hardware Components

- Raspberry Pi (3 / 4)
- Relay Module
- LDR Sensor
- Push Button (Manual Switch)
- Jumper Wires
- Power Supply

---

## 🛠️ Software & Technologies Used

- Raspberry Pi OS (Linux)
- Python (GPIO Control)
- Asterisk (IVR Call Control)
- AI Voice Assistant (Google / Alexa)
- Mobile App / Web Interface
- GPIO Libraries

---

## 📞 IVR Call Control (Asterisk)

When a user calls the system:

- Press 1 → Turn ON Light
- Press 2 → Turn OFF Light


✔ Works without internet  
✔ Supports keypad phones  
✔ Ideal for emergency situations  

---

## 🌍 Use Cases

- Smart Home Automation
- Smart Street Lighting
- Rural & Low-Network Areas
- Elderly-friendly automation
- Hackathons & IoT Projects

---

## 🌱 Future Enhancements

- Power consumption monitoring
- Multi-room & zone-based control
- Web dashboard with analytics
- AI-based automation rules
- Smart city integration

---

## 🏆 Hackathon Highlight

> *A smart automation system that works anytime and anywhere — even without internet.*

---

## 🔗 Project Links

- **GitHub Repository:** _(Add link)_  
- **Demo Video:** _(Add YouTube link)_  
- **MVP Link:** _(Add live link if available)_  

---

## 👨‍💻 Author

**Rohit Kumar**  
Smart Automation & IoT Enthusiast  

---

⭐ If you like this project, don’t forget to star the repository!
