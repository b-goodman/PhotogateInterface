/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package photogatedriver;

import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.SerialPort;
import static jssc.SerialPort.MASK_RXCHAR;
import jssc.SerialPortEvent;
import jssc.SerialPortException;
import jssc.SerialPortList;

/**
 *
 * @author bgood_000
 */
public class PhotogateDriver {

    static SerialPort arduinoPort = null;
    public static String log = "";

    public static String[] listPorts() {
        String[] portNames = SerialPortList.getPortNames();
        if (portNames.length == 0) {
            System.out.println("No active ports found");
        }
        return portNames;

    }

    public static boolean connectArduino(String port) throws SerialPortException {
        boolean success = false;
        final ArrayList<String> out = new ArrayList<>();
        SerialPort serialPort = new SerialPort(port);
        try {
            serialPort.openPort();
            serialPort.setParams(
                    SerialPort.BAUDRATE_9600,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            serialPort.setEventsMask(MASK_RXCHAR);
            serialPort.addEventListener((SerialPortEvent serialPortEvent) -> {
                if (serialPortEvent.isRXCHAR()) {
                    try {
                        String st;
                        st = serialPort.readString(serialPortEvent.getEventValue());
                        log = log.concat(st);
                        System.out.print(st.replace("~", "\n"));
                    } catch (SerialPortException ex) {
                        Logger.getLogger(PhotogateDriver.class.getName())
                                .log(Level.SEVERE, null, ex);
                    }
                }
            });

            arduinoPort = serialPort;
            success = true;
        } catch (SerialPortException ex) {
            Logger.getLogger(PhotogateDriver.class.getName())
                    .log(Level.SEVERE, null, ex);
            System.out.println("SerialPortException: " + ex.toString());
        }
        if (success) {
            modeEventTime();
        }

        return success;

    }

    public static void serialWrite(String input) throws SerialPortException {
        arduinoPort.writeString(input);
    }

    public static void modeGapTime() throws SerialPortException {
        clearLog();
        serialWrite("1");
        System.out.println("\n --- Mode: Gap Timer --- \n");
    }

    public static void modeEventTime() throws SerialPortException {
        clearLog();
        serialWrite("2");
        System.out.println("\n --- Mode: Event Timer --- \n");
    }

    public static void modeEventCount() throws SerialPortException {
        clearLog();
        serialWrite("3");
        System.out.println("\n --- Mode: Event Counter --- \n");
    }

    public static String printLog() {
        return log;
    }

    public static void clearLog() throws SerialPortException {
        log = "";
        serialWrite("r");
    }

    public static void disconnectArduino() {
        if (arduinoPort != null) {
            try {
                arduinoPort.removeEventListener();
                if (arduinoPort.isOpened()) {
                    arduinoPort.closePort();
                }
            } catch (SerialPortException ex) {
                Logger.getLogger(PhotogateDriver.class.getName())
                        .log(Level.SEVERE, null, ex);
            }
        }
    }

    public static void main(String[] args) throws SerialPortException {
        disconnectArduino();
        connectArduino(args[0]);
    }

}
