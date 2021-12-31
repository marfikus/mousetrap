#include <Gyver433.h>


const byte LED = 3;
const byte RECEIVER = 2;

Gyver433_RX<RECEIVER, 20> rx;


void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    attachInterrupt(0, isr, CHANGE);
    Serial.println("ready");
}

void isr() {
    rx.tickISR();
}

void loop() {
    // gotData() вернёт количество удачно принятых байт
    if (rx.gotData()) {   // если больше 0
        byte data;
        // если данные совпадают по размеру - ок
        if (rx.readData(data)) {
            Serial.print(data);

            if (data == 1) {
                digitalWrite(LED, HIGH);
            } else if (data == 0) {
                digitalWrite(LED, LOW);
            }

        } else {
            Serial.print("Data error");
        }

        // качество соединения
        Serial.print(", RSSI: ");
        Serial.println(rx.getRSSI());
    }
}
