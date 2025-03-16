# Neureset: LENS Neurofeedback System Simulation 

A Qt-based simulation of a consumer EEG direct neurofeedback device inspired by the LENS Neurofeedback system. This project models the core functionality of a standalone neurofeedback therapy device, including session management, EEG signal processing, and user interface.
## Key Features
- **Real-time EEG Simulation**: Models 21-electrode EEG signal processing
- **Session Management**:
  - Automatic baseline calculation
  - 1-second treatment pulses with 5Hz offset
  - Session logging with before/after baselines
- **Qt-based UI**:
  - Session timer with progress bar
  - Connection status indicators (blue/green/red lights)
  - Session history log
- **Database Integration**: SQLite backend for persistent session storage
- **Developer Tools**: 
  - Battery simulation
  - Headset connection toggling
  - PC UI for detailed session analysis

## Installation
### Prerequisites
- Qt 5.15+ (Core, Widgets, SQL modules)
- C++17 compatible compiler
- SQLite 3.x
