# Crystalfontz CFA835-TFK Display Project

**Product Link:** [Crystalfontz CFA835-TFK](https://www.crystalfontz.com/product/cfa835tfk-display-graphic-lcd-244x68)

This project aims to implement functionality for the Crystalfontz CFA835 LCD display, which features a 244 x 68 pixel backlit screen and six buttons (UP, DOWN, LEFT, RIGHT, ENTER, EXIT). The project includes a comprehensive application with a layout system, text rendering, image display, and a progress bar.

## Key Features

- **Screen Resolution:** 244 x 68 pixels
- **User Interaction:** Utilizes six buttons for navigation
- **Text and Image Rendering:** Supports dynamic text and image display
- **Progress Bar Implementation:** Visual feedback for ongoing processes

### Challenges

While the project implements functionality based on the specifications provided by Crystalfontz, there are limitations with font rendering. Fonts created with the Crystalfontz Font Editor can appear broken due to pixel constraints. A recommended approach is to use bitmap files for better rendering. However, rendering new data at 30 frames per second can lead to performance issues, resulting in an empty screen during updates.

Given the time required for optimization and the cost of the device ($135 USD), this project may require careful consideration of resource allocation.

## Prerequisites

Before starting, ensure you have the necessary tools installed:

```bash
sudo apt-get install libboost-all-dev
pip install scons pre-commit Jinja2 Pillow
```

Adjust any paths as necessary.

## Using a Different Font

To use a custom font, follow these steps:

### 1. Download a Font

You can download `.ttf` fonts from websites like [Google Fonts](https://fonts.google.com/) or other font repositories.

### 2. Place the Font File

Copy the downloaded `.ttf` file to the `src/assets/fonts` directory.

### 3. Generate Bitmap Representations of the Font

Due to limitations with the Crystalfontz Font Editor, it is recommended to use bitmap files instead of direct font rendering. To generate bitmap representations, run the following command, which will use the font placed in `src/assets/fonts` and output the results to `src/assets/bitmaps`:

```bash
python generate_dependencies.py
```

### Limitations of the Crystalfontz Font Editor

- **Manual Character Adjustment:** Requires manual tweaks to each character to prevent size mismatches.
- **Limited Font Slots:** Only four slots are available for fonts.
- **Pre-loading Requirement:** Fonts must be loaded onto the LCD before use.

Using bitmap files alleviates many of these issues, allowing for better rendering quality.

## Building the Project

To build the project, run the following command:

```bash
scons
```

## Running the Program

1. Connect the LCD display.
2. Execute the program with:

```bash
sudo ./build/lcd_display.out
```