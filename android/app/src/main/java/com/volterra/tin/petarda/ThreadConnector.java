package com.volterra.tin.petarda;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import main.java.com.github.koraxiss.Client;
import main.java.com.github.koraxiss.Message;
import main.java.com.github.koraxiss.Packet;
import main.java.com.github.koraxiss.PacketType;

public class ThreadConnector {
    private static Thread clientThread;
    private static Thread pollThread;
    private static Thread sendThread;
    private static BlockingQueue<Message> readBlockingQueue;
    private static BlockingQueue<Message> writeBlockingQueue;
    private static HashMap<Short, String> nicknameMap;

    public static void init(File fileDir) {
        readBlockingQueue = new LinkedBlockingQueue<>();
        writeBlockingQueue = new LinkedBlockingQueue<>();
        pollThread = new Thread(new PollThread());
        sendThread = new Thread(new SendThread());
        nicknameMap = new HashMap<>();

        Thread thread = new Thread(() -> {
            try {
                clientThread = new Thread(new Client(readBlockingQueue, writeBlockingQueue, fileDir));
                clientThread.start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        thread.start();
        pollThread.start();
        sendThread.start();
    }

    public static void poll(){
        Message message = ThreadConnector.getReadBlockingQueue().poll();

        if (message != null){
            switch (message.getType()){
                case PACKET:
                    switch (message.getPacket().getType()){
                        case _SET_NAME:
                            ConfigActivity.setCanSetNickname(true);
                            break;
                        case _LOCATION:
                            receiveLocation(message);
                            break;
                        case _NEW_FOLLOWED:
                            receiveFollowed(message);
                            break;
                    }
                    break;
            }
        }
    }

    public static void sendLocation() throws InterruptedException {
        if (ConfigActivity.nicknameWasSet()) {
            Random random = new Random();
            Packet packet = new Packet(PacketType._MY_LOCATION);
            packet.setArg1(random.nextFloat());
            packet.setArg2(random.nextFloat());
            packet.setArg3(0L);
            Message message = new Message(Message.MessageType.PACKET, packet);

            writeBlockingQueue.put(message);
        }
    }

    public static void receiveLocation(Message message){
        ListActivity.insertOrUpdateSingleItem(message.getPacket());
    }

    public static void receiveFollowed(Message message){
        Packet packet = message.getPacket();
        nicknameMap.put((Short)packet.getArg1(), (String)packet.getArg2());
    }

    public static Thread getClientThread() {
        return clientThread;
    }

    public static BlockingQueue<Message> getReadBlockingQueue() {
        return readBlockingQueue;
    }

    public static BlockingQueue<Message> getWriteBlockingQueue() {
        return writeBlockingQueue;
    }

    public static String getNickname(Short s){
        return nicknameMap.get(s);
    }
}
