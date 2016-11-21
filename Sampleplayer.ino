
/* 
  Dieses Sketch liest den Wert von mehreren Knöpfen an 2 Analog Input Pins.Der gedrückte Knopf wird seriell ausgegeben und zusätzlich wird pro gedrücktem Knopf/Kombination ein Sound wiedergegeben.
  Diese Aktion erfolgt bei loßlassen des Knopfes/der Knöpfe.
  Dieser Code wurde für einen Arduino Uno R3 + Sparkfun MP3 Player Shield geschrieben und ist möglicherweise nicht zwingend mit allen anderen Boards kompatibel.Benötigt zwingend einen IC mit 32kb oder mehr Speicher,da Sketch > 16 kb.
  
  Erforderliche Library von Bill Porter für das MP3Playermodul: http://www.billporter.info/sparkfun-mp3-shield-arduino-library/
  Benötigt zwingend einen IC mit 32kb oder mehr Speicher,da Sketch > 16 kb.
  
  Andere Nützliche Seiten die bei Erstellung dieses Codes und der zugehörigen Schaltung geholfen haben (englischsprachig):
  http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1267115381
  http://www.sparkfun.com/products/10628
  http://www.instructables.com/id/Accessing-5-buttons-through-1-Arduino-pin-Revisi/
  
  Mögliche Verwendungszwecke(veraltete Module in diesen Beispielen):
  http://www.youtube.com/watch?v=tbR1wwDLnRg
  http://www.youtube.com/watch?v=0mdmasP_Z-Q
  http://www.youtube.com/watch?v=IVIN4vF5i2g
*/
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>

SFEMP3Shield MP3player;

byte temp;


 // Konstanten ändern sich nicht. Diese sind da um die verwendeten Analog Input Pins zu kennzeichnen:
 const int buttonPin1 = 0;
 const int buttonPin2 = 1;
 const int LEDPin = 13;

 // Die Knopfbezogenen Konstanten:
 
 //Anzahl der Knöpfe Physisch und Virtuell: (in diesem Fall die ersten 6 Physisch (100kOhm PulldownWiderstand zwischen jedem AnalogInput der verwendet wird und Masse ist und die Einzelnen Widerstände an den Tastern :1kOhm;10kOhm;20kOhm;33kOhm;68kOhm;2,2kOhm) der Rest ist virtuell über Tastenkombinationen)
 const int numButtons1 = 14;
 const int numButtons2 = 14;
 //Aufzählung der Knöpfe.
 const int button1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
 const int button2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
 // Unterer Aktivierungsbereich für Knopfwerte.
 const int buttonLowRange1[] = {1013,930,850,769,601,1001,950,941,909,884,836,959,1016,1005};
 const int buttonLowRange2[] = {1013,930,850,769,601,1001,950,941,909,884,836,959,1016,1005};
 // Oberer Aktivierungsbereich für Knopfwerte.
 const int buttonHighRange1[] = {1014,931,852,771,612,1003,951,942,910,886,840,960,1018,1007};
 const int buttonHighRange2[] = {1014,931,852,771,612,1003,951,942,910,886,840,960,1018,1007};
 
 // Variablen die sich verändern können:
 
 int buttonState1;             // Aktueller Lesewert an dem Ersten Input Pin.
 int buttonState2;             // Aktueller Lesewert an dem Zweiten Input Pin.
 int lastButtonState1 = LOW;   // Vorheriger Lesewert an dem Ersten Input Pin.
 int lastButtonState2 = LOW;   // Vorheriger Lesewert an dem Ersten Input Pin.
 
 void setup() {
   MP3player.begin();           //startet das MP3 Player Shield.
   MP3player.SetVolume (0,0);
   char bootup[] = "bootup00.mp3";
   MP3player.playMP3(bootup);
   pinMode(LEDPin, OUTPUT);     //Steuert eine LED an Pin 13 und GND an.
   pinMode(buttonPin1, INPUT);  // Pinmodus für den ersten Pin.
   pinMode(buttonPin2, INPUT);  // Pinmodus für den zweiten Pin.
   Serial.begin(9600);          // Verwendete Baudrate.
    
 }
 void loop() {
   
   digitalWrite(LEDPin,LOW);
   digitalWrite(LEDPin,HIGH);
   checkActiveButtons1();     // Überprüft ob ein Knopf an Input 1 gedrückt wurde.
   checkActiveButtons2();     // Überprüft ob ein Knopf an Input 2 gedrückt wurde.
   dealWithButtonPress1();    // Führt Aktion/en aus nachdem ein Knopf an Input 1 gedrückt wurde.
   dealWithButtonPress2();    // Führt Aktion/en aus nachdem ein Knopf an Input 2 gedrückt wurde.
   additionalSerialCommand(); // Erlaubt Kommandozeilenbefehl.
   buttonState1 = LOW;        // Setzt den Knopf vor dem nächsten Check zurück.
   buttonState2 = LOW;        // Setzt den Knopf vor dem nächsten Check zurück.
   delay(50);                 // zum Entprellen. ( http://de.wikipedia.org/wiki/Prellen )
 }
 
 
 void additionalSerialCommand(){
   
   if(Serial.available()){
     temp = Serial.read();
     
     Serial.print("Aktion Ausgefuehrt!");
     Serial.write(temp);
     Serial.println(" ");
     
     //Eintippen von S im Seriellen Monitor beendet aktive Wiedergaben.   
     if (temp == 's') {
       MP3player.stopTrack();
       
      
    }
   }
 }
 
 void checkActiveButtons1(){
   // liest den Status eines Knopfes in eine lokale Variable:
   int reading1 = analogRead(buttonPin1);
   int tmpButtonState1 = LOW;               // Der jetzige Wert des Input Pins.
   
   if(reading1>100){                        // Alles darunter ist vermutlich Störung.
     for(int i=numButtons1; i--; i>=0){
       if(reading1>=buttonLowRange1[i] && 
           reading1<=buttonHighRange1[i]){     
         tmpButtonState1 = button1[i];
         break;
       }
     }
   }
   
   // Setzt den Wert erst Aktiv wenn der Knopf losgelassen wurde.
   if(lastButtonState1!=tmpButtonState1 && tmpButtonState1 == LOW){
     buttonState1 = lastButtonState1;
   }

   // Speichert das Leseergebnis.  Nächstes mal in der Schleife,wird es lastButtonState1:
   lastButtonState1 = tmpButtonState1;
 }
 
  void checkActiveButtons2(){
   // liest den Status eines Knopfes in eine lokale Variable:
   int reading2 = analogRead(buttonPin2);
   int tmpButtonState2 = LOW;               // Der jetzige Wert des Input Pins.
   
   if(reading2>100){                        // Alles darunter ist vermutlich Störung.
     for(int i=numButtons2; i--; i>=0){
       if(reading2>=buttonLowRange2[i] && 
           reading2<=buttonHighRange2[i]){     
         tmpButtonState2 = button2[i];
         break;
       }
     }
   }
   
   // Setzt den Wert erst Aktiv wenn der Knopf losgelassen wurde.
   if(lastButtonState2!=tmpButtonState2 && tmpButtonState2 == LOW){
     buttonState2 = lastButtonState2;
   }

   // Speichert das Leseergebnis.  Nächstes mal in der Schleife,wird es lastButtonState2:
   lastButtonState2 = tmpButtonState2;
 }
 
 void dealWithButtonPress1(){
   switch(buttonState1){
     case 1:
       Serial.println("Knopf 1.1 gedrueckt!");
       MP3player.playTrack(1);
       break;
     case 2:
       Serial.println("Knopf 2.1 gedrueckt!");
       MP3player.playTrack(2);
       break;
     case 3:
       Serial.println("Knopf 3.1 gedrueckt!");
       MP3player.playTrack(3);
       break;
     case 4:
       Serial.println("Knopf 4.1 gedrueckt!");
       MP3player.playTrack(4);
       break;
     case 5:
       Serial.println("Knopf 5.1 gedrueckt!");
       MP3player.playTrack(5);
       break;
     case 6:
       Serial.println("Knopf 6.1 gedrueckt!");
       MP3player.playTrack(6);
       break;
     case 7:
       Serial.println("Knopf 7.1 gedrueckt!");
       MP3player.playTrack(13);
       break;
     case 8:
       Serial.println("Knopf 8.1 gedrueckt!");
       MP3player.playTrack(14);
       break;
     case 9:
       Serial.println("Knopf 9.1 gedrueckt!");
       MP3player.playTrack(15);
       break;
     case 10:
       Serial.println("Knopf 10.1 gedrueckt!");
       MP3player.playTrack(16);
       break;
     case 11:
       Serial.println("Knopf 11.1 gedrueckt!");
       MP3player.playTrack(17);
       break;
     case 12:
       Serial.println("Knopf 12.1 gedrueckt!");
       MP3player.playTrack(18);
       break;
     case 13:
       Serial.println("Knopf 13.1 gedrueckt!");
       MP3player.playTrack(19);
       break;
     case 14:
       Serial.println("Knopf 14.1 gedrueckt!");
       MP3player.playTrack(20);
       break;
   }
 }

void dealWithButtonPress2(){
   switch(buttonState2){
     case 1:
       Serial.println("Knopf 1.2 gedrueckt!");
       MP3player.playTrack(7);
       break;
     case 2:
       Serial.println("Knopf 2.2 gedrueckt!");
       MP3player.playTrack(8);
       break;
     case 3:
       Serial.println("Knopf 3.2 gedrueckt!");
       MP3player.playTrack(9);
       break;
     case 4:
       Serial.println("Knopf 4.2 gedrueckt!");
       MP3player.playTrack(10);
       break;
     case 5:
       Serial.println("Knopf 5.2 gedrueckt!");
       MP3player.playTrack(11);
       break;
     case 6:
       Serial.println("Knopf 6.2 gedrueckt!");
       MP3player.playTrack(12);
       break;
     case 7:
       Serial.println("Knopf 7.2 gedrueckt!");
       MP3player.playTrack(21);
       break;
     case 8:
       Serial.println("Knopf 8.2 gedrueckt!");
       MP3player.playTrack(22);
       break;
     case 9:
       Serial.println("Knopf 9.2 gedrueckt!");
       MP3player.playTrack(23);
       break;
     case 10:
       Serial.println("Knopf 10.2 gedrueckt!");
       MP3player.playTrack(24);
       break;
     case 11:
       Serial.println("Knopf 11.2 gedrueckt!");
       MP3player.playTrack(25);
       break;
     case 12:
       Serial.println("Knopf 12.2 gedrueckt!");
       MP3player.playTrack(26);
       break;
     case 13:
       Serial.println("Knopf 13.2 gedrueckt!");
       MP3player.playTrack(27);
       break;
     case 14:
       Serial.println("Knopf 14.2 gedrueckt!");
       MP3player.playTrack(28);
       break;
   }
 }

