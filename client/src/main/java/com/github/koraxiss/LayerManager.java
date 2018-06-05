package main.java.com.github.koraxiss;

import java.io.IOException;

public class LayerManager {
    private ProtocolMessenger protocolMessenger;
    private EncryptedMessenger encryptedMessenger;
    private SimpleMessenger simpleMessenger;
    private boolean isEncryptionOn;

    public LayerManager() throws IOException {
        simpleMessenger = new SimpleMessenger("25.46.207.96", 5050);
        encryptedMessenger = new EncryptedMessenger(simpleMessenger);
        protocolMessenger = new ProtocolMessenger(encryptedMessenger);
        isEncryptionOn = false;
    }

    public void closeSocket() throws IOException {
        simpleMessenger.closeSocket();
    }

    public void send(Packet packet) throws IOException {
        if(isEncryptionOn)
            protocolMessenger.send(packet);
        else
            simpleMessenger.send(packet);
    }

    public Packet receive() throws Exception {
        byte[] buffer = new byte[1024];
        if(isEncryptionOn)
            return protocolMessenger.receive(buffer);
        return simpleMessenger.receive(buffer);
    }

    public void setEncryptionOn(boolean encryptionOn) {
        isEncryptionOn = encryptionOn;
    }
}
