import processing.core.PApplet;
import java.lang.Math;
import java.util.ArrayList;
import java.util.List;

public class main extends PApplet{
    class Coord {
        float x, y;

        public Coord(float x, float y) {
            this.x = x;
            this.y = y;
        }
    }

    class PPC {
            List<Integer> targetCandidates = new ArrayList<>();
            public List<Coord> path = new ArrayList<>();

            int index = 0;
            boolean fromNegative = false;
            private static final int MAX_STEPS = 10000;

            static double calculateDistance(Coord c1, Coord c2) {
                return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
            }

            public PPC(int pixelWidth) {
                final float points = 1000;
                float resolution = pixelWidth / points;
                for (int i = 0; i < 1000; i++) {
                    Coord position = new Coord(resolution * i, 50.0f * sin(i / 600.0f * 2 * PI) + 300.0f);
                    path.add(position);
                }

            }

            Coord getTarget(double lookahead, Coord position) {
                for (int i = index; i < path.size(); i++) {
                    Coord current = path.get(i);
                    double distance = calculateDistance(position, current);

                    if (fromNegative && distance - lookahead >= 0) {
                        fromNegative = false;
                        targetCandidates.add(i);
                    } else if (distance - lookahead < 0) {
                        fromNegative = true;
                    }

                    // To save time, break after 100 iterations if we found a candidate
                    if (i > index + MAX_STEPS && !targetCandidates.isEmpty()) {
                        break;
                    }

                    if (i == path.size() - 1 && targetCandidates.isEmpty()) {
                        return path.get(path.size() - 1);
                    }
                }

                if (targetCandidates.isEmpty()) {
                    return path.get(index);
                }

                index = targetCandidates.get(targetCandidates.size() - 1);
                return path.get(index);
            }

            boolean atEnd(double lookahead, Coord position, Coord target) {
                if (sqrt(pow(position.y - target.y, 2) + pow(position.x - target.x, 2)) < 0.2f * lookahead) {
                    return true;
                }
                return false;
            }
        }
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
        boolean shouldUpdate = true;

        PPC ppc = new PPC(1200);
        Coord target = new Coord(0,0);

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
            if (!shouldUpdate) return;

            this.x += vel * cos(rz);
            this.y += vel * sin(rz);
            float d_rz = tan(delta) * vel / WIDTH;;
            this.rz += d_rz;

            Coord trailerPosition = new Coord(xTrailer, yTrailer);
            target = ppc.getTarget(80.0f, trailerPosition);
            if (ppc.atEnd(80.0f, trailerPosition, target)) {
                shouldUpdate = false;
                return;
            }

            this.xTrailer += vel * cos(rz - theta) * cos(theta);
            this.yTrailer += vel * cos(rz - theta) * sin(theta);

            float d_theta = vel * sin(rz - theta) / TRAILER_WIDTH;
            this.theta += d_theta;

            angle = atan2(target.y - yTrailer, target.x - xTrailer);
            error = angle - theta + PI;

            float angleDesired = -(-angle-error);
            float d_angleDesired = (angleDesired - oldAngleDesired);

            oldAngleDesired = angleDesired;

            float deltaTarget = -atan2((d_theta - d_angleDesired - d_rz) * WIDTH, vel);

            delta += max(-0.25f, min(0.25f, deltaTarget - delta));
            delta = max(-30*PI/180, min(30*PI/180, delta));
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
                pushStyle();
                noFill();
                stroke(0,0,255);
                circle(0,0, 200);
                popStyle();

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

            for (Coord pos : ppc.path) {
                circle(pos.x, pos.y, 2);
            }

            pushStyle();
            fill(255,0,0);
            circle(target.x, target.y, 10);
            popStyle();
        }
    }

    Car car = new Car(0, 300, 15*PI/180+PI, -2f);
    public void settings(){
        size(1200, 800);
    }

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