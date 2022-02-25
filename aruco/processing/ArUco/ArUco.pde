import processing.net.*;
import java.util.Scanner;

Server s;

void setup() {
  size(700,700,P3D);
  s = new Server(this, 12000);
  
  camera(40.0, 40.0, 120.0,
         40.0, 40.0, 0.0, 
         0.0, 1.0, 0.0);
}

void draw() {  
  // z-direction is flipped, positive z is from paper
  //background(0);
  Marker marker1 = new Marker(40, 0, 0, -PI/2, 0, 0);
  Marker marker2 = new Marker(0, 40, 0, -PI/2, PI/2, 0);
  //marker.draw();

  pushMatrix();
  translate(0,40,0);
  //sphere(5);
  popMatrix();
  
  // check if a client sent something
  Client c = s.available();
  if (c != null) {
    background(0);
    
    String input = c.readStringUntil(byte(0));
    
    Scanner scanner = new Scanner(input);
    while (scanner.hasNextLine()) {
      String tvec = scanner.nextLine();
      String rvec = scanner.nextLine();
      String joined = (tvec + ", " + rvec).replace("[", "").replace("]", "");
      
      String[] parts = joined.split(", ",0);
      
      int scalar = 40;
      float x = Float.parseFloat(parts[0])*scalar;
      float y = Float.parseFloat(parts[1])*scalar;  
      float z = Float.parseFloat(parts[2])*scalar;  
      float rx = Float.parseFloat(parts[3]);  
      float ry = Float.parseFloat(parts[4]);  
      float rz = Float.parseFloat(parts[5]);
      
      pushMatrix();
      translate(x, y, z);
      rectMode(CENTER);
      rotateZ(rz);
      
      fill(0, 0, 255, 150);
      box(5);
      stroke(255,0,0);
      line(0,0,0,0,-100,0);
      popMatrix();
    }
    scanner.close();    
  }
}

class Marker {
  private static final int WIDTH = 6;
  float x,y,z;
  float rx, ry, rz;
  public Marker(float x, float y, float z, float rx, float ry, float rz) {
    this.x = x; this.y = y; this.z = z;
    this.rx = rx; this.ry = ry; this.rz = rz;
  }
  
  public void draw() {
    pushMatrix();
      translate(x, y, z);
      rectMode(CENTER);
      rotateX(rx);
      rotateY(ry);
      rotateZ(rz);
      fill(0, 0, 255, 150);
      rect(0, 0, WIDTH, WIDTH);
      pushStyle();
      stroke(255,255,255);
      line(0, 0, 0, 0, 0, WIDTH);
      popStyle();
      popMatrix();
  }
}
