# Jinx LED Engine

![Build](https://img.shields.io/badge/build-passing-8A2BE2?style=for-the-badge)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-9340ff?style=for-the-badge&logo=platformio)
![C++](https://img.shields.io/badge/C%2B%2B-Embedded-purple?style=for-the-badge&logo=c%2B%2B)
![NeoPixel](https://img.shields.io/badge/GRBW-SK6812-DA70D6?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-9370DB?style=for-the-badge)

---

## ✨ Animated Showcase  

<p align="center">
  <img src="assets/jinx.gif" alt="Jinx LED Engine Demo" style="border-radius:12px;">
</p>

---

## ⚙️ Engineering Summary

The **Jinx LED Engine** is a custom embedded lighting controller designed for a Jinx statue display.  
It runs on an **Arduino Nano Every** and drives **14× GRBW SK6812 NeoPixels** using two independent real-time animation engines:

- **Cloud Lightning Engine (LEDs 0–11)**  
  Randomized micro-timed lightning flashes running under diffused acrylic "clouds."

- **Core Breathing Engine (LEDs 12–13)**  
  A cinematic breathing and color-mode core for the statue’s central energy node.

The system uses fully non-blocking scheduling (`millis()`), enabling deterministic timing and multi-layered physical lighting depth.

---

## 🌩️ Cloud Lightning Engine (LEDs 0–11)

- Independent micro-timed LED engines  
- ~70% ambient glow baseline  
- Random lightning bursts  
- Intensity & interval variation per LED  
- Designed for translucent cloud diffusion  
- **Goal:** dynamic "storm under glass" motion

---

## 💗 Core Breathing Engine (LEDs 12–13)

- Fade-in → hold → fade-out breathing cycle  
- Color-mode states: Pink / Amethyst / White / Gem-tones  
- Half or full brightness scaling per cycle  
- Mode changes only at blackout for seamless transitions  
- **Goal:** rhythmic “energy core” pulse

---

## 🧱 System Architecture

### 1. Cloud Lightning Engine
- Lightning probability per LED  
- Brightness shaping  
- Randomized timing windows  

### 2. Core Breathing Engine
- Mode selection logic  
- Per-cycle brightness multiplier  
- Controlled color palette  

### Hardware Stack
- Arduino Nano Every  
- 14× GRBW SK6812 LEDs  
- 5 V regulated rail  

### Software Stack
- PlatformIO  
- Adafruit NeoPixel  
- VS Code  
- Git (SSH)

---

## 📸 Gallery

<table>
<tr>

<td style="vertical-align: top; padding-right: 20px;">

<b>Prototype Sparkle Engine</b><br>
Early breadboard validation of timing, colour curves, and GRBW output.
<br><br>
<img src="assets/IMG_0795.jpg" width="320"
     style="box-shadow: 0 2px 8px rgba(0,0,0,0.25); border-radius: 4px;" />

</td>

<td style="vertical-align: top; padding-right: 20px;">

<b>Internal Lighting Layout</b><br>
Cloud-zone LEDs (0–11) mounted under the acrylic diffuser layer.
<br><br>
<img src="assets/IMG_0799.jpg" width="320"
     style="box-shadow: 0 2px 8px rgba(0,0,0,0.25); border-radius: 4px;" />

</td>

</tr>
<tr>

<td style="vertical-align: top; padding-right: 20px;">

<b>Nano Every Driver Board</b><br>
The embedded control stack running both animation engines with non-blocking scheduling.
<br><br>
<img src="assets/IMG_0801.jpg" width="320"
     style="box-shadow: 0 2px 8px rgba(0,0,0,0.25); border-radius: 4px;" />

</td>

<td style="vertical-align: top; padding-right: 20px;">

<b>Cloud Lightning Effect</b><br>
Randomized micro-bursts firing through the translucent resin cloud structure.
<br><br>
<img src="assets/IMG_0802.jpg" width="320"
     style="box-shadow: 0 2px 8px rgba(0,0,0,0.25); border-radius: 4px;" />

</td>

</tr>
<tr>

<td style="vertical-align: top; padding-right: 20px;">

<b>Final Assembly</b><br>
Fully integrated lighting system inside the statue base with balanced diffusion.
<br><br>
<img src="assets/IMG_0808.jpg" width="320"
     style="box-shadow: 0 2px 8px rgba(0,0,0,0.25); border-radius: 4px;" />

</td>

</tr>
</table>
---
