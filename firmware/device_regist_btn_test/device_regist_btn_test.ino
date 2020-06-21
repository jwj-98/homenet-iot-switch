int sw= 16; //Tact switch PIN
boolean preSw= false;
int led= 2;
unsigned long times;

void setup() {
  Serial.begin(115200);
  pinMode(sw, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  boolean val= digitalRead(sw);
  if(val){ //버튼을 누를때
    if(!preSw){ //버튼 off 에서 on 으로 바뀔때 1회 실행
      times= millis();
      preSw= true;
    }
    //버튼을 누르는 중이라면 실행
    if (  (millis() - times) > 2000  ){ //3초이상 버튼을 눌렀다면
      Serial.println(times - millis());
      digitalWrite(led, HIGH);
    }
  }
  if(!val) { //버튼을 누르지않았을때
    if(preSw){//버튼 on 에서 off 로 바뀔때 1회 실행
      preSw= false;
    }
  }
}
