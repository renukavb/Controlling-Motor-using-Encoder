// for increasing constant error, increase kp
// for small increase, use ki

#define channelB 4
#define channelA 2
#define pwm 11
#define dir 9

volatile int count = 0;
volatile float angle = 0.0;
float vel;
float error = 0; float pre_error = 0;
float ang = 170; 
float I;
float kp = 0.5; float ki = 0.005; 

void on(){
  if (error < 0)
  {
  digitalWrite(dir,HIGH);
  } 
  else
  {
    digitalWrite(dir, LOW);
    }
  analogWrite(pwm,vel);
  }

void off(){
  digitalWrite(dir,LOW);
  analogWrite(pwm,0);
  }

void isr(){    
    if ((count%600) == 0)
    {
    count = 0;
    }

    if(digitalRead(channelB) == digitalRead(channelA))
    {
      count ++;                 //clockwise
    }
    else if(digitalRead(channelB)!= digitalRead(channelA))
    {
     count --;                //anticlockwise
     }
    angle = count*3/5;  
  }

void setup() {
  // put your setup code here, to run once:

  pinMode(channelA,INPUT_PULLUP);
  pinMode(channelB,INPUT_PULLUP);

  pinMode(pwm, OUTPUT);
  pinMode(dir,OUTPUT);

  Serial.begin(9600);
  Serial.print("Serial monitor started");

  attachInterrupt(digitalPinToInterrupt(channelA), isr, FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:
  on();
  error = ang - angle;
  I = error + pre_error;
  vel = kp*error + ki*I;
  
  Serial.print(vel);
  Serial.print("  ");
  Serial.println(error);
  
  if (angle == ang)
   {
      off();
      }

}
