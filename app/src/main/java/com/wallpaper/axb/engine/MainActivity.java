package com.wallpaper.axb.engine;

import android.R.layout;
import android.app.ListActivity;
import android.app.WallpaperManager;
import android.content.ComponentName;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends ListActivity {

    private List<String> mListValues;

    static WallpaperService sWallpaperService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        createClusterBinary(1000, "temp.dat");

        setContentView(R.layout.activity_main);

        mListValues = Arrays.asList(getResources().getStringArray(R.array.wallpapers));
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, layout.simple_list_item_1,
                mListValues);
        setListAdapter(adapter);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);

        createClusterBinary(1000, "WAE.dat");

        final SharedPreferences preferences = getSharedPreferences("AxB", MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("WallpaperType", mListValues.get(position));
        editor.apply();

        Intent intent = new Intent(
                WallpaperManager.ACTION_CHANGE_LIVE_WALLPAPER)
                .putExtra(WallpaperManager.EXTRA_LIVE_WALLPAPER_COMPONENT,
                        new ComponentName(this, WallpaperService.class))
                .putExtra("WallpaperType", mListValues.get(position));
        startActivityForResult(intent, 123);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(sWallpaperService != null && requestCode == 123 && resultCode == RESULT_OK) {
            sWallpaperService.restart();
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private void createClusterBinary(int nrNodes, String name) {
        List<Cluster> clusters = new ArrayList<>();
        clusters.add(new Cluster(nrNodes));
        createBinary(clusters, name);
    }

    private void createBinary(List<Cluster> clusters, String name) {

        try {
            name = this.getFilesDir().getAbsolutePath() + "/" + name;
            FileOutputStream file = new FileOutputStream(name, false);
            DataOutputStream data = new DataOutputStream(file);

            data.writeInt(clusters.size()); // nr clusters

            for(Cluster c : clusters)
                c.write(data);

            file.flush();
            file.close();

        } catch (IOException e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }
}
