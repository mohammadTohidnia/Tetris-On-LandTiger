# LandTiger Emulator Installation Guide

## Preliminary Steps
1. Install the **`VC_redist.x86.exe`** file.

## Installation
2. Locate the files extracted from the `LandTiger.zip` folder (e.g., the **`LandTiger.dll`** file and the **`DOCS`** folder).
3. Place these extracted files into your Keil installation directory, specifically in the following path: **`<KEIL_INSTALL_PATH>_v5\ARM\BIN`**. 
   * *Note: You can find your specific Keil installation path by opening Keil and clicking the "wrench/tools" icon in the toolbar.*

## Usage and Configuration
4. Open the **KEIL** application.
5. Go to **Options for Target** and navigate to the **Debug** tab (specifically for the target `SW_Debug`).
6. Find the **Parameter** textbox located next to **Dialog DLL**.
7. Type **`-dLandTiger`** into this textbox. When you are finished, the full text in the parameter box should read: **`-pLPC1768 -dLandTiger`**.