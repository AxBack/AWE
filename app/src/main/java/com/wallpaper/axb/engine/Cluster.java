package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by AxB on 1/25/2018.
 */

public class Cluster {

    List<State> mStates = new ArrayList<>();

    public void write(DataOutputStream stream) throws IOException {

        stream.writeFloat(0); // x
        stream.writeFloat(0); // y
        stream.writeFloat(0); // y

        stream.writeFloat(0); // yaw
        stream.writeFloat(0); // pitch
        stream.writeFloat(0); // roll

        // charge
        writePath(stream, new float[]{1.0f}, new float[][]{{0,0.2f, 1.0f}});

        // nr nodes
        stream.writeInt(1000);

        mStates.add(new State());

        // nr states
        stream.writeInt(mStates.size());

        for(State s : mStates)
            s.write(stream);
    }

    private static void writePath(DataOutputStream stream, float[] times, float[][] parts) throws IOException {
        stream.writeInt(parts.length); // nr parts

        for(int i=0; i<parts.length; ++i) {
            stream.writeFloat(times[0]);
            stream.writeInt(parts[i].length);
            for(float f : parts[i])
                stream.writeFloat(f);
        }
    }

    private static void writePath(DataOutputStream stream, float[] times, Float3[][] parts) throws IOException {
        stream.writeInt(parts.length); // nr parts

        for(int i=0; i<parts.length; ++i) {
            stream.writeFloat(times[0]);
            stream.writeInt(parts[i].length);
            for(Float3 f : parts[i])
                f.write(stream);
        }
    }

    class Float3 {
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

    class State {

        public void write(DataOutputStream stream) throws IOException {
            // position
            writePath(stream, new float[]{1.0f}, new Float3[][] {{new Float3(0,0,0)}});

            // rotation
            writePath(stream, new float[]{1.0f}, new Float3[][] {{new Float3(0,0,0)}});

            // max offset
            writePath(stream, new float[]{1.0f}, new float[][]{{0,-20,-150}});

            // max offset
            writePath(stream, new float[]{1.0f}, new float[][]{{0,20,150}});

            // color
            writePath(stream, new float[]{1.0f}, new Float3[][] {{new Float3(1,0.2f,0), new Float3(0,0.2f, 1)}});

            // size
            writePath(stream, new float[] {1.0f }, new float[][] {{2,2,3}});

            // yaw spread
            writePath(stream, new float[]{1.0f}, new float[][]{{90.0f}});

            // pitch spread
            writePath(stream, new float[]{1.0f}, new float[][]{{180.0f}});
        }
    }
}
