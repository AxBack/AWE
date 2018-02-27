package com.wallpaper.axb;

import android.content.Context;
import android.support.annotation.Nullable;

import com.wallpaper.axb.engine.ClusterState;
import com.wallpaper.axb.engine.R;

/**
 * Predefined presets.
 */
public enum Preset {

    RANDOM(ClusterState.createRandomState(), R.string.preset_random_title),
    HOUR_GLASS(ClusterState.createHourglass(), R.string.preset_hour_glass_title),
    SPIRAL(ClusterState.createSpiral(), R.string.preset_spiral_title),
    AXB(ClusterState.createAxB(), R.string.preset_axb_title),
    PP(ClusterState.createPP(), R.string.preset_pp_title),
    CUSTOM(null, R.string.preset_custom_title);

    public ClusterState clusterState;
    public int titleResId;

    Preset(ClusterState clusterState, int titleResId) {
        this.clusterState = clusterState;
        this.titleResId = titleResId;
    }

    /**
     * Returns a {@link Preset} that matches the given String.
     */
    @Nullable
    public static Preset getFromTitle(Context context, String title) {
        if (title.equals(context.getString(RANDOM.titleResId))) {
            return RANDOM;
        } else if (title.equals(context.getString(HOUR_GLASS.titleResId))) {
            return HOUR_GLASS;
        } else if (title.equals(context.getString(SPIRAL.titleResId))) {
            return SPIRAL;
        } else if (title.equals(context.getString(AXB.titleResId))) {
            return AXB;
        } else if (title.equals(context.getString(PP.titleResId))) {
            return PP;
        } else if (title.equals(context.getString(CUSTOM.titleResId))) {
            return CUSTOM;
        }

        return null;
    }
}
