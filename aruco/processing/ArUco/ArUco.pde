import processing.net.*;

Server s;

void setup() {
  size(700,700,P3D);
  s = new Server(this, 12000);
}

void draw() {  
  // check if a client sent something
  Client c = s.available();
  if (c != null) {
    String input = c.readStringUntil(byte('\n'));
    println(input);
    String[] parts = input.split(" ",0);
    
    float x = Float.parseFloat(parts[0]);  
    float y = Float.parseFloat(parts[1]);  
    float z = Float.parseFloat(parts[2]);  
    float ry = Float.parseFloat(parts[3]);  
    float rp = Float.parseFloat(parts[4]);  
    float rr = Float.parseFloat(parts[5]);
    
    println(x,y,z,ry,rp,rr);
    
    background(0);
    
    int moveFactor = 300;
    
    pushMatrix();
    translate(x*moveFactor+350, y*moveFactor+350, -z*moveFactor);
    rectMode(CENTER);
    rotateX(-ry);
    rotateY(-rr);
    rotateZ(ry > 0 ? -rp : rp);
    fill(0, 0, 255, 150);
    rect(0, 0, 200, 200);
    popMatrix();
  }
}
