# 🌐 Smart Home & Street Light Automation using Raspberry Pi (Online & Offline)

![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-IoT-red)
![Asterisk](https://img.shields.io/badge/Asterisk-IVR%20Call%20Control-orange)
![AI Assistant](https://img.shields.io/badge/AI-Voice%20Assistant-blue)
![Status](https://img.shields.io/badge/Status-Working-success)

---

## 🚀 Project Overview

This project implements a **Smart Home and Street Light Automation System** using **Raspberry Pi** that allows users to control **home electrical appliances and street lights** using **multiple control methods**, working **with and without internet connectivity**.

Unlike traditional smart automation systems that stop working without internet or smartphones, this system ensures **continuous accessibility** using **call-based control**, making it suitable for **rural areas, emergencies, and basic keypad phone users**.

---

## ❗ Problem Statement

Most existing smart home and street light automation systems are **dependent on internet connectivity and smartphones**.  
During **network failures**, or for users who do not have smartphones, these systems become **unusable**.

There is no reliable system that allows:
- Appliance control using **phone calls**
- Automation **without internet**
- A single platform supporting **multiple control modes**

---

## ✅ Proposed Solution

We propose a **multi-mode automation system** that enables users to control **electrical appliances and street lights** using:

- 📞 **Phone Call (IVR using Asterisk)** – works without internet  
- 📱 **Mobile Application** – remote control from anywhere  
- 🤖 **AI Assistant** – voice-based control  
- 🔘 **Manual Physical Switch** – local and reliable  
- 🌙 **LDR Sensor** – automatic street light control  

All control methods are **real-time synchronized** using a **Raspberry Pi as the central controller**.

---

## 🎯 Key Features

- Works **with and without internet**
- Call-based control using **Asterisk IVR**
- AI assistant voice control
- Mobile app remote access
- Manual physical button support
- LDR-based automatic street light operation
- Real-time appliance state synchronization
- Low-cost, scalable, and reliable system

---

## 🎥 Project Demo (Animated View)

<p align="center">
  <img src="https://media.giphy.com/media/3o7btPCcdNniyf0ArS/giphy.gif" width="500"/>
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
