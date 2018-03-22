package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Random;

public abstract class Floats {

    public abstract void write(DataOutputStream stream) throws IOException;
    public abstract void randomize(Random random, Floats min, Floats max);
    public abstract void write(List<java.lang.Float> floats);

    public static class Float extends Floats {
        public float v;

        public Float() {this(0); }
        public Float(float v) {
            this.v = v;
        }

        @Override
        public void write(DataOutputStream stream) throws IOException {
            stream.writeFloat(v);
        }

        @Override
        public void write(List<java.lang.Float> floats) {
            floats.add(v);
        }

        @Override
        public void randomize(Random random, Floats min, Floats max) {
            v = ((Floats.Float)min).v + (random.nextFloat() * (((Floats.Float)max).v- ((Floats.Float)min).v));
        }
    }

    public static class Float3 extends Floats {
        public float x;
        public float y;
        public float z;

        public Float3() {
            this(0,0,0);
        }

        public Float3(float x, float y, float z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        @Override
        public void write(DataOutputStream stream) throws IOException {
            stream.writeFloat(x);
            stream.writeFloat(y);
            stream.writeFloat(z);
        }

        @Override
        public void write(List<java.lang.Float> floats) {
            floats.add(x);
            floats.add(y);
            floats.add(z);
        }

        @Override
        public void randomize(Random random, Floats min, Floats max) {
            x = ((Floats.Float3)min).x + (random.nextFloat() * (((Floats.Float3)max).x - ((Floats.Float3)min).x));
            y = ((Floats.Float3)min).y + (random.nextFloat() * (((Floats.Float3)max).y - ((Floats.Float3)min).y));
            z = ((Floats.Float3)min).z + (random.nextFloat() * (((Floats.Float3)max).z - ((Floats.Float3)min).z));
        }
    }
}
