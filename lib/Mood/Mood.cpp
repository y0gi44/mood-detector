#include "Mood.h"

#include <Arduino.h>
#include <RTClib.h>
#include <WiFi.h>

#include "Historiseur.h"
#include "Votes.h"
#include "KeyBoard.h"

#include "Affichage.h"

#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>


#include <Preferences.h>
Preferences preferences;


#define RED_PIN 13
#define GREEN_PIN 12
#define YELLOW_PIN 14

unsigned long loopCount;
unsigned long startTime;
String msg;

// Replace with your network credentials
const char *ssid = "Mood1";
const char *password = "123456789";

// Set web server port number to 80
WiFiServer server(80);
WebServer ota_server(8080);


// Variable to store the HTTP request
String header;

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

DateTime start_date;

class HistoriseurVotes : public Historiseur<Votes> {
public:
  HistoriseurVotes() : Historiseur<Votes>(){};
};

//HistoriseurVotes historiseurVotes;
Votes votes_en_cours;
Keyboard clavier;
Affichage affichage;

#define MAX_CANIDATS 10
String candidats[MAX_CANIDATS] = {"Equipe 0", "Equipe 1", "Equipe 2", "Equipe 3", "Equipe 4", "Equipe 5",
                                  "Equipe 6", "Equipe 7", "Equipe 8", "Equipe 9"};
int selected_candidat;


void initRTC();
void initAp();
void initOTA();

// put function declarations here:
void handleWifiClient();
void gestionBoutonsVote();
void getAndDisplayDate(WiFiClient & client);
void getAndDisplayVotes(WiFiClient & client);
void getAndDisplayStats(WiFiClient & client);

bool isBoutonVote(char c);
int getMood(char c);

void gererVote(int mood) ;
void process_keyPressed(char key_pressed);
void gererToucheCandidat(char c);

void save_vote_en_cours();
void restore_vote_en_cours();



void save_vote_en_cours(){
  Serial.println("Sauvegarde des votes");
  preferences.putString("current", votes_en_cours.to_string());
}

void restore_vote_en_cours(){
  Serial.println("Restauration des votes");
  votes_en_cours.load_from_String(preferences.getString("current"));
}

void gererToucheCandidat(char c){ 
  int key = String(c).toInt();
  if (key >= MAX_CANIDATS || key < 0){
    Serial.println("Candidat inconnu - affectation du vote sur l'equipe par défaut 0 ");
    //affichage.afficherErreurDeSaisie();
  }else {
    Serial.print("Candidat ");
    Serial.print(key);
    Serial.print(" : ");
    Serial.println(candidats[key]);
    selected_candidat = key;
  }
}

void process_keyPressed(char key_pressed) {
  if (key_pressed == '_') {
    // Pas de touche pressée, on ne fait rien
    return;
  }

  if (isBoutonVote(key_pressed)) {
    if (selected_candidat == -1) {
      selected_candidat = 0;
    } 
    gererVote(getMood(key_pressed));
  } else {
    gererToucheCandidat(key_pressed);
  }
}


unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void Mood::init()
{
  affichage.initAffichage(false);
  Serial.begin(115200);
  initRTC();
  initAp();

  // On stocke la date de démarrage
  start_date = rtc.now();

  votes_en_cours.init(candidats, MAX_CANIDATS);
  clavier.initKeyboard(true);
  
  server.begin();

  initOTA();

  Serial.print("Setup Complete !");
  affichage.afficherInitEnCours();

  preferences.begin("mood-stats", false); 


}



void initOTA(){
  Serial.println("Start OTA initialization...");

  ota_server.on("/", []() {
    ota_server.send(200, "text/plain", "Hi! This is ElegantOTA Demo Oh yeaaahh !!!.");
  });

  ota_server.on("/download", HTTP_GET, []() {
    String s = "Candidat;Heureux;Indifferent;Triste\n";
    int allHappy= 0 , allIndifferent = 0, allSad = 0 ;
    for (int i = 0; i < MAX_CANIDATS; i++)
    {
      s+= String(votes_en_cours.getItemName(i)->getItemName());
      s+= ";";
      s+= String(votes_en_cours.getItemName(i)->getMoods(1));
      s+= ";";
      s+= String(votes_en_cours.getItemName(i)->getMoods(2));
      s+= ";";
      s+= String(votes_en_cours.getItemName(i)->getMoods(3));
      s+= "\n";
      allHappy += votes_en_cours.getItemName(i)->getMoods(1);
      allIndifferent += votes_en_cours.getItemName(i)->getMoods(2);
      allSad += votes_en_cours.getItemName(i)->getMoods(3);
    }
    s+= "----------;------------;--------------;------------\n";
    s+= "Total;";
    s+= String(allHappy);
    s+= ";";
    s+= String(allIndifferent);
    s+= ";";
    s+= String(allSad);
    s+= "\n";
    ota_server.send(200, "text/plain", s.c_str());
    
  });

  ota_server.on("/save", HTTP_GET, []() {
    save_vote_en_cours();
    ota_server.send(200, "text/plain", "Hi, vote en cours sauvegardé. ");    
  });

  ota_server.on("/restore", HTTP_GET, []() {
    save_vote_en_cours();
    ota_server.send(200, "text/plain", "Hi, vote en cours restaurés. ");    
  });

  ota_server.on("/reset", HTTP_GET, []() {
    preferences.clear();
      Serial.println("Suppression de la sauvegarde en flash");
    ota_server.send(200, "text/plain", "memory cleared. ");    
  });

  ElegantOTA.begin(&ota_server);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  ota_server.begin();
  Serial.println("OTA initialization Finished");
}

void initAp(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  //WiFi.softAP(ssid, password);
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}


void Mood::loop()
{
  handleWifiClient();
  char key_pressed = clavier.gererTouches();
  process_keyPressed(key_pressed);

  affichage.processAffichage();
  ota_server.handleClient();
  ElegantOTA.loop();
}

void gererVote(int mood) {
  Serial.print("Vote pour ");
  Serial.print(candidats[selected_candidat]);
  Serial.print(" : ");
  Serial.println(mood);

  votes_en_cours.getItemName(selected_candidat)->incrementVotes(mood);
  selected_candidat = -1;
  switch (mood)
  { 
    case 1:
      affichage.afficherVotesPrisEnCompte(RED_PIN);
      break;
    case 2:
      affichage.afficherVotesPrisEnCompte(YELLOW_PIN);
      break;
    case 3:
      affichage.afficherVotesPrisEnCompte(GREEN_PIN);
      break;
    default:
      Serial.println("Mood inconnu");
  }
  
}


void initResponseOk(WiFiClient & client){
  client.println("HTTP/1.0 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
}

void initResponseOkCsv(WiFiClient & client){
  client.println("HTTP/1.0 200 OK");
  client.println("Content-type:text/csv");
  client.println("Connection: close");
  client.println();
}

String extractTimeStringFromHeader(String & header){
  int start = header.indexOf("GET /set-time/?datetimestr=") + 27;
  int end = start + 14;
  return header.substring(start, end);
}

void handleSetTimeUrl(WiFiClient & client, String & header){
  initResponseOk(client);

  String time = extractTimeStringFromHeader(header);  
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

void handleDonwload(WiFiClient & client){
  Serial.println("Download");
  initResponseOk(client);
  client.println();
  client.println("Candidat;Happy;Indifferent;Sad");
  int allHappy= 0 , allIndifferent = 0, allSad = 0 ;

  for (int i = 0; i < MAX_CANIDATS; i++)
  {
    client.print(votes_en_cours.getItemName(i)->getItemName());
    client.print(";");
    client.print(votes_en_cours.getItemName(i)->getMoods(1));
    client.print(";");
    client.print(votes_en_cours.getItemName(i)->getMoods(2));
    client.print(";");
    client.print(votes_en_cours.getItemName(i)->getMoods(3));
    client.print("\n");
    allHappy += votes_en_cours.getItemName(i)->getMoods(1);
    allIndifferent += votes_en_cours.getItemName(i)->getMoods(2);
    allSad += votes_en_cours.getItemName(i)->getMoods(3);
  }
  client.print("----------;------------;--------------;------------\n");
  client.print("Total;"); 
  client.print(allHappy);
  client.print(";");
  client.print(allIndifferent);
  client.print(";");
  client.print(allSad);
  client.print("\n");
  client.println();

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
            if (header.indexOf("GET /set-time/") >= 0)
            {
              handleSetTimeUrl(client, header);
              break;
            } else if (header.indexOf("GET /download") >= 0)
            {
              handleDonwload(client);
              break;
            } else {
              initResponseOk(client);
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
              client.println("<a href=\"download\"><button class=\"button button-data\">Download Data</button></a>");
              
              client.println("</body></html>");

              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            }
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
  client.println("<form action=\"/set-time/\" method=\"get\"><label for=\"datestr\">Date time (YYYYMMDDHH24MISS) :</label><br><input type=\"text\" id=\"datetimestr\" name=\"datetimestr\"><br> <input type=\"submit\" value=\"Mettre A jour\"> </form>");

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
  client.print("<table style=\"width:100%\">");
  client.print("<tr>");
  client.print("<th>Candidat</th>");
  client.print("<th>Happy</th>");
  client.print("<th>Indifferent</th>");
  client.print("<th>Sad</th>");
  client.print("</tr>");
  int allHappy= 0 , allIndifferent = 0, allSad = 0 ;
  for (int i = 0; i < MAX_CANIDATS; i++)
  {
    client.print("<tr>");
    client.print("<td>");
    client.print(votes_en_cours.getItemName(i)->getItemName());
    client.print("</td>");
    client.print("<td>");
    client.print(votes_en_cours.getItemName(i)->getMoods(1));
    client.print("</td>");
    client.print("<td>");
    client.print(votes_en_cours.getItemName(i)->getMoods(2));
    client.print("</td>");
    client.print("<td>");
    client.print(votes_en_cours.getItemName(i)->getMoods(3));
    client.print("</td>");
    client.print("</tr>");
    allHappy += votes_en_cours.getItemName(i)->getMoods(1);
    allIndifferent += votes_en_cours.getItemName(i)->getMoods(2);
    allSad += votes_en_cours.getItemName(i)->getMoods(3);
  }
  client.print("<tr>");
  client.print("<td> Total </td>");
  client.print("<td>");
  client.print(allHappy);
  client.print("</td>");
  client.print("<td>");
  client.print(allIndifferent);
  client.print("</td>");
  client.print("<td>");
  client.print(allSad);
  client.print("</td>");

  client.print("</tr>");
  client.print("</table>");
}


bool isBoutonVote(char c){
  return c == 'A' || c == 'B' || c == 'C' || c == 'D';
}

int getMood(char c){
  switch (c)
  {
  case 'A':
    return 1;
    break;
  case 'B':
    return 2;
    break;
  case 'C':
    return 3;
    break;
  case 'D':
    return 4;
    break;
  default:
    return -1;
    break;
  }
}




Mood::Mood(/* args */)
{
}

Mood::~Mood()
{
}



