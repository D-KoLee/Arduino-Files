import processing.serial.*;
Serial myPort;
float background_color ;
int prev_state;
int clickEvent = 10;

void setup() {
  size (300, 300);
  myPort = new Serial (this, "COM4", 9600);
  myPort.bufferUntil('\n');
}

void serialEvent(Serial myPort) {
  background_color = float (myPort.readStringUntil('\n'));
}

void draw() {
  if (clickEvent == 11) {
    background (background_color, 0, 0);
  } else if (clickEvent == 12) {
    background (0, background_color, 0);
  } else if (clickEvent == 13) {
    background (0, 0, background_color);
  } else if (clickEvent == 10) {
    background (0, 0, 0);
  }

  if (mousePressed && (mouseButton == LEFT)) {
    if(++clickEvent==14)clickEvent=11;
    if (clickEvent == 11) myPort.write('1');
    else if (clickEvent == 12) myPort.write('2');
    else if (clickEvent == 13) myPort.write('3');
    prev_state = clickEvent;
    delay(100);
  }
  if (mousePressed && (mouseButton == RIGHT)) {
    myPort.write('0');
    if (prev_state == clickEvent)  clickEvent = 10;
    delay(100);
  }
  if (keyPressed) {
    myPort.write(key);
    delay(100);
  }
}
