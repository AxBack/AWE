package com.wallpaper.axb.engine;

import android.R.layout;
import android.app.ListActivity;
import android.app.WallpaperManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.time.Duration;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends ListActivity {

    private List<String> mListValues;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mListValues = Arrays.asList(getResources().getStringArray(R.array.wallpapers));
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, layout.simple_list_item_1,
                mListValues);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);

        String type = mListValues.get(position);
        if(type.equals("Electric")) {
            createBinary();
        }

        final SharedPreferences preferences = getSharedPreferences("AxB", MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("WallpaperType", mListValues.get(position));
        editor.apply();

        Intent intent = new Intent(
                WallpaperManager.ACTION_CHANGE_LIVE_WALLPAPER)
                .putExtra(WallpaperManager.EXTRA_LIVE_WALLPAPER_COMPONENT,
                        new ComponentName(this, WallpaperService.class))
                .putExtra("WallpaperType", mListValues.get(position));
        startActivity(intent);
    }

    protected void createBinary() {

        try {
            String name = this.getFilesDir().getAbsolutePath() + "/WAE.dat";
            FileOutputStream file = new FileOutputStream(name, false);
            DataOutputStream data = new DataOutputStream(file);
            data.writeInt(1); // nr clusters

            data.writeFloat(0); // x
            data.writeFloat(0); // y
            data.writeFloat(0); // y

            data.writeFloat(0); // yaw
            data.writeFloat(0); // pitch
            data.writeFloat(0); // roll

            data.writeInt(1); //nr parts in charge path;
            data.writeFloat(1.0f); // roll
            data.writeInt(3); // nr points in charge path
            data.writeFloat(0); // point 1
            data.writeFloat(0.2f); // point 2
            data.writeFloat(1.0f); // point 2

            data.writeInt(500); // nr nodes

            data.writeInt(1); // nr states

            data.writeInt(1); // nr parts of cluster positions path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(1); // nr points in part
            data.writeFloat(0); // x component of point
            data.writeFloat(0); // y component of point
            data.writeFloat(0); // z component of point

            data.writeInt(1); // nr parts of cluster rotation path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(1); // nr points in part
            data.writeFloat(0); // x component of point
            data.writeFloat(0); // y component of point
            data.writeFloat(0); // z component of point

            data.writeInt(1); // nr parts of cluster minOffset path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(3); // nr points in part
            data.writeFloat(0); // point 1
            data.writeFloat(-20); // point 1
            data.writeFloat(-150); // point 1

            data.writeInt(1); // nr parts of cluster maxOffset path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(3); // nr points in part
            data.writeFloat(0); // point 1
            data.writeFloat(20); // point 1
            data.writeFloat(150); // point 1

            data.writeInt(1); // nr parts of cluster color path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(2); // nr points in part
            data.writeFloat(1); // r component of point 1
            data.writeFloat(1); // g component of point 1
            data.writeFloat(0); // b component of point 1
            data.writeFloat(0); // r component of point 2
            data.writeFloat(0.2f); // g component of point 2
            data.writeFloat(1); // b component of point 2

            data.writeInt(1); // nr parts of cluster size path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(3); // nr points in part
            data.writeFloat(2); // point 1
            data.writeFloat(2); // point 1
            data.writeFloat(3); // point 1

            data.writeInt(1); // nr parts of cluster yaw spread path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(1); // nr points in part
            data.writeFloat(180); // point 1

            data.writeInt(1); // nr parts of cluster pitch spread path parts
            data.writeFloat(1.0f); // time of path part
            data.writeInt(1); // nr points in part
            data.writeFloat(180); // point 1

            file.flush();
            file.close();

        } catch (IOException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }
}
