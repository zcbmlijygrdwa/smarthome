  
// Example by Tom Igoe

import processing.serial.*;

Serial myPort;  // The serial port

void setup() {
  // List all the available serial ports
  printArray(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[1], 9600);
}

void draw() {
  while (myPort.available() > 0) {
    //int inByte = myPort.read();
    String inString = myPort.readString();
    println(inString);
  }
}
