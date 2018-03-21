package com.wallpaper.axb;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.wallpaper.axb.engine.ClusterState;
import com.wallpaper.axb.engine.R;

/**
 * Predefined presets.
 */
public enum Preset {

    RANDOM(R.string.preset_random_title),
    HOUR_GLASS(R.string.preset_hour_glass_title),
    SPIRAL(R.string.preset_spiral_title),
    AXB(R.string.preset_axb_title),
    PP(R.string.preset_pp_title),
    CUSTOM(R.string.preset_custom_title);

    public int titleResId;

    Preset(int titleResId) {
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

    public static ClusterState setupFromPreset(Preset preset, @NonNull ClusterState state) {
        switch (preset) {
            case RANDOM:
                ClusterState.createRandomState(state);
                break;
            case HOUR_GLASS:
                ClusterState.createHourglass(state);
                break;
            case SPIRAL:
                ClusterState.createSpiral(state);
                break;
            case AXB:
                ClusterState.createAxB(state);
                break;
            case PP:
                ClusterState.createPP(state);
                break;
            default:
                break;
        }
        return state;
    }
}
