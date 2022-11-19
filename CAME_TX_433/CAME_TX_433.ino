#define pinTX 8

#define CM_MAX_TE 450
#define CM_MIN_TE 250
#define CM_BITS12 12
#define CM_BITS24 24
#define Te 320

volatile byte level = 255;
volatile unsigned long last;
volatile unsigned long len;
byte p_level;
unsigned long p_len;
unsigned long p_len_prev;

int PB10 = 0;
int PB11 = 0;
int PB12 = 0;




void setup()
{
    Serial.begin(9600);
    while (!Serial);
    pinMode(pinTX, OUTPUT);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
}

void loop()
{
    char *code1 = "110010000001"; // C81
    char *code2 = "110010000011"; // C83
    char *code3 = "110010000101"; // C85

    PB10 = digitalRead(10);
    PB11 = digitalRead(11);
    PB12 = digitalRead(12);
    
    if (PB10 == LOW) {
      RfTransmitt(code1, 4);
    }
    if (PB11 == LOW) {
      RfTransmitt(code2, 4);
    }
    if (PB12 == LOW) {
      RfTransmitt(code3, 4);
    }
    
    delay(2000); 
}

void RfTransmitt(char *codeString, unsigned int numberOfShipments)
{
    int codeLength = strlen(codeString);
    if (codeLength != 12 && codeLength != 24)
    {
        Serial.println("incorrect code.");
        return;
    }

    byte code[codeLength];
    for (int i = 0; i < codeLength; i++) {
        code[i] = codeString[i] == '0' ? 0 : 1;
    }

    byte codeUpdate[codeLength];

    int number = 0;

    switch (codeLength) {
    case 12:
        for (int i = 0; i < 12; i++) {
            codeUpdate[number] = code[i];
            Serial.print(codeUpdate[number]);
            number++;
        }
        /*Serial.print(" ");
        for (int i = 6; i < 12; i++) {
            codeUpdate[number] = code[i];
            Serial.print(codeUpdate[number]);
            number++;
        }*/
        break;
    case 24:
        for (int i = 1; i <= 3; i++) {
            for (int j = i * 8 - 1; j >= 8 * (i - 1); j--) {
                codeUpdate[number] = code[j];
                Serial.print(codeUpdate[number]);
                number++;
            }
            Serial.print("-");
        }
        break;
    }

    Serial.println();

    for (int i = 0; i < numberOfShipments; i++)
    {
        digitalWrite(pinTX, HIGH);
        delayMicroseconds(Te);
        digitalWrite(pinTX, LOW);
        for (int j = 0; j < codeLength; j++) {
            SendCameBit(codeUpdate[j]);д
        }
        delay(16);
    }
}

void SendCameBit(byte b)
{
    delayMicroseconds(Te);
    if (!b) digitalWrite(pinTX, HIGH);
    delayMicroseconds(Te);
    digitalWrite(pinTX, HIGH);
    delayMicroseconds(Te);
    digitalWrite(pinTX, LOW);
}
