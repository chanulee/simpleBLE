#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

    ofSerial serial;
    bool buttonState;
    float buttonX;
    float buttonY;
    float buttonSize;
};

