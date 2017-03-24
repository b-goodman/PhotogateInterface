# PhotogateInterface

Allows interface between Arduino photogate timer and computer.  

Written in Java and using the JSSC library, it should be OS independent.  Built and tested on Windows only so far.

Java class allows communication over serial port allowing capture of data and device control.  Interfaces with Mathematica notebook which provides GUI controlls and live view of captured data via Java terminal.

Still a prototype, the Java application is command line only and the jar file may be run via OS terminal.  The connecting serial port must be included when calling the application.

E.G., java -jar "...\PhotogateDriver.jar" COM4
