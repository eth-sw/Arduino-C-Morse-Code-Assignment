# Arduino SACode Interpreter README

## Overview

The Arduino SACode Interpreter is a program written in Arduino language that interprets SACode, a custom communication protocol. SACode consists of a set of symbols such as '@', '>', and '+', each representing specific commands. The program translates SACode input into corresponding actions, controlling LEDs based on the interpreted commands.
I achieved a mark of 75% for this assignment.

## Table of Contents

- [Introduction](#introduction)
- [Setup](#setup)
- [Usage](#usage)
- [SACode Commands](#sacode-commands)
- [Input Processing](#input-processing)
- [LED Output](#led-output)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This Arduino program is designed to interpret SACode input and perform various actions based on the SACode commands received. The program uses predefined arrays to map SACode symbols to specific actions, allowing users to control LEDs, read infrared signals, and adjust LED brightness.

## Setup

To use the Arduino SACode Interpreter, follow these steps:

1. Open the Arduino IDE.
2. Copy and paste the provided code into a new Arduino sketch.
3. Upload the sketch to your Arduino board.

## Usage

The program continuously listens for SACode input from the serial monitor. Users can enter SACode sequences, and the program will interpret and execute the corresponding commands. The SACode interpreter supports commands for controlling LEDs, reading infrared signals, and adjusting LED brightness.

## SACode Commands

The SACode Interpreter supports the following SACode commands:

- '@': Turn on and off the LED quickly.
- '>': Turn on and off the LED with a longer duration.
- '+': Turn on and off the LED with a medium duration.
- '/': Pause between SACode commands.

Additionally, the program recognizes specific command combinations like "RA" and "RIR" to perform more advanced actions.

## Input Processing

The program processes SACode input entered through the serial monitor. It distinguishes between SACode commands and performs the corresponding actions. Error handling is implemented to ensure correct input processing.

## LED Output

The program controls LED output based on the interpreted SACode commands. LEDs are connected to specific pins on the Arduino board, and their behavior is determined by the SACode input.

## Dependencies

The Arduino SACode Interpreter does not have external dependencies. It is designed to run on standard Arduino boards.

## Contributing

Contributions to the Arduino SACode Interpreter are welcome. If you have suggestions, improvements, or bug fixes, feel free to create a pull request.

## License

The Arduino SACode Interpreter is released under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code as per the terms of the license.
