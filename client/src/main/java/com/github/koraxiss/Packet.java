package main.java.com.github.koraxiss;

import java.security.Key;
import java.security.PublicKey;

public class Packet {
    private PacketType type;
    private Object arg1;
    private Object arg2;
    private Object arg3;
    private Object arg4;
    private int size;

    public Packet(PacketType type) {
        this.type = type;
    }

    public int getSize() {
        return size;
    }

    public PacketType getType() {
        return this.type;
    }

    public byte[] getBuffer() {
        switch (type) {
            case _ALIVE:
            case _CLOSE:
            case _OPEN:
            case _ACK_ERR:
            case _ACK_OK:
            case _OPEN_ENCR:
            case _OPEN_PROT:
                byte[] bytes = new byte[1];
                byte b = Converter.intToByte(PacketType.reversedMap.get(type));
                bytes[0] = b;
                return bytes;
            case _PUBLIC_KEY:
            case _SYMMETRIC_KEY:
            case _TEST_KEY:
                return bufferEncryptedPacket();
            case _SET_NAME:
                return bufferSizeName();
            case _ADD_FOLLOWER:
                return bufferSizeName();
            case _NEW_FOLLOWED:
                return bufferNewFollowed();
            case _REMOVE_FOLLOWER:
                return bufferSizeName();
            case _REMOVE_FOLLOWED:
                return bufferSizeName();
            case _MY_LOCATION:
                return bufferMyLocation();
            case _LOCATION:
                return bufferLocation();
            default:
                return null;
        }
    }

    private byte[] bufferEncryptedPacket() {
        byte[] bytes = new byte[1 + ((byte[])arg1).length];
        bytes[0] = Converter.intToByte(PacketType.reversedMap.get(type));
        byte[] string = (byte[]) arg1;
        System.arraycopy(string, 0, bytes, 1, string.length);
        return bytes;
    }

    private byte[] bufferLocation() {
        byte[] bytes = new byte[19];
        bytes[0] = Converter.intToByte(PacketType.reversedMap.get(type));
        byte[] shor = Converter.shortToByte((short) this.getArg1());
        System.arraycopy(shor, 0, bytes, 1, shor.length);
        byte[] lat = Converter.floatToByte((float) this.getArg2());
        System.arraycopy(lat, 0, bytes, 3, lat.length);
        byte[] lon = Converter.floatToByte((float) this.getArg3());
        System.arraycopy(lon, 0, bytes, 7, lon.length);
        byte[] t = Converter.longToByte((long) this.getArg4());
        System.arraycopy(t, 0, bytes, 11, t.length);
        return bytes;
    }

    private byte[] bufferMyLocation() {
        byte[] bytes = new byte[17];
        bytes[0] = Converter.intToByte(PacketType.reversedMap.get(type));
        byte[] lat = Converter.floatToByte((float) this.getArg1());
        System.arraycopy(lat, 0, bytes, 1, lat.length);
        byte[] lon = Converter.floatToByte((float) this.getArg2());
        System.arraycopy(lon, 0, bytes, 5, lon.length);
        byte[] dt = Converter.longToByte((long) this.getArg3());
        System.arraycopy(dt, 0, bytes, 9, dt.length);
        return bytes;
    }

    private byte[] bufferNewFollowed() {
        byte[] bytes = new byte[1 + 2 + ((String) this.getArg2()).length()];
        bytes[0] = Converter.intToByte(PacketType.reversedMap.get(type));
        byte[] shor;
        shor = Converter.shortToByte((short) this.getArg1());
        bytes[1] = shor[0];
        bytes[2] = shor[1];
        byte[] string = Converter.stringToByte((String) this.getArg2());
        System.arraycopy(string, 0, bytes, 3, string.length);
        return bytes;
    }

    private byte[] bufferSizeName() {
        byte[] bytes = new byte[1 + ((String) this.getArg1()).length()];
        bytes[0] = Converter.intToByte(PacketType.reversedMap.get(type));
        byte[] string = Converter.stringToByte((String) this.getArg1());
        System.arraycopy(string, 0, bytes, 1, string.length);
        return bytes;
    }

    public Object getArg1() {
        return arg1;
    }

    public void setArg1(Object arg1) {
        this.arg1 = arg1;
    }

    public Object getArg2() {
        return arg2;
    }

    public void setArg2(Object arg2) {
        this.arg2 = arg2;
    }

    public Object getArg3() {
        return arg3;
    }

    public void setArg3(Object arg3) {
        this.arg3 = arg3;
    }

    public Object getArg4() {
        return arg4;
    }

    public void setArg4(Object arg4) {
        this.arg4 = arg4;
    }

    public void setSize(int size) {
        this.size = size;
    }
}
