import processing.core.PApplet;

public class main extends PApplet{
    class PIRegulator {
        float kp;
        float ki;
        float prevError = 0;
        float integral = 0;

        public PIRegulator(float proportionalConst, float integratorConst) {
            this.kp = proportionalConst;
            this.ki = integratorConst;
        }

        public float update(float reference, float actual) {
            float error = reference - actual;
            float propTerm = kp * error;

            integral += error * 1/100;
            float intTerm = ki * integral;

            return intTerm + propTerm;
        }
    }
    class Car {
        private static final int WIDTH = 20;
        private static final int HEIGHT = 12;
        float x, y, rz;
        float delta;
        float vel;
        PIRegulator piRegulator = new PIRegulator(1,1);

        public Car(float x, float y, float rz, float vel) {
            this.x = x;
            this.y = y;
            this.rz = rz;
            this.vel = vel;
        }

        void setAngle(float angle) {
            final float MAX_ANGLE = 5.0f * PI / 180.0f;
            delta = max(-MAX_ANGLE, min(MAX_ANGLE, angle)) * 0.05f;
        }

        void update() {
            this.x += vel * cos(rz);
            this.y += vel * sin(rz);
            this.rz += tan(delta) * vel;
        }

        void draw() {
            pushMatrix();
            translate(this.x, this.y);
            rotate(this.rz);

            rect(0, 0, WIDTH, HEIGHT);
            popMatrix();
        }
    }

    public void settings(){
        size(1200, 200);
    }

    Car car = new Car(0, 100, 45*PI/180, 2f);
    PIRegulator piRegulator = new PIRegulator(0.1f, 2f);
    float delta = 0;
    public void draw() {
        //piRegulator.integral = 0;

        background(0);
        stroke(255,0,0);
        line(0,100,1200,100);
        /*strokeWeight(3);
        stroke(255,0,0);
        line(0, 200, 200, 200);
        line(200, 200, 200, 100);
        line(200, 100, 1200, 100);

        stroke(0,0,255);
        delta = 0;
        for (int i = 0; i < 200; i++) {
            float diff = piRegulator.update(0, delta);
            delta += diff;
            System.out.println(i + " " + diff);
            circle(i, 200 - delta*100, 2);
        }

        for (int i = 200; i < 1000; i++) {
            float diff = piRegulator.update(1, delta);
            delta += diff;
            circle(i, 200 - delta*100, 2);
        }*/

        float angle = atan2(100 - car.y, 1200 - car.x);
        float error = angle - car.rz;
        car.setAngle(error + angle);

        noStroke();
        car.update();
        car.draw();
    }

    public static void main(String... args) {
        PApplet.main("main");
    }
}