import processing.net.*;
import java.util.Scanner;

Server s;

void setup() {
  size(700,700,P3D);
  s = new Server(this, 12000);
}

void draw() {  
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
      
      float x = Float.parseFloat(parts[0]);  
      float y = Float.parseFloat(parts[1]);  
      float z = Float.parseFloat(parts[2]);  
      float ry = Float.parseFloat(parts[3]);  
      float rp = Float.parseFloat(parts[4]);  
      float rr = Float.parseFloat(parts[5]);
      
      int moveFactor = 300;
      
      pushMatrix();
      translate(x*moveFactor+350, y*moveFactor+350, -z*moveFactor);
      rectMode(CENTER);
      rotateX(-ry);
      rotateY(-rr);
      rotateZ(ry > 0 ? -rp : rp);
      fill(0, 0, 255, 150);
      rect(0, 0, 50, 50);
      popMatrix();
    }
    scanner.close();
    
    
    
  }
}
