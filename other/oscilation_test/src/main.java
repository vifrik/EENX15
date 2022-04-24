import processing.core.PApplet;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class main extends PApplet{
    public class OptionManager {
        enum Option {
            LOOKAHEAD,
            ANGULAR_VELOCITY,
            MAX_ANGLE,
            STEPS,
            TRAILER_LENGTH,
            TRUCK_LENGTH,
            TRUCK_X,
            TRUCK_Y,
            TRUCK_ANGLE,
            TRUCK_VELOCITY,
            REROLL_COLOR,
            CHANGE_PATH
        }

        class PathOptionData {
            enum PathType {
                SINE,
                STRAIGHT
            }

            float lookahead = 80.0f;
            float angularVelocity = 0.25f;
            int steps = 200;
            float angleMax = 30.0f;
            float trailerLength = 100.0f;
            float truckLength = 12.0f;
            float truckX = 0.0f;
            float truckY = 300.0f;
            float truckAngle = 15.0f;
            float truckVelocity = -2.0f;
            float r, g, b;
            PathType pathType = PathType.SINE;

            void rollColor() {
                r = random(255);
                g = random(255);
                b = random(255);
            }

            public PathOptionData() {
                rollColor();
            }
        }

        List<PathOptionData> pathOptionDatas = new ArrayList<>();
        Map<Option, String> optionNames = new HashMap<>();

        Option selectedOption;
        int selectedIndex;

        boolean editEnabled;
        String inputString = "";

        public OptionManager() {
            selectedOption = Option.LOOKAHEAD;
            selectedIndex = 0;

            optionNames.put(Option.LOOKAHEAD, "[d] Lookahead");
            optionNames.put(Option.ANGULAR_VELOCITY, "[w] Angular velocity");
            optionNames.put(Option.STEPS, "[s] Steps");
            optionNames.put(Option.MAX_ANGLE, "[a] Max angle");
            optionNames.put(Option.TRAILER_LENGTH, "[l] Trailer length");
            optionNames.put(Option.TRUCK_LENGTH, "[t] Truck length");
            optionNames.put(Option.TRUCK_X, "[x] Truck x");
            optionNames.put(Option.TRUCK_Y, "[y] Truck y");
            optionNames.put(Option.TRUCK_ANGLE, "[o] Truck angle");
            optionNames.put(Option.TRUCK_VELOCITY, "[v] Truck velocity");
            optionNames.put(Option.REROLL_COLOR, "[c] Reroll color");
            optionNames.put(Option.CHANGE_PATH, "[p] Change path");

            addNewOption();
        }

        public void addNewOption() {
            PathOptionData pathOptionData = new PathOptionData();
            pathOptionDatas.add(pathOptionData);
            selectedIndex = pathOptionDatas.size() - 1;
        }

        public void removeCurrentOption() {
            if (pathOptionDatas.size() == 0) return;

            if (selectedIndex == 0) {
                pathOptionDatas.remove(0);
            } else {
                pathOptionDatas.remove(selectedIndex);
                selectedIndex--;
            }
        }

        public String optionToString(Option option) {
            switch (option) {
                case LOOKAHEAD -> {return  optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).lookahead;}
                case ANGULAR_VELOCITY -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).angularVelocity;}
                case STEPS -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).steps;}
                case MAX_ANGLE -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).angleMax;}
                case TRAILER_LENGTH -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).trailerLength;}
                case TRUCK_LENGTH -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).truckLength;}
                case TRUCK_X -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).truckX;}
                case TRUCK_Y -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).truckY;}
                case TRUCK_ANGLE -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).truckAngle;}
                case TRUCK_VELOCITY -> {return optionNames.get(option) + ": " + pathOptionDatas.get(selectedIndex).truckVelocity;}
                case REROLL_COLOR, CHANGE_PATH -> {return optionNames.get(option);}
            }
            return "Unknown";
        }

        public void parseString() {
            try {
                if (selectedOption == Option.ANGULAR_VELOCITY) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).angularVelocity = parsed;
                } else if (selectedOption == Option.LOOKAHEAD) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).lookahead = parsed;
                } else if (selectedOption == Option.STEPS) {
                    int parsed = Integer.parseInt(inputString.strip());
                    pathOptionDatas.get(selectedIndex).steps = parsed;
                } else if (selectedOption == Option.MAX_ANGLE) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).angleMax = parsed;
                } else if (selectedOption == Option.TRAILER_LENGTH) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).trailerLength = parsed;
                } else if (selectedOption == Option.TRUCK_LENGTH) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).truckLength = parsed;
                } else if (selectedOption == Option.TRUCK_X) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).truckX = parsed;
                } else if (selectedOption == Option.TRUCK_Y) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).truckY = parsed;
                } else if (selectedOption == Option.TRUCK_ANGLE) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).truckAngle = parsed;
                } else if (selectedOption == Option.TRUCK_VELOCITY) {
                    float parsed = Float.parseFloat(inputString);
                    pathOptionDatas.get(selectedIndex).truckVelocity = parsed;
                }
            } catch (Exception e) {
                System.out.println("Error parsing value");
            }

        }

        public void draw() {
            pushMatrix();
            textSize(16);

            translate(0, height - 100);
            int xOffset = 0;
            int yOffset = 0;

            for (Map.Entry<Option, String> pair : optionNames.entrySet()) {
                pushStyle();
                if (selectedOption == pair.getKey()) {
                    fill(0,255,0);
                }
                String optionString = optionToString(pair.getKey());
                text(optionString, xOffset, yOffset);
                //xOffset += textWidth(optionString) + 20;
                xOffset += 200;
                if (xOffset > width - 200) {
                    xOffset = 0;
                    yOffset += 30;
                }
                popStyle();
            }
            xOffset = 0;
            yOffset += 30;

            String pathString = "Path: ";
            text(pathString, 0, yOffset);
            xOffset += textWidth(pathString) + 20;

            for (int i = 0; i < pathOptionDatas.size(); i++) {
                pushStyle();
                if (selectedIndex == i) {
                    fill(0,255,0);
                    text("*", xOffset, yOffset);
                    xOffset += 8;
                }
                fill(pathOptionDatas.get(i).r, pathOptionDatas.get(i).g, pathOptionDatas.get(i).b);
                text(i, xOffset, yOffset);
                xOffset += 20;
                popStyle();
            }

            String infoString = "<enter> to modify, <n> to create new path, <m> to delete selected path";
            text(infoString, xOffset, yOffset);
            xOffset += textWidth(infoString) + 20;

            if (editEnabled) {
                pushStyle();
                fill(0,255,0);
                text("Input: " + inputString, xOffset, yOffset);
                popStyle();
            }

            popMatrix();
        }
    }

    class Coord {
        float x, y;

        public Coord(float x, float y) {
            this.x = x;
            this.y = y;
        }
    }

    class PPC {
        List<Integer> targetCandidates = new ArrayList<>();
        List<Coord> path;

        int index = 0;
        boolean fromNegative = false;
        private static final int MAX_STEPS = 10000;

        static double calculateDistance(Coord c1, Coord c2) {
            return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
        }

        public PPC(List<Coord> path) {
            this.path = path;
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

        float lookahead;
        float angularVelocity;
        float angleMax;
        float trailerLength;
        float truckLength;

        PPC ppc;
        Coord target = new Coord(0,0);

        public Car(float x, float y, float rz, float vel, float lookahead, float angularVelocity, List<Coord> path, float angleMax, float trailerLength, float truckLength) {
            this.x = x;
            this.y = y;
            this.rz = rz;
            this.vel = vel;
            this.lookahead = lookahead;
            this.angularVelocity = angularVelocity;
            this.angleMax = angleMax;
            this.trailerLength = trailerLength;
            this.truckLength = truckLength;

            rz = rz - PI;
            xTrailer = x + cos(rz) * trailerLength;
            yTrailer = y + sin(rz) * trailerLength;
            theta = this.rz;

            ppc = new PPC(path);
        }

        float sumError = 0;
        float k_i = 1.0f;
        float k_p = 2.0f;
        float k_d = 1.0f;
        float oldErr = 0;

        void update() {
            if (!shouldUpdate) return;

            this.x += vel * cos(rz);
            this.y += vel * sin(rz);
            float d_rz = tan(delta) * vel / truckLength;
            this.rz += d_rz;

            Coord trailerPosition = new Coord(xTrailer, yTrailer);
            target = ppc.getTarget(lookahead, trailerPosition);
            if (ppc.atEnd(lookahead, trailerPosition, target)) {
                shouldUpdate = false;
                return;
            }

            this.xTrailer += vel * cos(rz - theta) * cos(theta);
            this.yTrailer += vel * cos(rz - theta) * sin(theta);

            float d_theta = vel * sin(rz - theta) / trailerLength;
            this.theta += d_theta;

            angle = atan2(target.y - yTrailer, target.x - xTrailer);
            error = angle - theta + PI;

            sumError += k_i * error / 100;
            float d_error = error - oldErr;

            oldErr = error;

            float angleDesired = k_p * error + k_i * sumError + k_d * d_error;
            float d_angleDesired = (angleDesired - oldAngleDesired);

            oldAngleDesired = angleDesired;

            float deltaTarget = -atan2((d_theta - d_angleDesired - d_rz) * truckLength, vel);

            delta += max(-angularVelocity, min(angularVelocity, deltaTarget - delta));
            delta = max(-angleMax*PI/180, min(angleMax*PI/180, delta));
        }
    }

    class PositionData {
        Coord trailer;
        Coord truck;

        public PositionData(Coord trailer, Coord truck) {
            this.trailer = trailer;
            this.truck = truck;
        }
    }

    OptionManager optionManager = new OptionManager();
    List<List<PositionData>> listOfPaths = new ArrayList<>();
    List<Coord> refPath = new ArrayList<>();

    public void setSinePath() {
        refPath.clear();
        final float points = 1000;
        float resolution = 1200 / points;
        for (int i = 0; i < 1000; i++) {
            Coord position = new Coord(resolution * i, 50.0f * sin(i / 600.0f * 2 * PI) + 300.0f);
            refPath.add(position);
        }
    }

    public void setStraightPath() {
        refPath.clear();
        final float points = 1000;
        float resolution = 1200 / points;
        for (int i = 0; i < 1000; i++) {
            Coord position = new Coord(resolution * i, 300.0f);
            refPath.add(position);
        }
    }

    public void settings(){
        size(1200, 800);
        smooth(2);

        setSinePath();

        cachePath(0, 200);
    }

    void cachePath(int pathIndex, int duration) {
        OptionManager.PathOptionData pathOptionData = optionManager.pathOptionDatas.get(optionManager.selectedIndex);
        while (listOfPaths.size() < optionManager.pathOptionDatas.size()) {
            listOfPaths.add(new ArrayList<>());
        }

        listOfPaths.get(pathIndex).clear();

        Car car = new Car(pathOptionData.truckX, pathOptionData.truckY, pathOptionData.truckAngle*PI/180+PI,
                pathOptionData.truckVelocity, pathOptionData.lookahead, pathOptionData.angularVelocity, refPath,
                pathOptionData.angleMax, pathOptionData.trailerLength, pathOptionData.truckLength);

        for (int i = 0; i < duration; i++) {
            listOfPaths.get(pathIndex).add(new PositionData(
                    new Coord(car.xTrailer, car.yTrailer),
                    new Coord(car.x, car.y)
            ));
            car.update();
        }
    }

    void drawPaths() {
        final int roadWidth = 80;
        final int roadThickness = 2;

        pushStyle();
//        for (Coord coord : refPath) {
//            fill(255,255,255);
//            circle(coord.x, coord.y, roadWidth);
//        }
//
//        for (Coord coord : refPath) {
//            fill(30);
//            circle(coord.x, coord.y, roadWidth - roadThickness);
//        }

        for (Coord coord : refPath) {
            fill(230);
            circle(coord.x, coord.y, roadThickness);
        }
        popStyle();

        for (int i = 0; i < listOfPaths.size(); i++) {
            OptionManager.PathOptionData pathOptionData = optionManager.pathOptionDatas.get(i);
            pushStyle();
            noFill();
            stroke(pathOptionData.r * 0.5f,
                    pathOptionData.g * 0.5f,
                    pathOptionData.b * 0.5f);
            circle(pathOptionData.truckX, pathOptionData.truckY, pathOptionData.lookahead * 2);
            noStroke();
            popStyle();

            for (PositionData positionData : listOfPaths.get(i)) {
                pushStyle();
                fill(pathOptionData.r * 0.5f,
                        pathOptionData.g * 0.5f,
                        pathOptionData.b * 0.5f);
                circle(positionData.truck.x, positionData.truck.y, 2);

                float width = i == optionManager.selectedIndex ? 8 : 3;

                fill(pathOptionData.r,
                        pathOptionData.g,
                        pathOptionData.b);
                circle(positionData.trailer.x, positionData.trailer.y, width);
                popStyle();
            }
        }
    }

    public void keyPressed() {
        OptionManager.PathOptionData pathOptionData = optionManager.pathOptionDatas.get(optionManager.selectedIndex);

        if (key == '\n') {
            if (optionManager.editEnabled) {
                optionManager.editEnabled = false;
                optionManager.parseString();
                optionManager.inputString = "";
            } else {
                optionManager.editEnabled = true;
            }
        }
        if (optionManager.editEnabled && key != '\n')
            optionManager.inputString += (char) key;
        else if (key == 'd') {
            optionManager.selectedOption = OptionManager.Option.LOOKAHEAD;
        }
        else if (key == 'w') {
            optionManager.selectedOption = OptionManager.Option.ANGULAR_VELOCITY;
        }
        else if (key == 's') {
            optionManager.selectedOption = OptionManager.Option.STEPS;
        }
        else if (key == 'a') {
            optionManager.selectedOption = OptionManager.Option.MAX_ANGLE;
        }
        else if (key == 'l') {
            optionManager.selectedOption = OptionManager.Option.TRAILER_LENGTH;
        }
        else if (key == 't') {
            optionManager.selectedOption = OptionManager.Option.TRUCK_LENGTH;
        }
        else if (key == 'x') {
            optionManager.selectedOption = OptionManager.Option.TRUCK_X;
        }
        else if (key == 'y') {
            optionManager.selectedOption = OptionManager.Option.TRUCK_Y;
        }
        else if (key == 'o') {
            optionManager.selectedOption = OptionManager.Option.TRUCK_ANGLE;
        }
        else if (key == 'c') {
            optionManager.pathOptionDatas.get(optionManager.selectedIndex).rollColor();
        }
        else if (key == 'v') {
            optionManager.selectedOption = OptionManager.Option.TRUCK_VELOCITY;
        }
        else if (key == 'p') {
            if (pathOptionData.pathType == OptionManager.PathOptionData.PathType.SINE) {
                setStraightPath();
                pathOptionData.pathType = OptionManager.PathOptionData.PathType.STRAIGHT;
            } else if (pathOptionData.pathType == OptionManager.PathOptionData.PathType.STRAIGHT) {
                setSinePath();
                pathOptionData.pathType = OptionManager.PathOptionData.PathType.SINE;
            }
        }
        else if (key == 'n') {
            optionManager.addNewOption();
        }
        else if (key == 'm') {
            listOfPaths.remove(optionManager.selectedIndex);
            optionManager.removeCurrentOption();
        } else {
            for (int i = 0; i <= 9; i++) {
                if (key == i + 48) {
                    if (optionManager.pathOptionDatas.size() > i)
                        optionManager.selectedIndex = i;
                }
            }
        }

        cachePath(optionManager.selectedIndex, optionManager.pathOptionDatas.get(optionManager.selectedIndex).steps);
    }

    public void draw() {
        background(30);
        noStroke();
        optionManager.draw();
        drawPaths();
    }

    public static void main(String... args) {
        PApplet.main("main");
    }
}