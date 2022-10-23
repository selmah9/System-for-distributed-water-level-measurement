
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(14,15);
int loraSelect = 4;
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 MySQL_Cursor *cur_mem;
IPAddress server_addr(192,168,200,152);  // IP of the MySQL *server* here
char user[] = "bmt";              // MySQL user login username
char password[] = "1234";        // MySQL user login password
String query;
// Sample query
char INSERT_SQL[100] = "INSERT INTO water_level.data (sensor, nivo_vode, level) VALUES ('1','1','1')";
char msg[20];
char msg2[20];
String strMsg;
String message1, message2;
EthernetClient client;
MySQL_Connection conn((Client *)&client);
float nivo;
double level1;
double level2;
double level;
int senzor;
const int aread = A0;
void setup() {
  Serial.begin(9600);
  
  while (!Serial); // wait for serial port to connect
  iskljuciLoraModul();
  Ethernet.begin(mac_addr);
  printIPAddress();
  Serial.println("Connecting to database!");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
     cur_mem = new MySQL_Cursor(&conn);

    Serial.println("Connected");
   
  }
  else
    Serial.println("Connection to database failed.");

  Serial.println("LoRa Receiver:");

 mySerial.begin(9600);
}


void loop() {
   // try to parse packet
  primiPodatke();
 //updateSerial();
}

void iskljuciLoraModul(){
  pinMode(4, OUTPUT); 
  digitalWrite(4, HIGH);
  }
void ukljuciLoraModul(){
  if (!LoRa.begin(868E6,4)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  }
void printIPAddress(){
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

void primiPodatke(){
  ukljuciLoraModul();
  while(1){

     if (LoRa.parsePacket()) {
        strMsg = "";
        while (LoRa.available()) {
              strMsg = strMsg + String((char)LoRa.read());
             
       }
       if(strMsg.indexOf("NV") > 0){
         strMsg.replace("-NV ","");
         nivo = strMsg.toFloat();
         strMsg.toCharArray(msg, 20);
         iskljuciLoraModul();
         level1 = nivo;
         level2 = (double)analogRead(aread)/1024 ;
         //level2=100;
         if(level1<level2)
          {
            senzor=1;
            level = level1*100;
            Serial.println(level);
            }
          else 
          {
            senzor = 2;
            level = level2*100;
            Serial.println(level);
            }
         upisiUBazu(senzor ,msg, level);
         ukljuciLoraModul();
         if( level < 50 ) {
          posaljiSMS();
          Serial.println("Poslano!!");
          delay(10000);
         }
        }    
      
      }
  }
  
}
void upisiUBazu(int senzor, char nivo[], int level){
    query ="INSERT INTO water_level.data (sensor, nivo_vode, level) VALUES ('" + String(senzor) + "','" + String(nivo) + "','" + String(level) + "')";
    query.toCharArray(INSERT_SQL, 100);
    cur_mem->execute(INSERT_SQL);
  }
void posaljiSMS(){

   Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+38763518858\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Proradit ce ti mapa, Kerla pomaze"); //text content
  updateSerial();
  mySerial.write(26);
}
  
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
} 
