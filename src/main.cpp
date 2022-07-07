#include <Arduino.h>
//BTS7960
#define RPWM D5
#define LPWM D6
#define speedStart 0
#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>



unsigned long messageInterval = 2000;
bool connected = false;
const char* ssid = "Robolab";
const char* password = "wifi123123123";
//const char* websockets_server_host = "servicerobot.pro"; // Enter server adress
const char* websockets_server_host = "192.168.0.101"; // Enter server adress
const uint16_t websockets_server_port = 8081; // Enter server port

using namespace websockets;

unsigned long lastUpdate = millis();
unsigned long lastUpdate15 = millis();

String output;

unsigned long timing;

StaticJsonDocument<200> doc;
StaticJsonDocument<512> doc2;

int stop = 0;
const char* method = "";
const char* username = "";
float messageL = 0;
float messageR = 0;
// int posMessage = 90;
// int posMessage2 = 90;
int accel = 1;

WebsocketsClient client;

void onMessageCallback(WebsocketsMessage messageSocket) {
    Serial.print("Got Message: ");
    Serial.println(messageSocket.data());

    DeserializationError error = deserializeJson(doc, messageSocket.data());
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    method = doc["method"];

    if(String(method) == "connection"){
        Serial.printf("[WSc] WStype_CONNECTED\n");
    }

    if(String(method) == "messages"){
        stop = doc["stop"];
        accel = doc["accel"];

        // messageL = doc["messageL"];
        // messageR = doc["messageR"];
        // Serial.printf("messageLjoyStick = %s\n", String((messageL)+speedStart));
        // Serial.printf("messageRjoyStick = %s\n", String((messageR)+speedStart));
        // doc2["method"] = "messages";
        // doc2["messageL"] = messageL;
        // doc2["messageR"] = messageR;
        // String output = doc2.as<String>();
        // client.send(output);


        // message = message * 27;
        // message2 = message2 * 75;
        //message = message * 255;
        //message2 = message2 * 255;
        //message = map(message, 0, 0, 0, 255);
        //message2 = map(message2, 0, 0, 0, 255);

        // if(stop == 1){
        //     // posMessage = message;
        //     // posMessage2 = message;
        //     analogWrite(RPWM, message);
        //     analogWrite(RPWM, message2);
        //     stop = 0;
        // }
    }
    if(String(method) == "messagesL"){
        stop = doc["stop"];
        accel = doc["accel"];
        messageL = doc["messageL"];
        //messageR = doc["messageR"];
        Serial.printf("messageL = %s\n", String(messageL));
        Serial.printf("messageR = %s\n", String(messageR));
        doc2["method"] = "messagesL";
        doc2["messageL"] = messageL;
        doc2["messageR"] = messageR;
        String output = doc2.as<String>();
        client.send(output);
    }

        if(String(method) == "messagesR"){
        stop = doc["stop"];
        accel = doc["accel"];
        //messageL = doc["messageL"];
        messageR = doc["messageR"];
        Serial.printf("messageL = %s\n", String(messageL));
        Serial.printf("messageR = %s\n", String(messageR));
        doc2["method"] = "messagesR";
        doc2["messageL"] = messageL;
        doc2["messageR"] = messageR;
        String output = doc2.as<String>();
        client.send(output);
    }

}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        connected = true;
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        connected = false;
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}


void socketSetup(){
    // run callback when messages are received
    client.onMessage(onMessageCallback);
    // run callback when events are occuring
    client.onEvent(onEventsCallback);
    // Connect to server
    client.connect(websockets_server_host, websockets_server_port, "/");
    // Send a message
    //client.send("Hello Server");
    doc2["method"] = "connection";
    //doc2["id"] = "b078167f69934795e54a54dc831acea8|a46d12213abfad52db817c17e1fec1ae";
    doc2["id"] = "123";
    output = doc2.as<String>();
    client.send(output);
    // Send a ping
    client.ping();
};

void setup() {
    //BTS7960
    pinMode(RPWM,OUTPUT);
    analogWrite(RPWM, 0);
    pinMode(LPWM,OUTPUT);
    analogWrite(LPWM, 0);

    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    };

    socketSetup();

    // message = message * 90;
    // message2 = message2 * 90;

    // message = map(message, 0, 90, 90, 180);
    // message2 = map(message2, 0, 90, 90, 180);

}

// void socetConnected(){
//     client.connect(websockets_server_host, websockets_server_port, "/");
//     Serial.println("[WSc] SENT: Simple js client message!!");
//     doc2["method"] = "connection";
//     doc2["id"] = "b078167f69934795e54a54dc831acea8|a46d12213abfad52db817c17e1fec1ae";
//     output = doc2.as<String>();
//     client.send(output);  
// };

void loop(){

    client.poll();

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi.reconnect()-----------------------------------");
        Serial.println("WiFi.reconnect()-----------------------------------");
        //WiFi.disconnect();
        //WiFi.reconnect();
        ESP.restart();
        socketSetup();
        // ESP.reset(); 
        // WiFi.disconnect();
        // WiFi.reconnect();
    };

    if (lastUpdate + messageInterval < millis()){
        if (connected == false){
            Serial.printf(", connected =================================== %s\n", String(connected));
            socketSetup();
        };
        lastUpdate = millis();
    };

    if (lastUpdate15 + 10000 < millis()){
        Serial.printf("millis() = %s", String(millis()));
        Serial.printf(", WiFi.status() = %s", String(WiFi.status()));
        Serial.printf(", WL_CONNECTED = %s", String(WL_CONNECTED));
        Serial.printf(", connected = %s\n", String(connected));


        doc2["method"] = "messages";
        doc2["id"] = "123";
        doc2["messageL"] = String(messageL);
        doc2["messageR"] = String(messageR);
        output = doc2.as<String>();
        client.send(output);

        lastUpdate15 = millis();
    };


    if(messageL > 0){
        analogWrite(LPWM, (messageL)+speedStart);
    }

    if(messageL == 0){
        analogWrite(LPWM, 0);
    }

    if(messageR > 0){
        analogWrite(RPWM, (messageR)+speedStart);
    }

    if(messageR == 0){
        analogWrite(RPWM, 0);
    }



    // if(message2 > posMessage2){
    //   analogWrite(RPWM, posMessage2);

    //     posMessage2++;                 
    // }
    // else if(message2 < posMessage2){
    //     analogWrite(RPWM, posMessage2);   
    //     posMessage2--; 
    // }

}