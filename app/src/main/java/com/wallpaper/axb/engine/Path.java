package com.wallpaper.axb.engine;

import android.util.Log;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Path<T extends Floats> {

    public interface OnPathChangedListener {
        void onPathChanged(Path path);
    }

    class Part {
        float time;
        List<Floats> points = new ArrayList<>();

        public Part(float time, Floats[] points) {
            this.time = time;
            this.points = Arrays.asList(points);
        }
    }

    OnPathChangedListener mOnPathChangedListener;

    List<Part> parts = new ArrayList<>();
    Class<T> clazz;
    Object tag;

    public Path(Class<T> clazz, OnPathChangedListener listener, Object tag) {
        this.clazz = clazz;
        mOnPathChangedListener = listener;
        this.tag = tag;
    }

    private Floats newFloatInstance() {
        try {
            return clazz.newInstance();
        } catch (IllegalAccessException | InstantiationException e) {
            Log.e("Path", "Could not instantiate Floats");
        }
        return null;
    }

    void add(float time, Floats[] points, boolean quiet) {
        parts.add(new Part(time, points));
        if (!quiet && mOnPathChangedListener != null) {
            mOnPathChangedListener.onPathChanged(this);
        }
    }

    void add(float time, Floats[] points) {
        add(time, points, true);
    }

    void clear() {
        parts.clear();
    }

    float getTotal() {
        float total = 0.0f;
        for(Part p : parts)
            total += p.time;
        return total;
    }

    public void write(DataOutputStream stream) throws IOException {
        float total = getTotal();

        stream.writeInt(parts.size()); // nr parts

        for (Part p : parts) {
            stream.writeFloat(total == 0 ? p.time : p.time / total);
            stream.writeInt(p.points.size());
            for (Floats f : p.points)
                f.write(stream);
        }
    }

    public void write(List<Float> floats) {
        float total = getTotal();

        floats.add((float)parts.size());
        for(Part p : parts) {
            floats.add(total == 0 ? p.time : p.time / total);
            floats.add((float)p.points.size());
            for(Floats f: p.points) {
                f.write(floats);
            }
        }
    }

    public void randomize(T min, T max, int minNrPoints, int maxNrPoints) {
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
