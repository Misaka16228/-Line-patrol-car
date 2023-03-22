//舵机PWM引脚定义
const int servo_gpio = 11;

//左电机PWM引脚定义
const int motor_pwm_L = 6;

//右电机PWM引脚定义
const int motor_pwm_R = 5;

//左电机方向控制引脚定义
const int motor_gpio_L1 = 3;
const int motor_gpio_L2 = 4;

//右电机方向控制引脚定义
const int motor_gpio_R1 = 13;
const int motor_gpio_R2 = 2;

//传感器引脚定义
const int L1_gpio = 8;
const int L2_gpio = 7;
const int M_gpio = 9;
const int R1_gpio = 10;
const int R2_gpio = 12;

//传感器状态定义
int L1, L2, L3, M, R1, R2, R3;

//传感器相关赋值
int A, B, C, a, b, c, L = 0, R = 0, S = 0;

//时钟
int LT, RT, ST = 0;

//转向递减
int D = 20;

void setup() {
  // put your setup code here, to run once:
  pinMode(servo_gpio, OUTPUT);     //舵机PWM
  pinMode(motor_pwm_L, OUTPUT);    //左电机PWM
  pinMode(motor_pwm_R, OUTPUT);    //右电机PWM
  pinMode(motor_gpio_L1, OUTPUT);  //左电机前进后退
  pinMode(motor_gpio_L2, OUTPUT);
  pinMode(motor_gpio_R1, OUTPUT);  //右电机前进后退
  pinMode(motor_gpio_R2, OUTPUT);
  pinMode(L1_gpio, INPUT);  //传感器接口设置
  pinMode(L2_gpio, INPUT);
  pinMode(M_gpio, INPUT);
  pinMode(R1_gpio, INPUT);
  pinMode(R2_gpio, INPUT);
  Serial.begin(9600);            //开启串口通讯
  analogWrite(servo_gpio, 150);  //纠正方向
}

//数据处理
void Process() {
  if (L1 == LOW) {
    A = 1;
  } else {
    A = -1;
  }
  if (L2 == LOW) {
    B = 2;
  } else {
    B = -2;
  }
  if (R1 == HIGH) {
    a = 1;
  } else {
    a = -1;
  }
  if (R2 == HIGH) {
    b = 2;
  } else {
    b = -2;
  }
  L = A + B;
  R = a + b;
  S = L + R;
}

//计数
void Clock() {
  if (M == LOW && S == 0) {
    ST = ST + 1;
  }
}

//直行
void straight() {
  analogWrite(servo_gpio, 135);
  analogWrite(motor_pwm_L, 200);
  analogWrite(motor_pwm_R, 200);
  digitalWrite(motor_gpio_L1, HIGH);
  digitalWrite(motor_gpio_L2, LOW);
  digitalWrite(motor_gpio_R1, HIGH);
  digitalWrite(motor_gpio_R2, LOW);
  delay(20);
  ST = ST + 1;
}

//左转
void Left1() {
  analogWrite(servo_gpio, 135 - D);
  analogWrite(motor_pwm_L, 100);
  analogWrite(motor_pwm_R, 160);
  digitalWrite(motor_gpio_L1, HIGH);
  digitalWrite(motor_gpio_L2, LOW);
  digitalWrite(motor_gpio_R1, HIGH);
  digitalWrite(motor_gpio_R2, LOW);
  ST = ST + 1;
}

//大幅左转
void Left2() {
  analogWrite(servo_gpio, 95);
  analogWrite(motor_pwm_L, 100);
  analogWrite(motor_pwm_R, 200);
  digitalWrite(motor_gpio_L1, LOW);
  digitalWrite(motor_gpio_L2, HIGH);
  digitalWrite(motor_gpio_R1, HIGH);
  digitalWrite(motor_gpio_R2, LOW);
  ST = 0;
  D = 20;
}

//大大幅左转
void Left3() {
  analogWrite(servo_gpio, 40);
  analogWrite(motor_pwm_L, 255);
  analogWrite(motor_pwm_R, 255);
  digitalWrite(motor_gpio_L1, LOW);
  digitalWrite(motor_gpio_L2, HIGH);
  digitalWrite(motor_gpio_R1, HIGH);
  digitalWrite(motor_gpio_R2, LOW);
  delay(20);
}

//右转
void Right1() {
  analogWrite(servo_gpio, 135 + D);
  analogWrite(motor_pwm_L, 160);
  analogWrite(motor_pwm_R, 100);
  digitalWrite(motor_gpio_L1, HIGH);
  digitalWrite(motor_gpio_L2, LOW);
  digitalWrite(motor_gpio_R1, HIGH);
  digitalWrite(motor_gpio_R2, LOW);
  ST = ST + 1;
}

//大幅右转
void Right2() {
  analogWrite(servo_gpio, 200);
  analogWrite(motor_pwm_L, 200);
  analogWrite(motor_pwm_R, 100);
  digitalWrite(motor_gpio_L1, HIGH);
  digitalWrite(motor_gpio_L2, LOW);
  digitalWrite(motor_gpio_R1, LOW);
  digitalWrite(motor_gpio_R2, HIGH);
  ST = 0;
  D = 20;
}

//大大幅右转
void Right3() {
  analogWrite(servo_gpio, 240);
  analogWrite(motor_pwm_L, 255);
  analogWrite(motor_pwm_R, 255);
  digitalWrite(motor_gpio_L1, HIGH);
  digitalWrite(motor_gpio_L2, LOW);
  digitalWrite(motor_gpio_R1, LOW);
  digitalWrite(motor_gpio_R2, HIGH);
  delay(20);
}

//后退
void Back() {
  analogWrite(motor_pwm_L, 150);
  analogWrite(motor_pwm_R, 150);
  digitalWrite(motor_gpio_L1, LOW);
  digitalWrite(motor_gpio_L2, HIGH);
  digitalWrite(motor_gpio_R1, LOW);
  digitalWrite(motor_gpio_R2, HIGH);
}

//纠正方向
void Correct() {
  analogWrite(servo_gpio, 135);
  delay(20);
}

//调试舵机用(归正)
void Test1() {
  analogWrite(servo_gpio, 135);
  delay(20);
}

//调试舵机用(摇摆)
void Test2() {
  for (int a = 0; a <= 255; a++) {
    analogWrite(servo_gpio, a);
    Serial.println(a);
    delay(20);
  }
  for (int a = 255; a >= 0; a--) {
    analogWrite(servo_gpio, a);
    Serial.println(a);
    delay(20);
  }
}

//读取引脚状态
void Read() {
  M = digitalRead(M_gpio);
  L1 = digitalRead(L1_gpio);
  L2 = digitalRead(L2_gpio);
  R1 = digitalRead(R1_gpio);
  R2 = digitalRead(R2_gpio);
}

void loop() {
  // put your main code here, to run repeatedly:
  Read();
  Process();
  Serial.println(S);
  if (M == HIGH && S == 0) {
    straight();
  }
  if (M == LOW && S == 2) {
    if (ST >= 3) {
      D = D - 5;
      Right1();
    } else {
      Right1();
    }
  }
  if (M == LOW && S == -2) {
    if (ST >= 3) {
      D = D - 5;
      Left1();
    } else {
      Left1();
    }
  }
  if (M == LOW && S == 4) {
    Right2();
  }
  if (M == LOW && S == -4) {
    Left2();
  }
  if (S == 6) {
    Right3();
  }
  if (S == -6) {
    Left3();
  }
  Clock();
}