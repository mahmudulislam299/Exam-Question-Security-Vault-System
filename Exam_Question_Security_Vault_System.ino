/* Mahmudul Islam
 * EEE,Bangladesh University of Engineering and Technology (BUET)
 * Email: mahmudulislam299@gmail.com
 * Project: Exam Question Security Vault System
 */
#include <Keypad.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <DS3231.h>
#include <Servo.h>


/*************************************************************************/
DS3231  rtc(SDA, SCL);
LiquidCrystal lcd(13,12,11,10,9,8);
Servo myservo;
#define buzzer 2

/*************************************************************************/


/*************************************************************************/
String timestr, datestr;
char hrP[2], minP[2], secP[2], dayP[2], mnP[2], yrP[2] ; //P stand for present

char dayE[2]="13", mnE[2]="09", yrE[2]="17" ; //E stand for exam
char hrE[2]="10", minE[2]="00", secE[2]="00"; //E stand for exam

String lcd_show_time ="13 SEP 17 | 11:00:00";

int openrotate= 90;

int opendate=0, openhr=0, openmin=0;
char doorTime[2];
char dd1, dd0;
/***********************************************************************/

/*************************************************************************/
char password[4];
char pass[4],pass1[4];
String passcode= "4321";

/*************************************************************************/

/*************************************************************************/
int i=0,flag;
char customKey=0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {28,30,32,34}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {36,38,40,42}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
/***********************************************************************************/

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  lcd.begin(20,4);

  
  pinMode(buzzer, OUTPUT);
  myservo.attach(4);
  
  myservo.write(0);
  flag=1;
  
  /*************************************************************************/
  lcd.print(" ELECTRONIC SECURITY  ");
  lcd.setCursor(0,1);
  lcd.print("     LOCK BOX ");
  delay(2000);
  lcd.clear();
  /*************************************************************************/

  /*************************************************************************/
  for(int j=0;j<4;j++)
  EEPROM.write(j,passcode[j]);
  for(int j=0;j<4;j++)
  pass[j]=EEPROM.read(j);
  /*************************************************************************/
}



void open_lock()
{
  customKey = customKeypad.getKey();
  
  if(customKey=='C')
  change();
  
  if(customKey=='D')
  date_time();

  if(customKey=='*')
  {
    myservo.write(0);
    customKey = 0;
  }
  
  if (customKey)
  {
     password[i++]=customKey;
     lcd.print("*");
     beep();
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
    if(!(strncmp(password, pass,4)))
    {
    
      lcd.clear();
      lcd.print("Password Accepted");
      
      correctbeep();
      
      lcd.setCursor(0,0);
      lcd.print("Password Accepted");
      delay(1000);
/****************************test experiment****************************************/
      dd0=minP[0]; //doorTime[0]
      dd1=minP[1]; //doorTime[1]
      if(dd1==9)
      {
        dd0=dd0+1;
        dd1="0";
      }

      else
      {
        dd0=dd0;
        dd1=dd1+1;
      }

      
      lcd.clear();
      while(1)
      {
//        Serial.print(minP[0]);
//        Serial.print(minP[1]);
//        Serial.print("----------");
//        Serial.print(dd0);
//        Serial.println(dd1);
        
        lcd.setCursor(0,1);
        lcd.print("Press * to open");
        

        datestr = rtc.getDateStr();
        timestr= rtc.getTimeStr();

        for(int i=0; i<2; i++)
        {
          hrP[i]= timestr.charAt(i);
          minP[i]=timestr.charAt(i+3);
          secP[i]=timestr.charAt(i+6);
          dayP[i]=datestr.charAt(i);
          mnP[i]=datestr.charAt(i+3);
          yrP[i]=datestr.charAt(i+8);
        }


        customKey = customKeypad.getKey();
  
       if(customKey=='*')
          {
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Door Is Opening");
            myservo.write(openrotate);
            delay(2000);
            lcd.clear();
            break;
          }

        customKey=0;

        if(dd0==minP[0] && dd1==minP[1])
          break;
        
      }

      
      
/****************************test experiment****************************************/
//      lcd.setCursor(0,1);
//      lcd.print("C.Change Password");
//      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0,1);
      i=0;
      
    }
    
    else
    {
      lcd.clear();
      lcd.print("Wrong Password...");
      lcd.setCursor(0,1);
      lcd.print(" ");
      lcd.setCursor(0,2);
      lcd.print("Please Try Again");
      wrongbeep();
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0,1);
      i=0;
  
    }
  }
}



void change()
{
  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print("*");
      beep();
    }
    key=0;
  }
  delay(500);
  
  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("Wrong Password...");
    wrongbeep();
    delay(1000);
    lcd.setCursor(0,2);
    lcd.print("Please Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    
  lcd.clear();
  lcd.print("Enter New Password:");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass[j]=key;
      lcd.print(key);
      EEPROM.write(j,key);
      j++;
      beep();
    }
  }
  lcd.print("  Done..");
  delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0,1);
  customKey=0;
}



void beep()
{
  analogWrite(buzzer, 255);
  delay(80);
  analogWrite(buzzer, 0);
}


void wrongbeep()
{
    analogWrite(buzzer, 200);
    delay(800);
    analogWrite(buzzer,0);
}



void correctbeep()
{
  analogWrite(buzzer, 200);
      delay(200);
      analogWrite(buzzer, LOW);
      delay(200);
      analogWrite(buzzer, 200);
      delay(200);
      analogWrite(buzzer, LOW);
      delay(200);
      analogWrite(buzzer, 200);
      delay(200);
      analogWrite(buzzer, LOW);
}



void date_time()
{
  lcd.clear();
  beep();
  lcd.setCursor(3,0);
  lcd.print(rtc.getDOWStr());
  lcd.setCursor(3,1);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(3,2);
  lcd.print(rtc.getTimeStr());
  delay(2000);
  lcd.clear();
  customKey=0;
  lcd.print("Enter Password:");
  lcd.setCursor(0,1);
}


void time_check()
{
   
  // Send date
  datestr = rtc.getDateStr();
//  Serial.println(datestr);

  // Send time
  timestr= rtc.getTimeStr();
//  Serial.println(timestr);

   for(int i=0; i<2; i++)
  {
    hrP[i]= timestr.charAt(i);
    minP[i]=timestr.charAt(i+3);
    secP[i]=timestr.charAt(i+6);
    dayP[i]=datestr.charAt(i);
    mnP[i]=datestr.charAt(i+3);
    yrP[i]=datestr.charAt(i+8);
  }



// Serial.print(hrP[0]);Serial.println(hrP[1]);
// Serial.print(minP[0]);Serial.println(minP[1]);
// Serial.print(secP[0]);Serial.println(secP[1]);
// 
// Serial.print(dayP[0]);Serial.println(dayP[1]);
// Serial.print(mnP[0]);Serial.println(mnP[1]);
// Serial.print(yrP[0]);Serial.println(yrP[1]);
  

  

  //delay (1000);


if(yrP[0] == yrE[0] && yrP[1] == yrE[1] && mnP[0] == mnE[0] && mnP[1] == mnE[1] && dayP[0]== dayE[0] && dayP[1]==dayE[1] ) //exam date and present soman hole 1 hobe
{
  opendate=1;
  if((hrP[0]>hrE[0])|| (hrP[0]==hrE[0] && hrP[1]>hrE[1])) //hour over hoye gese so box kholar time hoye gese
  {
    openhr=1;
    openmin=1;
  }
  else if(hrP[0]==hrE[0] && hrP[1]==hrE[1]) // hour same so min check kora joruri
  {
    if((minP[0]>minE[0])|| (minP[0]==minE[0] && minP[1]>=minE[1]))
    {
      openmin=1;
      openhr=1;
    }
      
  } 

}




if(opendate==1 && openhr==1 && openmin ==1)
{
  if(flag==1)
  {
     lcd.clear();
     lcd.print("Enter Password:");
     lcd.setCursor(0,1);
     flag++;
  }
  Serial.println("TO open box enter password");
  open_lock();
}
else 
{
  Serial.println("It is not right time to open the box");
  //lcd.clear();
  lcd.print("ITs NOT TIME");
  lcd.setCursor(0,1);
  lcd.print("TO OPEN THE BOX");
  lcd.setCursor(0,2);
  lcd.print("BOX OPEN AT:");
  lcd.setCursor(0,3);
  lcd.print(lcd_show_time);
  
  lcd.setCursor(0,0);
}

//Serial.println(opendate);
//Serial.println(openhr);
//Serial.println(openmin);

}


void loop()
{
  time_check();
  
}
