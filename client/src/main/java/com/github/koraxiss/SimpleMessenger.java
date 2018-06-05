package main.java.com.github.koraxiss;

import java.io.*;
import java.net.Socket;

public class SimpleMessenger implements Messenger {
    private Socket socket;
    private InputStream input;
    private OutputStream output;

    public SimpleMessenger(String host, int port) throws IOException {
            socket = new Socket(host, port);
            input = socket.getInputStream();
            output = socket.getOutputStream();
    }

    public void closeSocket() throws IOException {
        this.socket.close();
    }

    public void send(byte[] buffer, int n) throws IOException {
        byte[] message = new byte[n + 2];
        short size = (short) n;
        byte[] sizeByte = Converter.shortToByte(size);
        message[0] = sizeByte[0];
        message[1] = sizeByte[1];
        System.out.println("Sent message length: " + message.length);
        System.arraycopy(buffer, 0, message, 2, n);
        output.write(message, 0, message.length);
    }

    public int receive(byte[] buffer, int n) throws IOException {
        int j = input.read(buffer, 0, 2);
        if(j < 0)
            throw new IOException("Failed to read");
        byte[] size = new byte[2];
        size[0] = buffer[0];
        size[1] = buffer[1];
        short s = Converter.byteToShort(size);
        System.out.println("Bytes to receive: " + s);
        int i = input.read(buffer, 0, s);
        return i;
    }

    public void send(Packet packet) throws IOException {
        send(packet.getBuffer(), packet.getBuffer().length);
        System.out.println("Sent packet " + packet.getType());
    }

    public Packet receive(byte[] buffer) throws IOException {
        int i = receive(buffer, 0);
        int type = buffer[0];
        System.out.println("Received packet " + PacketType.packetTypeMap.get(type));
        PacketType packetType = PacketType.packetTypeMap.get(type);
        switch (packetType) {
            case _OPEN:
                return new Packet(PacketType._OPEN);
            case _CLOSE:
                return new Packet(PacketType._CLOSE);
            case _ACK_ERR:
                return new Packet(PacketType._ACK_ERR);
            case _ACK_OK:
                return new Packet(PacketType._ACK_OK);
            case _ALIVE:
                return new Packet(PacketType._ALIVE);
            case _OPEN_ENCR:
                return new Packet(PacketType._OPEN_ENCR);
            default:
                return null;
        }
    }
}
