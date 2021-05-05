//Receiver Channels
#define CH1PIN 2
#define CH2PIN 3
#define CH3PIN 4
#define CH4PIN 5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(CH1PIN, INPUT);
  pinMode(CH2PIN, INPUT);
  pinMode(CH3PIN, INPUT);
  pinMode(CH4PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long ch1, ch2, ch3, ch4;
  ch1 = pulseIn(CH1PIN, HIGH); //altitude
  ch2 = pulseIn(CH2PIN, HIGH); //yaw
  ch3 = pulseIn(CH3PIN, HIGH); //pitch
  ch4 = pulseIn(CH4PIN, HIGH); //roll

  Serial.print(ch1);
  Serial.print("\t");
  Serial.print(ch2);
  Serial.print("\t");
  Serial.print(ch3);
  Serial.print("\t");
  Serial.println(ch4);
}
