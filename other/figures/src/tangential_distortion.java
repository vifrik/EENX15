import processing.core.PApplet;
import processing.core.PGraphics;

public class tangential_distortion extends PApplet {

    public void dottedLine(int x1, int y1, int x2, int y2, int spacing) {
        float len = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        float steps = len / spacing;
        for (int i = 0; i <= steps; i++) {
            float x = lerp(x1, x2, i/steps);
            float y = lerp(y1, y2, i/steps);
            point(x, y);
        }
    }

    public void arrow(int x1, int y1, int x2, int y2) {
        line(x1, y1, x2, y2);
        float angle = atan2(y2 - y1, x2 - x1);
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        line(-10, -10, 0, 0);
        line(-10, 10, 0, 0);
        popMatrix();
    }

    public void arrowc(int x1, int y1, int x2, int y2) {
        line(x1, y1, x2, y2);
        float angle = atan2(y2 - y1, x2 - x1);
        pushMatrix();
        translate((x2 + x1) / 2, (y2 + y1) / 2);
        rotate(angle);
        line(-10, -10, 0, 0);
        line(-10, 10, 0, 0);
        popMatrix();
    }

    public void settings(){
        size(700, 500);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/tangential_distortion.svg");
        noStroke();
    }

    public void draw() {
        background(240);

        int height = 400;
        int yCenter = 400;
        int yOffset = yCenter - height / 2;

        translate(0, -150);
        
        noStroke();
        fill(100);
        ellipse(200, yCenter, 80, height);

        noFill();
        stroke(30);
        strokeWeight(3);
        //line(50, 200, 200, 200);
        //line(50, 600, 200, 600);
        for (int i = 1; i < 8; i++) {
            int yCoord = yOffset + i * height / 8;
            arrowc(50, yCoord, 200, yCoord);
            line(200, 200 + i * 400 / 8, 650, 800 - (200 + i * 400 / 8));
        }

        pushStyle();
        noStroke();
        fill(240);
        pushMatrix();
        float angle = 15 * PI / 180;
        translate(600,400);
        pushMatrix();
        rotate(angle);
        rect(0, -200, 50, 400);
        popMatrix();
        rect(0, 0, 60, 220);
        popMatrix();
        rect(200 - 25, 200 - 10, 50, 20);
        rect(200 - 25, 600 - 10, 50, 20);
        popStyle();

        pushStyle();
        pushMatrix();
        translate(600,400);
        rotate(15 * PI / 180);
        stroke(30, 240);
        strokeWeight(5);
        line(0, -200, 0, 200);
        popMatrix();
        stroke(255,0,0, 120);
        line(600, 200, 600, 600);
        popStyle();

        pushStyle();
        stroke(255,0,0);

        strokeWeight(5);
        arc(600, 400, 350, 350, PI/2, PI/2 + 15 * PI / 180);
        textSize(32);
        fill(240);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        textAlign(LEFT, TOP);
        stroke(255,0,0);
        fill(255,0,0);
        text("$p_{1}$", 606, 400 + 150);
        popStyle();

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("tangential_distortion");
    }
}
