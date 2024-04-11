const int PinLedSemaforo [] = {11, 13, 12}; //vermelho,verde, amarelo
const int PinLedPedestre [] = {5, 4}; // Vermelho, Verde
const int TemposLedSem [] = {15000, 10000, 5000};
int EstadoAtualSemaforo = 0; //0=vermelho, 1=verde, 2=amarelo
unsigned long int TempoDifLeds = millis();

bool BT_Pressionado = false, SemaforoInterrompido = false;
const int PinPedestreBT = 2;
unsigned long int TempoSemInterrompido = 0;

const int PinLedMulta = 10;
#define TriggerPin 9
#define EchoPin 8
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void setup () {
  Serial.begin(9600);
  delay(10);
  pinMode(PinPedestreBT, INPUT_PULLUP);
  pinMode(PinLedMulta, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(PinLedSemaforo [i], OUTPUT);
    digitalWrite(PinLedSemaforo [i], LOW);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(PinLedPedestre [i], OUTPUT);
    digitalWrite(PinLedPedestre [i], LOW);
  }

}

void loop() {

  if (digitalRead(PinPedestreBT) == 0 and BT_Pressionado == false) {
    delay(20);
    if (digitalRead(PinPedestreBT) == 0) {
      BT_Pressionado = true;
      SemaforoInterrompido = true;
      TempoSemInterrompido = millis();
      EstadoAtualSemaforo = 0;
    }
  }

  if (SemaforoInterrompido == false) {
    if ((millis() - TempoDifLeds) > TemposLedSem[EstadoAtualSemaforo]) {
      EstadoAtualSemaforo++;
      TempoDifLeds = millis();
      if (EstadoAtualSemaforo > 2) {
        EstadoAtualSemaforo = 0;
      }
    }
  } else {
    if ((millis() - TempoSemInterrompido) > 10000) {
      EstadoAtualSemaforo = 0;
      TempoDifLeds = millis();
      SemaforoInterrompido = false;
      BT_Pressionado = false;
    }
  }


  for (int i = 0; i < 3; i++) {
    if (i == EstadoAtualSemaforo) {
      digitalWrite(PinLedSemaforo [i], HIGH);
    } else {
      digitalWrite(PinLedSemaforo [i], LOW);
    }
  }

  if (EstadoAtualSemaforo == 0) {
    digitalWrite(PinLedPedestre [0], LOW);
    digitalWrite(PinLedPedestre [1], HIGH);
    int cm = 0.01723 * readUltrasonicDistance(TriggerPin, EchoPin);
    if (cm < 50) {
      digitalWrite(PinLedMulta, HIGH);
    }

  } else {
    digitalWrite(PinLedPedestre [0], HIGH);
    digitalWrite(PinLedPedestre [1], LOW);
    digitalWrite(PinLedMulta, LOW);
  }

  delay(10);
}
