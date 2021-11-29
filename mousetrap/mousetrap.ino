#include <Servo.h>
#include <Gyver433.h>


const byte SERV = 2;
const byte IR_VD = 3;
const byte OPTO_VT = 4;
const byte TOGGLE_BT = 5;
const byte TRANSMIT = 6;
const byte LED = 13;

Servo servo;
Gyver433_TX<TRANSMIT> tx;

const byte ON = 140;
const byte OFF = 90;

bool servoInStateON = false;
bool calibrationMode = false;


void setState(bool state) {
    servo.attach(SERV);
    delay(50); // задержка на включение сервопривода
    if (state) {
        digitalWrite(IR_VD, HIGH);
        servo.write(ON);
        servoInStateON = true;
    } else {
        digitalWrite(IR_VD, LOW);
        servo.write(OFF);
        servoInStateON = false;
    }
    delay(500); // дожидаемся, когда сервопривод отработает
    servo.detach();
    sendState();
}

void toggleState() {
    if (servoInStateON) {
        setState(false);
    } else {
        setState(true);
    }
}

void toggleMode() {
    if (calibrationMode) {
        calibrationMode = false;
        digitalWrite(LED, LOW);
    } else {
        calibrationMode = true;
    }
    blink();
}

void blink() {
    digitalWrite(LED, LOW);
    delay(100);

    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
}

void sendState() {
    byte data = 1;
    if (servoInStateON) {
        data = 0;
    }
    tx.sendData(data);
}


void setup() {
    pinMode(IR_VD, OUTPUT);
    pinMode(OPTO_VT, INPUT);
    pinMode(TOGGLE_BT, INPUT);
    pinMode(LED, OUTPUT);

    setState(false);
    calibrationMode = false;
}

void loop() {

    if (digitalRead(TOGGLE_BT) == HIGH) {
        delay(10);
        if (digitalRead(TOGGLE_BT) == HIGH) {
            delay(1000);
            if (digitalRead(TOGGLE_BT) == HIGH) {
                toggleMode();
            } else {
                toggleState();
            }
        }
    }

    if (calibrationMode) {
        if (digitalRead(OPTO_VT) == HIGH) {
            digitalWrite(LED, HIGH);
        } else {
            digitalWrite(LED, LOW);
        }

    } else {
        if (servoInStateON) {
            if (digitalRead(OPTO_VT) == HIGH) {
                setState(false);
            }
        } else {
            sendState();
            delay(500);
        }
    }
}
