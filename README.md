# Gantom DMX Programmer Tool

A firmware tool for the M5Stack DinMeter device that allows you to program and control Gantom DMX fixtures.

## Features

- **DMX Address Programming**: Set DMX addresses for Gantom fixtures (values 1-512)
- **Static Look Configuration**: Configure and save up to 8 DMX channels for static looks
- **Flash Boot Setting**: Toggle the flash on boot setting for fixtures
- **Visual Interface**: Intuitive UI with clear visual indicators and feedback
- **Encoder Control**: Navigate menus and adjust values using the rotary encoder

## Screenshots

![Main Menu](docs/images/main_menu.png)
*Main launcher menu with 4 options*

![DMX Address Menu](docs/images/dmx_address.png)
*DMX address programming interface*

![Static Look Menu](docs/images/static_look.png)
*Static look configuration with visual channel bars*

*Note: Add actual screenshots by creating a docs/images directory and adding your images there.*

## Hardware Requirements

- M5Stack DinMeter or compatible device
- Gantom DMX fixtures

## Software Components

The tool is organized into several key components:

- **Main Launcher**: Entry point for selecting different functions
- **DMX Address Menu**: Set and apply DMX addresses
- **Static Look Menu**: Configure and save channel values for static looks
- **Boot Flash Menu**: Toggle the flash-on-boot feature
- **Boot Splash**: Display startup splash screen

## User Interface

The interface is designed to be intuitive and responsive:

- **Rotary Encoder**: 
  - Turn to navigate menus and adjust values
  - Press to select options
  - Long press to exit menus
  
- **Visual Feedback**:
  - Color-coded menu items
  - Progress indicators and clear status messages
  - Real-time visual representation of channel values in Static Look menu

## Installation and Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) (recommended to use with VSCode)
- Git
- USB connection to your M5Stack DinMeter device

### Installing PlatformIO

1. **Install VSCode**:
   - Download and install [Visual Studio Code](https://code.visualstudio.com/)

2. **Install PlatformIO Extension**:
   - Open VSCode
   - Go to Extensions (Ctrl+Shift+X or Cmd+Shift+X on macOS)
   - Search for "PlatformIO"
   - Click "Install" on the PlatformIO IDE extension

### Getting the Project

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/playztag/gantom_din_programmer.git
   cd gantom_din_programmer
   ```

2. **Open the Project in PlatformIO**:
   - Launch VSCode
   - Click on the PlatformIO icon in the sidebar
   - Select "Open Project"
   - Navigate to the cloned repository folder
   - Click "Open"

### Installing Dependencies

PlatformIO will automatically install the required dependencies defined in the `platformio.ini` file when you first open the project. If you need to manually install or update dependencies:

1. **Update Library Dependencies**:
   - Open the PlatformIO sidebar
   - Click on "Libraries"
   - Search for and install any missing libraries:
     - LovyanGFX
     - SmoothUIToolKit
     - ESP32Encoder
     - Button

## Building and Flashing

### Build the Firmware

1. **Compile the Project**:
   - In the PlatformIO sidebar, click on "Project Tasks"
   - Expand your environment (usually "esp32-s3-devkitc-1")
   - Click on "Build" to compile the firmware

   Or using command line:
   ```bash
   platformio run
   ```

### Flash to Device

1. **Connect your M5Stack DinMeter** to your computer via USB

2. **Upload the Firmware**:
   - In the PlatformIO sidebar, click on "Project Tasks"
   - Click on "Upload" to compile and flash the firmware

   Or using command line:
   ```bash
   platformio run -t upload
   ```

### Troubleshooting Connection Issues

If you have trouble connecting to the device:

1. **Check USB Connection**:
   - Ensure the device is properly connected
   - Try a different USB cable or port

2. **Identify COM Port**:
   - Windows: Check Device Manager > Ports (COM & LPT)
   - macOS: In Terminal, run `ls /dev/cu.*`
   - Linux: In Terminal, run `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`

3. **Enter Boot Mode** (if needed):
   - Hold the Boot button while connecting the device
   - Release after 2-3 seconds

## Updating the Firmware

To update your device with the latest firmware:

1. **Pull Latest Changes**:
   ```bash
   git pull origin main
   ```

2. **Clean the Project** (recommended before major updates):
   ```bash
   platformio run -t clean
   ```

3. **Rebuild and Upload**:
   ```bash
   platformio run -t upload
   ```

## Development

The firmware is built using:

- Arduino framework for ESP32
- LovyanGFX for the display interface
- SmoothUIToolKit for menu animations and interactions

## License

[Specify License]

## Acknowledgments

- Based on M5Stack DinMeter firmware
- Uses the SmoothUIToolKit library for improved UI interactions

## Contributing

Contributions to improve the tool are welcome. Please feel free to submit pull requests or open issues for bugs and feature requests.

## Recent Updates

### UI Improvements (March 2024)
- **Enhanced Static Look Menu**: Added visual bars for real-time channel level visualization
- **Improved Button Handling**: Fixed issues with button debouncing and click detection
- **Consistent Navigation**: Implemented long-press to exit across all menus
- **Color Themes**: Updated color schemes for better readability and visual appeal
- **Input Feedback**: Added audible and visual feedback for user actions
