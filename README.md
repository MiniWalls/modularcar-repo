# Modular Robot Car

This is a project done during second semester in a group of 3 students!

# The function

Currently the car is driven with a bluetooth PS4 controller connected to an USB-shield on the car. You are able to accurately and responsively control the direction and speed of the car. There is infrared sensor on the wheel which allows us to calculate the RPM of the wheel and speed of the car. This data is transmitted using a radio RX/TX module to another computer to be viewed while the vehicle is moving. On the other computer we have second arduino which recieves the data, but also has an 8 by 8 LED matrix which displays the direction the car is moving with scrolling arrows.

Due to an SPI communication issue the USB-shield does not work while radio module is connected. Because of this radio module is disconnected when using the radio module, you must use a wired controller to be able to drive and have radio connection.


![Image of finished car](Images/robottiauto_kuva1.jpg?raw=true "Robot car")
