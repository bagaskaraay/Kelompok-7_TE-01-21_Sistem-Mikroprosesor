#include <SPI.h>
#include <MFRC522.h>

const int reedsensor = 3;
const int buzzer = 2;
const int ledPin_G = 5;
const int ledPin_R = 4;
const int trigPin = 7;
const int echoPin = 8;
int sinyal;
long duration;
int distance;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
pinMode(ledPin_G, OUTPUT);
pinMode(ledPin_R, OUTPUT);
pinMode(reedsensor, INPUT);
Serial.begin(9600);
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance= (duration/2)/29.1;

  sinyal = digitalRead(reedsensor);
  if (distance <= 5 && sinyal == HIGH){
    digitalWrite(ledPin_G, HIGH);
    digitalWrite(ledPin_R, HIGH);
    tone(buzzer, 300);
    delay(300);
    noTone(buzzer);
    digitalWrite(ledPin_R, LOW);
    delay(2500);
  }

  if (sinyal == HIGH){
    digitalWrite(ledPin_G, HIGH);
    digitalWrite(ledPin_R, LOW);
  }
  
  while(sinyal == LOW)
  {
    tone(buzzer, 300);
    digitalWrite(ledPin_R, HIGH);
    digitalWrite(ledPin_G, LOW);

    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }

    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    
    if (content.substring(1) != "39 BF D2 1E"){
      Serial.println();
      Serial.print("Message : ");
      content.toUpperCase();
      Serial.println("You have access");
      Serial.println();
    }

    do {
      noTone(buzzer);
      digitalWrite(ledPin_R, HIGH);
      delay(50);
      digitalWrite(ledPin_R, LOW);
      delay(50);
      digitalWrite(ledPin_R, HIGH);
      delay(50);
      digitalWrite(ledPin_R, LOW);

      sinyal=digitalRead(reedsensor);
      if (sinyal == HIGH){
        digitalWrite (buzzer, HIGH);
        delay (200);
        noTone(buzzer);
        delay (25);
        digitalWrite (buzzer, HIGH);
        delay (200);
        noTone(buzzer);
        break;
      }
    } while (content.substring(1) == "39 BF D2 1E"); //change here the UID of the card/cards that you want to give access)
  }
}
