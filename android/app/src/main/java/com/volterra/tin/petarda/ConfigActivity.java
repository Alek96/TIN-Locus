package com.volterra.tin.petarda;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import main.java.com.github.koraxiss.Message;
import main.java.com.github.koraxiss.Packet;
import main.java.com.github.koraxiss.PacketType;


public class ConfigActivity extends AppCompatActivity {
    private static boolean canSetNickname = false;
    private static boolean nicknameWasSet = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_config);
        ThreadConnector.init();
    }

    public void sendNickname(View view) throws InterruptedException {
        if (canSetNickname) {
            EditText editText = findViewById(R.id.editText);
            String message = editText.getText().toString();

            Packet packet = new Packet(PacketType._SET_NAME);
            packet.setArg1(message);
            ThreadConnector.getWriteBlockingQueue().put(new Message(Message.MessageType.PACKET, packet));
            nicknameWasSet = true;
        }
    }

    public void toListActivity(View view){
        Intent intent = new Intent(this, ListActivity.class);
        startActivity(intent);
    }

    public void addFollower(View view) throws InterruptedException {
        if (nicknameWasSet()) {
            EditText editText = findViewById(R.id.editText2);
            String message = editText.getText().toString();

            Packet packet = new Packet(PacketType._ADD_FOLLOWER);
            packet.setArg1(message);
            ThreadConnector.getWriteBlockingQueue().put(new Message(Message.MessageType.PACKET, packet));
        }
    }

    public void removeFollower(View view) throws InterruptedException {
        if (nicknameWasSet()) {
            EditText editText = findViewById(R.id.editText3);
            String message = editText.getText().toString();

            Packet packet = new Packet(PacketType._REMOVE_FOLLOWER);
            packet.setArg1(message);
            ThreadConnector.getWriteBlockingQueue().put(new Message(Message.MessageType.PACKET, packet));
        }
    }

    public void unfollow(View view) throws InterruptedException {
        if (nicknameWasSet()) {
            EditText editText = findViewById(R.id.editText4);
            String message = editText.getText().toString();

            Packet packet = new Packet(PacketType._REMOVE_FOLLOWED);
            packet.setArg1(message);
            ThreadConnector.getWriteBlockingQueue().put(new Message(Message.MessageType.PACKET, packet));
        }
    }

    public static boolean canSetNickname() {
        return canSetNickname;
    }

    public static boolean nicknameWasSet() {
        return nicknameWasSet;
    }

    public static void setCanSetNickname(boolean canSetNickname) {
        ConfigActivity.canSetNickname = canSetNickname;
    }
}
