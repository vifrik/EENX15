import processing.core.PApplet;

public class main extends PApplet{
    class Car {
        private static final int WIDTH = 20;
        private static final int HEIGHT = 12;
        float x, y, rz;
        float delta;
        float vel;

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

            fill(255,0,0);

            rect(-WIDTH/2, -HEIGHT/2, WIDTH, HEIGHT);

            pushMatrix();
            fill(0,0,255);

            translate(WIDTH / 2 * 0.8f, HEIGHT / 2);
            pushMatrix();
            rotate(delta / 0.05f * 2);
            rect(-10,-2, 20, 4);
            popMatrix();

            translate(0, -HEIGHT);
            pushMatrix();
            rotate(delta / 0.05f * 2);
            rect(-10,-2, 20, 4);
            popMatrix();

            popMatrix();

            popMatrix();
        }
    }

    public void settings(){
        size(1200, 200);
    }

    Car car = new Car(0, 100, 45*PI/180+PI, -2f); // reverse
    //Car car = new Car(0, 100, 45*PI/180, 2f); // forward

    public void draw() {
        background(0);
        stroke(255,0,0);
        line(0,100,1200,100);

        float angle = atan2(100 - car.y, 1200 - car.x);
        float error = angle - car.rz + PI; // reverse
        //float error = angle - car.rz; // forward
        car.setAngle(-(angle + error)); // reverse
        //car.setAngle(angle + error); // forward


        noStroke();
        car.update();
        car.draw();
    }

    public static void main(String... args) {
        PApplet.main("main");
    }
}