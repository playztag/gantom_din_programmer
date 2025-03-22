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

## Building and Flashing

This project uses PlatformIO for building and flashing:

```bash
# Build the project
platformio run

# Upload to the device
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
