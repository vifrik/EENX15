import processing.core.PApplet;
import processing.core.net;

public class ArUco extends PApplet {
    Client client;

    @Override
    public void settings() {

    }

    @Override
    public void setup() {
        size(800, 800);

        // Setup HTTP client
        client = new Client(this, "127.0.0.1", 50007);
    }

    @Override
    public void draw() {

    }

    public static void main(String[] args) {
        PApplet.main("ArUco");
    }
}

