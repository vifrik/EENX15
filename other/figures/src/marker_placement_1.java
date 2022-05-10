import processing.core.PApplet;
import processing.core.PGraphics;

public class marker_placement_1 extends PApplet {

    public void dottedLine(int x1, int y1, int x2, int y2, int spacing) {
        float len = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        float steps = len / spacing;
        for (int i = 0; i <= steps; i++) {
            float x = lerp(x1, x2, i/steps);
            float y = lerp(y1, y2, i/steps);
            point(x, y);
        }
    }

    public void arrowd(int x1, int y1, int x2, int y2) {
        line(x1, y1, x2, y2);
        float angle = atan2(y2 - y1, x2 - x1);
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        line(-4, -4, 0, 0);
        line(-4, 4, 0, 0);
        popMatrix();
    }

    public void arrow(int x2, int y2, float angle) {
        pushMatrix();
        translate(x2, y2);
        rotate(angle);
        float size = 20;
        pushStyle();
        strokeWeight(2);
        arrowd((int)-size / 2, (int)-size / 2, 0, (int)-size);
        arrowd((int)-size / 2, (int)size / 2, 0, (int)size);
        popStyle();

        line(-size, -size, 0, 0);
        line(-size, size, 0, 0);
        popMatrix();
    }


    public void settings(){
        size(800, 800);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/marker_placement_1.svg");
        noStroke();
    }

    public void draw() {
        background(240);
        strokeWeight(5);
        stroke(30);
        dottedLine(200, height / 2, width - 200, height / 2, 10);


        arrow(width - 200, height / 2, PI);

        for (int i = 1; i < 3; i++) {
            arrow(width - 200 - 100 * i, height / 2 + (int)pow(-1, i) * 50, PI);
        }

        translate(10, 0);
        stroke(200);
        arrowd(width - 200 - 100 * 1, height / 2 + 50 + 40, width - 200, height / 2 + 50 + 40);
        arrowd(width - 200, height / 2 + 50 + 40, width - 200 - 100 * 1, height / 2 + 50 + 40);

        arrowd(width - 200 - 100 * 2, height / 2 + 50 + 40, width - 200 - 100 * 1, height / 2 + 50 + 40);
        arrowd(width - 200 - 100 * 1, height / 2 + 50 + 40, width - 200 - 100 * 2, height / 2 + 50 + 40);

        arrowd(width - 170, height / 2, width - 170, height / 2 + 50 + 40);
        arrowd(width - 170, height / 2 + 50 + 40, width - 170, height / 2);

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("marker_placement_1");
    }
}
