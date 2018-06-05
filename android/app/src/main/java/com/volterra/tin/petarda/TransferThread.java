package com.volterra.tin.petarda;

import java.util.Random;

import main.java.com.github.koraxiss.Packet;
import main.java.com.github.koraxiss.PacketType;

public class TransferThread implements Runnable{
    private static boolean isRunning = true;

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (isRunning) {
           ListActivity.runOnUIThread(() -> {
               /*Random random = new Random();
               Packet packet = new Packet(PacketType._LOCATION);

               short s = (short) random.nextInt(Short.MAX_VALUE + 1);
               packet.setArg1(s);
               packet.setArg2(random.nextFloat());
               packet.setArg3(random.nextFloat());
               packet.setArg4(random.nextLong());

               ListActivity.insertOrUpdateSingleItem(packet);*/
           });
            ThreadConnector.poll();
            try {
                ThreadConnector.sendLocation();
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                setNotRunning();
                e.printStackTrace();
            }
        }
    }

    public static void setNotRunning() {
        TransferThread.isRunning = false;
    }
}
