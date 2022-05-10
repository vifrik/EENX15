import processing.core.PApplet;
import processing.core.PGraphics;
import processing.core.PImage;

public class markerincoordinate extends PApplet {

    public void dottedLine(int x1, int y1, int x2, int y2, int spacing) {
        float len = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        float steps = len / spacing;
        for (int i = 0; i <= steps; i++) {
            float x = lerp(x1, x2, i/steps);
            float y = lerp(y1, y2, i/steps);
            point(x, y);
        }
    }

    public void arrow(int x1, int y1, int x2, int y2, String text, int alignx, int aligny) {
        text(" ",0, 0);

        line(x1, y1, x2, y2);
        float angle = atan2(y2 - y1, x2 - x1);
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        line(-10, -10, 0, 0);
        line(-10, 10, 0, 0);
        popMatrix();
    }

    PImage marker;

    public void settings(){
        size(650, 400);
        this.g = new PGraphics();
        this.surface = initSurface();
        noLoop();
        beginRecord(SVG, "exported/markerincoordinate.svg");
        noStroke();

        marker = loadImage("marker0.png");
    }

    public void draw() {
        background(240);
        translate(-100, -60);

        int markerSize = 80;
        int markerX = 600;
        int markerY = 200;
        image(marker, markerX - markerSize / 2, markerY- markerSize / 2, markerSize, markerSize);

        int vecLength = 100;
        int vecDiag = 28*100/40;
        strokeWeight(5);
        noFill();
        stroke(255,0,0);
        fill(255,0,0);
        arrow(markerX, markerY, markerX + vecLength, markerY, "$x$", LEFT, BOTTOM);
        stroke(0,255,0);
        fill(0,255,0);
        arrow(markerX, markerY, markerX, markerY - vecLength, "$y$", LEFT, BOTTOM);
        stroke(0,0,255);
        fill(0,0,255);
        arrow(markerX, markerY, markerX - vecDiag, markerY + vecDiag, "$z$", RIGHT, BOTTOM);

        fill(0xFF26C5F3);
        noStroke();
        translate(400, 400);
        pushMatrix();
        rotate(PI / 4);
        beginShape();
        vertex(-8, -30);
        vertex(8, -30);
        vertex(6, -18);
        vertex(-6, -18);
        endShape();
        rect(-12, -15, 24, 40);
        popMatrix();

        vecLength = 100;
        vecDiag = -28*100/40;
        strokeWeight(5);
        noFill();
        stroke(255,0,0);
        fill(255,0,0);
        arrow(0, 0, -vecLength, 0, "$x$", LEFT, BOTTOM);
        stroke(0,255,0);
        fill(0,255,0);
        arrow(0, 0, 0, -vecLength, "$y$", LEFT, BOTTOM);
        stroke(0,0,255);
        fill(0,0,255);
        arrow(0, 0, -vecDiag, vecDiag, "$z$", RIGHT, BOTTOM);

        translate(-200, -300);
        vecLength = 200;
        vecDiag = 28*100/40;
        stroke(255,0,0);
        fill(255,0,0);
        arrow(0, 0, vecLength, 0, "$x$", LEFT, BOTTOM);
        stroke(0,0,255);
        fill(0,0,255);
        arrow(0, 0, 0, vecLength, "$z$", LEFT, BOTTOM);
        stroke(0,255,0);
        fill(0,255,0);
        arrow(0, 0, -vecDiag, vecDiag, "$y$", RIGHT, BOTTOM);

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("markerincoordinate");
    }
}
