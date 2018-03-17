package com.wallpaper.axb.view;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;

import com.wallpaper.axb.engine.ClusterState;
import com.wallpaper.axb.engine.R;

import java.util.List;

/**
 * Adapter for displaying a list of {@link ClusterState}.
 */
public class ClusterStateAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private static final int VIEW_TYPE_ADD_CLUSTER = 0;
    private static final int VIEW_TYPE_EDIT_CLUSTER = 1;

    private final List<ClusterState> mList;
    private final int mItemLayoutResId;

    /**
     * Constructor.
     */
    public ClusterStateAdapter(List<ClusterState> list, int itemLayoutResId) {
        mList = list;
        mItemLayoutResId = itemLayoutResId;
    }

    /**
     * {@inheritDoc}
     */
    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        if (viewType == VIEW_TYPE_ADD_CLUSTER) {
            return new AddClusterViewHolder(LayoutInflater.from(parent.getContext()).inflate(
                    R.layout.add_cluster_layout, parent, false));
        } else if (viewType == VIEW_TYPE_EDIT_CLUSTER) {
            return new EditClusterViewHolder(LayoutInflater.from(parent.getContext()).inflate(
                    mItemLayoutResId, parent, false));
        } else {
            throw new RuntimeException("Unknown view type");
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getItemViewType(int position) {
        return position == Math.max(0, mList.size()) ? VIEW_TYPE_ADD_CLUSTER : VIEW_TYPE_EDIT_CLUSTER;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        if (holder instanceof EditClusterViewHolder) {
            ((EditClusterViewHolder) holder).bind(getItemAt(position));
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getItemCount() {
        // Compensate the item count because of the bottom view
        return mList.size() + 1;
    }

    /**
     * Clears the item list.
     */
    public void clearItems() {
        mList.clear();
    }

    /**
     * Returns the item at the given position. Compensates for bottom view.
     */
    private ClusterState getItemAt(int position) {
        return mList.get(Math.max(0, position - 1));
    }

    /**
     * View holder for add cluster item.
     */
    private class AddClusterViewHolder extends RecyclerView.ViewHolder {

        /**
         * Constructor.
         */
        AddClusterViewHolder(View itemView) {
            super(itemView);
            itemView.findViewById(R.id.add_cluster).setOnClickListener(view -> {
                        mList.add(new ClusterState());
                        notifyItemInserted(mList.size() - 1);
                    });
        }
    }

    /**
     * View holder for edit cluster item
     */
    private static class EditClusterViewHolder extends RecyclerView.ViewHolder {

        private EditText mEditName;
        private EditText mEditPosX;
        private EditText mEditPosY;
        private EditText mEditPosZ;
        private EditText mEditRotX;
        private EditText mEditRotY;
        private EditText mEditRotZ;
        private EditText mEditOffset;
        private EditText mEditSpread;
        private EditText mEditYaw;
        private EditText mEditPitch;

        /**
         * Constructor.
         */
        public EditClusterViewHolder(View itemView) {
            super(itemView);
            mEditName = itemView.findViewById(R.id.edit_cluster_name);
            mEditPosX = itemView.findViewById(R.id.edit_cluster_position_x);
            mEditPosY = itemView.findViewById(R.id.edit_cluster_position_y);
            mEditPosZ = itemView.findViewById(R.id.edit_cluster_position_z);
            mEditRotX = itemView.findViewById(R.id.edit_cluster_rotation_x);
            mEditRotY = itemView.findViewById(R.id.edit_cluster_rotation_y);
            mEditRotZ = itemView.findViewById(R.id.edit_cluster_rotation_z);
            mEditOffset = itemView.findViewById(R.id.edit_cluster_offset);
            mEditSpread = itemView.findViewById(R.id.edit_cluster_spread);
            mEditYaw = itemView.findViewById(R.id.edit_cluster_yaw);
            mEditPitch = itemView.findViewById(R.id.edit_cluster_pitch);

            new MultiTextWatcher()
                    .registerEditText(mEditName)
                    .registerEditText(mEditPosX)
                    .registerEditText(mEditPosY)
                    .registerEditText(mEditPosZ)
                    .registerEditText(mEditRotX)
                    .registerEditText(mEditRotY)
                    .registerEditText(mEditRotZ)
                    .registerEditText(mEditOffset)
                    .registerEditText(mEditSpread)
                    .registerEditText(mEditYaw)
                    .registerEditText(mEditPitch)
                    .setCallback((editText, editable) -> {
                        switch (editText.getId()) {
                            case R.id.edit_cluster_name:
                                Log.d("atte2", "edited cluster name: " + editable.toString());

                                break;
                            case R.id.edit_cluster_position_x:
                                Log.d("atte2", "edited cluster pos x: " + editable.toString());

                                break;
                            case R.id.edit_cluster_position_y:
                                Log.d("atte2", "edited cluster pos y: " + editable.toString());

                                break;
                            case R.id.edit_cluster_position_z:
                                Log.d("atte2", "edited cluster pos z: " + editable.toString());

                                break;
                            case R.id.edit_cluster_rotation_x:
                                Log.d("atte2", "edited cluster rot x: " + editable.toString());

                                break;
                            case R.id.edit_cluster_rotation_y:
                                Log.d("atte2", "edited cluster rot y: " + editable.toString());

                                break;
                            case R.id.edit_cluster_rotation_z:
                                Log.d("atte2", "edited cluster rot z: " + editable.toString());

                                break;
                            case R.id.edit_cluster_offset:
                                Log.d("atte2", "edited cluster offset: " + editable.toString());

                                break;
                            case R.id.edit_cluster_spread:
                                Log.d("atte2", "edited cluster spread: " + editable.toString());

                                break;
                            case R.id.edit_cluster_yaw:
                                Log.d("atte2", "edited cluster yaw: " + editable.toString());

                                break;
                            case R.id.edit_cluster_pitch:
                                Log.d("atte2", "edited cluster pitch: " + editable.toString());

                                break;
                        }
                    });
        }

        /**
         * Binds the UI of this view holder to the given {@link ClusterState}.
         */
        public void bind(ClusterState clusterState) {
            mEditName.setText(clusterState.id);
            // TODO: get clusterState data and set them to their edit texts
//            mEditPosX.setText(clusterState.position.x);
        }

        /**
         * {@link TextWatcher} that can listen for multiple {@link EditText}s.
         */
        private static class MultiTextWatcher {

            private TextWatcherWithInstance callback;

            /**
             * Constructor.
             */
            MultiTextWatcher setCallback(TextWatcherWithInstance callback) {
                this.callback = callback;
                return this;
            }

            /**
             * Registers text changes listening for the given {@link EditText}.
             */
            MultiTextWatcher registerEditText(final EditText editText) {
                editText.addTextChangedListener(new TextWatcher() {
                    @Override
                    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                        // Ignored.
                    }

                    @Override
                    public void onTextChanged(CharSequence s, int start, int before, int count) {
                        // Ignored.
                    }

                    @Override
                    public void afterTextChanged(Editable editable) {
                        callback.afterTextChanged(editText, editable);
                    }
                });

                return this;
            }

            /**
             * Interface for receiving callback when any of the registered {@link EditText}s text
             * changes.
             */
            interface TextWatcherWithInstance {
                void afterTextChanged(EditText editText, Editable editable);
            }
        }
    }
}
