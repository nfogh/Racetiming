/*
  Reading multiple RFID tags, simultaneously!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 3rd, 2016
  https://github.com/sparkfun/Simultaneous_RFID_Tag_Reader

  Constantly reads and outputs any tags heard

  If using the Simultaneous RFID Tag Reader (SRTR) shield, make sure the serial slide
  switch is in the 'SW-UART' position
*/

#include <SoftwareSerial.h> //Used for transmitting to the device

SoftwareSerial softSerial(2, 3); //RX, TX

#include "SparkFun_UHF_RFID_Reader.h" //Library for controlling the M6E Nano module
RFID nano; //Create instance

#define BUZZER1 9
//#define BUZZER1 0 //For testing quietly
#define BUZZER2 10

int power = 2700;

int buzzerTone = 0;
unsigned long buzzerTimeout = 0;

struct DetectedTag
{
  byte id[16] = {0};
  int idLength = 0;
  uint32_t detectedTime = 0;
  bool taken = false;
};

struct DetectedTag* FindTag(struct DetectedTag* const start, const int listSize, const byte* const id, const int idLength)
{
  for (int i = 0; i < listSize; i++) {
    {
      if (start[i].taken)
        if (memcmp(start[i].id, id, idLength) == 0)
          return &start[i];
    }
  }
  return 0;
}

struct DetectedTag* FindFreeSpace(struct DetectedTag* const start, const int listSize)
{
  for (int i = 0; i < listSize; i++) {
    if (!start[i].taken) {
      return &start[i];
    }
  }
  return 0;
}

#define COOLDOWNTIME_MS 5000
#define NUMDETECTEDTAGS 30
struct DetectedTag detectedTags[NUMDETECTEDTAGS];

byte nibble(const char c)
{
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;

  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;

  return 0;  // Not a valid hexadecimal character
}

void hexCharacterStringToBytes(byte *byteArray, const char * const hexString)
{
  const bool oddLength = strlen(hexString) & 1;

  byte currentByte = 0;
  byte byteIndex = 0;

  for (byte charIndex = 0; charIndex < strlen(hexString); charIndex++)
  {
    const bool oddCharIndex = charIndex & 1;

    if (oddLength)
    {
      // If the length is odd
      if (oddCharIndex)
      {
        // odd characters go in high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Even characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
    else
    {
      // If the length is even
      if (!oddCharIndex)
      {
        // Odd characters go into the high nibble
        currentByte = nibble(hexString[charIndex]) << 4;
      }
      else
      {
        // Odd characters go into low nibble
        currentByte |= nibble(hexString[charIndex]);
        byteArray[byteIndex++] = currentByte;
        currentByte = 0;
      }
    }
  }
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.setTimeout(100);

  if (setupNano(38400) == false)
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1);
  }

  nano.setRegion(REGION_EUROPE);

  nano.setReadPower(power);
  nano.setWritePower(power);
  
  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  digitalWrite(BUZZER2, LOW);

  Serial.println("Initialized");

  nano.startReading();

  pinMode(7, OUTPUT);

  buzzerTone = 1;
  buzzerTimeout = millis();
}

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

void loop()
{
  if (millis() > buzzerTimeout) {
    switch (buzzerTone) {
      case 1:
        digitalWrite(7, HIGH);
        buzzerTone = 2349;
        buzzerTimeout = millis() + 10;
        break;
      case 2349:
        buzzerTone = 2637;
        buzzerTimeout = millis() + 150;
        break;
      case 2637:
        buzzerTone = 3136;
        buzzerTimeout = millis() + 150;
        break;
      case 3136:
        digitalWrite(7, LOW);
        buzzerTone = 0;
        break;
    }
    if (buzzerTone) {
        tone(BUZZER1, buzzerTone);
    } else {
        noTone(BUZZER1);
    }
    buzzerTimeout = millis() + 100;
  }

  if (Serial.available() > 0) {
    String s = Serial.readString();
    if (s.startsWith("setid")) {
      String id = s.substring(6, 6+32);
      id.trim();
      byte hex[16];
      hexCharacterStringToBytes(hex, id.c_str());

      Serial.print("Writing ID '");
      for (int i = 0; i < id.length()/2; i++)
        Serial.print(hex[i], HEX);
      Serial.println("'");
      
      nano.writeTagEPC((const char*)hex, id.length()/2);
    } else if (s.startsWith("stop")) {
      Serial.println("Stopping reading");
      nano.stopReading();
    } else if (s.startsWith("start")) {
      Serial.println("Starting reading");
      nano.startReading();
    } else if (s.startsWith("power")) {
      s.remove(0,5);
      power = s.toInt();
      if (power > 2700)
        power = 2700;
      if (power < 0)
        power = 0;
      nano.setReadPower(power);
      nano.setWritePower(power);
      Serial.print("Power is ");
      Serial.println(power);
    }
  }
  
  if (nano.check() == true)
  {
    const byte responseType = nano.parseResponse();

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      Serial.println("KEEPALIVE");
    }
    else if (responseType == RESPONSE_IS_TAGFOUND)
    {
      byte tagEPCBytes = nano.getTagEPCBytes();
      if (tagEPCBytes > 16)
        tagEPCBytes = 16;
      byte tagEPC[16] = {0};

      for (byte x = 0 ; x < tagEPCBytes ; x++)
        tagEPC[x] = nano.msg[31 + x]; 

      struct DetectedTag* tag = FindTag(detectedTags, ARRAYSIZE(detectedTags), tagEPC, tagEPCBytes);
      if (tag != 0) {
          // The tag was detected within the cooldown time
          const uint32_t msSinceDetection = millis() - tag->detectedTime;

          if (msSinceDetection < COOLDOWNTIME_MS) {
              // We already detected this tag within the cooldown time. Don't register it.
              return;
          } else {
              // Cooldowntime has elapsed, reregister the tag
              tag->detectedTime = millis();
          }
      } else {
        struct DetectedTag* tag = FindFreeSpace(detectedTags, ARRAYSIZE(detectedTags));
        if (tag != 0) {
            memcpy(tag->id, tagEPC, ARRAYSIZE(tag->id));
            tag->taken = true;
            tag->detectedTime = millis();
        } else {
            Serial.println("Not enough space in list");
            return;
        }
      }

      Serial.print("TAGID,");
      for (int i = 0; i < tagEPCBytes; i++) {
        if (tagEPC[i] < 16)
          Serial.print("0"); // Prepend 0 if HEX output is just 1 character.
        Serial.print(tagEPC[i], HEX);
      }
      Serial.println();

      buzzerTone = 1;
      buzzerTimeout = millis();

      for (int i = 0; i < ARRAYSIZE(detectedTags); i++) {
        struct DetectedTag* tag = &detectedTags[i];
        if (tag->taken) {
          const uint32_t msSinceDetection = millis() - tag->detectedTime;
          if (msSinceDetection > COOLDOWNTIME_MS) {
            tag->taken = false;
            tag->detectedTime = 0;
            memset(tag->id, 0, ARRAYSIZE(tag->id));
            tag->idLength = 0;
          }
        }
      }

    }
    else if (responseType == ERROR_CORRUPT_RESPONSE)
    {
      Serial.println("Bad CRC");
    }
    else
    {
      //Unknown response
      Serial.print("Unknown error");
    }
  }
}

//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.begin(softSerial); //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  softSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while (softSerial.isListening() == false); //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (softSerial.available()) softSerial.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    softSerial.begin(115200); //Start software serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    softSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate

    delay(250);
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //We are ready to rock
}
