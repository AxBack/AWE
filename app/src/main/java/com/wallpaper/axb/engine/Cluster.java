package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Cluster {

    List<ClusterState> mStates = new ArrayList<>();

    Path<Floats.Float> mChargePath = new Path<>(Floats.Float.class);

    float mSwitchInterval = 30.0f;
    float mTransitionTime = 1.0f;

    int mNrNodes = 500;

    static Cluster createRandom(int nrNodes, int nrStates) {
        Cluster cluster = new Cluster(nrNodes);
        Random random = new Random();
        cluster.mSwitchInterval = 30.0f + (random.nextFloat() * 30.0f);

        for(int x=0; x < nrStates; ++x) {
            cluster.mStates.add(ClusterState.createRandomState());
        }

        return cluster;
    }

    public Cluster(int nrNodes) {
        mNrNodes = nrNodes;

        Floats.Float[] points = {new Floats.Float(0.0f), new Floats.Float(1.0f) };
        mChargePath.add(1.0f, points);
    }

    public void add(ClusterState state) {
        mStates.add(state);
    }

    public void write(DataOutputStream stream) throws IOException {

        stream.writeFloat(0); // x
        stream.writeFloat(0); // y
        stream.writeFloat(0); // y

        stream.writeFloat(0); // yaw
        stream.writeFloat(0); // pitch
        stream.writeFloat(0); // roll

        stream.writeFloat(mSwitchInterval);
        stream.writeFloat(mTransitionTime);

        // charge
        mChargePath.write(stream);

        // nr nodes
        stream.writeInt(mNrNodes);

        // nr states
        stream.writeInt(mStates.size());

        for (ClusterState s : mStates)
            s.write(stream);
    }
}
