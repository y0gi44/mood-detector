#include "Mood.h"

#include <Arduino.h>
#include <RTClib.h>
#include <WiFi.h>

#include "Historiseur.h"
#include "Votes.h"
#include "KeyBoard.h"

#include "Affichage.h"
#include "AppConfig.h"

#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>


#include <Preferences.h>
Preferences preferences;

String version = "0.0.4" ;

#define RED_PIN 13
#define GREEN_PIN 14
#define YELLOW_PIN 12

unsigned long loopCount;
unsigned long startTime;
String msg;

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
/* Legacy dashboard block retained only as migration reference.
              client.println("<!doctype html><html lang=\"fr\"><head>");
              client.println("<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">");
              client.println("<meta name=\"theme-color\" content=\"#102a43\"><title>Mood Detector</title>");
              client.println("<style>");
              client.println(":root{color-scheme:dark;--ink:#102a43;--paper:#f4f7f9;--muted:#627d98;--line:#d9e2ec;--accent:#e76f51;--green:#2a9d8f;--yellow:#e9c46a;--red:#e76f51}");
              client.println("*{box-sizing:border-box}body{margin:0;background:var(--paper);color:var(--ink);font:16px/1.5 system-ui,-apple-system,BlinkMacSystemFont,\"Segoe UI\",sans-serif}main{max-width:1080px;margin:auto;padding:20px 14px 44px}.topbar{display:flex;align-items:flex-start;justify-content:space-between;gap:16px;margin-bottom:22px}.eyebrow{margin:0;color:var(--accent);font-size:12px;font-weight:800;letter-spacing:1.5px;text-transform:uppercase}.title{margin:3px 0 0;font-size:clamp(28px,7vw,46px);line-height:1.05;letter-spacing:0}.subtitle{margin:9px 0 0;color:var(--muted)}.badge{background:var(--ink);color:white;border-radius:999px;padding:7px 11px;font-size:12px;white-space:nowrap}.panel{background:white;border:1px solid var(--line);border-radius:8px;box-shadow:0 8px 24px rgba(16,42,67,.08);padding:18px;margin-top:16px}.panel-head{display:flex;align-items:baseline;justify-content:space-between;gap:12px;border-bottom:1px solid var(--line);padding-bottom:12px;margin-bottom:14px}.panel h2{margin:0;font-size:20px}.panel-note{margin:0;color:var(--muted);font-size:13px}.stats{display:grid;grid-template-columns:repeat(3,1fr);gap:10px}.stat{border-left:4px solid var(--accent);padding:12px;background:#f8fafc}.stat:nth-child(2){border-color:var(--green)}.stat:nth-child(3){border-color:var(--yellow)}.stat-label{display:block;color:var(--muted);font-size:12px;text-transform:uppercase;font-weight:700}.stat-value{display:block;margin-top:3px;font-size:25px;font-weight:800}.table-wrap{overflow-x:auto}table{width:100%;border-collapse:collapse;min-width:560px}th,td{padding:12px 10px;text-align:right;border-bottom:1px solid var(--line)}th:first-child,td:first-child{text-align:left}th{color:var(--muted);font-size:12px;text-transform:uppercase}tbody tr:hover{background:#f8fafc}.mood-happy{color:#21867a;font-weight:700}.mood-neutral{color:#a57800;font-weight:700}.mood-sad{color:#c94c32;font-weight:700}.total td{border-top:2px solid var(--ink);border-bottom:0;font-weight:800}.actions{display:flex;flex-wrap:wrap;gap:10px}.button{display:inline-block;border:0;border-radius:6px;background:var(--ink);color:white;padding:11px 15px;font:inherit;font-weight:700;text-decoration:none;cursor:pointer}.button:hover{background:#1d4e73}.button.secondary{background:#e6eef5;color:var(--ink)}.button.danger{background:#a33b2b}.forms{display:grid;grid-template-columns:1fr auto;gap:10px;align-items:end}.field label{display:block;color:var(--muted);font-size:13px;font-weight:700;margin-bottom:5px}.field input{width:100%;border:1px solid #bcccdc;border-radius:5px;padding:11px;font:inherit;color:var(--ink);background:white}.footer{color:var(--muted);font-size:13px;text-align:center;margin-top:24px}@media(max-width:600px){main{padding:16px 10px 32px}.topbar{display:block}.badge{display:inline-block;margin-top:14px}.stats{grid-template-columns:1fr}.panel{padding:14px}.forms{grid-template-columns:1fr}.button{width:100%;text-align:center}}");
              client.println("</style></head><body><main>");
              client.println("<header class=\"topbar\"><div><p class=\"eyebrow\">Plateforme locale</p><h1 class=\"title\">Mood Detector</h1><p class=\"subtitle\">Suivi des votes de l'equipe en temps reel.</p></div><span class=\"badge\">ESP32 · hors ligne</span></header>");
void initRTC();
void initAp();
void initOTA();
              client.println("<section class=\"panel\"><div class=\"panel-head\"><h2>Outils</h2><p class=\"panel-note\">Actions locales</p></div><div class=\"actions\"><a class=\"button\" href=\"/download\">Telecharger le CSV</a><a class=\"button secondary\" href=\"/save\">Sauvegarder les votes</a><a class=\"button secondary\" href=\"/restore\">Restaurer les votes</a><a class=\"button secondary\" href=\"http://192.168.4.1:8080/update\">Ouvrir OTA</a></div></section>");
              client.println("<section class=\"panel\"><div class=\"panel-head\"><h2>Regler l'heure</h2><p class=\"panel-note\">Format YYYYMMDDHHMMSS</p></div><form class=\"forms\" action=\"/set-time/\" method=\"get\"><div class=\"field\"><label for=\"datetimestr\">Date et heure</label><input id=\"datetimestr\" name=\"datetimestr\" inputmode=\"numeric\" pattern=\"[0-9]{14}\" maxlength=\"14\" placeholder=\"20260924143000\" required></div><button class=\"button\" type=\"submit\">Mettre a jour</button></form></section>");
              client.println("<p class=\"footer\">Connecte au point d'acces Mood1 · Portail local 192.168.4.1</p></main></body></html>");
*/
};

Votes votes_en_cours;
Keyboard clavier;
Affichage affichage;

#define MAX_CANIDATS 10
String candidats[MAX_CANIDATS] = {"Equipe 0", "Equipe 1", "Equipe 2", "Equipe 3", "Equipe 4", "Equipe 5",
                                  "Equipe 6", "Equipe 7", "Equipe 8", "Equipe 9"};
int selected_candidat = -1;

void initRTC();
void initAp();
void initOTA();
void handleWifiClient();
void gestionBoutonsVote();
void getAndDisplayDate(WiFiClient & client);
void getAndDisplayVotes(WiFiClient & client);
void getAndDisplayStats(WiFiClient & client);
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
String getBasicAuthToken();
bool isMainServerAuthorized(const String & request);



void save_vote_en_cours(){
  Serial.println("Sauvegarde des votes");
  preferences.putString("current", votes_en_cours.to_string());
}

void restore_vote_en_cours(){
  Serial.println("Restauration des votes");
  votes_en_cours.load_from_String(preferences.getString("current"));
}

String getBasicAuthToken(){
  const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String input = String(AppConfig::webUser) + ":" + AppConfig::webPassword;
  String encoded;
  for (unsigned int i = 0; i < input.length(); i += 3) {
    const unsigned char first = input.charAt(i);
    const unsigned char second = i + 1 < input.length() ? input.charAt(i + 1) : 0;
    const unsigned char third = i + 2 < input.length() ? input.charAt(i + 2) : 0;
    encoded += alphabet[first >> 2];
    encoded += alphabet[((first & 0x03) << 4) | (second >> 4)];
    encoded += i + 1 < input.length() ? alphabet[((second & 0x0f) << 2) | (third >> 6)] : '=';
    encoded += i + 2 < input.length() ? alphabet[third & 0x3f] : '=';
  }
  return encoded;
}

bool isMainServerAuthorized(const String & request){
  const String marker = "Authorization: Basic ";
  const int start = request.indexOf(marker);
  if (start < 0) {
    return false;
  }
  const int tokenStart = start + marker.length();
  const int tokenEnd = request.indexOf('\n', tokenStart);
  String token = request.substring(tokenStart, tokenEnd < 0 ? request.length() : tokenEnd);
  token.trim();
  return token == getBasicAuthToken();
}

void sendMainAuthRequired(WiFiClient & client){
  client.println("HTTP/1.0 401 Unauthorized");
  client.println("WWW-Authenticate: Basic realm=\"Mood Detector\"");
  client.println("Content-Type: text/plain; charset=utf-8");
  client.println("Connection: close");
  client.println();
  client.println("Authentication required");
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
      Serial.println("Veuillez selectionner un candidat d'abord");
      affichage.afficherErreurDeSaisie();
      return;
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
  Serial.begin(115200);
  affichage.initAffichage(false);
  initRTC();
  initAp();

  // On stocke la date de démarrage
  start_date = rtc.now();

  votes_en_cours.init(candidats, MAX_CANIDATS);
  clavier.initKeyboard(true);
  if (!preferences.begin("mood-stats", false)) {
    Serial.println("Impossible d'ouvrir les preferences");
  } else {
    restore_vote_en_cours();
  }
  
  server.begin();

  initOTA();

  Serial.print("Setup Complete !");
  affichage.afficherInitEnCours();
}



void initOTA(){
  Serial.println("Start OTA initialization...");

  ota_server.on("/", []() {
    if (!ota_server.authenticate(AppConfig::webUser, AppConfig::webPassword)) {
      ota_server.requestAuthentication();
      return;
    }
    ota_server.send(200, "text/plain", "Hi! This is ElegantOTA Demo Oh yeaaahh !!!. version "+version);
  });

  ota_server.on("/download", HTTP_GET, []() {
    if (!ota_server.authenticate(AppConfig::webUser, AppConfig::webPassword)) {
      ota_server.requestAuthentication();
      return;
    }
    String s = "Version;" + String(version);
    s+= "----------;------------;--------------;------------\n";
    s += "Candidat;Heureux;Indifferent;Triste\n";
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
    s+= "----------;------------;--------------;------------\n";
    ota_server.send(200, "text/plain", s.c_str());
    
  });

  ota_server.on("/save", HTTP_GET, []() {
    if (!ota_server.authenticate(AppConfig::webUser, AppConfig::webPassword)) {
      ota_server.requestAuthentication();
      return;
    }
    save_vote_en_cours();
    ota_server.send(200, "text/plain", "Hi, vote en cours sauvegardé. ");    
  });

  ota_server.on("/restore", HTTP_GET, []() {
    if (!ota_server.authenticate(AppConfig::webUser, AppConfig::webPassword)) {
      ota_server.requestAuthentication();
      return;
    }
    restore_vote_en_cours();
    ota_server.send(200, "text/plain", "Hi, vote en cours restaurés. ");    
  });

  ota_server.on("/reset", HTTP_GET, []() {
    if (!ota_server.authenticate(AppConfig::webUser, AppConfig::webPassword)) {
      ota_server.requestAuthentication();
      return;
    }
    preferences.clear();
    votes_en_cours.resetVotes();
      Serial.println("Suppression de la sauvegarde en flash");
    ota_server.send(200, "text/plain", "memory cleared. ");    
  });

  ElegantOTA.begin(&ota_server, AppConfig::webUser, AppConfig::webPassword);    // Start ElegantOTA
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
  WiFi.softAP(AppConfig::wifiSsid, AppConfig::wifiPassword);

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
  if (selected_candidat < 0 || selected_candidat >= MAX_CANIDATS ||
      mood < 1 || mood > MAX_MOODS) {
    Serial.println("Vote invalide");
    affichage.afficherErreurDeSaisie();
    selected_candidat = -1;
    return;
  }

  Serial.print("Vote pour ");
  Serial.print(candidats[selected_candidat]);
  Serial.print(" : ");
  Serial.println(mood);

  votes_en_cours.getItemName(selected_candidat)->incrementVotes(mood);
  selected_candidat = -1;
  switch (mood)
  { 
    case 1:
      affichage.afficherVotesPrisEnCompte(GREEN_PIN);
      break;
    case 2:
      affichage.afficherVotesPrisEnCompte(YELLOW_PIN);
      break;
    case 3:
      affichage.afficherVotesPrisEnCompte(RED_PIN);
      break;
    default:
      Serial.println("Mood inconnu");
  }
}

void initResponseOk(WiFiClient & client){
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Cache-Control: no-store, no-cache, must-revalidate");
  client.println("Pragma: no-cache");
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
  const int markerStart = header.indexOf("GET /set-time/?datetimestr=");
  if (markerStart < 0) {
    return String();
  }
  int start = markerStart + 27;
  int end = start + 14;
  return header.substring(start, end);
}

bool isValidTimeString(const String & time){
  if (time.length() != 14) {
    return false;
  }
  for (unsigned int i = 0; i < time.length(); i++) {
    if (!isDigit(time.charAt(i))) {
      return false;
    }
  }
  return true;
}

void handleSetTimeUrl(WiFiClient & client, String & header){
  String time = extractTimeStringFromHeader(header);  
  if (!isValidTimeString(time)) {
    client.println("HTTP/1.0 400 Bad Request");
    client.println("Content-type:text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Invalid date format");
    return;
  }

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
  DateTime newDate(year, month, day, hour, min, sec);
  if (!newDate.isValid()) {
    client.println("HTTP/1.0 400 Bad Request");
    client.println("Content-type:text/plain");
    client.println("Connection: close");
    client.println();
    client.println("Invalid date value");
    return;
  }

  rtc.adjust(newDate);
  initResponseOk(client);
  client.println("Date updated");
  
}

void handleDonwload(WiFiClient & client){
  Serial.println("Download");
  initResponseOk(client);
  client.println();

  client.print("----------;------------;--------------;------------\n");
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
    header = "";
    header.reserve(1024);
    String currentLine = "";       // make a String to hold incoming data from the client
    unsigned long lastActivity = millis();
    while (client.connected())
    { // loop while the client's connected
      if (millis() - lastActivity > 2000) {
        break;
      }
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        lastActivity = millis();
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (header.length() > 1024) {
          break;
        }
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            if (!isMainServerAuthorized(header))
            {
              sendMainAuthRequired(client);
              break;
            }

            if (header.indexOf("GET /set-time/") >= 0)
            {
              handleSetTimeUrl(client, header);
              break;
            } else if (header.indexOf("GET /download") >= 0)
            {
              handleDonwload(client);
              break;
            }else if (header.indexOf("GET /save") >= 0)
            {
              save_vote_en_cours();
               initResponseOk(client);
               client.println();
               client.print("sauvegarde des votes");
              break;
            }else if (header.indexOf("GET /restore") >= 0)
            {
              restore_vote_en_cours();
              initResponseOk(client);
               client.println();
               client.print("restauration des votes");
              break;
            }else if (header.indexOf("GET /reset?confirm=1") >= 0)
            {
              preferences.clear();
              votes_en_cours.resetVotes();
              initResponseOk(client);
              client.println();
              client.print("votes reinitialises");
              break;
            }else if (header.indexOf("GET /reset") >= 0)
            {
              initResponseOk(client);
              client.println("<!doctype html><html lang=\"fr\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Confirmer la reinitialisation</title></head><body style=\"font:16px system-ui,sans-serif;max-width:480px;margin:40px auto;padding:0 16px\">");
              client.println("<h1>Reinitialiser les votes ?</h1><p>Cette action efface tous les votes en memoire et en sauvegarde flash. Elle est irreversible.</p>");
              client.println("<p><a href=\"/reset?confirm=1\" style=\"display:inline-block;padding:11px 14px;border-radius:6px;background:#c94c32;color:white;text-decoration:none;font-weight:700\">Confirmer la reinitialisation</a> <a href=\"/\" style=\"margin-left:8px\">Annuler</a></p></body></html>");
              break;
            } else {
              initResponseOk(client);
              client.println("<!doctype html><html lang=\"fr\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><meta http-equiv=\"refresh\" content=\"30\"><title>Mood Detector</title>");
              client.println("<style>:root{--ink:#102a43;--paper:#f4f7f9;--muted:#627d98;--line:#d9e2ec;--accent:#e76f51;--green:#2a9d8f;--yellow:#e9c46a;--red:#c94c32}*{box-sizing:border-box}body{margin:0;background:var(--paper);color:var(--ink);font:16px/1.5 system-ui,sans-serif}main{max-width:1080px;margin:auto;padding:20px 14px 44px}.top{display:flex;justify-content:space-between;gap:12px;margin-bottom:20px}.eyebrow{margin:0;color:var(--accent);font-size:12px;font-weight:800;text-transform:uppercase;letter-spacing:1px}h1{margin:3px 0;font-size:clamp(30px,7vw,46px);line-height:1.05}.subtitle,.note{color:var(--muted)}.badge{background:var(--ink);color:white;border-radius:999px;padding:7px 11px;font-size:12px;height:max-content;white-space:nowrap}.panel{background:white;border:1px solid var(--line);border-radius:8px;padding:18px;margin-top:16px;box-shadow:0 8px 24px #102a4314}.head{display:flex;justify-content:space-between;gap:10px;border-bottom:1px solid var(--line);padding-bottom:10px;margin-bottom:14px}.head h2{margin:0;font-size:20px}.head p{margin:0;font-size:13px}.stats{display:grid;grid-template-columns:repeat(3,1fr);gap:10px}.stat{padding:12px;border-left:4px solid var(--accent);background:#f8fafc}.stat:nth-child(2){border-color:var(--green)}.stat:nth-child(3){border-color:var(--yellow)}.stat strong{display:block;font-size:25px}.table{overflow:auto}table{width:100%;min-width:540px;border-collapse:collapse}th,td{padding:11px 9px;border-bottom:1px solid var(--line);text-align:right}th:first-child,td:first-child{text-align:left}th{font-size:12px;color:var(--muted);text-transform:uppercase}.happy{color:#21867a;font-weight:700}.neutral{color:#a57800;font-weight:700}.sad{color:var(--red);font-weight:700}.total td{border-top:2px solid var(--ink);font-weight:800}.actions{display:flex;flex-wrap:wrap;gap:9px}.button{display:inline-block;padding:11px 14px;border-radius:6px;background:var(--ink);color:white;text-decoration:none;font-weight:700}.button.alt{background:#e6eef5;color:var(--ink)}.button.danger{background:var(--red)}.form{display:grid;grid-template-columns:1fr auto;gap:10px;align-items:end}.form label{display:block;color:var(--muted);font-size:13px;font-weight:700}.form input{width:100%;padding:11px;border:1px solid #bcccdc;border-radius:5px;font:inherit}@media(max-width:600px){main{padding:16px 10px 32px}.top{display:block}.badge{display:inline-block;margin-top:12px}.stats{grid-template-columns:1fr}.form{grid-template-columns:1fr}.button{width:100%;text-align:center}}</style></head><body><main>");
              client.println("<header class=\"top\"><div><p class=\"eyebrow\">Plateforme locale</p><h1>Mood Detector</h1><p class=\"subtitle\">Suivi des votes depuis le point d'acces ESP32.</p></div><span class=\"badge\">Hors ligne</span></header>");
              getAndDisplayDate(client);
              getAndDisplayStats(client);
              getAndDisplayVotes(client);
              client.println("<section class=\"panel\"><div class=\"head\"><h2>Actions</h2><p class=\"note\">Disponibles localement</p></div><div class=\"actions\"><a class=\"button\" href=\"/download\">Telecharger CSV</a><a class=\"button alt\" href=\"/save\">Sauvegarder</a><a class=\"button alt\" href=\"/restore\">Restaurer</a><a class=\"button danger\" href=\"/reset\">Reinitialiser</a><a class=\"button alt\" href=\"http://192.168.4.1:8080/update\">OTA</a></div></section>");
              client.println("<section class=\"panel\"><div class=\"head\"><h2>Regler l'heure</h2><p class=\"note\">YYYYMMDDHHMMSS</p></div><form class=\"form\" action=\"/set-time/\" method=\"get\"><div><label for=\"datetimestr\">Date et heure</label><input id=\"datetimestr\" name=\"datetimestr\" inputmode=\"numeric\" pattern=\"[0-9]{14}\" maxlength=\"14\" placeholder=\"20260924143000\" required></div><button class=\"button\" type=\"submit\">Mettre a jour</button></form></section><p class=\"note\" style=\"text-align:center;font-size:13px\">Wi-Fi Mood1 · 192.168.4.1</p></main></body></html>");

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

  client.print("<section class=\"panel\"><div class=\"panel-head\"><h2>Horloge</h2><p class=\"panel-note\">");
  client.print(daysOfTheWeek[now.dayOfTheWeek()]);
  client.print("</p></div><p style=\"font-size:clamp(28px,8vw,48px);font-weight:800;margin:0;letter-spacing:0\">");
  if (now.hour() < 10) client.print('0');
  client.print(now.hour());
  client.print(':');
  if (now.minute() < 10) client.print('0');
  client.print(now.minute());
  client.print(':');
  if (now.second() < 10) client.print('0');
  client.print(now.second());
  client.print("</p><p class=\"panel-note\">");
  client.print(now.year());
  client.print('-');
  if (now.month() < 10) client.print('0');
  client.print(now.month());
  client.print('-');
  if (now.day() < 10) client.print('0');
  client.print(now.day());
  client.println("</p></section>");

}

void getAndDisplayStats(WiFiClient & client){
  DateTime now = rtc.now();
  TimeSpan timeSinceStart = now - start_date;

  client.print("<section class=\"panel\"><div class=\"panel-head\"><h2>Activite</h2><p class=\"panel-note\">Depuis le demarrage</p></div><div class=\"stats\"><div class=\"stat\"><span class=\"stat-label\">Jours</span><span class=\"stat-value\">");
  client.print(timeSinceStart.days());
  client.print("</span></div><div class=\"stat\"><span class=\"stat-label\">Heures</span><span class=\"stat-value\">");
  client.print(timeSinceStart.hours());
  client.print("</span></div><div class=\"stat\"><span class=\"stat-label\">Secondes totales</span><span class=\"stat-value\">");
  client.print(timeSinceStart.totalseconds());
  client.println("</span></div></div></section>");
}


void getAndDisplayVotes(WiFiClient & client){
  client.print("<section class=\"panel\"><div class=\"panel-head\"><h2>Votes par equipe</h2><p class=\"panel-note\">A, B et C sur la borne</p></div><div class=\"table-wrap\"><table><thead><tr><th>Equipe</th><th class=\"mood-happy\">Mood 1</th><th class=\"mood-neutral\">Mood 2</th><th class=\"mood-sad\">Mood 3</th></tr></thead><tbody>");
  int allHappy= 0 , allIndifferent = 0, allSad = 0 ;
  for (int i = 0; i < MAX_CANIDATS; i++)
  {
    client.print("<tr><td>");
    client.print(votes_en_cours.getItemName(i)->getItemName());
    client.print("</td>");
    client.print("<td class=\"mood-happy\">");
    client.print(votes_en_cours.getItemName(i)->getMoods(1));
    client.print("</td>");
    client.print("<td class=\"mood-neutral\">");
    client.print(votes_en_cours.getItemName(i)->getMoods(2));
    client.print("</td>");
    client.print("<td class=\"mood-sad\">");
    client.print(votes_en_cours.getItemName(i)->getMoods(3));
    client.print("</td>");
    client.print("</tr>");
    allHappy += votes_en_cours.getItemName(i)->getMoods(1);
    allIndifferent += votes_en_cours.getItemName(i)->getMoods(2);
    allSad += votes_en_cours.getItemName(i)->getMoods(3);
  }
  client.print("<tr class=\"total\">");
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

  client.print("</tr></tbody></table></div></section>");
}


bool isBoutonVote(char c){
  return c == 'A' || c == 'B' || c == 'C' ;
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



