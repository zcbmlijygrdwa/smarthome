// Particles, by Daniel Shiffman.
import java.io.*;
import java.net.*;

ParticleSystem ps;
PImage sprite;  

double temperature = 19;

DatagramSocket socket;

byte[] buf = new byte[10*1024];

DatagramPacket packet = new DatagramPacket(buf, buf.length);

void setup() {
  size(1024, 768, P2D);

  orientation(LANDSCAPE);
  sprite = loadImage("sprite.png");
  ps = new ParticleSystem(1000);

  // Writing to the depth buffer is disabled to avoid rendering
  // artifacts due to the fact that the particles are semi-transparent
  // but not z-sorted.
  hint(DISABLE_DEPTH_MASK);


  try {
    socket = new DatagramSocket(6000, 
      InetAddress.getByName("255.255.255.255"));  // the INADDR_BROADCAST address (255.255.255.255)
  } 
  catch (SocketException e) {
    e.printStackTrace();
  } 
  catch (IOException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
  }
} 

void draw () {
  background(0);

  if (frameCount % 30 == 0) {
    thread("requestData");
  }


  ps.update();
  ps.display();

  ps.setEmitter(width/2, height*0.2, (int)(temperature-16)*20);
  textSize(16);
  text("Frame rate: " + int(frameRate), 10, 20);
}

void keyPressed() {
  //for debugging
  if (key == 'a') {
    temperature = temperature+1;
  }
  if (key == 'z') {
    temperature = temperature-1;
  }
}

// This happens as a separate thread and can take as long as it wants
void requestData() {

  try {
    socket.receive(packet);

    byte data[] = packet.getData();
    InetAddress address = packet.getAddress();
    String dataString = new String(data);
    System.out.println("dataString: " + dataString);
    //System.out.println("from IP" + address.toString());
    //System.out.println("from port:" + packet.getPort()); // 9004

    String[] list = split(dataString, ':');

    /*
      for (int i = 0; i < list.length; i++)
     {
     System.out.println("list["+i+"]: " + list[i]);
     }
     */

    String[] list2 = split(list[4], '|');

    /*
      for (int i = 0; i < list2.length; i++)
     {
     System.out.println("list2["+i+"]: " + list2[i]);
     }
     */

    temperature = Double.parseDouble(list2[0]);
    System.out.println("temperature: " + temperature);
  } 
  catch (SocketException e) {
    e.printStackTrace();
  } 
  catch (IOException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
  }
}
