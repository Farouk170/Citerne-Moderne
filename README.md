# Water Quality Control and Monitoring System

This project manages and monitors the quality of water in a storage tank, utilizing a microcontroller, sensors, and various actuators. It includes a day and night mode for customized control, and stores water quality thresholds and data in EEPROM.

## Project Overview

The system has two operational modes (Day and Night) and a Rest state. It continuously monitors water quality and controls actions such as filtration and refilling, displaying real-time data and messages on an LCD.

### System States

1. **Rest State**:
   - All actuators (buzzer, motors, LEDs) are inactive.
   - The LCD displays: `Bienvenue Eau= xx%`, where `xx%` shows the current water cleanliness level.

2. **Day Mode**:
   - Activated by pressing the `Mode_jour` button.
   - The LCD shows: `Mode Jour` on the first line.
   - Functionality includes:
     - **Low Water Level Alert**: Pressing `Niveau_B` activates the buzzer for 1 second and blinks the red LED 3 times every 2 seconds, displaying `Remplir citerne` on the LCD until manual intervention.
     - **Start Filling**: Pressing the `valider` button turns on a blue LED and starts the electrovalve for filling.
     - **Stop Filling**: Pressing `Niveau_A` turns off the blue LED and stops the electrovalve.
     - **Water Quality Check**: 
       - Water quality is stored in EEPROM with each change.
       - If water quality exceeds 75, the orange LED blinks 5 times, filtration starts for 10 seconds, and the LCD displays the quality and `l'eau est sale`.
       - If quality is below 75, the orange LED turns off, filtration stops, and the LCD displays the quality and `l'eau est propre`.
     - **Waste Prevention**: If `Niveau_A` is pressed 10 times, the LCD shows `Gaspillage`, and the buzzer sounds until the `RAZ` button is pressed to reset the counter.

3. **Night Mode**:
   - Activated by pressing the `mode_nuit` button.
   - The LCD displays: `Mode nuit` on the first line.
   - Functionality includes:
     - **Silent Filling**: Pressing `Niveau_C` turns on the blue LED and activates the electrovalve for silent filling.
     - **Stop Filling**: Pressing `Niveau_A` turns off the blue LED and stops the electrovalve.
     - **Water Quality Check**:
       - Water quality data is stored in EEPROM with each update.
       - If water quality exceeds 75, the orange LED blinks 5 times, filtration runs for 10 seconds, and the LCD shows the quality with `l'eau est sale`.
       - If quality is below 75, the orange LED turns off, filtration stops, and the LCD shows the quality with `l'eau est propre`.
     - **Waste Prevention**: Pressing `Niveau_C` 5 times displays `Gaspillage` on the LCD, activating the buzzer until the `RAZ` button resets the counter.

## Components

- **Microcontroller**: PIC with EEPROM storage for quality thresholds
- **Actuators**:
  - Buzzer for alerts
  - Motor for water management
  - Electrovalve for water flow control
- **LEDs**:
  - Red: Low water alert
  - Blue: Active filling mode
  - Orange: Filtration status indicator
- **Buttons**:
  - `Mode_jour`: Start Day Mode
  - `mode_nuit`: Start Night Mode
  - `Niveau_A`, `Niveau_B`, `Niveau_C`: Control water levels and quality monitoring
  - `valider`: Start filling
  - `RAZ`: Reset counters
- **LCD Display**: Shows system messages and water quality status

## Project Structure

1. **Design**:
   - Define system inputs and outputs
   - Develop operational flowcharts for each mode

2. **Circuit Assembly**:
   - Build the circuit on Proteus ISIS, mapping components to appropriate microcontroller pins

3. **Coding**:
   - Implement functionality in C for MikroC for PIC
   - Integrate all features for complete system functionality

## Getting Started

### Prerequisites

- **Software**:
  - MikroC for PIC (for C coding)
  - Proteus ISIS (for circuit simulation)

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/Farouk170/Citerne-Moderne
