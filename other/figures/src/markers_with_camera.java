import processing.core.PApplet;
import processing.core.PGraphics;

public class markers_with_camera extends PApplet {

    public void drawAxes() {
        pushStyle();
        textSize(32);
        textAlign(CENTER, CENTER);

        // X-axis
        strokeWeight(5);
        stroke(255, 0, 0);
        line(50, 50, 750, 50);
        line(750 - 10, 50 - 10, 750, 50);
        line(750 - 10, 50 + 10, 750, 50);
//        fill(255, 0, 0);
//        text("x",750 + 14, 50 - 6);
//        noFill();

        // Y-axis
        stroke(0, 255, 0);
        line(50, 50, 50, 750);
        line(50 - 10, 750 - 10, 50, 750);
        line(50 + 10, 750 - 10, 50, 750);
//        fill(0, 255, 0);
//        text("y",50, 750 + 10);
//        noFill();

        // Z-axis
        stroke(0, 0, 255);
        line(50 - 10, 50 - 10, 50 + 10, 50 + 10);
        line(50 + 10, 50 - 10, 50 - 10, 50 + 10);
        popStyle();
    }

    public void settings(){
        size(800, 800);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/markers_with_camera.svg");
        noStroke();
    }

    public void draw() {
        background(30);

        drawAxes();

        // Marker
        stroke(240);
        rect((800 - 80) / 2, 50 + 5, 80, 5);
        rect((800 - 80) / 2 - 200, 50 + 5, 80, 5);
        rect((800 - 80) / 2 + 200, 50 + 5, 80, 5);

        // Truck and trailer silhouette
        noStroke();
        fill(50);
        rect(400 - 40, 400 - 40, 80, 100);
        rect(400 - 40, 400 + 60 + 10, 80, 300);

        // Camera triangle
        fill(0, 240, 240);
        triangle(400, 400, 400 + 30, 400 - 30, 400 - 30, 400 - 30);

        // Camera fov
        fill(0, 240, 240, 20);
        beginShape();
        vertex(400 + 30, 400 - 30);
        vertex(400 - 30, 400 - 30);
        vertex(250, 50);
        vertex(550, 50);
        endShape();

//        noFill();
//        stroke(255, 255, 0);
//        line(50, 50, 400, 50);
//        line(400, 400 - 15, 400, 50);
//        line(400, 400 - 15, 50, 50);

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("markers_with_camera");
    }
}
