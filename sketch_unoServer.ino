/**************************************************************
File Created by: Anindya Sundar Manna
Date:6th Feb 2015


Description: Home Automation Server in Arduino Uno 
  where the webpage i.htm is placed inside a SD card.

Modified on Example templates in Arduino 1.0.6
/**************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// size of buffer used to capture HTTP requests
#define BUF_SZ   40

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80
File webFile;
char HTTP_req[BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

void setup()
{
    // disable Ethernet chip
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);    
     //debug stuff   
    // initialize SD card
    if (!SD.begin(4)) {
            return;    // init failed
    }
    // check for index.htm file
    if (!SD.exists("I.HTM")) {
            return;  // can't find index file
    }
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    
                    if (StrContains(HTTP_req, "On_a")) {
                        Serial.write('A');
                    }
                    else if (StrContains(HTTP_req, "On_b")) {
                        Serial.write('B');
                    }
                    else if (StrContains(HTTP_req, "On_c")) {
                        Serial.write('C');
                    }
                    else if (StrContains(HTTP_req, "On_d")) {
                        Serial.write('D');
                    }
                    else if (StrContains(HTTP_req, "On_e")) {
                        Serial.write('E');
                    }
                    else if (StrContains(HTTP_req, "On_f")) {
                        Serial.write('F');
                    }
                    else if (StrContains(HTTP_req, "On_g")) {
                        Serial.write('G');
                    }
                    else if (StrContains(HTTP_req, "On_h")) {
                        Serial.write('H');
                    }
                    else if (StrContains(HTTP_req, "Off_a")) {
                        Serial.write('a');
                    }
                    else if (StrContains(HTTP_req, "Off_b")) {
                        Serial.write('b');
                    }
                    else if (StrContains(HTTP_req, "Off_c")) {
                        Serial.write('c');
                    }
                    else if (StrContains(HTTP_req, "Off_d")) {
                        Serial.write('d');
                    }
                    else if (StrContains(HTTP_req, "Off_e")) {
                        Serial.write('e');
                    }
                    else if (StrContains(HTTP_req, "Off_f")) {
                        Serial.write('f');
                    }
                    else if (StrContains(HTTP_req, "Off_g")) {
                        Serial.write('g');
                    }
                    else if (StrContains(HTTP_req, "Off_h")) {
                        Serial.write('h');
                    }
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send XML file containing input states
                        Serial.write('O');
                        XML_response(client);
                        
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send web page
                        webFile = SD.open("I.HTM");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// send the state of the switch to the web browser
void XML_response(EthernetClient cl)
{
          cl.print("<?xml version = \"1.0\" ?>");
          cl.print("<inputs>");
          char arr[8];
          for (int i=0;i<8;i++) 
          {repeat:  if (Serial.available()>0)
              {
              arr[i]=Serial.read();
             // cl.println(arr[i]);
              }
              else{goto repeat;} 
           }
          
          for (int j=0;j<8;j++){
              
              switch(arr[j])
              {
                case 'A':{
                cl.print("<a>ON</a>");
                break;}
                case 'a':{
                cl.print("<a>OFF</a>");
                break;}
                case 'B':{
                cl.print("<b>ON</b>");
                break;}
                case 'b':{
                cl.print("<b>OFF</b>");
                break;}
                case 'C':{
                cl.print("<c>ON</c>");
                break;}
                case 'c':{
                cl.println("<c>OFF</c>");
                break;}
                case 'D':{
                cl.println("<d>ON</d>");
                break;}
                case 'd':{
                cl.println("<d>OFF</d>");
                break;}
                case 'E':{
                cl.println("<e>ON</e>");
                break;}
                case 'e':{
                cl.println("<e>OFF</e>");
                break;}
                case 'F':{
                cl.println("<f>ON</f>");
                break;}
                case 'f':{
                cl.println("<f>OFF</f>");
                break;}
                case 'G':{
                cl.println("<g>ON</g>");
                break;}
                case 'g':{
                cl.println("<g>OFF</g>");
                break;}
                case 'H':{
                cl.println("<h>ON</h>");
                break;}
                case 'h':{
                cl.println("<h>OFF</h>");
                break;}
              }
          }
          cl.print("</inputs>");
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}
