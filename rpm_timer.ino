#define channelB 4
#define channelA 2
#define pwm 11
#define dir 9

float a;
int base_pwm = 60;
volatile int counter = 0;
volatile float cur_rpm;
float mapped_pwm;
float error = 0; float last_error = 0;
float term = 0; 
float kp = 1.3; 
float kd = 0.009; 
float ki = 0.009;  // pid term

void count_pulse(){
  counter++;
  }

ISR(TIMER1_COMPA_vect)        //timer compare interrupt service routine
{
  cur_rpm = counter;              // cur_pwm = rot*10*60;
  Serial.print("RPM: ");
  Serial.print(counter);
  Serial.print("   PWM: ");
  Serial.print(a);
  Serial.print("   MAPPED PWM: ");
  Serial.print(mapped_pwm);
  Serial.print("   ERROR: ");
  Serial.println(error);
  counter = 0;
  }

void setup(){
  pinMode(channelA, INPUT_PULLUP);
  pinMode(channelB, INPUT_PULLUP);
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelA), count_pulse, FALLING);  
  digitalWrite(dir, HIGH);

  noInterrupts();    // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;        // A and B 

  TCNT1 = 0;         // preloaded timer
  OCR1A = 6250;      // compare match register

  TCCR1B |= (1 << WGM12);     //CTC MODE
  TCCR1B |= (1 << CS12);      // 256 PRESCALER
  TIMSK1 |= (1 << OCIE1A);    // enable timer overflow enable
  interrupts();               // enable all interrupts

  }

void loop(){
  a = base_pwm + term;
  analogWrite(11,a);   // providing base pwm
  
  mapped_pwm = map(cur_rpm, 0, 700, 0, 255);
  
  error = base_pwm - mapped_pwm;
  term = error*kp + (last_error + error)*ki ;
  last_error = error;
}
