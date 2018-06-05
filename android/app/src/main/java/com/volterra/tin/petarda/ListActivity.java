package com.volterra.tin.petarda;

import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import main.java.com.github.koraxiss.Packet;

public class ListActivity extends AppCompatActivity {
    RecyclerView mRecyclerView;
    static RecyclerView.Adapter mAdapter;
    RecyclerView.LayoutManager mLayoutManager;
    static List<String> userNames;
    static Handler UIHandler;

    static {
        userNames = new ArrayList<>();
        UIHandler = new Handler(Looper.getMainLooper());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Intent intent = getIntent();

        mRecyclerView = findViewById(R.id.user_recycler_view);
        mRecyclerView.setHasFixedSize(true);

        // use a linear layout manager
        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        // specify an adapter
        mAdapter = new UserAdapter(userNames);
        mRecyclerView.setAdapter(mAdapter);
    }

    public static void insertOrUpdateSingleItem(Packet packet) {
        String location = packet.getArg2() + " " + packet.getArg3() + " " + new Date((Long) packet.getArg4());
//        List<String> tempList = new LinkedList<>(userNames);
//        tempList.add(location);
//
//        userNames.clear();
//        userNames.addAll(tempList);
        userNames.add(location);
        if (mAdapter != null)
            mAdapter.notifyDataSetChanged();
    }

    public static void runOnUIThread(Runnable runnable) {
        UIHandler.post(runnable);
    }
}
