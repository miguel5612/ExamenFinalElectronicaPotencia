
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "MIGUELANGEL";
const char* password = "administrador5612";
int Reles[]= {14,12,13};//array to store pin nos
int Status[]= {16,5};//array to store pin nos bad,good
int contador = 0;
bool estadoApagado = HIGH;
bool estadoEncendido = LOW;
bool estado = false;
ESP8266WebServer server(80);

String webPage = "";

//int gpio0_pin = 15;
//int gpio2_pin = 5;

void setup(void){
  webPage = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Control domotico 2017</title><style>body{background: linear-gradient(to right, red , blue);}.tittleOnOff{color:white;}.buttonOnOff{display:block;margin:3px 3px 0 0;padding:0.5%;border:0;width:100%;}.buttonPanel{border-style: solid;border-color: black;height: 50%;align-items: center;}.section{margin:0 auto;border:0;padding: 0;display:inline-block;width:25%;height: 25%;}.circleBase{border-radius: 50%; behavior: url(PIE.htc); /* remove if you don't care about IE8 */ display:table; margin: 0 auto;}.type1{width: 100px; height: 100px; background: white; border: 3px solid red;}.checkboxClass{display:inline-block;margin:0 auto;font-size:5px;}</style></head><body><h1>Control domotico V1.0</h1><section class='section' id='onEquipos'><h4 class='tittleOnOff'>Realiza el encendido/apagado haciendo clic</h4><div class='buttonPanel' id='RelayController'><button class='buttonOnOff' id='onLamp' onclick='onLamps()'>Encneder luces</button><div id='lampWebLed' class='onLamp circleBase type1' onclick='onLamps()'></div><button class='buttonOnOff' id='onFan' onclick='onFans()'>Encender ventilador</button><div id='lampWebFan' onclick='onFans()' class='onFan circleBase type1'></div><button onclick='onOpenDoors()' class='buttonOnOff' id='onOpenDoor'>Abrir la puerta</button><div id='lampWebDoor' onclick='onOpenDoors()' class='onOpenDoor circleBase type1'></div></div></section><section class='section' id='AdvancedConfig'><h4 class='tittleOnOff'>Realiza los cambios avanzados</h4><div class='buttonPanel' id='RelayController'><label><input type='checkbox' onclick='checkboxClick(this)' class='checkboxClass' id='checkbox1' value='1'/>1/2 onda</label><br><label><input type='checkbox' onclick='checkboxClick(this)' class='checkboxClass' id='checkbox2' value='2'/>1 onda</label><br><label><input type='checkbox' onclick='checkboxClick(this)' class='checkboxClass' id='checkbox3' value='3'/>1 1/2 onda</label><br><label><input type='checkbox' onclick='checkboxClick(this)' class='checkboxClass' id='checkbox4' value='4'/>2 onda</label><br><label><input type='checkbox' onclick='checkboxClick(this)' class='checkboxClass' id='checkbox5' value='5'/>2 1/2 onda</label><br></div></section><script>function httpGetAsync(e,t){var o=new XMLHttpRequest;o.onreadystatechange=function(){4==o.readyState&&200==o.status&&t(o.responseText)},o.open('GET',e,!0),o.send(null)}function checkboxClick(e){console.log('Clicked, new value = '+e.checked),console.log(e.value);var t=e.id;console.log(t);for(var o=1;o<=5;o++)document.getElementById('checkbox'+o).checked=!1;document.getElementById(t).checked=!0,checked=e.value}function contador(){(new Date).getTime()>=time&&(document.getElementById('onOpenDoor').style.backgroundColor='white',document.getElementById('lampWebDoor').style.backgroundColor='white')}function onLamps(){ligthState?(document.getElementById('lampWebLed').style.backgroundColor='white',document.getElementById('onLamp').style.backgroundColor='white',ligthState=!1,httpGetAsync('/lampsOff',function(e){alert('lamparas apagadas')})):(document.getElementById('lampWebLed').style.backgroundColor='yellow',document.getElementById('onLamp').style.backgroundColor='yellow',ligthState=!0,httpGetAsync('/lampsOn',function(e){alert('lamparas encendidas')}))}function onFans(){!fanState&&checked>0?(document.getElementById('onFan').style.backgroundColor='yellow',document.getElementById('lampWebFan').style.backgroundColor='yellow',fanState=!0,checked>0?(console.log(checked),httpGetAsync('/fanOn?onda='+checked.trim(),function(e){alert('lamparas encendidas')})):alert('seleccione por favor un tipo de onda')):fanState?(document.getElementById('onFan').style.backgroundColor='white',document.getElementById('lampWebFan').style.backgroundColor='white',fanState=!1,httpGetAsync('/fanOff',function(e){alert('lamparas apagadas')})):alert('seleccione por favor un tipo de onda')}function onOpenDoors(){doorState||(document.getElementById('onOpenDoor').style.backgroundColor='yellow',document.getElementById('lampWebDoor').style.backgroundColor='yellow',httpGetAsync('/openDoor',function(e){alert('Abriendo puerta')}),time=(new Date).getTime()+1200,setInterval('contador()',500))}var ligthState=!1,fanState=!1,doorState=!1,time=0,checked=0;</script><footer>Realizado por:<p><a href='https://www.facebook.com/miguelCalifa'>Miguel Angel Califa urquiza</a></p><p><a href='https://www.facebook.com/dumarhidalgo95'>Dumar Hidalgo</a></p><p><a href='mailto:yaancarlospa@ufps.edu.co'>Yaan Carlos Pabón</a></p></footer></body></html>";
  // preparing GPIOs
    for(int i = 0; i<3 ; i++)
    {
      pinMode(Reles[i], OUTPUT);
      digitalWrite(Reles[i],estadoApagado);
    
    }
    digitalWrite(Status[1],OUTPUT);
    digitalWrite(Status[2],OUTPUT);
    for(int i = 0; i<10 ; i++)
    {
      digitalWrite(Reles[1],estadoApagado);
      digitalWrite(Reles[2],estadoApagado);
      digitalWrite(Reles[3],estadoApagado);
      digitalWrite(Status[1],estadoApagado);
      digitalWrite(Status[2],estadoApagado);
      
      estado = !estado;
      Serial.print(estado);
      delay(500);
    }
    
    
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    estado = !estado;
    digitalWrite(Status[1],estado);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  server.on("/lampsOn", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(Reles[1], estadoEncendido);
    delay(500);
  });
  server.on("/lampsOff", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(Reles[1], estadoApagado);
    delay(500); 
  });
   server.on("/openDoor", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(Reles[2], estadoEncendido);
    delay(2000);
    digitalWrite(Reles[2], estadoApagado);
  });

  
  server.begin();
  Serial.println("Servidor iniciado");
}
 
void loop(void){
  server.handleClient();
} 

