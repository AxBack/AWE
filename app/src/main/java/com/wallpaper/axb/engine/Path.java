package com.wallpaper.axb.engine;

import android.util.Log;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Path<T extends Floats> {
    class Part {
        float time;
        List<Floats> points = new ArrayList<>();

        public Part(float time, Floats[] points) {
            this.time = time;
            this.points = Arrays.asList(points);
        }
    }

    List<Part> parts = new ArrayList<>();

    Class<T> clazz;

    public Path(Class<T> clazz) { this.clazz = clazz; }

    private Floats newFloatInstance() {
        try {
            return clazz.newInstance();
        } catch (IllegalAccessException | InstantiationException e) {
            Log.e("Path", "Could not instantiate Floats");
        }
        return null;
    }

    void add(float time, Floats[] points) {
        parts.add(new Part(time, points));
    }

    void clear() {
        parts.clear();
    }

    public void write(DataOutputStream stream) throws IOException {
        stream.writeInt(parts.size()); // nr parts

        for (Part p : parts) {
            stream.writeFloat(p.time);
            stream.writeInt(p.points.size());
            for (Floats f : p.points)
                f.write(stream);
        }
    }

    public <T extends Floats> void randomize(T min, T max, int minNrPoints, int maxNrPoints) {
        parts.clear();

        Random random = new Random();

        int nr = Math.abs(random.nextInt() % maxNrPoints) + minNrPoints;
        Floats[] points = new Floats[nr];
        for(int i=0; i < nr; ++i) {
            points[i] = newFloatInstance();
            points[i].randomize(random, min, max);
        }
        add(1.0f, points);
    }
}
