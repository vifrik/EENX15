import processing.core.PApplet;
import processing.core.PGraphics;

public class focal_length extends PApplet {

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
        size(800, 400);
        this.g = new PGraphics();
        noLoop();
        beginRecord(SVG, "exported/focal_length.svg");
        noStroke();
    }

    public void draw() {
        background(240);

        translate(40, 200);
        pushMatrix();

        // Camera triangle
        noStroke();
        fill(50);
        beginShape();
        vertex(12+6, -6);
        vertex(12+6, 6);
        vertex(12+6+12, 8);
        vertex(12+6+12, -8);
        endShape();
        rotate(PI / 2);

        rect(-12, -15, 24, 40);
        popMatrix();

        translate(20 + 15, 0);
        float scalar = 13.5f;
        noStroke();
        textAlign(CENTER, BOTTOM);

        fill(0x64B429F9);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xFFB429F9);
        text("Fisheye", 0, -10*scalar/2);
        textAlign(CENTER, TOP);
        text("180°", 0, 10*scalar/2);
        popStyle();
        arc(0, 0, 10*scalar, 10*scalar, -180/2.0f * PI / 180.0f, 180/2.0f * PI / 180.0f);

        fill(0x649C43F8);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xFF9C43F8);
        text("14mm", cos(-114/2.0f * PI / 180.0f) * 14*scalar / 2, sin(-114/2.0f * PI / 180.0f) * 14*scalar / 2);
        textAlign(CENTER, TOP);
        text("114°", cos(-114/2.0f * PI / 180.0f) * 14*scalar / 2, -sin(-114/2.0f * PI / 180.0f) * 14*scalar / 2);
        popStyle();
        arc(0, 0, 14*scalar, 14*scalar, -114/2.0f * PI / 180.0f, 114/2.0f * PI / 180.0f);

        fill(0x64855DF7);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xff855DF7);
        text("24mm", cos(-84/2.0f * PI / 180.0f) * 24*scalar / 2, sin(-84/2.0f * PI / 180.0f) * 24*scalar / 2);
        textAlign(CENTER, TOP);
        text("84°", cos(-84/2.0f * PI / 180.0f) * 24*scalar / 2, -sin(-84/2.0f * PI / 180.0f) * 24*scalar / 2);
        popStyle();
        arc(0, 0, 24*scalar, 24*scalar, -84/2.0f * PI / 180.0f, 84/2.0f * PI / 180.0f);

        fill(0x646D77F6);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xff6D77F6);
        text("35mm", cos(-63/2.0f * PI / 180.0f) * 35*scalar / 2, sin(-63/2.0f * PI / 180.0f) * 35*scalar / 2);
        textAlign(CENTER, TOP);
        text("63°", cos(-63/2.0f * PI / 180.0f) * 35*scalar / 2, -sin(-63/2.0f * PI / 180.0f) * 35*scalar / 2);
        popStyle();
        arc(0, 0, 35*scalar, 35*scalar, -63/2.0f * PI / 180.0f, 63/2.0f * PI / 180.0f);

        fill(0x645591F5);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xff5591F5);
        text("50mm", cos(-46/2.0f * PI / 180.0f) * 50*scalar / 2, sin(-46/2.0f * PI / 180.0f) * 50*scalar / 2);
        textAlign(CENTER, TOP);
        text("46°", cos(-46/2.0f * PI / 180.0f) * 50*scalar / 2, -sin(-46/2.0f * PI / 180.0f) * 50*scalar / 2);
        popStyle();
        arc(0, 0, 50*scalar, 50*scalar, -46/2.0f * PI / 180.0f, 46/2.0f * PI / 180.0f);

        fill(0x643EABF4);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xff3EABF4);
        text("85mm", cos(-28/2.0f * PI / 180.0f) * 85*scalar / 2, sin(-28/2.0f * PI / 180.0f) * 85*scalar / 2);
        textAlign(CENTER, TOP);
        text("28°", cos(-28/2.0f * PI / 180.0f) * 85*scalar / 2, -sin(-28/2.0f * PI / 180.0f) * 85*scalar / 2);
        popStyle();
        arc(0, 0, 85*scalar, 85*scalar, -28/2.0f * PI / 180.0f, 28/2.0f * PI / 180.0f);

        fill(0x6426C5F3);
        pushStyle();
        fill(30);            // weird bug
        text(" ",0, 0); // something is not cleared properly and drawing empty text seems to fix it
        fill(0xff26C5F3);
        text("100mm", cos(-24/2.0f * PI / 180.0f) * 100*scalar / 2, sin(-24/2.0f * PI / 180.0f) * 100*scalar / 2);
        textAlign(CENTER, TOP);
        text("24°", cos(-24/2.0f * PI / 180.0f) * 100*scalar / 2, -sin(-24/2.0f * PI / 180.0f) * 100*scalar / 2);
        popStyle();
        arc(0, 0, 100*scalar, 100*scalar, -24/2.0f * PI / 180.0f, 24/2.0f * PI / 180.0f);

        /*fill(0xAAFF0000);
        pushStyle();
        text("3.04mm", cos(-164/2.0f * PI / 180.0f) * 3.04f*scalar / 2, sin(-164/2.0f * PI / 180.0f) * 3.04f*scalar / 2);
        textAlign(CENTER, TOP);
        text("164$\deg$", cos(-164/2.0f * PI / 180.0f) * 3.04f*scalar / 2, -sin(-164/2.0f * PI / 180.0f) * 3.04f*scalar / 2);
        popStyle();
        arc(0, 0, 3.04f*scalar, 3.04f*scalar, -164/2.0f * PI / 180.0f, 164/2.0f * PI / 180.0f);*/

        endRecord();
    }

    public static void main(String... args) {
        PApplet.main("focal_length");
    }
}
