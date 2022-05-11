import processing.core.PApplet;
import processing.core.PGraphics;

public class marker_placement_2 extends PApplet {

    public void dottedLine(int x1, int y1, int x2, int y2, int spacing) {
        float len = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        float steps = len / spacing;
        for (int i = 0; i <= steps; i++) {
            float x = lerp(x1, x2, i/steps);
            float y = lerp(y1, y2, i/steps);
            point(x, y);
        }
    }

    public void dottedCurve(int x, int y, float startang, float endang, int radius, int spacing) {
        float anglediff = endang - startang;
        float circumference = radius * anglediff;
        int steps = (int) abs(circumference / spacing);
        float anglestep = (endang - startang) / steps;
        for (int i = 0; i <= steps; i++) {
            float xnew = x + radius * cos(i * anglestep + startang);
            float ynew = y + radius * sin(i * anglestep + startang);
            point(xnew, ynew);
        }
    }

    public void arrow(int x1, int y1, int x2, int y2) {
        line(x1, y1, x2, y2);
        float angle = atan2(y2 - y1, x2 - x1);
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        line(-4, -4, 0, 0);
        line(-4, 4, 0, 0);
        popMatrix();
    }

    public void marker(int x2, int y2, float angle) {
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        float size = 20;
        pushStyle();
        strokeWeight(2);
        arrow((int)-size / 2, (int)-size / 2, 0, (int)-size);
        arrow((int)-size / 2, (int)size / 2, 0, (int)size);
        popStyle();

        line(-size, -size, 0, 0);
        line(-size, size, 0, 0);
        popMatrix();
    }


    public void settings(){
        size(800, 800);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/marker_placement_2.svg");
        noStroke();
    }

    public void draw() {
        background(240);
        strokeWeight(5);
        stroke(30);
        dottedLine(200 + 100, height / 2, width - 200, height / 2, 10);
        dottedCurve(200 + 100, height / 2 + 100, -PI, -PI / 2, 100, 10);
        dottedLine(200, height / 2 + 100, 200, height / 2 + 200, 10);


        marker(width - 200, height / 2, PI);

        for (int i = 1; i < 3; i++) {
            marker(width - 200 - 100 * i, height / 2 + (int)pow(-1, i) * 50, PI);
        }

        marker(width - 200, height / 2, PI);

        marker(200, height / 2 - 50, PI / 2);

        translate(10, 0);
        stroke(200);
        arrow(width - 200 - 100 * 1, height / 2 + 50 + 40, width - 200, height / 2 + 50 + 40);
        arrow(width - 200, height / 2 + 50 + 40, width - 200 - 100 * 1, height / 2 + 50 + 40);

        arrow(width - 200 - 100 * 2, height / 2 + 50 + 40, width - 200 - 100 * 1, height / 2 + 50 + 40);
        arrow(width - 200 - 100 * 1, height / 2 + 50 + 40, width - 200 - 100 * 2, height / 2 + 50 + 40);

        arrow(width - 170, height / 2, width - 170, height / 2 + 50);
        arrow(width - 170, height / 2 + 50, width - 170, height / 2);

        arrow(width - 170, height / 2, width - 170, height / 2 - 50);
        arrow(width - 170, height / 2 - 50, width - 170, height / 2);

        arrow(192, height / 2 - 50 - 40, width - 200, height / 2 - 50 - 40);
        arrow(width - 200, height / 2 - 50 - 40, 192, height / 2 - 50 - 40);

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("marker_placement_2");
    }
}
