#include <Gyver433.h>


const byte LED = 3;
const byte RECEIVE = 2;

Gyver433_RX<RECEIVE, 20> rx;


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
        // ЧИТАЕМ. СПОСОБ 1
        // я знаю, что передатчик отправляет char[15]
        // char data[15];
        byte data;

        // читаем принятые данные в data
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

        // ЧИТАЕМ. СПОСОБ 2
        // вывести сырые данные из буфера в порт
        //Serial.write(rx.buffer, rx.size);

        // выведем также качество соединения
        Serial.print(", RSSI: ");
        Serial.println(rx.getRSSI());
    }

}
