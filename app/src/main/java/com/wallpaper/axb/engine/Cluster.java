package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Cluster {

    List<State> mStates = new ArrayList<>();

    Path<Float> chargePath = new Path<>();

    int nrNodes = 1000;

    public Cluster() {
        //temp
        randomize(chargePath, 0.0f, 0.8f);

        Random random = new Random();
        int i = Math.abs(random.nextInt() % 10) + 10;

        for(int x=0; x < i; ++x)
            mStates.add(new State());
    }

    public void write(DataOutputStream stream) throws IOException {

        stream.writeFloat(0); // x
        stream.writeFloat(0); // y
        stream.writeFloat(0); // y

        stream.writeFloat(0); // yaw
        stream.writeFloat(0); // pitch
        stream.writeFloat(0); // roll

        // charge
        writePath(stream, chargePath);

        // nr nodes
        stream.writeInt(nrNodes);

        // nr states
        stream.writeInt(mStates.size());

        for (State s : mStates)
            s.write(stream);
    }

    private static void writePath(DataOutputStream stream, Path<Float> path) throws IOException {
        stream.writeInt(path.parts.size()); // nr parts

        for (Path<Float>.Part<Float> p : path.parts) {
            stream.writeFloat(p.time);
            stream.writeInt(p.points.size());
            for (float f : p.points)
                stream.writeFloat(f);
        }
    }

    private static void writePath3(DataOutputStream stream, Path<Float3> path) throws IOException {
        stream.writeInt(path.parts.size()); // nr parts

        for (Path<Float3>.Part<Float3> p : path.parts) {
            stream.writeFloat(p.time);
            stream.writeInt(p.points.size());
            for (Float3 f : p.points)
                f.write(stream);
        }
    }

    static void randomize(Path<Float> path, float min, float max) {
        path.clear();

        Random random = new Random();

        int nr = Math.abs(random.nextInt() % 3) + 1;
        Float[] points = new Float[nr];
        for(int i=0; i < nr; ++i) {
            points[i] = min + (random.nextFloat() * (max - min));
        }
        path.add(1.0f, points);
    }

    static void randomize(Path<Float3> path, Float3 min, Float3 max) {
        path.clear();

        Random random = new Random();

        int nr = Math.abs(random.nextInt() % 3) + 1;
        Float3[] points = new Float3[nr];
        for(int i=0; i < nr; ++i) {
            points[i] = new Float3(
                    min.x + (random.nextFloat() * (max.x - min.x)),
                    min.y + (random.nextFloat() * (max.y - min.y)),
                    min.z + (random.nextFloat() * (max.z - min.z))
            );
        }
        path.add(1.0f, points);
    }

    static class Float3 {
        public float x;
        public float y;
        public float z;

        public Float3(float x, float y, float z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public void write(DataOutputStream stream) throws IOException {
            stream.writeFloat(x);
            stream.writeFloat(y);
            stream.writeFloat(z);
        }
    }

    class Path<T> {

        class Part<T> {
            float time;
            List<T> points = new ArrayList<>();

            public Part(float time, T[] points) {
                this.time = time;
                this.points = Arrays.asList(points);
            }
        }

        List<Part<T>> parts = new ArrayList<>();

        void add(float time, T[] points) {
            parts.add(new Part<T>(time, points));
        }

        void clear() {
            parts.clear();
        }
    }

    class State {

        public Path<Float3> position = new Path<>();
        public Path<Float3> rotation = new Path<>();
        public Path<Float> minOffset = new Path<>();
        public Path<Float>  maxOffset = new Path<>();
        public Path<Float3> color = new Path<>();
        public Path<Float> size = new Path<>();
        public Path<Float> yaw = new Path<>();
        public Path<Float> pitch = new Path<>();

        public State() {
            //temp
            randomize(position, new Float3(-50, -50, -50), new Float3(50,50,50));
            randomize(rotation, new Float3(0, 0, 0), new Float3(360,360,360));
            randomize(minOffset, -300, 0);
            randomize(maxOffset, 0, 300);
            randomize(color, new Float3(0, 0, 0), new Float3(1,1,1));
            randomize(size, 0.5f, 5f);
            randomize(yaw, 0, 180);
            randomize(pitch, 0, 180);
        }

        public void write(DataOutputStream stream) throws IOException {

            //Temp
            Random random = new Random();

            // position
            writePath3(stream, position);

            // rotation
            writePath3(stream, rotation);

            // max offset
            writePath(stream, minOffset);

            // max offset
            writePath(stream, maxOffset);

            // color
            writePath3(stream, color);

            // size
            writePath(stream, size);

            // yaw spread
            writePath(stream, yaw);

            // pitch spread
            writePath(stream, pitch);
        }
    }
}
