package main.java.com.github.koraxiss;

import javax.crypto.NoSuchPaddingException;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Client implements Runnable{
    private LayerManager layerManager;
    private BlockingQueue<Message> childMessages;
    private BlockingQueue<Message> sendInstructions;
    private BlockingQueue<Message> androidAppMessages;
    private Thread readerThread;
    private Thread writerThread;
    private Thread mainThread;
    private boolean running;
    private Boolean sending;
    private Boolean receiveNext;
    private final Object sendingLock;
    private final Object receivingLock;

    public Client(BlockingQueue<Message> androidAppMessages, BlockingQueue<Message> childMessages) throws IOException {
        layerManager = new LayerManager();
        this.childMessages = childMessages;
        sendInstructions = new LinkedBlockingQueue<>();
        this.androidAppMessages = androidAppMessages;
        readerThread = new Thread(new ReaderThread());
        writerThread = new Thread(new WriterThread());
        running = true;
        receiveNext = true;
        sendingLock = new Object();
        receivingLock = new Object();
    }

    @Override
    public void run() {
        mainThread = Thread.currentThread();
        writerThread.start();
        readerThread.start();
        Message message;
        while (running) {
            try {
                message = childMessages.take();
                if (message.getType() == Message.MessageType.PACKET) {
                    switch (message.getPacket().getType()) {
                        case _OPEN:
                            break;
                        case _OPEN_PROT:
                            androidAppMessages.put(new Message(Message.MessageType.PACKET, new Packet(PacketType._SET_NAME)));
                            break;
                        case _ALIVE:
                            sendInstructions.put(new Message(Message.MessageType.PACKET, new Packet(PacketType._ACK_OK)));
                            break;
                        case _OPEN_ENCR:
                            System.out.println("Received open encr");
                            CipherModule.initializeClientCiphers();
                            CipherModule.initializeServerCiphers();
                            layerManager.setEncryptionOn(true);
                            CipherModule.setState(CipherModule.State.SERVER_PUBLIC);
                            Packet packet6 = new Packet(PacketType._PUBLIC_KEY);
                            packet6.setArg1(CipherModule.getKeyPair().getPublic().getEncoded());
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet6));
                            sending = true;
                            System.out.println("Ordered to send public key");
                            synchronized (sendingLock) {
                                while (sending) {
                                    sendingLock.wait();
                                }
                                CipherModule.setState(CipherModule.State.CLIENT_PRIVATE_SERVER_PUBLIC);
                            }
                            break;
                        case _CLOSE:
                            stop();
                            break;
                        case _SYMMETRIC_KEY:
                            CipherModule.setSessionKey((byte[]) message.getPacket().getArg1());
                            sendInstructions.put(new Message(Message.MessageType.PACKET, message.getPacket()));
                            sending = true;
                            synchronized (sendingLock) {
                                while (sending) {
                                    sendingLock.wait();
                                }
                                CipherModule.setState(CipherModule.State.SESSION);
                                CipherModule.initializeSessionCiphers();
                            }
                            break;
                        case _TEST_KEY:
                            Packet packet = new Packet(PacketType._TEST_KEY);
                            packet.setArg1(message.getPacket().getArg1());
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet));
                            break;
                        case _SET_NAME:
                            Packet packet1 = message.getPacket();
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet1));
                            break;
                        case _ADD_FOLLOWER:
                            Packet packet4 = message.getPacket();
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet4));
                            break;
                        case _NEW_FOLLOWED:
                            androidAppMessages.put(message);
                        case _REMOVE_FOLLOWED:
                            Packet packet3 = message.getPacket();
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet3));
                            break;
                        case _REMOVE_FOLLOWER:
                            Packet packet2 = message.getPacket();
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet2));
                            break;
                        case _LOCATION:
                            androidAppMessages.put(message);
                            break;
                        case _ACK_ERR:
                            break;
                        case _ACK_OK:
                            break;
                        case _MY_LOCATION:
                            Packet packet5 = message.getPacket();
                            sendInstructions.put(new Message(Message.MessageType.PACKET, packet5));
                            break;
                    }
                } else
                    stop();
                Thread.sleep(100);
                synchronized(receivingLock) {
                    receiveNext = true;
                    receivingLock.notify();
                }
            } catch (InterruptedException | IOException | NoSuchPaddingException | NoSuchAlgorithmException | InvalidKeySpecException | InvalidKeyException e) {
                stop();
//                e.printStackTrace();
            }
        }
    }

    public void stop() {
        running = false;
        readerThread.interrupt();
        writerThread.interrupt();
        System.out.println("Threads closed");
        try {
            layerManager.closeSocket();
            System.out.println("Socket closed");
        } catch (IOException e) {
//            e.printStackTrace();
        }
    }

    private class ReaderThread implements Runnable {

        @Override
        public void run() {
            Packet packet;
            while (running) {
                try {
                    synchronized(receivingLock) {
                        while(!receiveNext)
                            receivingLock.wait();
                    }
                    packet = layerManager.receive();
                    Message message = new Message(Message.MessageType.PACKET, packet);
                    childMessages.put(message);
                    receiveNext = false;
                    Thread.sleep(1000);
                } catch (Exception e) {
//                    e.printStackTrace();
                    try {
                        childMessages.put(new Message(Message.MessageType.ERROR, null));
                        mainThread.interrupt();
                        Thread.currentThread().interrupt();
                    } catch (InterruptedException e1) {
//                        e1.printStackTrace();
                    }
                }
            }
        }
    }

    private class WriterThread implements Runnable {

        @Override
        public void run() {
            Packet packet = null;
            Message message = null;
            while (running) {
                try {
                    message = sendInstructions.take();
                    synchronized (sendingLock) {
                        if (message.getType() == Message.MessageType.PACKET) {
                            packet = message.getPacket();
                            layerManager.send(packet);
                        }
                        sending = false;
                        sendingLock.notify();
                    }
                    Thread.sleep(100);
                } catch (IOException | InterruptedException e1) {
//                    e1.printStackTrace();
                    try {
                        childMessages.put(new Message(Message.MessageType.ERROR, null));
                        mainThread.interrupt();
                        Thread.currentThread().interrupt();
                    } catch (InterruptedException e) {
//                        e.printStackTrace();
                    }
                }
            }
        }
    }
}

