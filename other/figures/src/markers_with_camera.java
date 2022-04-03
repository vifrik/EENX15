import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PGraphics;

public class markers_with_camera extends PApplet {

    public void drawAxes() {
        pushStyle();
        // X-axis
        strokeWeight(5);
        stroke(255, 0, 0);
        line(50, 50, 750, 50);
        line(750 - 10, 50 - 10, 750, 50);
        line(750 - 10, 50 + 10, 750, 50);

        // Y-axis
        stroke(0, 255, 0);
        line(50, 50, 50, 750);
        line(50 - 10, 750 - 10, 50, 750);
        line(50 + 10, 750 - 10, 50, 750);

        // Z-axis
        stroke(0, 0, 255);
        line(50 - 10, 50 - 10, 50 + 10, 50 + 10);
        line(50 + 10, 50 - 10, 50 - 10, 50 + 10);
        popStyle();

        pushStyle();
        textSize(32);
        textAlign(LEFT, TOP);
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0, 255, 0);
        text("$y$",50, 750);
        fill(255, 0, 0);
        text("$x$",750, 50);
        popStyle();
    }

    public void dottedLine(int x1, int y1, int x2, int y2, int spacing) {
        float len = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        float steps = len / spacing;
        for (int i = 0; i <= steps; i++) {
            float x = lerp(x1, x2, i/steps);
            float y = lerp(y1, y2, i/steps);
            point(x, y);
        }
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

        strokeWeight(5.0f);
        stroke(50);
        dottedLine(750, 50, 750, 750, 10);
        dottedLine(50, 750, 750, 750, 10);
        noStroke();
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
        rect(400 - 40, 400 + 60 + 10, 80, 250);

        // Camera triangle
        fill(0, 240, 240);
        //triangle(400, 400, 400 + 8, 400 - 30, 400 - 8, 400 - 30);
        beginShape();
        vertex(400 - 8, 400 - 30);
        vertex(400 + 8, 400 - 30);
        vertex(400 + 6, 400 - 18);
        vertex(400 - 6, 400 - 18);

        endShape();
        rect(400 - 12, 400 - 15, 24, 40);

        // Camera fov
        fill(0, 240, 240, 20);
        beginShape();
        vertex(400 + 8, 400 - 30);
        vertex(400 - 8, 400 - 30);
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
