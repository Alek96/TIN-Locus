package main.java.com.github.koraxiss;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

public class Converter {
    public static byte intToByte(int i) {
        return (byte)i;
    }

    public static int byteToInt(byte b) {
        return b;
    }

    public static byte[] floatToByte(float f) {
        byte[] bytes = new byte[4];
        ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).putFloat(f);
        return bytes;
    }

    public static float byteToFloat(byte[] b) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(b);
        return byteBuffer.order(ByteOrder.LITTLE_ENDIAN).getFloat();
    }

    public static byte[] shortToByte(short s) {
        byte[] bytes = new byte[2];
        ByteBuffer buffer = ByteBuffer.allocate(bytes.length);
        buffer.order(ByteOrder.LITTLE_ENDIAN).putShort(s);
        return buffer.array();
    }

    public static short byteToShort(byte[] bytes) {
        ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);
        return byteBuffer.order(ByteOrder.LITTLE_ENDIAN).getShort();
    }

    public static byte[] stringToByte(String string) {
        return string.getBytes(StandardCharsets.UTF_8);
    }

    public static String byteToString(byte[] bytes) {
        return new String(bytes, StandardCharsets.UTF_8);
    }

    public static long byteToLong(byte[] longBytes){
        ByteBuffer byteBuffer = ByteBuffer.wrap(longBytes);
        return byteBuffer.order(ByteOrder.LITTLE_ENDIAN).getLong();
    }

    public static byte[] longToByte(long l) {
        byte[] bytes = new byte[8];
        ByteBuffer buffer = ByteBuffer.allocate(bytes.length);
        buffer.order(ByteOrder.LITTLE_ENDIAN).putLong(l);
        return buffer.array();
    }
}
