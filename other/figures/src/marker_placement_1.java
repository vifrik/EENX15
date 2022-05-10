import processing.core.PApplet;
import processing.core.PGraphics;
import processing.svg.*;

public class template extends PApplet {

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
        size(800, 800);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/template.svg");
        noStroke();
    }

    public void draw() {
        background(30);

        circle(width / 2, height / 2, 200);

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("template");
    }
}
