package com.volterra.tin.petarda;

import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import main.java.com.github.koraxiss.Message;
import main.java.com.github.koraxiss.Packet;
import main.java.com.github.koraxiss.PacketType;

public class ThreadConnector {
    private static Thread clientThread;
    private static Thread transferThread;
    private static BlockingQueue<Message> readBlockingQueue;
    private static BlockingQueue<Message> writeBlockingQueue;
    private static HashMap<Short, String> nicknameMap;

    public static void init() {
        readBlockingQueue = new LinkedBlockingQueue<>();
        writeBlockingQueue = new LinkedBlockingQueue<>();
        transferThread = new Thread(new TransferThread());
        nicknameMap = new HashMap<>();

        /*try {
            clientThread = new Thread(new Client(readBlockingQueue, writeBlockingQueue));
        } catch (IOException e) {
            e.printStackTrace();
        }
        clientThread.start();*/
        transferThread.start();
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
            packet.setArg3(0);
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
}
