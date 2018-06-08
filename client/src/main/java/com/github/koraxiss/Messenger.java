package main.java.com.github.koraxiss;

import java.io.IOException;

import main.java.com.github.koraxiss.Packet;

public interface Messenger {
    void send(byte[] buffer, int n) throws IOException;
    int receive(byte[] buffer, int i) throws IOException;
    void send(Packet packet) throws IOException;
    Packet receive(byte[] buffer) throws Exception;
}
