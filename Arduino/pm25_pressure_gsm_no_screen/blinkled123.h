void blinkled(int led){
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
}

void doubleblinkled(int led){
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
}

void tribleblinkled(int led){
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
  digitalWrite(led,HIGH);
  delay(50);
  digitalWrite(led,LOW);
  delay(50);
}