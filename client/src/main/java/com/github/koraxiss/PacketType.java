package main.java.com.github.koraxiss;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public enum PacketType {
    _OPEN,
    _CLOSE,

    _OPEN_ENCR,
    _PUBLIC_KEY,
    _SYMMETRIC_KEY,
    _TEST_KEY,

    _OPEN_PROT,
    _SET_NAME,
    _ADD_FOLLOWER,
    _NEW_FOLLOWED,
    _REMOVE_FOLLOWER,
    _REMOVE_FOLLOWED,
    _MY_LOCATION,
    _LOCATION,
    _ACK_OK,
    _ACK_ERR,
    _ALIVE,
    _OTHER;

    public static Map<Integer, PacketType> packetTypeMap = new HashMap<>();
    public static Map<PacketType, Integer> reversedMap = new HashMap<>();

    static {
        Map<Integer, PacketType> map = new HashMap<>();
        map.put(0, PacketType._OPEN);
        map.put(1, PacketType._CLOSE);

        map.put(8, PacketType._OPEN_ENCR);
        map.put(9, PacketType._PUBLIC_KEY);
        map.put(10, PacketType._SYMMETRIC_KEY);
        map.put(11, PacketType._TEST_KEY);

        map.put(16, PacketType._OPEN_PROT);
        map.put(17, PacketType._SET_NAME);
        map.put(18, PacketType._ADD_FOLLOWER);
        map.put(19, PacketType._NEW_FOLLOWED);
        map.put(20, PacketType._REMOVE_FOLLOWER);
        map.put(21, PacketType._REMOVE_FOLLOWED);
        map.put(22, PacketType._MY_LOCATION);
        map.put(23, PacketType._LOCATION);
        map.put(2, PacketType._ACK_OK);
        map.put(3, PacketType._ACK_ERR);
        map.put(4, PacketType._ALIVE);

        packetTypeMap = Collections.unmodifiableMap(map);

        for(Integer i: packetTypeMap.keySet())
            reversedMap.put(packetTypeMap.get(i), i);
    }
}
