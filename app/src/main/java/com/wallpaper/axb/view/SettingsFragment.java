package com.wallpaper.axb.view;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.transition.TransitionManager;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.Spinner;

import com.wallpaper.axb.Preset;
import com.wallpaper.axb.engine.Cluster;
import com.wallpaper.axb.engine.ClusterState;
import com.wallpaper.axb.engine.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Settings fragment.
 */
public class SettingsFragment extends Fragment {

    public static final String TAG = SettingsFragment.class.getSimpleName();

    private OnSettingsChangedListener mListener;

    /**
     * Creates new instance.
     */
    public static SettingsFragment newInstance() {
        return new SettingsFragment();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnSettingsChangedListener) {
            mListener = (OnSettingsChangedListener) context;
        }
    }

    /**
     * {@inheritDoc}
     */
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.settings_fragment, container, false);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // Init cluster state list
        final RecyclerView recyclerView = view.findViewById(R.id.cluster_list);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        // TODO: load current cluster states.
        final List<ClusterState> clusterStateList = new ArrayList<>();
        final ClusterStateAdapter clusterStateAdapter = new ClusterStateAdapter(
                clusterStateList, R.layout.edit_cluster_view);
        recyclerView.setAdapter(clusterStateAdapter);
        recyclerView.addItemDecoration(new DividerItemDecoration(getActivity(), LinearLayout.VERTICAL));

        // init preset spinner
        final List<String> presetList = new ArrayList<>(Preset.values().length);
        for (final Preset preset : Preset.values()) {
            presetList.add(getString(preset.titleResId));
        }

        final ArrayAdapter<String> spinnerAdapter = new ArrayAdapter<>(
                getContext(), android.R.layout.simple_spinner_item, presetList);
        spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        final Spinner spinner = view.findViewById(R.id.settings_preset_spinner);
        spinner.setAdapter(spinnerAdapter);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapter, View view, int pos, long id) {
                final String title = (String) adapter.getItemAtPosition(pos);
                final Preset preset = Preset.getFromTitle(getContext(), title);
                // TODO: load new cluster states.
                clusterStateAdapter.clearItems();

                if (mListener != null) {
                    mListener.onPresetChanged(preset);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapter) {
                // Ignore
            }
        });


    }

}
