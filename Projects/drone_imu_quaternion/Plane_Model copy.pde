import processing.serial.*;

Serial myPort;

float q0=1, q1=0, q2=0, q3=0;

void setup() {
  size(1000, 800, P3D);
  println(Serial.list());
  myPort = new Serial(this, "/dev/cu.usbserial-11110", 115200);
  myPort.bufferUntil('\n');
}

void draw() {
  background(10);
  lights();
  
  translate(width/2, height/2, 0);

  applyQuaternionRotation(q0, q1, q2, q3);
  
  drawPlane();
}

void applyQuaternionRotation(float w, float x, float y, float z) {

  float[][] m = new float[4][4];

  m[0][0] = 1 - 2*y*y - 2*z*z;
  m[0][1] = 2*x*y - 2*z*w;
  m[0][2] = 2*x*z + 2*y*w;
  m[0][3] = 0;

  m[1][0] = 2*x*y + 2*z*w;
  m[1][1] = 1 - 2*x*x - 2*z*z;
  m[1][2] = 2*y*z - 2*x*w;
  m[1][3] = 0;

  m[2][0] = 2*x*z - 2*y*w;
  m[2][1] = 2*y*z + 2*x*w;
  m[2][2] = 1 - 2*x*x - 2*y*y;
  m[2][3] = 0;

  m[3][0] = m[3][1] = m[3][2] = 0;
  m[3][3] = 1;

  applyMatrix(
    m[0][0], m[0][1], m[0][2], m[0][3],
    m[1][0], m[1][1], m[1][2], m[1][3],
    m[2][0], m[2][1], m[2][2], m[2][3],
    m[3][0], m[3][1], m[3][2], m[3][3]
  );
}

void drawPlane() {
  fill(0,120,255);
  box(220,25,60);

  fill(255,0,0);
  box(60,8,260);

  pushMatrix();
  translate(-90,-35,0);
  box(40,70,5);
  popMatrix();
}

void serialEvent(Serial myPort) {
  String data = myPort.readStringUntil('\n');
  
  if (data != null) {
    data = trim(data);
    String[] v = split(data, ',');
    
    if (v.length == 4) {
      q0 = float(v[0]);
      q1 = float(v[1]);
      q2 = float(v[2]);
      q3 = float(v[3]);
    }
  }
}
