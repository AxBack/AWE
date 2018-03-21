package com.wallpaper.axb.view;

import android.app.Activity;
import android.app.WallpaperManager;
import android.content.ComponentName;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.widget.Toast;

import com.wallpaper.axb.Preset;
import com.wallpaper.axb.engine.Cluster;
import com.wallpaper.axb.engine.ClusterState;
import com.wallpaper.axb.engine.EditorView;
import com.wallpaper.axb.engine.R;
import com.wallpaper.axb.engine.WallpaperService;

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * The main activity for this app.
 */
public class MainActivity extends Activity implements OnSettingsChangedListener {

    // TODO: this should probably be bounded by a ServiceConnection.
    public static WallpaperService sWallpaperService;
    private EditorView mEditorView;

    /**
     * {@inheritDoc}
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mEditorView = findViewById(R.id.editor);

        // Add settings fragment
        getFragmentManager().beginTransaction()
                .replace(R.id.slide_in_fragment_fragment_container,
                        SettingsFragment.newInstance(), SettingsFragment.TAG)
                .commit();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (sWallpaperService != null && requestCode == 123 && resultCode == RESULT_OK) {
            sWallpaperService.restart();
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void onPause() {
        mEditorView.onPause();
        super.onPause();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void onResume() {
        super.onResume();
        mEditorView.onResume();
    }

    private String getWallpaperFilePath() {
        return this.getFilesDir().getAbsolutePath() + "/WAE.dat";
    }

    /**
     * Request to set this LWP through Android wallpaper picker.
     */
    private void setAsWallpaper(String type) {
        List<Cluster> clusters = new ArrayList<>();
        clusters.add(Cluster.createRandom(1000, 10));
        Cluster.createBinary(clusters, getWallpaperFilePath());

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

    /**
     * {@inheritDoc}
     */
    @Override
    public void onPresetChanged(Preset preset) {
        if (preset == Preset.CUSTOM) {
            // TODO: probably do other stuff when custom
            Snackbar.make(findViewById(android.R.id.content), "Custom not available", Snackbar.LENGTH_SHORT).show();
        } else {
            Preset.setupFromPreset(preset, mEditorView.getClusterState());
        }
    }
}
