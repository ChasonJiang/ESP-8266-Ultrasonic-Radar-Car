#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>  //sg90舵机
// #include <DHT.h>  //温度检测模块
#include "HCSR04.h"
#include "html.h"
#include "Controller.h"
#include "GPIO.h"
#define ARDUINOJSON_USE_DOUBLE 1

//input your wifi ssid and password
const char *ssid="WiFi";
const char *password="Jh201020";

char json_output[200];
GPIO *gpio=new GPIO();
Controller controller(gpio);

// Server
AsyncWebServer server(80); 
AsyncWebSocket ws("/");

//Ticker radar_scan_timer;
Ticker massage_timer;

UltraSonicDistanceSensor distanceSensor(gpio->TrigPin, gpio->EchoPin);
Servo myservo;


// 超声波雷达扫描
int degree=180;
int flag=1;
double distance = 0.0;
void radar_scan(){
  if(flag && degree<180){
    myservo.write(degree); 
    degree++;
  }else if(degree==180){
    myservo.write(degree); 
    flag=0;
    degree--;
  }else if(degree==0){
    myservo.write(degree); 
    flag=1;
    degree++;
    }else if(!flag && degree>0){
    myservo.write(degree); 
    degree--;
  }
  distance=distanceSensor.measureDistanceCm(26.0);
//  Serial.printf("%lf",distance);
//  Serial.printf(" %f\n",degree/2.0);
  
}

// 接收网页端的摇杆坐标和速度上限
void receiveMassage(char* input){
  StaticJsonDocument<48> doc;
  DeserializationError error = deserializeJson(doc, input);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }else{
    controller.pos.x = doc["x"];
    controller.pos.y = doc["y"];
    controller.speedCar = doc["power"];
//    Serial.println("Received successfully!");
  }

}

// 发送数据到网页端
StaticJsonDocument<200> send_doc;
void sendMassage(){
  
    radar_scan();
    send_doc["rssi"] = WiFi.RSSI();
    send_doc["distance"] = distance;
    send_doc["degree"] = degree/2.0;

    serializeJson(send_doc, json_output);
    ws.printfAll(json_output);
//    Serial.println("Sent successfully!");
    
}

// WebSocket事件回调函数
void onEventHandle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT) // 有客户端建立连接
  {
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Client %u Connected!\n", client->id()); // 向客户端发送数据
    client->ping();                                    // 向客户端发送ping
  }
  else if (type == WS_EVT_DISCONNECT) // 有客户端断开连接
  {
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  }
  else if (type == WS_EVT_ERROR) // 发生错误
  {
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
  }
  else if (type == WS_EVT_PONG) // 收到客户端对服务器发出的ping进行应答（pong消息）
  {
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
  }
  else if (type == WS_EVT_DATA) // 收到来自客户端的数据
  {
    data[len] = 0;
    receiveMassage((char *)data);
  }
}

void htmlroot(AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(200, "text/html", html);
  request->send(response);
}

void setup()
{
  pinMode(gpio->ENA,OUTPUT);
  pinMode(gpio->ENB,OUTPUT);
  pinMode(gpio->IN1,OUTPUT);
  pinMode(gpio->IN2,OUTPUT);
  pinMode(gpio->IN3,OUTPUT);
  pinMode(gpio->IN4,OUTPUT);
  myservo.attach(gpio->ServoPin);
  myservo.write(180);

  Serial.begin(2000000);
  Serial.println();
//  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

//  WiFi.softAP(ssid, password);
//    IPAddress IP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(IP);
//
//  // Print ESP8266 Local IP Address
//  Serial.println(WiFi.localIP());

  ws.onEvent(onEventHandle); // 绑定回调函数
  server.addHandler(&ws);    // 将WebSocket添加到服务器中

  server.on("/", HTTP_GET, htmlroot);

  server.begin(); //启动服务器

  // 设置定时器，每0.04s发送数据
  massage_timer.attach(0.04,sendMassage);

//  radar_scan_timer.attach(0.020,radar_scan);
  Serial.println("Web server started");
}

void loop()
{
  controller.control(); //控制小车
  ws.cleanupClients();     // 关闭过多的WebSocket连接以节省资源
}
