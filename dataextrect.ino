boolean dataTrueFlag = 0, Request_flag = 0, Command_flag = 0;
unsigned int mode = 0, I_1 = 220, I_2 = 110, I_3 = 50, I_4 = 25;
char data_send[8];
char light_data1 = 0, light_data2 = 0, R_C = 0,

     I_1_1 = 0,
     I_1_10 = 0,
     I_1_100 = 0,

     I_2_1 = 0,
     I_2_10 = 0,
     I_2_100 = 0,

     I_3_1 = 0,
     I_3_10 = 0,
     I_3_100 = 0,

     I_4_1 = 0,
     I_4_10 = 0,
     I_4_100 = 0;
unsigned int CS = 0, c0 = 0, c1 = 0, CS_C = 0;
void fletcher_encode(unsigned short );
void dataExtrect();
void data_by_command();
void data_send_by_request();

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(200);
  digitalWrite(3,LOW);
  delay(200);
  digitalWrite(4,LOW);
  delay(200);
  digitalWrite(5,LOW);
  delay(200);
  digitalWrite(6,LOW);
  delay(200);
  digitalWrite(7,LOW);
  delay(200);
  digitalWrite(8,LOW);
  delay(200);
  digitalWrite(9,LOW);
  delay(200);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  Serial.print("AT");
  Serial.write(13);
  Serial.write(10);
  delay(500);
  Serial.print("AT+CWMODE=1");
  Serial.write(13);
  Serial.write(10);
  delay(500);
  Serial.println("AT+CIPMUX=1");
  Serial.write(13);
  Serial.write(10);
  delay(500);
  Serial.println("AT+CIPSERVER=1,1234");
  Serial.write(13);
  Serial.write(10);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (dataTrueFlag == 1)
  {
    
    if (Command_flag == 1)
    {
      data_by_command();
      Command_flag = 0;
    }
    if (Request_flag == 1)
    {
      data_send_by_request();
      Request_flag = 0;
    }
    dataTrueFlag=0;
  }
}
void data_by_command()
{
  if (bitRead(light_data1, 7) == 1)digitalWrite(9, HIGH);
  if (bitRead(light_data1, 7) == 0)digitalWrite(9, LOW);
  if (bitRead(light_data1, 6) == 1)digitalWrite(8, HIGH);
  if (bitRead(light_data1, 6) == 0)digitalWrite(8, LOW);
  if (bitRead(light_data1, 5) == 1)digitalWrite(7, HIGH);
  if (bitRead(light_data1, 5) == 0)digitalWrite(7, LOW);
  if (bitRead(light_data1, 4) == 1)digitalWrite(6, HIGH);
  if (bitRead(light_data1, 4) == 0)digitalWrite(6, LOW);
  if (bitRead(light_data1, 3) == 1)digitalWrite(5, HIGH);
  if (bitRead(light_data1, 3) == 0)digitalWrite(5, LOW);
  if (bitRead(light_data1, 2) == 1)digitalWrite(4, HIGH);
  if (bitRead(light_data1, 2) == 0)digitalWrite(4, LOW);
  if (bitRead(light_data1, 1) == 1)digitalWrite(3, HIGH);
  if (bitRead(light_data1, 1) == 0)digitalWrite(3, LOW);
  if (bitRead(light_data1, 0) == 1)digitalWrite(2, HIGH);
  if (bitRead(light_data1, 0) == 0)digitalWrite(2, LOW);
}
void data_send_by_request()
{
  c0 = 0;
  c1 = 0;
  data_send[0] = light_data1;
  fletcher_encode(data_send[0]);//decode data
  data_send[1] = light_data2;
  fletcher_encode(data_send[1]);
  data_send[2] = I_1;
  fletcher_encode(data_send[2]);
  data_send[3] = I_2;
  fletcher_encode(data_send[3]);
  data_send[4] = I_3;
  fletcher_encode(data_send[4]);
  data_send[5] = I_4;
  fletcher_encode(data_send[5]);
  data_send[6] = (unsigned short)(c0 - c1);
  data_send[7] = (unsigned short)(c1 - 2 * c0); // calculated CheckSum for receive data
  c0=0;
  c1=0;
  Serial.write(data_send);
}

void dataExtrect() {
  I_1_100 = (I_1 / 100) + 48;
  I_1_10 = ((I_1 % 100) / 10) + 48;
  I_1_1 = (I_1 % 10) + 48;
  I_2_100 = (I_2 / 100) + 48;
  I_2_10 = ((I_2 % 100) / 10) + 48;
  I_2_1 = (I_2 % 10) + 48;
  I_3_100 = (I_3 / 100) + 48;
  I_3_10 = ((I_3 % 100) / 10) + 48;
  I_3_1 = (I_3 % 10) + 48;
  I_4_100 = (I_4 / 100) + 48;
  I_4_10 = ((I_4 % 100) / 10) + 48;
  I_4_1 = (I_4 % 10) + 48;
}

void serialEvent() {
  while (Serial.available()) {
    char data_buff = (char)Serial.read();
    switch (mode)
    {
      case 0: // case 0-3 +IPD
        {
          CS = 0;
          c0 = 0;
          c1 = 0;
          if (data_buff == '+')
          {
            mode = 1;
          }
          else
          {
            mode = 0;
          }
          break;
        }
      case 1:
        {
          if (data_buff == 'I')
          {
            mode = 2;
          }
          else
          {
            mode = 0;
          }
          break;
        }
      case 2:                  //Serial_Number
        {
          if (data_buff == 'P')
          {
            mode = 3;
          }
          else
          {
            mode = 0;
          }
          break;
        }

      case 3:
        {
          if (data_buff == 'D')
          {
            mode = 4;
          }
          else
          {
            mode = 0;
          }
          break;
        }
      case 4:                  //Packet_type
        {
          if (data_buff == ':') {
            mode = 5;
          }
          break;
        }

      case 5:
        {
          CS = 0, c0 = 0, c1 = 0, CS_C = 0;
          fletcher_encode(data_buff);
          R_C = data_buff;
          mode = 6;
          if (R_C == 'R')
          {
            mode = 10;
            Request_flag = 1;
          }
          if (R_C == 'C')
          {
            mode = 6;
            Command_flag = 1;
          }
          break;
        }
      case 6:
        {
          fletcher_encode(data_buff);
          light_data1 = data_buff;
          mode = 7;
          break;
        }
      case 7:
        {
          fletcher_encode(data_buff);
          light_data2 = data_buff;
          mode = 10;
          break;
        }
      case 10:
        {
          CS_C = (unsigned short)(c0 - c1);
          CS_C = CS_C << 8;
          CS_C |= (unsigned short)(c1 - 2 * c0); // calculated CheckSum for receive data
          CS = (data_buff << 8 ); //receive CheckSum1
          mode = 11;
          break;
        }
      case 11:
        {
          CS = CS + data_buff;
          mode = 0;
          if (CS == CS_C)
          {
            dataTrueFlag = 1;
          }
          break;
        }
    }
  }
}



void fletcher_encode(unsigned short buff)
{
  c0 = (unsigned short)(c0 + buff);
  c1 = (unsigned short)(c1 + c0);
}

