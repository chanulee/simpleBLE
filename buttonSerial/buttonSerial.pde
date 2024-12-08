// Processing Code (save as button_control.pde)
import processing.serial.*;

Serial myPort;  // Create object from Serial class
boolean buttonState = false;  // Track button state
int buttonX = 150;
int buttonY = 150;
int buttonSize = 100;

void setup() {
  size(400, 400);
  // Connect directly to the specified port
  myPort = new Serial(this, "/dev/tty.usbserial-0001", 115200);
}

void draw() {
  background(200);
  
  // Draw button
  if (buttonState) {
    fill(0, 255, 0);  // Green when pressed
  } else {
    fill(255, 0, 0);  // Red when not pressed
  }
  
  ellipse(buttonX, buttonY, buttonSize, buttonSize);
  
  // Add text
  fill(0);
  textAlign(CENTER, CENTER);
  text(buttonState ? "ON" : "OFF", buttonX, buttonY);
}

void mousePressed() {
  // Check if mouse is inside button
  float d = dist(mouseX, mouseY, buttonX, buttonY);
  if (d < buttonSize/2) {
    buttonState = !buttonState;  // Toggle button state
    // Send '1' when button is pressed, '0' when released
    myPort.write(buttonState ? '1' : '0');
  }
}
