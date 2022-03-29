import processing.core.PApplet;
import java.lang.Math;

public class main extends PApplet{
    class Car {
        private static final int WIDTH = 20;
        private static final int HEIGHT = 12;
        private static final int TRAILER_WIDTH = 100;
        float x, y, rz;
        float delta;
        float vel;

        float theta;
        float xTrailer;
        float yTrailer;

        float oldAngleDesired = 0;

        float angle = 0;
        float error = 0;

        public Car(float x, float y, float rz, float vel) {
            this.x = x;
            this.y = y;
            this.rz = rz;
            this.vel = vel;

            rz = rz - PI;
            xTrailer = x + cos(rz) * TRAILER_WIDTH;
            yTrailer = y + sin(rz) * TRAILER_WIDTH;
            theta = this.rz;
        }

        void update() {
            this.x += vel * cos(rz);
            this.y += vel * sin(rz);
            float d_rz = tan(delta) * vel / WIDTH;;
            this.rz += d_rz;

            this.xTrailer += vel * cos(rz - theta) * cos(theta);
            this.yTrailer += vel * cos(rz - theta) * sin(theta);

            float d_theta = vel * sin(rz - theta) / TRAILER_WIDTH;
            this.theta += d_theta;

            angle = atan2(300 - yTrailer, 1200 - xTrailer);
            error = angle - theta + PI;

            float angleDesired = -(-angle-error);
            float d_angleDesired = (angleDesired - oldAngleDesired);

            oldAngleDesired = angleDesired;

            float deltaTarget = -atan2((d_theta - d_angleDesired) * WIDTH, vel);

            delta += max(-0.01f, min(0.01f, deltaTarget - delta));
            delta = max(-10*PI/180, min(10*PI/180, delta));
        }

        void draw() {
            pushMatrix();
            translate(this.x, this.y);
            rotate(this.rz);

            fill(255,0,0);

            rect(-WIDTH/2, -HEIGHT/2, WIDTH, HEIGHT);

            pushMatrix();
            fill(0,0,255);
            translate(WIDTH / 2 * 0.8f, HEIGHT / 2);
            pushMatrix();
            rotate(delta);
            rect(-10,-2, 20, 4);
            popMatrix();

            translate(0, -HEIGHT);
            pushMatrix();
            rotate(delta);
            rect(-10,-2, 20, 4);
            popMatrix();

            popMatrix();

            popMatrix();

            pushMatrix();
                translate(xTrailer, yTrailer);
                rotate(theta + PI);

                rect(0,-HEIGHT / 2, -TRAILER_WIDTH, HEIGHT);
                circle(0,0, 5);
                pushStyle();
                    stroke(255,0,0);
                    line(0, 0, 100, 0);

                    stroke(0,255,0);
                    line(0, 0, cos(angle) * 20, sin(angle) * 20);

                    stroke(0,0,255);
                    line(0, 0, cos(error) * 20, sin(error) * 20);
                popStyle();

                rect(0,-HEIGHT / 2, -TRAILER_WIDTH, HEIGHT);
                circle(0,0, 5);
            popMatrix();
        }
    }

    public void settings(){
        size(1200, 800);
    }

    Car car = new Car(300, 550, -15*PI/180+PI, -2f);
    public void draw() {
        background(0);
        stroke(255,0,0);
        line(0,300,1200,300);

        noStroke();
        if (keyCode == ENTER) {
            car.update();
        }

        car.draw();
    }

    public static void main(String... args) {
        PApplet.main("main");
    }
}