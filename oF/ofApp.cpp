#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowShape(400, 400);
    
    // List all available serial devices
    serial.listDevices();
    
    // Connect to the serial port
    // Note: You'll need to change the port number based on your system
    serial.setup("/dev/tty.usbserial-0001", 115200);
    
    buttonState = false;
    buttonX = 150;
    buttonY = 150;
    buttonSize = 100;
}

void ofApp::update() {
}

void ofApp::draw() {
    ofBackground(200);
    
    // Draw button
    if (buttonState) {
        ofSetColor(0, 255, 0);  // Green when pressed
    } else {
        ofSetColor(255, 0, 0);  // Red when not pressed
    }
    
    ofDrawCircle(buttonX, buttonY, buttonSize/2);
    
    // Add text
    ofSetColor(0);
    string text = buttonState ? "ON" : "OFF";
    ofDrawBitmapString(text, buttonX - 10, buttonY + 5);
}

void ofApp::mousePressed(int x, int y, int button) {
    // Check if mouse is inside button
    float d = ofDist(x, y, buttonX, buttonY);
    if (d < buttonSize/2) {
        buttonState = !buttonState;
        // Send '1' when button is pressed, '0' when released
        char value = buttonState ? '1' : '0';
        serial.writeByte(value);
    }
}

void ofApp::keyPressed(int key) {
    // Handle numeric keys 1-8
    if (key >= '1' && key <= '8') {
        serial.writeByte(key);
        ofLog() << "Sent number: " << key;
    }
}