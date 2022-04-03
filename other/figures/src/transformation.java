import processing.core.PApplet;
import processing.core.PGraphics;

public class transformation extends PApplet {

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

    public void settings(){
        size(810, 470);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/transformation.svg");
        noStroke();
    }

    public void draw() {
        background(30);

        // Marker
        //stroke(240);
        //rect((800 - 80) / 2 + 200, 50 - 2.5f, 80, 5);

        noStroke();
        translate(20, 20);

        pushStyle();
        noFill();
        strokeWeight(5);
        stroke(255, 0, 0);
        arrow(50, 50, 550 * 8 / 10 + 50, 50);

        stroke(0, 255, 0);
        arrow(550 * 8 / 10 + 50, 50, 600, 400);

        stroke(255, 255, 0);
        arrow(50, 50, 600, 400);
        popStyle();

        pushStyle();
        stroke(240);
        circle(50, 50, 10);
        circle(550 * 8 / 10 + 50, 50, 10);
        circle(600, 400, 10);

        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(240);
        textAlign(LEFT, BOTTOM);
        text("$(0, 0)$", 50, 50 - 5);
        textAlign(LEFT, BOTTOM);
        text("$(x_m, y_m)$", 550 * 8 / 10 + 50, 50 - 5);
        textAlign(LEFT, TOP);
        text("$(x_k, y_k)$", 620, 400);

        textSize(32);
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        textAlign(CENTER, BOTTOM);
        fill(255, 0, 0);
        text("$T_{mv}$", (550 * 8 / 10 + 50 - 50) / 2 + 50, 50 - 5);
        textAlign(LEFT, BOTTOM);
        fill(0, 255, 0);
        text("$T_{km}$", (600 - 550 * 8 / 10 + 50) / 2 + 550 * 8 / 10, (400 - 50) / 2 + 50);
        textAlign(RIGHT, TOP);
        fill(255, 255, 0);
        text("$T_{kv}$", (600 - 50) / 2 + 50, (400 - 50) / 2 + 50);
        popStyle();

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("transformation");
    }
}
