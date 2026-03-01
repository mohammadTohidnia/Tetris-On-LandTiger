# Tetris on LandTiger LPC1768

## Overview
This project is a simplified, embedded implementation of the classic game Tetris, designed for the **LandTiger development board** featuring the **NXP LPC1768** microcontroller. The game utilizes the onboard GLCD for visual rendering, the joystick for gameplay control, and push buttons for state management.

The software is written in C using Keil µVision and relies heavily on hardware interrupts, timers, and software debouncing to manage game physics, user input, and state transitions seamlessly.

---

## Hardware & Software Requirements
* **Hardware:** LandTiger NXP LPC1768 Development Board
* **Display:** GLCD (Graphic Liquid Crystal Display)
* **Inputs:** Onboard Joystick, KEY1, and KEY2 buttons
* **IDE:** Keil µVision (with standard LPC17xx startup files and CMSIS)
* **Testing:** Physical board or Keil Simulator

---

## Controls
* **Joystick Left/Right:** Move the tetromino horizontally.
* **Joystick Up:** Rotate the tetromino 90 degrees clockwise.
* **Joystick Down (Hold):** Soft Drop (accelerates gravity).
* **KEY1:** Pause/Resume the game, or Restart after a Game Over.
* **KEY2:** Hard Drop (instantly drops and locks the tetromino).

---

## Project Architecture & File Organization
The source code is modularized to separate hardware initialization, game logic, and interrupt handling:

### `sample.c`
The entry point of the application. It initializes the hardware peripherals (PLL, GLCD, GPIO, Timers, RIT), sets up the initial game field, and enters a low-power sleep mode loop (`__ASM("wfi")`), waiting for interrupts to drive the game forward.

### `my_func.c` / `my_func.h`
Contains the core game logic, primary data structures, scoring mechanisms, and GLCD drawing primitives (including custom block rendering).

### `IRQ_timer.c`
Houses the interrupt service routines for **Timer0** and **Timer1**:
* **Timer0:** Manages the game's physics engine (gravity and downward movement).
* **Timer1:** Polls the joystick for user input.

### `IRQ_button.c` & `IRQ_RIT.c`
Manages external button interrupts (KEY1, KEY2). To prevent hardware bouncing, the **Repetitive Interrupt Timer (RIT)** is dynamically enabled to provide a 50ms software debounce before executing button actions.

---

## Core Data Structures

### 1. Playing Field Grid
The playing area is represented by a 20x10 2D array of structures (`field_state`). Each coordinate holds:
* **state:** A boolean-like integer (0 for empty, 1 for occupied).
* **color:** The specific color of the block, allowing the screen to redraw accurately without losing shape identities.

### 2. Tetromino Look-Up Table
To avoid computationally expensive matrix rotations, the game uses a 3D array `shapes[7][4][4]` to store every possible state:
* **Shape ID:** 7 distinct shapes (I, O, T, J, L, S, Z).
* **Rotation State:** 4 orientations (0°, 90°, 180°, 270°).
* **Blocks:** 4 specific (x, y) coordinates relative to the anchor point.

---

## Key Algorithms

### Game Physics & Collision (Timer0)
Timer0 acts as the heartbeat of the game, triggering every 1 second by default.
* It checks if the current shape can move down by scanning the bottom edge and the `field_state` array.
* **Collision:** If detected, the shape locks into the grid, the score increments by 10, line checks are performed, and a new shape is generated.
* **Clear Path:** If clear, the old shape is erased, Y-coordinates are incremented, and the shape is redrawn.

### Controller Polling (Timer1)
Triggers every 50ms to poll the joystick:
* **Movement:** Uses "look-ahead" validation to prevent moving out of bounds or into existing blocks.
* **Rotation:** Calculates the coordinate difference between orientations to swap the shape while maintaining its anchor point.
* **Soft Drop:** Dynamically modifies the Match Register (`MR0`) of Timer0 to double gravity speed while the stick is held.

### Line Clearing Algorithm
The `check_lines` function scans the grid from bottom (row 19) to top. When a full row is detected:
1.  The row is visually cleared.
2.  Every cell from the rows above is copied down by one.
3.  The top row is wiped clean.
4.  **Crucial Logic:** The iterator index is manually adjusted after a shift to re-evaluate the newly shifted row, ensuring consecutive lines are cleared correctly.

### Hard Drop Logic
Triggered by **KEY2**, this calculates the minimum vertical distance between the falling shape and the nearest obstacle. It applies this distance instantly, forcing a lockdown and line check.

---

## How to Test and Run
1.  Open the project in **Keil µVision**.
2.  Set the target to the **NXP LPC1768**.
3.  Build the project (**F7**) and ensure there are zero errors.

### Using the Simulator
* Start the Debug session (**Ctrl + F5**).
* Open the GLCD, Joystick, and GPIO simulation windows to interact with the game.

### Using the LandTiger Board
* Connect the board via USB/JTAG.
* Flash the program using the **Download** button (**F8**).
* Press the physical **RESET** button. The game will begin automatically on the GLCD.

---

## Acknowledgements
The foundational libraries for GLCD, Joystick, and Timers were utilized as part of the standard LPC1768 development suite.