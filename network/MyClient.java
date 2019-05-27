//https://jindongpu.wordpress.com/2012/02/06/javac-socket-communication/

import java.io.*;  
import java.net.*;  
public class MyClient {  
    public static void main(String[] args) {  
        try{      
            Socket s=new Socket("localhost",2017);  
            //Socket s=new Socket("localhost",6666);  

            BufferedWriter wr = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));
            wr.write("helloFromJavaClient!");
            wr.flush(); // flushes the stream

            s.close();  
        }catch(Exception e){System.out.println(e);}  
    }  
}
