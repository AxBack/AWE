package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;

/**
 * Created by AxB on 2/24/2018.
 */

public class ClusterState {
    public Path<Floats.Float3> position = new Path<>(Floats.Float3.class);
    public Path<Floats.Float3> rotation = new Path<>(Floats.Float3.class);
    public Path<Floats.Float> offset = new Path<>(Floats.Float.class);
    public Path<Floats.Float> spread = new Path<>(Floats.Float.class);
    public Path<Floats.Float> yaw = new Path<>(Floats.Float.class);
    public Path<Floats.Float> pitch = new Path<>(Floats.Float.class);
    public Path<Floats.Float3> color = new Path<>(Floats.Float3.class);
    public Path<Floats.Float> size = new Path<>(Floats.Float.class);

    public ClusterState() {

    }

    public void write(DataOutputStream stream) throws IOException {

        // position
        position.write(stream);

        // rotation
        rotation.write(stream);

        // offset
        offset.write(stream);

        // spread
        spread.write(stream);

        // yaw spread
        yaw.write(stream);

        // pitch spread
        pitch.write(stream);

        // color
        color.write(stream);

        // size
        size.write(stream);
    }

    static ClusterState createRandomState() {

        ClusterState state = new ClusterState();
        state.position.randomize(new Floats.Float3(-20, -20, -20), new Floats.Float3(20,20,20), 1, 3);
        state.rotation.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(360,360,360), 2, 5);
        state.offset.randomize(new Floats.Float(0), new Floats.Float(200), 1, 3);
        state.spread.randomize(new Floats.Float(10), new Floats.Float(100), 1, 3);
        state.color.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(1,1,1), 2, 4);
        state.size.randomize(new Floats.Float(0.5f), new Floats.Float(5f), 2, 4);
        state.yaw.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        state.pitch.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        return state;
    }
}
