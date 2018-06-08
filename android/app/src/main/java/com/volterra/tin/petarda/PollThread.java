package com.volterra.tin.petarda;


public class PollThread implements Runnable{
    private static boolean isRunning = true;

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
        while (isRunning) {
            try {
                ListActivity.runOnUIThread(ThreadConnector::poll);
                Thread.sleep(100);
            } catch (InterruptedException e) {
                setNotRunning();
                e.printStackTrace();
            }
        }
    }

    public static void setNotRunning() {
        PollThread.isRunning = false;
    }
}
