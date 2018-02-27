package com.wallpaper.axb.view;

import com.wallpaper.axb.Preset;

/**
 * Interface for receiving events when settings changes.
 */
public interface OnSettingsChangedListener {

    /**
     * Called when a preset is changed.
     */
    void onPresetChanged(Preset preset);
}
