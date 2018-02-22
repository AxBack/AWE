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
    EditorView mEditorView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mEditorView = (EditorView)findViewById(R.id.editor);
        updateEditorView();

        mListValues = Arrays.asList(getResources().getStringArray(R.array.options));
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

        switch (position) {
            case 0:
                updateEditorView();
                break;
            case 1:
                updateWallpaper(mListValues.get(position));
                break;
        }
    }

    private void updateEditorView() {
        List<Cluster> clusters = new ArrayList<>();
        clusters.add(Cluster.createRandom(1000, 1));
        createBinary(clusters, "temp.dat");
        mEditorView.reset();
    }

    private void updateWallpaper(String type) {
        List<Cluster> clusters = new ArrayList<>();
        clusters.add(Cluster.createRandom(1000, 10));
        createBinary(clusters, "WAE.dat");

        final SharedPreferences preferences = getSharedPreferences("AxB", MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putString("WallpaperType", type);
        editor.apply();

        Intent intent = new Intent(
                WallpaperManager.ACTION_CHANGE_LIVE_WALLPAPER)
                .putExtra(WallpaperManager.EXTRA_LIVE_WALLPAPER_COMPONENT,
                        new ComponentName(this, WallpaperService.class))
                .putExtra("WallpaperType", type);
        startActivityForResult(intent, 123);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(sWallpaperService != null && requestCode == 123 && resultCode == RESULT_OK) {
            sWallpaperService.restart();
        }
        super.onActivityResult(requestCode, resultCode, data);
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

    @Override
    protected void onPause() {
        super.onPause();
        mEditorView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mEditorView.onResume();
    }
}
