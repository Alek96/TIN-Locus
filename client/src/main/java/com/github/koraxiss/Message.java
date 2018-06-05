package main.java.com.github.koraxiss;

public class Message {
    private MessageType type;
    private Packet packet;

    public Message(MessageType type, Packet packet) {
        this.type = type;
        this.packet = packet;
    }

    public Packet getPacket() {
        return packet;
    }

    public MessageType getType() {
        return type;
    }

    public enum MessageType {
        PACKET,
        CLOSE,
        ERROR
    }
}