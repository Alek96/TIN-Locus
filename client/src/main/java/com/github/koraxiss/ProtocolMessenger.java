package main.java.com.github.koraxiss;

import java.io.IOException;

public class ProtocolMessenger implements Messenger {
    Messenger encryptedMessenger;

    public ProtocolMessenger(Messenger encryptedMessenger) {
        this.encryptedMessenger = encryptedMessenger;
    }

    public void send(byte[] buffer, int n) throws IOException {
        encryptedMessenger.send(buffer, n);
    }

    public int receive(byte[] buffer, int n) throws IOException {
        int i = encryptedMessenger.receive(buffer, n);
        return i;
    }

    public void send(Packet packet) throws IOException {
        byte[] buffer = packet.getBuffer();
        send(buffer, buffer.length);
    }

    public Packet receive(byte[] buffer) throws Exception {
        Packet packet = encryptedMessenger.receive(buffer);
        if (packet == null)
            return null;
        else if (packet.getType() != PacketType._OTHER)
            return packet;
        int size = packet.getSize();
        int type = Converter.byteToInt(buffer[0]);
        System.out.println((int)buffer[0]);
        System.out.println("Received packet " + PacketType.packetTypeMap.get(type));
        PacketType packetType = PacketType.packetTypeMap.get(type);
        switch (packetType) {
            case _OPEN_PROT:
                return new Packet(PacketType._OPEN_PROT);
            case _SET_NAME:
                packet = new Packet(PacketType._OPEN_PROT);
                byte[] name = new byte[size - 1];
                System.arraycopy(buffer, 1, name, 0, size - 1);
                String string = Converter.byteToString(name);
                packet.setArg1(string);
                return packet;
            case _ADD_FOLLOWER:
                return null;
            case _NEW_FOLLOWED:
                Packet packet1 = new Packet(PacketType._NEW_FOLLOWED);
                byte[] id = new byte[2];
                id[0] = buffer[1];
                id[1] = buffer[2];
                short sId = Converter.byteToShort(id);
                packet1.setArg1(sId);
                byte[] name1 = new byte[size - 3];
                System.arraycopy(buffer, 3, name1, 0, size - 3);
                String string1 = Converter.byteToString(name1);
                packet1.setArg2(string1);
                return packet1;
            case _REMOVE_FOLLOWER:
                return null;
            case _REMOVE_FOLLOWED:
                Packet packet2 = new Packet(PacketType._REMOVE_FOLLOWED);
                byte[] name2 = new byte[size - 1];
                System.arraycopy(buffer, 1, name2, 0, size - 3);
                String string2 = Converter.byteToString(name2);
                packet2.setArg1(string2);
                return packet2;
            case _MY_LOCATION:
                return null;
            case _LOCATION:
                Packet packet3 = new Packet(PacketType._LOCATION);
                byte[] id1 = new byte[2];
                id1[0] = buffer[1];
                id1[1] = buffer[2];
                short sId1 = Converter.byteToShort(id1);
                packet3.setArg1(sId1);
                byte[] lat = new byte[4];
                System.arraycopy(buffer, 3, lat, 0, 4);
                float latitude = Converter.byteToFloat(lat);
                packet3.setArg2(latitude);
                byte[] lon = new byte[4];
                System.arraycopy(buffer, 7, lon, 0, 4);
                float longitude = Converter.byteToFloat(lon);
                packet3.setArg3(longitude);
                byte[] t = new byte[8];
                System.arraycopy(buffer, 11, t, 0, 8);
                long time = Converter.byteToLong(t);
                packet3.setArg4(time);
                return packet3;
            case _ACK_ERR:
                return new Packet(PacketType._ACK_ERR);
            case _ACK_OK:
                return new Packet(PacketType._ACK_OK);
            case _CLOSE:
                return new Packet(PacketType._CLOSE);
            case _ALIVE:
                return new Packet(PacketType._ALIVE);
            default:
                throw new Exception("Wrong packet");
        }
    }
}

