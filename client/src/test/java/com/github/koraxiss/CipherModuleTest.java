//package test.java.com.github.koraxiss;
//
//import org.junit.Test;
//import static org.junit.Assert.*;
//import static org.assertj.core.api.Assertions.assertThat;
//
//import javax.crypto.BadPaddingException;
//import javax.crypto.IllegalBlockSizeException;
//import java.io.IOException;
//import java.security.KeyPair;
//import java.security.NoSuchAlgorithmException;
//import java.security.spec.InvalidKeySpecException;
//
//public class CipherModuleTest {
////    @Test
////    public void textUponEncryptionAndDecryptionIsTheSame() throws BadPaddingException, IllegalBlockSizeException {
////        String plainText = "testinho";
////        byte[] cipherText = CipherModule.encrypt(Converter.stringToByte(plainText), 0, plainText.length());
////        byte[] decryptedText = CipherModule.decrypt(cipherText, 0, cipherText.length);
////
////        assertEquals(plainText, Converter.byteToString(decryptedText));
////    }
//
//    @Test
//    public void generateKeyPairTest() throws IOException, NoSuchAlgorithmException {
//        CipherModule.generateKeyPair();
//    }
//
//    @Test
//    public void loadKeyPairTest() throws IOException, InvalidKeySpecException, NoSuchAlgorithmException {
//        CipherModule.loadKeyPair();
//    }
//
//    @Test
//    public void keysUponGenerationAndLoadingAreTheSame() throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
//        KeyPair keyPair = CipherModule.generateKeyPair();
//        assertThat(CipherModule.loadKeyPair().getPublic().toString()).isEqualTo(keyPair.getPublic().toString());
//        assertThat(CipherModule.loadKeyPair().getPrivate().toString()).isEqualTo(keyPair.getPrivate().toString());
//    }
//}
