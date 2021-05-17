//about button switch
short S[4]={0}; short now[4]={1}; short was[4]={0};
//
int _resetPosition = 0;

//어떻게 입력이 되었나?
void controljudge()
{
  now[0]=digitalRead(UPpin);
  now[1]=digitalRead(DOWNpin);
  now[2]=digitalRead(RIGHTpin);
  now[3]=digitalRead(LEFTpin);
  for(int i=0;i<4;i++)
  {
  if(now[i]!=was[i]&&now[i]==0) S[i]=1;
  else S[i]=0;
      was[i]=now[i];
  }
}

//설정된 핀번호 풀업저항으로 선언
void pinset()
{
  pinMode(UPpin,INPUT_PULLUP);
  pinMode(DOWNpin,INPUT_PULLUP);
  pinMode(RIGHTpin,INPUT_PULLUP);
  pinMode(LEFTpin,INPUT_PULLUP);
  
  pinMode(PWMPin1,OUTPUT);
  digitalWrite(PWMPin1,LOW);

  pinMode(PWMPin2,OUTPUT);
  digitalWrite(PWMPin2,LOW);

  pinMode(RelayPin1,OUTPUT);
  pinMode(RelayPin2,OUTPUT);
  pinMode(RelayPin3,OUTPUT);
  pinMode(RelayPin4,OUTPUT);
  digitalWrite(RelayPin1,LOW);
  digitalWrite(RelayPin2,LOW);
  digitalWrite(RelayPin3,LOW);
  digitalWrite(RelayPin4,LOW);
}

void buttonRead()
{
  controljudge();
  if(subm != 2)
  {
    if(S[2]) subm++;
    else if(S[3]) subm = 0;
    
    if(S[0]&&menu>1) menu--;
    else if(S[1]&&menu<2) menu++;
  }
  else
  {
    switch(menu)
    {
      case 1: // POSITION CONTROL
        motorControl();
        break;

      case 2: // STEPMODE SET & RESET POSITION
        MSmodeControl();
        break;
        
    }
  }
}
