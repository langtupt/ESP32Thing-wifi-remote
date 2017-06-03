/////////////////////////////////////////////////////////////////////
//This code uses the input from a remote to control groups of switches via wifi.
//A timer checks every 0.2 seconds if the state of the remote's buttons have changed.
//If the state has changed, the new state is sent to the switch's virtual variable V0 using Blynk's bridge widget.
///////////////////////////////////////////////////////////////////////

#define BLYNK_NO_BUILTIN   // Disable built-in analog & digital pin operations
#define BLYNK_NO_FLOAT     // Disable float operations

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <esp_wifi.h>

// WiFi credentials:
char auth[] = "your_auth_token";
char* ssid[] = {"SSID1", "SSID2"};//replace SSID1 and SSID2 by your SSID names. If a 3rd SSID name has to be added, nbKnownAP = 3
char* pass[] = {"PASS1", "PASS2"};//replace PASS1 and PASS2 by your passwords. If a 3rd password has to be added, nbKnownAP = 3
const int nbKnownAP = 2;//number of elements in ssid[] and pass[]
char ssidChosen[20] = "\0";//increase the number of characters if longest ssid is longer than 20 characters
char passChosen[20] = "\0";//increase the number of characters if longest pass is longer than 20 characters

IPAddress ip (45,55,96,146);//replace with your local server

//Global variables:
const int buttonPin[6] = {2, 27, 23, 34, 22, 32};//pins of the ESP32 on which buttons are connected
short int buttonState[6] = {0, 0, 0, 0, 0, 0};//state of each button (0=off, 1=on)
bool isFirstConnect = true;//true if the remote hasn't been connected to wifi yet

BlynkTimer timer;
int timerId;

WidgetBridge bridge0(V0);//bridge to communicate with switch 0
WidgetBridge bridge1(V1);//bridge to communicate with switch 1
WidgetBridge bridge2(V2);//bridge to communicate with switch 2
WidgetBridge bridge3(V3);//bridge to communicate with switch 3
WidgetBridge bridge4(V4);//bridge to communicate with switch 4
WidgetBridge bridge5(V5);//bridge to communicate with switch 5
WidgetBridge bridge6(V6);//bridge to communicate with switch 6
WidgetBridge bridge7(V7);//bridge to communicate with switch 7
WidgetBridge bridge8(V8);//bridge to communicate with switch 8
WidgetBridge bridge9(V9);//bridge to communicate with switch 9
WidgetBridge bridge10(V10);//bridge to communicate with switch 10
WidgetBridge bridge14(V11);//bridge to communicate with switch 11

BLYNK_CONNECTED()
{
  if (isFirstConnect)
  {
    timerId = timer.setInterval(200, checkInputs);//check input state every 200 milliseconds
    Blynk.syncAll();//Syncs all values from the app
    isFirstConnect = false;
  }

  //sets bridges at each reconnection:
  bridge0.setAuthToken("e0fd3490c8f5466bb4addbb1d3979b28");
  bridge1.setAuthToken("507d001925004a8ea106beafadfd6b59");
  bridge2.setAuthToken("ce930df17d4e4797947bb2ed80b6eced");
  bridge3.setAuthToken("f182c676a84843d399b3d677984caed8");
  bridge4.setAuthToken("772c1babd518455082f129250d450c7b");
  bridge5.setAuthToken("257da002f38e48f6b21c1a9814ee3730");
  bridge6.setAuthToken("b381f1a832784b12b1b9da3f31981093");
  bridge7.setAuthToken("eaf0b8b2dcb0493199e87719b312c3f7");
  bridge8.setAuthToken("da1111fbb4a24cfd86c4afea674776d1");
  bridge9.setAuthToken("94cb6c60cc5b402596955a001e317df7");
  bridge10.setAuthToken("8c69b9beeba9408ab89fee90a824f40c");
  bridge14.setAuthToken("472ea7b9976d46b396a65dc96d15eae4");
}

//The function checkInputs() checks the status of the input (the buttons) to see if it changed.
//if it changed, it calls the function sendState(i).
void checkInputs()
{ 
  int i = 0;
  int thisPin = 0;
 
  for(i=0; i<6; i++)
  {    
    thisPin = digitalRead(buttonPin[i]);//reads the new state
    if(thisPin != buttonState[i])//compares the new state and the old state of the input
    {
      buttonState[i] = thisPin;
      sendState(i);
    }
  }
}

//The function sendState(int pin) sends the new state of the buttons to a group of switchs
//the argument corresponds to the button that changed state
//button 0 controls switchs 0, 1, 2
//button 1 controls switchs 3, 4
//etc...
void sendState(int pin)
{ 
  switch(pin)
  {
    case 0:
      bridge0.virtualWrite(V0, buttonState[pin]);
      bridge1.virtualWrite(V0, buttonState[pin]);
      bridge2.virtualWrite(V0, buttonState[pin]);
      break;
    case 1:
      bridge3.virtualWrite(V0, buttonState[pin]);
      bridge4.virtualWrite(V0, buttonState[pin]);
      break;
    case 2:
      bridge5.virtualWrite(V0, buttonState[pin]);
      bridge6.virtualWrite(V0, buttonState[pin]);
      bridge7.virtualWrite(V0, buttonState[pin]);
      break;
    case 3:
      bridge8.virtualWrite(V0, buttonState[pin]);
      bridge9.virtualWrite(V0, buttonState[pin]);
      break;
    case 4:
      bridge10.virtualWrite(V0, buttonState[pin]);
      break;
    case 5:
      bridge14.virtualWrite(V0, buttonState[pin]);
      break;
    default:
      break;
  }
}

void setup()
{
  int numSsid = 0;//Number of ssid found
  int i = 0;
  int j = 0;

  pinMode(5, OUTPUT);//blue LED
  digitalWrite(5, LOW);//turns off blue led

  for(i=0; i<6; i++)
    pinMode(buttonPin[i], INPUT);

  numSsid = WiFi.scanNetworks();
  if (numSsid == -1)//In case there is no wifi connection
  {
    while (true)
    {delay(1000);}
  }

  for (i = 0; i < numSsid; i++)
    for(j=0; j<nbKnownAP; j++)
      if(WiFi.SSID(i) == ssid[j])//in case 1 of the networks found is a known network
      {
        strcpy(ssidChosen, ssid[j]);
        strcpy(passChosen, pass[j]);
      }
  
  if(strlen(ssidChosen) == 0)//in case no known network was found
  {
    while (true)
    {delay(1000);}
  }

  //Blynk.begin(auth, ssidChosen, passChosen);//you might have to use this line instead of the one below
  Blynk.begin(auth, ssidChosen, passChosen, ip, 8442);//you might have to use this line instead of the one above
  
  while(Blynk.connected() == false);  
}

void loop()
{  
  Blynk.run();
  timer.run();
}

