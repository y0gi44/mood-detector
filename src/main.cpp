#include <Arduino.h>
#include <RTClib.h>
#include <WiFi.h>
#include "Historiseur.h"
#include "Votes.h"
#include <Keypad.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
{'1','2','3', 'A'},
{'4','5','6', 'B'},
{'7','8','9', 'C'},
{'*','0','#', 'D'}
};


byte pin_rows[ROWS]   = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLS] = {16, 4, 2, 15};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;



// Replace with your network credentials
const char *ssid = "Mood1";
const char *password = "123456789";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

DateTime start_date;

class HistoriseurVotes : public Historiseur<Votes> {
public:
  HistoriseurVotes() : Historiseur<Votes>(){};
};

//HistoriseurVotes historiseurVotes;
//Votes votes_en_cours;


void initRTC();
void initPins();
void initAp();

// put function declarations here:
void handleWifiClient();
void gestionBoutonsVote();
void getAndDisplayDate(WiFiClient & client);
void getAndDisplayVotes(WiFiClient & client);
void getAndDisplayStats(WiFiClient & client);

void setup()
{
  Serial.begin(115200);
  initRTC();
  initPins();
  // On stocke la date de démarrage
  start_date = rtc.now();

  initAp();
  
  server.begin();
}

void initAp(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

}

void initPins(){
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
}

void initRTC(){
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}


void loop()
{
  handleWifiClient();
  gestionBoutonsVote();
  //Serial.println("loop");
}

void gestionBoutonsVote(){
  
  if (keypad.getKeys())
  {
      for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
      {
          if ( keypad.key[i].stateChanged )   // Only find keys that have changed state.
          {
              switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                  case PRESSED:
                  msg = " PRESSED.";
              break;
                  case HOLD:
                  msg = " HOLD.";
              break;
                  case RELEASED:
                  msg = " RELEASED.";
              break;
                  case IDLE:
                  msg = " IDLE.";
              }
              Serial.print("Key ");
              Serial.print(keypad.key[i].kchar);
              Serial.println(msg);
          }
      }
  }
}

void handleWifiClient(){
  WiFiClient client = server.available(); // Listen for incoming clients
  if (client)
  {                                // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0)
            {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            }
            else if (header.indexOf("GET /26/off") >= 0)
            {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }
            else if (header.indexOf("GET /27/on") >= 0)
            {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            }
            else if (header.indexOf("GET /27/off") >= 0)
            {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            else if (header.indexOf("GET /set-time/") >= 0)
            {
              int start = header.indexOf("GET /set-time/?datetimestr=") + 27;
              int end = start + 14;
              String time = header.substring(start, end);
              Serial.println("header : " + header);
              Serial.println("time : " + time);
              

              int year = time.substring(0, 4).toInt();
              int month = time.substring(4, 6).toInt();
              int day = time.substring(6, 8).toInt();
              int hour = time.substring(8, 10).toInt();
              int min = time.substring(10, 12).toInt();
              int sec = time.substring(12, 14).toInt();
              Serial.print("year : " );
              Serial.println(year);
              Serial.print("month : " );
              Serial.println(month);
              Serial.print("day : " );
              Serial.println(day);
              Serial.print("hour : " );
              Serial.println(hour);
              Serial.print("min : ");
              Serial.println(min);
              Serial.print("sec : " );
              Serial.println(sec);
              rtc.adjust(DateTime(year, month, day, hour, min, sec));

            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            getAndDisplayDate(client);
            getAndDisplayStats(client);
            getAndDisplayVotes(client);
            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output26State == "off")
            {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output27State == "off")
            {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<form action=\"/set-time/\" method=\"get\"><label for=\"datestr\">Date time (YYYYMMDDHH24MISS) :</label><br><input type=\"text\" id=\"datetimestr\" name=\"datetimestr\"><br> <input type=\"submit\" value=\"Mettre A jour\"> </form>");
            
            

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void getAndDisplayDate(WiFiClient & client){
  DateTime now = rtc.now();

  client.print("</a><p><h2>Current Date & Time : ");

  client.print(now.year(), DEC);
  client.print('/');
  client.print(now.month(), DEC);
  client.print('/');
  client.print(now.day(), DEC);
  client.print(" (");
  client.print(daysOfTheWeek[now.dayOfTheWeek()]);
  client.print(") ");
  client.print(now.hour(), DEC);
  client.print(':');
  client.print(now.minute(), DEC);
  client.print(':');
  client.print(now.second(), DEC);
  client.print("</h2></p>");
}

void getAndDisplayStats(WiFiClient & client){
  DateTime now = rtc.now();
  TimeSpan timeSinceStart = now - start_date;

  client.print("</a><p><h3>Stats : ");
  client.print("</h3></p>");
  client.print("<p>Time since start : ");
  client.print(timeSinceStart.days());
  client.print(" days ");
  client.print(timeSinceStart.hours());
  client.print(" hours ");
  client.print(timeSinceStart.minutes());
  client.print(" minutes ");
  client.print(timeSinceStart.seconds());
  client.print(" seconds (");
  client.print(timeSinceStart.totalseconds());
  client.print(" total seconds)");
  client.print("</p>");
}


void getAndDisplayVotes(WiFiClient & client){
  client.print("</a><p><h2>Current Votes : ");
  client.print("</h2></p>");

}


void showDate(const char *txt, const DateTime &dt)
{
  Serial.print(txt);
  Serial.print(' ');
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(' ');
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);

  Serial.print(" = ");
  Serial.print(dt.unixtime());
  Serial.print("s / ");
  Serial.print(dt.unixtime() / 86400L);
  Serial.print("d since 1970");

  Serial.println();
}

void showTimeSpan(const char *txt, const TimeSpan &ts)
{
  Serial.print(txt);
  Serial.print(" ");
  Serial.print(ts.days(), DEC);
  Serial.print(" days ");
  Serial.print(ts.hours(), DEC);
  Serial.print(" hours ");
  Serial.print(ts.minutes(), DEC);
  Serial.print(" minutes ");
  Serial.print(ts.seconds(), DEC);
  Serial.print(" seconds (");
  Serial.print(ts.totalseconds(), DEC);
  Serial.print(" total seconds)");
  Serial.println();
}
