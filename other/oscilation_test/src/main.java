import processing.core.PApplet;
import java.lang.Math;

public class main extends PApplet{
    static final double TOL = 0.0000001;

    public static Circle circleFromPoints(final Point p1, final Point p2, final Point p3)
    {
        final double offset = Math.pow(p2.x,2) + Math.pow(p2.y,2);
        final double bc =   ( Math.pow(p1.x,2) + Math.pow(p1.y,2) - offset )/2.0;
        final double cd =   (offset - Math.pow(p3.x, 2) - Math.pow(p3.y, 2))/2.0;
        final double det =  (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x)* (p1.y - p2.y);

        if (Math.abs(det) < TOL) { throw new IllegalArgumentException("Yeah, lazy."); }

        final double idet = 1/det;

        final double centerx =  (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
        final double centery =  (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
        final double radius =
                Math.sqrt( Math.pow(p2.x - centerx,2) + Math.pow(p2.y-centery,2));

        return new Circle(new Point(centerx,centery),radius);
    }

    static class Circle
    {
        final Point center;
        final double radius;
        public Circle(Point center, double radius)
        {
            this.center = center; this.radius = radius;
        }
        @Override
        public String toString()
        {
            return new StringBuilder().append("Center= ").append(center).append(", r=").append(radius).toString();
        }
    }

    static class Point
    {
        double x,y;

        public Point(double x, double y)
        {
            this.x = x; this.y = y;
        }
        @Override
        public String toString()
        {
            return "("+x+","+y+")";
        }

    }

    class Point1 {
        float x, y, rot;
        public Point1(float x, float y, float rot) {
            this.x = x;
            this.y=y;
            this.rot = rot;
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

        float relAngle = 0;
        float oldAngleDesired = 0;

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

        private void updatePos() {
            this.x += vel * cos(rz);
            this.y += vel * sin(rz);

            float d_rz = tan(delta) * vel / WIDTH;
            this.rz += d_rz; //θ1

            old1=old0;
            old0 = new Point(xTrailer, yTrailer);
            this.xTrailer += vel * cos(rz - theta) * cos(theta);
            this.yTrailer += vel * cos(rz - theta) * sin(theta);

            float d_theta = vel * sin(rz - theta) / TRAILER_WIDTH; //θ2
            this.theta += d_theta;
        }

        Point old0 = new Point(0,0);
        Point old1 = new Point(0, 0);
        private void updateSteering() {
            Point1 ref = new Point1(1200, 500, 0);
            float ex = ref.x - xTrailer;
            float ey = ref.y - yTrailer;

            float ex1 = ex*cos(theta)+ey*sin(theta);
            float ey1 = ey*cos(theta)-ex*sin(theta);

            float thetaerr = 0;
            if(abs(ref.rot - theta) <= PI)
                thetaerr = ref.rot - theta;
            else
                thetaerr = ref.rot - theta - 2*PI*Math.signum(ref.rot - theta);

            Circle c = circleFromPoints(new Point(xTrailer,yTrailer), old0, old1);
            Circle c1 = circleFromPoints(new Point(xTrailer, yTrailer), old0, new Point(1200,500));
            float curveerr = 1.0f/(float)c1.radius - 1.0f/(float)c.radius;

            float angle = atan2(500 - yTrailer, 1200 - xTrailer);
            float ref1 = -(rz - angle);

            ref1= ey1 - thetaerr + 2*curveerr;

            int k = 1;
            float target = atan2(WIDTH/vel*(vel/TRAILER_WIDTH*sin(rz - theta) + k*(ref1 - (rz - theta))), 1);
            delta = max(-15f*PI/180, min(15f*PI/180, target));

            //System.out.println(thetaerr*180/PI );
            System.out.println(c.radius);
        }

        void update() {
            //updatePos();
            //updateSteering();

            this.x += vel * cos(rz);
            this.y += vel * sin(rz);
            float d_rz = tan(delta) * vel / WIDTH;;
            this.rz += d_rz;

            this.xTrailer += vel * cos(rz - theta) * cos(theta);
            this.yTrailer += vel * cos(rz - theta) * sin(theta);

            float d_theta = vel * sin(rz - theta) / TRAILER_WIDTH;
            this.theta += d_theta;

            float angle = atan2(300 - yTrailer, 1200 - xTrailer);
            float error = angle - theta + PI; // reverse

            float angleDesired = -(-angle-error);
            float d_angleDesired = (angleDesired - oldAngleDesired);

            oldAngleDesired = angleDesired;

            float deltaTarget = -atan2((d_theta - d_angleDesired) * WIDTH, vel);
            if(deltaTarget < delta) {
                delta += max(-0.01f, deltaTarget - delta);
            }
            else {
                delta += min(0.01f, deltaTarget - delta);
            }
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
            popMatrix();
        }
    }

    public void settings(){
        size(1200, 800);
    }

    Car car = new Car(300, 550, -15*PI/180+PI, -2f); // reverse
    //Car car = new Car(0, 100, 45*PI/180, 2f); // forward

    public void draw() {
        background(0);
        stroke(255,0,0);
        line(0,300,1200,300);

        //float angle = atan2(100 - car.yTrailer, 1200 - car.xTrailer);
        //float error = angle - car.theta + PI; // reverse
        //float error = angle - car.rz; // forward
        //car.setAngle(-angle - error); // reverse
        //car.setAngle(angle + error); // forward

        noStroke();
        if (keyCode == ENTER) {
            car.update();
        }

        car.draw();
    }

    public static void main(String... args) {
        Point p1 = new Point(0.0,1.0);
        Point p2 = new Point(1.0,0.0);
        Point p3 = new Point(5.0,1.0);
        Circle c = circleFromPoints(p1, p2, p3);
        System.out.println(c.radius);

        PApplet.main("main");
    }
}