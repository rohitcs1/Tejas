# Call-Based Home Automation System

## Works With and Without Internet

---

## 1. Project Overview

This project presents a Raspberry Pi-based home automation system designed to control electrical appliances using multiple input methods, including telephone calls, mobile applications, AI voice assistants, and physical switches. The system's distinguishing feature is its ability to operate independently of internet connectivity by leveraging traditional telecommunication infrastructure and DTMF (Dual-Tone Multi-Frequency) signaling.

The architecture is built around Asterisk PBX running on a Raspberry Pi, which processes incoming voice calls and executes Python scripts to toggle GPIO pins connected to relay modules. This approach ensures that users with basic keypad phones can control home appliances without requiring smartphones or internet access, making the solution accessible to elderly users, rural populations, and environments with unreliable connectivity.

The system is engineered for reliability, inclusivity, and real-world deployment in scenarios where internet-dependent solutions would fail.

---

## 2. Key Features

- Appliance control via standard telephone calls (IVR system)
- Functions with or without internet connectivity
- DTMF-based input processing (keypad press 1 for ON, press 2 for OFF)
- Direct GPIO control using Raspberry Pi hardware
- Support for AI assistants and mobile applications (optional)
- Manual physical switch integration
- Low-cost, scalable architecture suitable for residential and institutional deployment

---

## 3. System Architecture

The system follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────┐
│   User Phone (Keypad/Smartphone)│
└────────────┬────────────────────┘
             │
             │ Voice Call (GSM/PSTN/SIP)
             │
             ▼
┌─────────────────────────────────┐
│  Telecom Network Infrastructure │
└────────────┬────────────────────┘
             │
             │ Incoming Call
             │
             ▼
┌─────────────────────────────────┐
│    Asterisk PBX Server          │
│    (Running on Raspberry Pi)    │
└────────────┬────────────────────┘
             │
             │ DTMF Detection
             │ Extensions Processing
             │
             ▼
┌─────────────────────────────────┐
│    Python Control Scripts       │
│    (light_on.py / light_off.py) │
└────────────┬────────────────────┘
             │
             │ RPi.GPIO Library
             │
             ▼
┌─────────────────────────────────┐
│    GPIO Pin (BCM 17)            │
└────────────┬────────────────────┘
             │
             │ Digital Signal (HIGH/LOW)
             │
             ▼
┌─────────────────────────────────┐
│    Relay Module (5V)            │
└────────────┬────────────────────┘
             │
             │ AC Circuit Switching
             │
             ▼
┌─────────────────────────────────┐
│    Home Appliance (Light/Fan)   │
└─────────────────────────────────┘
```

**Data Flow:**
1. User initiates a voice call to the configured system phone number
2. Telecom network routes the call to the Asterisk server
3. Asterisk answers and plays IVR prompts
4. User inputs DTMF tones via keypad (1 or 2)
5. Asterisk dial plan executes corresponding Python script
6. Python script toggles GPIO pin state
7. Relay module switches appliance power
8. System provides audio confirmation
9. Call terminates

---

## 4. Hardware Requirements

| Component | Specification | Purpose |
|-----------|--------------|---------|
| Raspberry Pi | Model 3B+, 4, or Zero 2 W | Main controller and Asterisk server host |
| Relay Module | 5V single/multi-channel | Electrical switching for appliances |
| Power Supply | 5V 3A (for Pi 4) or 5V 2.5A (for Pi 3) | Power for Raspberry Pi |
| Electrical Appliance | AC bulb/lamp (example) | Device to be controlled |
| Jumper Wires | Male-to-female GPIO connectors | GPIO to relay connections |
| GSM Dongle (optional) | USB GSM modem with SIM slot | For direct GSM connectivity without SIP |
| SIP Trunk Account (alternative) | VoIP service provider account | For internet-based call routing |

**Note:** Either a GSM dongle with SIM card or a SIP trunk account is required for receiving incoming calls.

---

## 5. Software Requirements

| Software | Version | Purpose |
|----------|---------|---------|
| Raspberry Pi OS | Bullseye or later (Debian-based) | Operating system |
| Asterisk | 16.x or 18.x (available via apt) | PBX and IVR engine |
| Python | 3.7 or later | GPIO control scripting |
| RPi.GPIO | Latest (via pip) | Python library for GPIO access |

---

## 6. Asterisk Installation on Raspberry Pi

Execute the following commands on the Raspberry Pi terminal with sudo privileges.

### Step 1: Update system packages

```bash
sudo apt update && sudo apt upgrade -y
```

### Step 2: Install Asterisk PBX

```bash
sudo apt install asterisk -y
```

The installation process will automatically:
- Install Asterisk binaries
- Create configuration directories
- Set up system service for automatic startup
- Install default configuration files

### Step 3: Verify Asterisk installation

```bash
asterisk -rvvv
```

Expected output:
```
Asterisk 18.x.x, Copyright (C) 1999 - 2021 Sangoma Technologies Corporation and others.
...
Connected to Asterisk 18.x.x currently running on raspberrypi (pid = XXXX)
raspberrypi*CLI>
```

To exit the Asterisk CLI:
```
exit
```

### Step 4: Enable Asterisk service

```bash
sudo systemctl enable asterisk
sudo systemctl start asterisk
```

### Step 5: Verify service status

```bash
sudo systemctl status asterisk
```

Expected output should show `active (running)`.

---

## 7. Important Asterisk Configuration Paths

| File Path | Purpose |
|-----------|---------|
| `/etc/asterisk/extensions.conf` | Dial plan configuration (IVR logic) |
| `/etc/asterisk/sip.conf` | SIP peer configuration (for VoIP trunks) |
| `/etc/asterisk/pjsip.conf` | Modern PJSIP configuration (alternative to sip.conf) |
| `/etc/asterisk/manager.conf` | Asterisk Manager Interface (AMI) settings |
| `/var/lib/asterisk/sounds/` | Audio prompt files (WAV/GSM format) |
| `/var/log/asterisk/` | Log files (messages, cdr, queue_log) |
| `/var/spool/asterisk/` | Voicemail and temporary files |

**Configuration reload commands:**

```bash
# Reload dial plan without restarting Asterisk
sudo asterisk -rx "dialplan reload"

# Reload SIP configuration
sudo asterisk -rx "sip reload"

# Restart Asterisk completely
sudo systemctl restart asterisk
```

---

## 8. IVR Dial Plan Configuration

The dial plan defines how incoming calls are processed. Edit the extensions configuration file:

```bash
sudo nano /etc/asterisk/extensions.conf
```

Add the following context to the file (or replace existing `[default]` context):

```ini
[ivr-menu]
; Answer incoming call
exten => s,1,Answer()
    same => n,Wait(1)
    
; Play welcome message
    same => n,Playback(welcome)
    
; Provide user instructions
    same => n,Playback(press-1)
    same => n,Playback(for-light-on)
    same => n,Playback(press-2)
    same => n,Playback(for-light-off)
    
; Wait for DTMF input (10 second timeout)
    same => n,WaitExten(10)
    
; Handle timeout
    same => n,Playback(goodbye)
    same => n,Hangup()

; Extension 1: Turn light ON
exten => 1,1,NoOp(User pressed 1 - Turning light ON)
    same => n,System(python3 /home/pi/automation/light_on.py)
    same => n,Playback(light-turned-on)
    same => n,Wait(1)
    same => n,Hangup()

; Extension 2: Turn light OFF
exten => 2,1,NoOp(User pressed 2 - Turning light OFF)
    same => n,System(python3 /home/pi/automation/light_off.py)
    same => n,Playback(light-turned-off)
    same => n,Wait(1)
    same => n,Hangup()

; Invalid extension handler
exten => i,1,Playback(invalid)
    same => n,Goto(s,1)

; Timeout handler
exten => t,1,Playback(timeout)
    same => n,Hangup()
```

**Configure incoming call routing in the `[from-external]` or appropriate context:**

```ini
[from-external]
exten => YOUR_PHONE_NUMBER,1,Goto(ivr-menu,s,1)
```

Replace `YOUR_PHONE_NUMBER` with the actual number receiving calls.

**Save and reload configuration:**

```bash
# Save file: Ctrl+O, Enter, Ctrl+X
sudo asterisk -rx "dialplan reload"
```

**Verify dial plan:**

```bash
sudo asterisk -rx "dialplan show ivr-menu"
```

---

## 9. How DTMF-Based Call Control Works

### DTMF Signal Processing Flow

1. **Call Initiation:** User dials the system's phone number using any telephone (landline or mobile, keypad or smartphone).

2. **Call Routing:** The telecom network (GSM, PSTN, or SIP) routes the call to the Asterisk server running on the Raspberry Pi.

3. **Call Answer:** Asterisk executes the `Answer()` application, establishing the voice channel.

4. **IVR Prompt Playback:** Pre-recorded audio files guide the user:
   - "Welcome to home automation system"
   - "Press 1 to turn light on"
   - "Press 2 to turn light off"

5. **DTMF Tone Detection:** When the user presses a key on their phone keypad, the phone generates dual-tone multi-frequency signals:
   - Each digit produces two simultaneous tones (e.g., "1" = 697Hz + 1209Hz)
   - Asterisk's DSP (Digital Signal Processing) engine detects these tones in real-time

6. **Extension Matching:** The detected digit is matched against dial plan extensions:
   - Digit "1" → Extension 1 → Execute `light_on.py`
   - Digit "2" → Extension 2 → Execute `light_off.py`

7. **Script Execution:** Asterisk uses the `System()` application to spawn a shell process that runs the Python script with root privileges (required for GPIO access).

8. **GPIO Control:** The Python script uses RPi.GPIO library to set the state of GPIO pin 17:
   - `GPIO.LOW` (0V) → Relay energized → Light ON
   - `GPIO.HIGH` (3.3V) → Relay de-energized → Light OFF

9. **Confirmation Feedback:** Asterisk plays an audio confirmation message to the user.

10. **Call Termination:** The `Hangup()` application closes the call channel.

### Technical Advantages

- **No Internet Required:** DTMF signaling operates at the telephony layer, independent of IP connectivity.
- **Universal Compatibility:** Works with all phone types, including ancient rotary dial phones (though they require pulse dialing conversion).
- **Low Latency:** Direct GPIO control provides near-instantaneous response (typically under 1 second from keypress to appliance state change).
- **Reliable:** Not affected by network congestion, WiFi issues, or cloud service outages.

---

## 10. Python GPIO Control Scripts

### Directory Structure

Create a dedicated directory for automation scripts:

```bash
mkdir -p /home/pi/automation
cd /home/pi/automation
```

### Script 1: Light ON Control

**File:** `/home/pi/automation/light_on.py`

```python
#!/usr/bin/env python3
"""
Light ON Control Script
Energizes relay module to turn on connected appliance.
"""

import RPi.GPIO as GPIO
import time
import sys

# Configuration
RELAY_PIN = 17  # BCM GPIO 17 (Physical Pin 11)
RELAY_ON_STATE = GPIO.LOW  # Relay active LOW (common configuration)

def setup_gpio():
    """Initialize GPIO configuration."""
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(RELAY_PIN, GPIO.OUT)

def turn_light_on():
    """Activate relay to turn light on."""
    try:
        setup_gpio()
        GPIO.output(RELAY_PIN, RELAY_ON_STATE)
        time.sleep(0.5)  # Ensure relay state stabilization
        print("Light turned ON successfully", file=sys.stderr)
    except Exception as e:
        print(f"Error turning light on: {e}", file=sys.stderr)
        sys.exit(1)
    finally:
        # Do NOT cleanup GPIO here - maintain state
        pass

if __name__ == "__main__":
    turn_light_on()
```

### Script 2: Light OFF Control

**File:** `/home/pi/automation/light_off.py`

```python
#!/usr/bin/env python3
"""
Light OFF Control Script
De-energizes relay module to turn off connected appliance.
"""

import RPi.GPIO as GPIO
import time
import sys

# Configuration
RELAY_PIN = 17  # BCM GPIO 17 (Physical Pin 11)
RELAY_OFF_STATE = GPIO.HIGH  # Relay inactive HIGH

def setup_gpio():
    """Initialize GPIO configuration."""
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(RELAY_PIN, GPIO.OUT)

def turn_light_off():
    """Deactivate relay to turn light off."""
    try:
        setup_gpio()
        GPIO.output(RELAY_PIN, RELAY_OFF_STATE)
        time.sleep(0.5)  # Ensure relay state stabilization
        print("Light turned OFF successfully", file=sys.stderr)
    except Exception as e:
        print(f"Error turning light off: {e}", file=sys.stderr)
        sys.exit(1)
    finally:
        # Do NOT cleanup GPIO here - maintain state
        pass

if __name__ == "__main__":
    turn_light_off()
```

### Make Scripts Executable

```bash
chmod +x /home/pi/automation/light_on.py
chmod +x /home/pi/automation/light_off.py
```

### Install Required Python Library

```bash
pip3 install RPi.GPIO
```

### Test Scripts Manually

```bash
# Test light ON
sudo python3 /home/pi/automation/light_on.py

# Test light OFF
sudo python3 /home/pi/automation/light_off.py
```

**Important Notes:**

- Scripts must run with `sudo` or as root because GPIO access requires elevated privileges.
- `GPIO.cleanup()` is intentionally omitted to maintain relay state after script execution.
- The scripts use BCM GPIO numbering (not physical pin numbers).

---

## 11. File Paths Summary

| File | Path | Purpose |
|------|------|---------|
| Light ON Script | `/home/pi/automation/light_on.py` | Activates relay (GPIO LOW) |
| Light OFF Script | `/home/pi/automation/light_off.py` | Deactivates relay (GPIO HIGH) |
| Asterisk Dial Plan | `/etc/asterisk/extensions.conf` | IVR logic and call routing |
| Asterisk Logs | `/var/log/asterisk/messages` | Debug and error logs |
| Audio Prompts | `/var/lib/asterisk/sounds/` | IVR voice files |

---

## 12. GPIO Wiring

### Pin Connection Table

| Raspberry Pi GPIO | Physical Pin | Relay Module Pin | Function |
|-------------------|--------------|------------------|----------|
| GPIO 17 (BCM)     | Pin 11       | IN / Signal      | Control signal |
| 5V Power          | Pin 2 or 4   | VCC              | Relay power supply |
| Ground            | Pin 6, 9, 14, 20, or 30 | GND | Common ground |

### Relay to Appliance Wiring

| Relay Terminal | Connection |
|----------------|------------|
| COM (Common)   | AC Live wire from wall outlet |
| NO (Normally Open) | AC Live wire to appliance |
| NC (Normally Closed) | Not used (or for inverted logic) |

**Wiring Diagram:**

```
Raspberry Pi                    Relay Module                 Appliance
┌──────────┐                   ┌──────────┐                ┌──────────┐
│          │                   │          │                │          │
│ GPIO 17 ─┼──────────────────▶│ IN       │                │          │
│  (Pin 11)│                   │          │                │          │
│          │                   │          │    AC Live     │          │
│ 5V ──────┼──────────────────▶│ VCC      │    ┌──────────│          │
│  (Pin 2) │                   │          │    │          │  Light   │
│          │                   │ COM ─────┼────┘          │          │
│ GND ─────┼──────────────────▶│ GND      │               │          │
│  (Pin 6) │                   │          │    AC Live    │          │
│          │                   │ NO ──────┼──────────────▶│          │
│          │                   │          │               │          │
│          │                   │ NC       │ (not used)    │          │
└──────────┘                   └──────────┘                └──────────┘
                                                            
                                                           AC Neutral
                                                           (direct)
```

**Safety Warning:**

- Always disconnect AC power before wiring.
- Verify relay voltage rating matches appliance voltage (e.g., 230V AC for India/Europe, 110V AC for USA).
- Use appropriate wire gauge for current load.
- Ensure proper insulation of all AC connections.
- Test with a low-power device (LED bulb) before connecting high-power appliances.

---

## 13. How the Call-Based System Works

### Step-by-Step Operational Flow

1. **User Initiates Call**
   - User dials the designated phone number from any telephone device.
   - Call can be made from a GSM mobile phone, landline, or VoIP client.

2. **Call Reaches Asterisk Server**
   - GSM dongle receives the call (if using direct GSM connection), or
   - SIP trunk routes the call to Asterisk (if using VoIP service).

3. **Call is Answered**
   - Asterisk executes the dial plan context `[ivr-menu]`.
   - The `Answer()` command picks up the call.

4. **IVR Welcome Message**
   - Pre-recorded audio file plays: "Welcome to home automation system."
   - Instruction prompts follow: "Press 1 to turn light on, press 2 to turn light off."

5. **User Input**
   - User presses either "1" or "2" on their phone keypad.
   - Phone converts keypress to DTMF tone (dual-frequency audio signal).

6. **DTMF Detection**
   - Asterisk's DSP core detects the DTMF tone.
   - Matches the tone to the corresponding dial plan extension.

7. **Script Execution**
   - If "1" was pressed: `System(python3 /home/pi/automation/light_on.py)` is executed.
   - If "2" was pressed: `System(python3 /home/pi/automation/light_off.py)` is executed.

8. **GPIO State Change**
   - Python script uses RPi.GPIO to set GPIO 17 to LOW (ON) or HIGH (OFF).
   - GPIO state change occurs within milliseconds.

9. **Relay Activation**
   - GPIO signal energizes or de-energizes the relay coil.
   - Relay contacts switch, closing or opening the AC circuit.

10. **Appliance State Change**
    - If relay is energized (GPIO LOW): AC circuit closes, light turns ON.
    - If relay is de-energized (GPIO HIGH): AC circuit opens, light turns OFF.

11. **Confirmation Message**
    - Asterisk plays confirmation: "Light turned on" or "Light turned off."

12. **Call Termination**
    - System executes `Hangup()` after 1-second delay.
    - Call ends, resources are released.

### Timing Analysis

| Step | Approximate Duration |
|------|---------------------|
| Call establishment | 2-5 seconds |
| IVR prompt playback | 5-8 seconds |
| User keypress detection | Instantaneous |
| Python script execution | 100-300 milliseconds |
| GPIO state change | <10 milliseconds |
| Relay physical switching | 5-10 milliseconds |
| Confirmation playback | 2-3 seconds |
| **Total (from dial to appliance control)** | **9-16 seconds** |

---

## 14. Testing Instructions

### Pre-Testing Checklist

- [ ] Raspberry Pi is powered on and connected to network (if using SIP) or GSM dongle is installed
- [ ] Asterisk service is running: `sudo systemctl status asterisk`
- [ ] Python scripts are executable and tested manually
- [ ] GPIO wiring is correct and relay module is powered
- [ ] Test appliance (LED bulb recommended for safety) is connected

### Test Procedure

#### Test 1: Manual Script Execution

```bash
# Test GPIO control without Asterisk
sudo python3 /home/pi/automation/light_on.py
# Observe: Relay clicks, light turns on

sudo python3 /home/pi/automation/light_off.py
# Observe: Relay clicks, light turns off
```

#### Test 2: Asterisk Dial Plan Verification

```bash
# Verify dial plan syntax
sudo asterisk -rx "dialplan show ivr-menu"

# Expected output should show extensions s, 1, and 2 with correct applications
```

#### Test 3: DTMF Tone Simulation

```bash
# Connect to Asterisk console
sudo asterisk -rvvv

# Simulate DTMF input (requires a test call or local channel)
# Alternatively, monitor logs during actual test call
```

#### Test 4: Live Call Testing

1. From a separate phone, dial the configured system number.
2. Listen for IVR welcome message.
3. Press "1" on keypad.
4. Observe: Light should turn ON, confirmation message plays.
5. Call again.
6. Press "2" on keypad.
7. Observe: Light should turn OFF, confirmation message plays.

### Troubleshooting

**Issue:** No answer on incoming call
- Verify Asterisk is running: `sudo systemctl status asterisk`
- Check SIP trunk registration: `sudo asterisk -rx "sip show peers"`
- Review logs: `sudo tail -f /var/log/asterisk/messages`

**Issue:** DTMF tones not detected
- Ensure `dtmfmode=rfc2833` or `dtmfmode=inband` is set in `sip.conf`
- Test with different phones (some VoIP clients have DTMF issues)

**Issue:** Python script not executing
- Verify script permissions: `ls -l /home/pi/automation/`
- Check Asterisk has permission to execute: Run as root or configure sudoers
- Review Python errors in Asterisk log

**Issue:** Relay not switching
- Verify GPIO wiring with multimeter
- Check relay module power supply (5V)
- Test relay manually by connecting IN pin to GND

---

## 15. Target Audience

This system is designed for the following user groups:

### Primary Users

- **Elderly Individuals:** Users who are not comfortable with smartphones or mobile apps can use familiar telephone interfaces.
- **Rural Communities:** Areas with limited or no internet connectivity can still benefit from home automation using basic phone calls.
- **Low-Income Households:** Cost-effective solution using basic keypad phones instead of expensive smart home devices.
- **Visually Impaired Users:** Audio-based IVR interface provides accessibility without requiring screen interaction.

### Secondary Users

- **Industrial Applications:** Remote control of machinery in environments where internet is unavailable or unreliable.
- **Emergency Services:** Backup control systems for critical infrastructure that must function during internet outages.
- **Educational Institutions:** Teaching platform for IoT, telephony, and embedded systems concepts.

### Use Cases

- Home appliance control (lights, fans, heaters)
- Agricultural automation (irrigation pumps, greenhouse ventilation)
- Security systems (gate control, alarm arming/disarming)
- Healthcare (medical equipment control in remote clinics)

---

## 16. Recognition

**Winner – Google TechSprint Hackathon**

This project was recognized as the winning solution at the Google TechSprint Hackathon for addressing digital inclusion challenges. The evaluation criteria focused on:

- **Accessibility:** Enabling technology access for users without smartphones or internet.
- **Real-World Impact:** Solving actual problems faced by underserved communities.
- **Technical Innovation:** Combining traditional telephony with modern embedded systems.
- **Scalability:** Architecture that can be deployed widely at low cost.

The judging panel acknowledged the system's unique approach to home automation that does not rely on cloud services or continuous internet connectivity, making it suitable for deployment in developing regions and as a resilient backup system in developed areas.

---

## 17. Future Enhancements

### Short-Term Improvements

1. **Multi-Appliance Support**
   - Extend dial plan to support multiple devices (Press 3 for fan, Press 4 for heater, etc.)
   - Use multi-channel relay modules

2. **Voice Feedback Synthesis**
   - Integrate text-to-speech engine (e.g., Festival, espeak) for dynamic status announcements
   - Example: "Living room light is currently ON, press 1 to turn OFF"

3. **SMS Status Notifications**
   - Send confirmation SMS after successful appliance control
   - Requires GSM modem with SMS capability or SMS gateway API

4. **Web Dashboard**
   - Develop a simple web interface showing appliance status
   - Use Flask/Django with real-time updates via WebSocket

### Medium-Term Enhancements

5. **User Authentication**
   - Implement caller ID verification to restrict access
   - Use PIN-based authentication for secure control

6. **Scheduling and Timers**
   - Add ability to schedule appliance operations
   - "Press 5 to turn on light for 30 minutes"

7. **Energy Monitoring**
   - Integrate current sensors (e.g., ACS712) to measure power consumption
   - Provide energy usage reports via IVR or web dashboard

8. **Multi-Language Support**
   - Record IVR prompts in multiple regional languages
   - Allow user to select language preference

### Long-Term Vision

9. **Natural Language Processing**
   - Replace DTMF with voice recognition for natural speech commands
   - "Turn on the bedroom light"

10. **Machine Learning Integration**
    - Learn user patterns and suggest automations
    - Predictive maintenance for appliances

11. **Hybrid Cloud-Local Architecture**
    - Use cloud services when internet is available for advanced features
    - Fall back to local call-based control during outages

12. **Commercial Product Development**
    - Design custom PCB integrating Raspberry Pi, relay, and power supply
    - Develop plug-and-play installation kit

---

## 18. Conclusion

This Call-Based Home Automation System demonstrates that advanced home automation can be achieved without dependence on internet connectivity or expensive smart devices. By leveraging mature telecommunication technologies (DTMF signaling, PBX systems) combined with modern embedded computing (Raspberry Pi, GPIO control), the system provides a reliable, accessible, and cost-effective solution for appliance control.

The architecture is particularly valuable in scenarios where internet infrastructure is unreliable or non-existent, and for users who lack access to smartphones or technical expertise. The use of standard telephone calls as the primary interface ensures universal accessibility while maintaining the flexibility to integrate with modern IoT ecosystems when internet connectivity is available.

This project validates the principle that inclusive technology design should not require users to adapt to complex systems, but rather adapt systems to meet users where they are—with the devices and infrastructure already available to them.

---

**Project Repository:** [GitHub Link]  
**License:** MIT License  
**Contact:** [Your Contact Information]

---

**Document Version:** 1.0  
**Last Updated:** February 2026


