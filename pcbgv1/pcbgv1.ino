#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>  
String readString;
int rele8 = 8;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 15, 9);
File webFile;
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  pinMode(rele8, OUTPUT);
  digitalWrite(rele8, HIGH); 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  // inicialização SD card  
   Serial.println("Carregando SD card...");  
   if (!SD.begin(4)) {  
     Serial.println("ERROR - SD card FAIL!");  
     return;    
   }  
   Serial.println("SUCCESS - SD card carregado.");  
   // arquivo index.htm   
   if (!SD.exists("index.htm")) {  
     Serial.println("ERROR - FAIL index.htm file!");  
     return; // sem localização do cartão SD  
   }  
   Serial.println("SUCCESS - arquivo index.htm .");  
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (readString.length() < 100) {
          readString += c;
        }
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
                                                                           //Serial.println(readString);
          //Liga o Rele 2
          if (readString.indexOf("?d4byWSt%$*btsuX") > 0)
          {
            if(digitalRead(rele8)){
              digitalWrite(rele8, LOW);
            }else{
              digitalWrite(rele8,HIGH);
            }
          }
    
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
       // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          webFile = SD.open("index.htm");      
           if (webFile) {  
             while(webFile.available()) {  
               client.write(webFile.read());   
             }  
             webFile.close();  
           }           
                
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
