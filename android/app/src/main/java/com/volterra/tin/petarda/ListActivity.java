package com.volterra.tin.petarda;

import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import main.java.com.github.koraxiss.Packet;

public class ListActivity extends AppCompatActivity {
    RecyclerView mRecyclerView;
    static RecyclerView.Adapter mAdapter;
    RecyclerView.LayoutManager mLayoutManager;
    static List<String> userNames;
    static HashMap<String, String> locations;
    static Handler UIHandler;

    static {
        userNames = new ArrayList<>();
        locations = new HashMap<>();
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
        String location = packet.getArg2() + " " + packet.getArg3() + " " + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date((Long) packet.getArg4() * 1000));
        System.out.println((float) packet.getArg2());
        System.out.println((float) packet.getArg3());
        System.out.println((long) packet.getArg4());
        String userName = ThreadConnector.getNickname((Short) packet.getArg1());
        if(userName != null){
            locations.put(userName, location);
        }
        LinkedList<String> tempList = new LinkedList<>();
        locations.forEach((k, v) -> tempList.add(k + " " + v));
        userNames.clear();
        userNames.addAll(tempList);

        if (mAdapter != null)
            mAdapter.notifyDataSetChanged();
    }

    public static void runOnUIThread(Runnable runnable) {
        UIHandler.post(runnable);
    }
}
