import processing.net.*;
import java.util.Scanner;

Server s;

void setup() {
  size(700,700,P3D);
  s = new Server(this, 12000);
  
  camera(400.0, 0.0, 60.0,
         0.0, 0.0, 60.0, 
         0.0, -1.0, 0.0);
}

void draw() {  
  //background(0);
  Marker marker = new Marker(1, 0, 0, 0, 0, 0);
  //marker.draw();
  
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
      
      int scalar = 200;
      float x = Float.parseFloat(parts[0])*scalar;  
      float y = Float.parseFloat(parts[1])*scalar;  
      float z = Float.parseFloat(parts[2])*scalar;  
      float rx = Float.parseFloat(parts[3]);  
      float ry = Float.parseFloat(parts[4]);  
      float rz = -Float.parseFloat(parts[5]); // z is flipped
      
      pushMatrix();
      translate(x, y, z);
      rectMode(CENTER);
      rotateX(rx);
      rotateY(ry);
      rotateZ(rz);
      
      fill(0, 0, 255, 150);
      box(40);
      stroke(255,0,0);
      line(0,0,0,0,0,100);
      popMatrix();
      
      marker.draw();
    }
    scanner.close();    
  }
}

class Marker {
  private static final int WIDTH = 50;
  float x,y,z;
  float pitch, roll, yaw;
  public Marker(float x, float y, float z, float pitch, float roll, float yaw) {
    this.x = x; this.y = y; this.z = z;
    this.pitch = pitch; this.roll = roll; this.yaw = yaw;
  }
  
  public void draw() {
    pushMatrix();
      translate(x, y, z);
      rectMode(CENTER);
      rotateX(roll);
      rotateY(pitch);
      rotateZ(yaw);
      fill(0, 0, 255, 150);
      rect(0, 0, WIDTH, WIDTH);
      pushStyle();
      stroke(255, 0, 0);
      line(0, 0, 0, WIDTH/2, 0, 0);
      stroke(0, 255, 0);
      line(0, 0, 0, 0, WIDTH/2, 0);
      stroke(0, 0, 255);
      line(0, 0, 0, 0, 0, WIDTH/2);
      popStyle();
      popMatrix();
  }
}
