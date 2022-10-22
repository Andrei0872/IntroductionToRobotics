# Introduction to Robotics

Learning interesting stuff about robotics.

- [Introduction to Robotics](#introduction-to-robotics)
  - [Homework 1](#homework-1)
  - [Useful resources](#useful-resources)

---

## Homework 1

Task requirements:

* there will be 3 potentiometers, each of which corresponding to an RGB color
* gather input from each of the potentiometers
* properly map the value from a `0-1023` range to a `0-255` range
* send the converted value to the right output PIN

The set-up:

<div style="text-align: center;">
  <img src="./homework/hw1/assets/setup.jpg">
</div>

The video showcasing the functionality can be found [here](https://youtube.com/shorts/_Dd5iX8rpv8?feature=share).

Interesting challenges:

* there is a single 5V pin on the Arduino board and 3 different potentiometers which require a source; then I recalled how [a breadboard works](https://thimble.io/how-to-use-a-breadboard-an-in-depth-guide/#:~:text=On%20most%20breadboards%2C%20they%20are,side%20of%20the%20center%20groove.) and found the solution: to use the *+* and *-* columns

## Useful resources

* https://roboticsbackend.com/arduino-uno-pins-a-complete-practical-guide/
* https://www.autodesk.com/products/eagle/blog/how-does-ground-work-in-electronics/
* [Arduino Tutorial 8: Understanding Pulse Width Modulation (PWM) and the Arduino Analog Write Command](https://www.youtube.com/watch?v=YfV-vYT3yfQ)

