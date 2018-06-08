package com.volterra.tin.petarda;

public class SendThread implements Runnable {
    private static boolean isRunning = true;

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (isRunning) {
            try {
                ThreadConnector.sendLocation();
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                setNotRunning();
                e.printStackTrace();
            }
        }
    }

    public static void setNotRunning() {
        SendThread.isRunning = false;
    }
}
