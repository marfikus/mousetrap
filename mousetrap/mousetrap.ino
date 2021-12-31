#include <Servo.h>
#include <Gyver433.h>


const byte SERVO = 2;
const byte IR_LED = 3;
const byte OPTO_VT = 4;
const byte TOGGLE_BT = 5;
const byte TRANSMITTER = 6;
const byte LED = 13;

Servo servo;
Gyver433_TX<TRANSMITTER> tx;

const byte SERVO_ANGLE_ON = 140;
const byte SERVO_ANGLE_OFF = 90;

bool servoInStateON = false;
bool calibrationMode = false;


void setState(bool state) {
    servo.attach(SERVO);
    delay(50); // задержка на включение сервопривода
    if (state) {
        digitalWrite(IR_LED, HIGH);
        servo.write(SERVO_ANGLE_ON);
        digitalWrite(LED, LOW);
        servoInStateON = true;
    } else {
        digitalWrite(IR_LED, LOW);
        servo.write(SERVO_ANGLE_OFF);
        digitalWrite(LED, HIGH);
        servoInStateON = false;
    }
    delay(500); // дожидаемся, когда сервопривод отработает
    servo.detach(); // выключаем, чтобы не трещал от нагрузки на вал
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
    delay(200);

    if (calibrationMode) {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(200);        
    }
}

void sendState() {
    byte data = 1;
    if (servoInStateON) {
        data = 0;
    }
    tx.sendData(data);
}


void setup() {
    pinMode(IR_LED, OUTPUT);
    pinMode(OPTO_VT, INPUT);
    pinMode(TOGGLE_BT, INPUT);
    pinMode(LED, OUTPUT);

    setState(false);
    calibrationMode = false;
}

void loop() {
    // Короткое нажатие (~1с) переключает состояние, 
    // а долгое (~2с) переключает режим работы.
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

    // В режиме калибровки светодиод показывает состояние оптотранзистора (открыт/закрыт)
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
        }
    }

    sendState();
    delay(500);
}
