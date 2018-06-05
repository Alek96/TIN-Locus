package main.java.com.github.koraxiss;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.security.*;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

public class CipherModule {
    private static KeyPairGenerator keyPairGenerator;
    private static KeyPair keyPair;
    private static Key serverPublicKey;
    private static SecretKey sessionKey;

    private static Cipher encryptClientPrivate;
    private static Cipher decryptClientPrivate;

    private static Cipher encryptServerPublic;
    private static Cipher decryptServerPublic;

    private static Cipher encryptSession;
    private static Cipher decryptSession;

    private static State state;

    enum State{
        SERVER_PUBLIC,
        CLIENT_PRIVATE_SERVER_PUBLIC,
        SESSION
    }

    static KeyPair getKeyPair() {
        return keyPair;
    }

    static KeyPair generateKeyPair() throws NoSuchAlgorithmException, IOException {
        keyPairGenerator = KeyPairGenerator.getInstance("RSA");
        keyPairGenerator.initialize(512);
        keyPair = keyPairGenerator.generateKeyPair();

        byte[] publicKey = keyPair.getPublic().getEncoded();
        byte[] privateKey = keyPair.getPrivate().getEncoded();

        File dir = new File("key");
        if(!dir.exists())   dir.mkdir();

        File publicKeyFile = new File("key/public_key");
        File privateKeyFile = new File("key/private_key");

        if(!publicKeyFile.exists())     publicKeyFile.createNewFile();
        if(!privateKeyFile.exists())    privateKeyFile.createNewFile();

        FileOutputStream publicKeyFOS = new FileOutputStream(publicKeyFile);
        FileOutputStream privateKeyFOS = new FileOutputStream(privateKeyFile);

        publicKeyFOS.write(publicKey);
        publicKeyFOS.close();

        privateKeyFOS.write(privateKey);
        privateKeyFOS.close();

        return keyPair;
    }

    static KeyPair loadKeyPair() throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        FileInputStream publicKeyFIS = new FileInputStream("key/public_key");
        FileInputStream privateKeyFIS = new FileInputStream("key/private_key");

        byte[] publicKeyBytes = new byte[publicKeyFIS.available()];
        byte[] privateKeyBytes = new byte[privateKeyFIS.available()];

        publicKeyFIS.read(publicKeyBytes);
        publicKeyFIS.close();

        privateKeyFIS.read(privateKeyBytes);
        privateKeyFIS.close();

        X509EncodedKeySpec pubKeySpec = new X509EncodedKeySpec(publicKeyBytes);
        KeyFactory pubKeyFactory = KeyFactory.getInstance("RSA");
        PublicKey pubKey = pubKeyFactory.generatePublic(pubKeySpec);

        PKCS8EncodedKeySpec privKeySpec = new PKCS8EncodedKeySpec(privateKeyBytes);
        KeyFactory privKeyFactory = KeyFactory.getInstance("RSA");
        PrivateKey privKey = privKeyFactory.generatePrivate(privKeySpec);

        keyPair = new KeyPair(pubKey, privKey);
        return keyPair;
    }

    static void printKeyPair(){
        System.out.println(keyPair.getPublic().toString());
        System.out.println(keyPair.getPrivate().toString());
    }

    static void loadServerPublic() throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        FileInputStream publicKeyFIS = new FileInputStream("key/server_public");
        byte[] serverPublicBytes = new byte[publicKeyFIS.available()];

        publicKeyFIS.read(serverPublicBytes);
        publicKeyFIS.close();

        X509EncodedKeySpec pubKeySpec = new X509EncodedKeySpec(serverPublicBytes);
        KeyFactory pubKeyFactory = KeyFactory.getInstance("RSA");
        serverPublicKey = pubKeyFactory.generatePublic(pubKeySpec);
    }

    static void setSessionKey(byte[] key){
        sessionKey = new SecretKeySpec(key, "AES");
    }

    static void initializeClientCiphers() throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, IOException, InvalidKeySpecException {
        loadKeyPair();
        encryptClientPrivate = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        decryptClientPrivate = Cipher.getInstance("RSA/ECB/PKCS1Padding");

        encryptClientPrivate.init(Cipher.ENCRYPT_MODE, keyPair.getPrivate());
        decryptClientPrivate.init(Cipher.DECRYPT_MODE, keyPair.getPrivate());
    }

    static void initializeServerCiphers() throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, IOException, InvalidKeySpecException {
        loadServerPublic();
        encryptServerPublic = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        decryptServerPublic = Cipher.getInstance("RSA/ECB/PKCS1Padding");

        encryptServerPublic.init(Cipher.ENCRYPT_MODE, serverPublicKey);
        decryptServerPublic.init(Cipher.DECRYPT_MODE, serverPublicKey);
    }

    static void initializeSessionCiphers() throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException {
        encryptSession = Cipher.getInstance("AES/ECB/PKCS5Padding");
        decryptSession = Cipher.getInstance("AES/ECB/PKCS5Padding");

        encryptSession.init(Cipher.ENCRYPT_MODE, sessionKey);
        decryptSession.init(Cipher.DECRYPT_MODE, sessionKey);
    }


    static byte[] encrypt(byte[] buffer, int offset, int n) throws BadPaddingException, IllegalBlockSizeException {
        switch (state){
            case SERVER_PUBLIC:
                return encryptServerPublic.doFinal(buffer, offset, n);
            case CLIENT_PRIVATE_SERVER_PUBLIC:
//                byte[] temp = encryptClientPrivate.doFinal(buffer, offset, n);
                return encryptServerPublic.doFinal(buffer, offset, n);
            case SESSION:
                return encryptSession.doFinal(buffer, offset, n);
            default:
                throw new RuntimeException("CipherModule state not known.");
        }
    }

    static byte[] decrypt(byte[] buffer, int offset, int n) throws BadPaddingException, IllegalBlockSizeException {
        switch (state){
            case CLIENT_PRIVATE_SERVER_PUBLIC:
                return decryptClientPrivate.doFinal(buffer, offset, n);
//                return decryptServerPublic.doFinal(temp);
            case SESSION:
                return decryptSession.doFinal(buffer, offset, n);
            default:
                throw new RuntimeException("CipherModule state not known.");
        }
    }

    static State getState() {
        return state;
    }

    static void setState(State state) {
        CipherModule.state = state;
    }
}
