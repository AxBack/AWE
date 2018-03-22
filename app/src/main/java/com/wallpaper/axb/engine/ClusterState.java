package com.wallpaper.axb.engine;

import java.io.DataOutputStream;
import java.io.IOException;

/**
 * A state for a Cluster.
 *
 * Each state consists of different paths of bezier curves (https://en.wikipedia.org/wiki/Bezier_curve)
 * of either Float or Float3.
 *
 * Path
 *      Curve
 *          Point, Point, Point, Point
 *      Curve
 *          Point, Point
 *      Curve
 *          Point, Point
 *
 * Each curve has a length which in the export stage will be normalized so that a value can
 * be extracted by using 0 - 1 as inputs.
 *
 *  Curve1 = 2.0f
 *  Curve2 = 3.0f
 *  Curve3 = 5.0f
 *  = Curve1(0.0f - 0.2f), Curve2(0.2f - 0.5f), Curve3(0.5f - 1.0f)
 *
 */
public class ClusterState implements Path.OnPathChangedListener{

    enum Type {
        POSITION(1), ROTATION(2), OFFSET(3), SPREAD(4), YAW(5), PITCH(6), COLOR(7), SIZE(8);

        final int id;

        Type(int id) { this.id = id; }
    }

    public String id;
    protected Path<Floats.Float3> mPosition = new Path<>(Floats.Float3.class, this, Type.POSITION);
    protected Path<Floats.Float3> mRotation = new Path<>(Floats.Float3.class, this, Type.ROTATION);
    protected Path<Floats.Float> mOffset = new Path<>(Floats.Float.class, this, Type.OFFSET);
    protected Path<Floats.Float> mSpread = new Path<>(Floats.Float.class, this, Type.SPREAD);
    protected Path<Floats.Float> mYaw = new Path<>(Floats.Float.class, this, Type.YAW);
    protected Path<Floats.Float> mPitch = new Path<>(Floats.Float.class, this, Type.PITCH);
    protected Path<Floats.Float3> mColor = new Path<>(Floats.Float3.class, this, Type.COLOR);
    protected Path<Floats.Float> mSize = new Path<>(Floats.Float.class, this, Type.SIZE);

    public ClusterState() {

    }

    public void write(DataOutputStream stream) throws IOException {

        // mPosition
        mPosition.write(stream);

        // mRotation
        mRotation.write(stream);

        // mOffset
        mOffset.write(stream);

        // mSpread
        mSpread.write(stream);

        // mYaw mSpread
        mYaw.write(stream);

        // mPitch mSpread
        mPitch.write(stream);

        // mColor
        mColor.write(stream);

        // mSize
        mSize.write(stream);
    }

    public void clear() {
        mPosition.clear();
        mRotation.clear();
        mOffset.clear();
        mSpread.clear();
        mYaw.clear();
        mPitch.clear();
        mColor.clear();
        mSize.clear();
    }

    protected void onInvalidated() {
    }

    @Override
    public void onPathChanged(Path path) {
        onInvalidated();
    }

    /*
    public void setPosition(Path<Floats.Float3> path) {this.mPosition = path; }
    public void setRotation(Path<Floats.Float3> path) {this.mRotation = path;}
    public void setOffset(Path<Floats.Float> path) { this.mOffset = path; }
    public void setSpread(Path<Floats.Float> path) { this.mSpread = path; }
    public void setYaw(Path<Floats.Float> path) { this.mYaw = path; }
    public void setPitch(Path<Floats.Float> path) { this.mPitch = path; }
    public void setColor(Path<Floats.Float3> path) { this.mColor = path; }
    public void setSize(Path<Floats.Float> path) { this.mSize = path; }
    */

    public Path<Floats.Float3> getPosition() { return mPosition; }
    public Path<Floats.Float3> getRotation() { return mRotation; }
    public Path<Floats.Float> getOffset() { return mOffset; }
    public Path<Floats.Float> getSpread() { return mSpread; }
    public Path<Floats.Float> getYaw() { return mYaw; }
    public Path<Floats.Float> getPitch() { return mPitch; }
    public Path<Floats.Float3> getColor() { return mColor; }
    public Path<Floats.Float> getSize() { return mSize; }

    public static ClusterState createRandomState(ClusterState state) {

        state.clear();
        state.mPosition.randomize(new Floats.Float3(-20, -20, -20), new Floats.Float3(20,20,20), 1, 3);
        state.mRotation.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(360,360,360), 2, 5);
        state.mOffset.randomize(new Floats.Float(0), new Floats.Float(200), 1, 3);
        state.mSpread.randomize(new Floats.Float(10), new Floats.Float(100), 1, 3);
        state.mColor.randomize(new Floats.Float3(0, 0, 0), new Floats.Float3(1,1,1), 2, 4);
        state.mSize.randomize(new Floats.Float(0.5f), new Floats.Float(5f), 2, 4);
        state.mYaw.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        state.mPitch.randomize(new Floats.Float(0), new Floats.Float(180), 1, 3);
        state.onInvalidated();
        return state;
    }

    public static ClusterState createHourglass(ClusterState state) {
        state.clear();
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,-100,0),
                new Floats.Float3(0,100,0)
        });
        state.mRotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });
        state.mOffset.add(0.5f, new Floats.Float[]{
                new Floats.Float(0.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(0.0f)
        });
        state.mOffset.add(0.5f, new Floats.Float[]{
                new Floats.Float(0.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(50.0f),
                new Floats.Float(0.0f)
        });
        state.mSpread.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.mColor.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(1,0,0)
        });
        state.mColor.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(0,0,1)
        });
        state.mSize.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
                new Floats.Float(1.0f),
                new Floats.Float(2.0f)
        });
        state.mYaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.mPitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.onInvalidated();
        return state;
    }

    public static ClusterState createAxB(ClusterState state) {
        state.clear();
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-150,-100,0),
                new Floats.Float3(-100,100,0)
        });
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-50,-100,0),
                new Floats.Float3(-100,100,0)
        });
        state.mPosition.add(0.25f, new Floats.Float3[]{
                new Floats.Float3(-125,0,0),
                new Floats.Float3(-75,0,0)
        });

        state.mPosition.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(-50,-100,0),
                new Floats.Float3(50,0,0)
        });
        state.mPosition.add(0.5f, new Floats.Float3[]{
                new Floats.Float3(50,-100,0),
                new Floats.Float3(-50,0,0)
        });

        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,-100,0),
                new Floats.Float3(50,100,0)
        });
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,100,0),
                new Floats.Float3(100,100,0),
                new Floats.Float3(100,50,0),
                new Floats.Float3(100,0,0),
                new Floats.Float3(50,0,0)
        });
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(50,0,0),
                new Floats.Float3(150,0,0),
                new Floats.Float3(150,-50,0),
                new Floats.Float3(150,-100,0),
                new Floats.Float3(50,-100,0)
        });

        state.mRotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });

        state.mOffset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });

        state.mSpread.add(1.0f, new Floats.Float[]{
                new Floats.Float(5.0f)

        });

        state.mColor.add(2.25f, new Floats.Float3[]{
                new Floats.Float3(1,0,0)
        });
        state.mColor.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,1,0)
        });
        state.mColor.add(3.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,1)
        });

        state.mSize.add(1.0f, new Floats.Float[]{
                new Floats.Float(3.0f)
        });

        state.mYaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.mPitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.onInvalidated();
        return state;
    }

    public static ClusterState createSpiral(ClusterState state) {
        state.clear();
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,-100,0),
                new Floats.Float3(0,100,0)
        });
        state.mRotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0),
                new Floats.Float3(0,720,0)
        });
        state.mOffset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f),
        });
        state.mSpread.add(1.0f, new Floats.Float[]{
                new Floats.Float(20.0f)
        });
        state.mColor.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(1,0,0),
                new Floats.Float3(0,1,0),
                new Floats.Float3(0,0,1)
        });
        state.mSize.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
                new Floats.Float(3.0f),
                new Floats.Float(2.0f),
                new Floats.Float(3.0f),
                new Floats.Float(2.0f),
        });
        state.mYaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.mPitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f)
        });
        state.onInvalidated();
        return state;
    }

    public static ClusterState createPP(ClusterState state) {
        state.clear();
        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-100,-25,0),
                new Floats.Float3(-100,25,0)
        });
        state.mRotation.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,0,0)
        });
        state.mOffset.add(1.0f, new Floats.Float[]{
                new Floats.Float(0.0f),
        });
        state.mSpread.add(1.0f, new Floats.Float[]{
                new Floats.Float(50.0f)
        });
        state.mColor.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(1,1,0),
                new Floats.Float3(0,1,1),
        });
        state.mSize.add(1.0f, new Floats.Float[]{
                new Floats.Float(2.0f),
        });
        state.mYaw.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });
        state.mPitch.add(1.0f, new Floats.Float[]{
                new Floats.Float(360.0f)
        });

        state.mPosition.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(-100,25,0),
                new Floats.Float3(25,25,0),
        });
        state.mColor.add(1.0f, new Floats.Float3[]{
                new Floats.Float3(0,1,1)
        });
        state.onInvalidated();
        return state;
    }
}
