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
}
