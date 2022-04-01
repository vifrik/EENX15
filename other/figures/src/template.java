import processing.core.PApplet;
import processing.core.PGraphics;
import processing.svg.*;

public class template extends PApplet {

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
