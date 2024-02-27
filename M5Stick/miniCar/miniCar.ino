#include "M5StickCPlus2.h"
#include <ESP32Time.h>
#include "Noto.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
ESP32Time rtc(0); 

//......................................colors
#define backColor 0x0026
#define blue 0x055D
#define dataColor 0x0311
#define purple 0xEA16
#define needleColor 0xF811
#define screen 0x8CF4
#define silver 0xC638
#define green 0x25E0
unsigned short tmpColor=0;
unsigned short txtColor=0;

double rad=0.01745;

//first gauge
int sx=65;
int sy=65;
int r=62;
float x[16];
float y[16];
float x2[16];
float y2[16];
float px[16];
float py[16];
float nx[360];
float ny[360];
float nx2[360];
float ny2[360];

//second gauge
int sx2=184;
int sy2=50;
int r2=42;
float dx[16];
float dy[16];
float dx2[16];
float dy2[16];
float dpx[16];
float dpy[16];
float dnx[360];
float dny[360];
float dnx2[360];
float dny2[360];

float speed=0;
int acc=1;
int acc2=0;
float accR[4]={3,1.2,0.5,0.2};
float accS[4]={0.4,0.3,0.2,0.1};
float rpm=0;
int gear=0;
String gearLBL[7]={"N","1","2","3","4","5","6"};
int gearMax[7]={0,15,35,60,90,120,140};
bool lightsOn=false;
bool leftD=false;
bool rightD=false;
int leftBlink=20;
int rightBlink=20;


int brightnes=1;
int briLevel[5]={16,40,48,64,96};
int deb=0;

void setup() {
    pinMode(37,INPUT_PULLUP);
    pinMode(39,INPUT_PULLUP);
    rtc.setTime(0, 0, 0, 1, 1, 2023,0);
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    StickCP2.Display.setRotation(1);
    StickCP2.Display.setBrightness(briLevel[brightnes]);
    sprite.createSprite(240,135);


    int temp=120;
      for(int i=0;i<16;i++)
    {
       x[i]=((r-8)*cos(rad*(temp)))+sx;
       y[i]=((r-8)*sin(rad*(temp)))+sy;
       x2[i]=((r-12)*cos(rad*(temp)))+sx;
       y2[i]=((r-12)*sin(rad*(temp)))+sy;
       
       px[i]=((r-22)*cos(rad*(temp)))+sx;
       py[i]=((r-22)*sin(rad*(temp)))+sy;
       temp=temp+20;
       if(temp>=360)
       temp=0;
    }

    temp=120;
    for(int i=0;i<300;i++)
    {
       nx[i]=((r-8)*cos(rad*(temp)))+sx;
       ny[i]=((r-8)*sin(rad*(temp)))+sy;
       nx2[i]=((r-35)*cos(rad*(temp)))+sx;
       ny2[i]=((r-35)*sin(rad*(temp)))+sy;
       temp++;
       if(temp>=360)
       temp=0;
    }

      temp=120;
      for(int i=0;i<11;i++)
    {
       dx[i]=((r2-8)*cos(rad*(temp)))+sx2;
       dy[i]=((r2-8)*sin(rad*(temp)))+sy2;
       dx2[i]=((r2-10)*cos(rad*(temp)))+sx2;
       dy2[i]=((r2-10)*sin(rad*(temp)))+sy2;
       
       dpx[i]=((r2-19)*cos(rad*(temp)))+sx2;
       dpy[i]=((r2-19)*sin(rad*(temp)))+sy2;
       temp=temp+42;
       if(temp>=360)
       temp=12;
    }

    temp=120;
    for(int i=0;i<300;i++)
    {
       dnx[i]=((r2-8)*cos(rad*(temp)))+sx2;
       dny[i]=((r2-8)*sin(rad*(temp)))+sy2;
       dnx2[i]=((r2-26)*cos(rad*(temp)))+sx2;
       dny2[i]=((r2-26)*sin(rad*(temp)))+sy2;
       temp++;
       if(temp>=360)
       temp=0;
    }
  
}


void draw()
{
  sprite.fillSprite(backColor);
  sprite.setTextColor(silver, backColor);
  sprite.drawString("m/h",sx,sy+14);
  sprite.setTextDatum(4);
  sprite.drawArc(sx, sy, r, r-2, 30, 330, blue, backColor);
  sprite.drawArc(sx, sy, r-4, r-6, 30, 330, silver, backColor);
  sprite.drawArc(sx, sy, r-8, r-10, 270, 330, purple, backColor);
  sprite.drawArc(sx, sy, 26, 25, 30, 330, silver, backColor);


  for(int i=0;i<16;i++){

    if(i<12 && i%2==0)
    {tmpColor=blue;
    txtColor=silver;}

    if(i<12 && i%2!=0)
    {tmpColor=silver;
    txtColor=silver;}

    if(i>=12){
    tmpColor=purple;
    txtColor=purple;
    }
   
  sprite.setTextColor(txtColor,TFT_BLACK);
  if(i%2==0)
  sprite.drawString(String(i*10),px[i],py[i]);
  
  sprite.drawWedgeLine(x[i], y[i], x2[i], y2[i], 2, 1, tmpColor); 
} 

  
//################second gauge
sprite.drawArc(sx2, sy2, r2, r2-1, 30, 330, blue, backColor);
sprite.drawArc(sx2, sy2, r2-3, r2-4, 30, 330, silver, backColor);
sprite.drawArc(sx2, sy2, r2-28, r2-29, 30, 330, silver, backColor);
 sprite.drawArc(sx2, sy2, r2-7, r2-8, 270, 330, purple, backColor);

for(int i=0;i<8;i++){

    if(i<12 && i%2==0)
    {tmpColor=blue;
    txtColor=silver;}

    if(i<12 && i%2!=0)
    {tmpColor=silver;
    txtColor=silver;}

    if(i>=12){
    tmpColor=purple;
    txtColor=purple;
    }
   
  sprite.setTextColor(txtColor,TFT_BLACK);
  sprite.drawString(String(i),dpx[i],dpy[i]);
  sprite.drawWedgeLine(dx[i], dy[i], dx2[i], dy2[i], 1, 1, blue); 
} 


   //needles
  sprite.drawWedgeLine(nx[(int)speed*2], ny[(int)speed*2], nx2[(int)speed*2], ny2[(int)speed*2], 2, 2, needleColor);
  sprite.drawWedgeLine(dnx[(int)rpm], dny[(int)rpm], dnx2[(int)rpm], dny2[(int)rpm], 2, 2, needleColor);

  // #direction right
  if(rightBlink<10) tmpColor=TFT_ORANGE; else tmpColor=dataColor;
  sprite.fillRect(134,10,8,8,tmpColor);
  sprite.fillTriangle(142, 7, 142, 21, 148, 13, tmpColor);
  
  // #direction rleft
   if(leftBlink<10) tmpColor=TFT_ORANGE; else tmpColor=dataColor;
  sprite.fillRect(118,10,8,8,tmpColor);
  sprite.fillTriangle(118, 7, 118, 21, 112, 13, tmpColor);
  
  //screen time
  sprite.fillRoundRect(145,96,80,30,2,dataColor);
  sprite.fillRoundRect(147,98,76,26,2,screen);
  sprite.setTextColor(backColor,screen);
  sprite.drawString("CHECK ENGINE",186,104);

   //screen miles
  sprite.fillRoundRect(44,110,42,16,2,dataColor);
  sprite.fillRoundRect(46,112,40,14,2,screen);
  sprite.drawString("55621",66,119);


  //lights
  if(lightsOn) tmpColor=green; else tmpColor=dataColor;
  sprite.fillCircle(136,82,5,tmpColor);
  sprite.fillRect(136,77,6,11,tmpColor);
  for(int i=0;i<4;i++)
  sprite.fillRect(144,78+(i*3),3,1,tmpColor);

  //batery
  sprite.drawRect(124,110,16,14,TFT_RED);
  sprite.fillRect(127,108,3,2,TFT_RED);
  sprite.fillRect(134,108,3,2,TFT_RED);
  sprite.drawLine(130,114,130,118,TFT_RED);
  sprite.drawLine(128,116,132,116,TFT_RED);

  //STRINGS
  sprite.loadFont(Noto);
  sprite.setTextColor(silver,backColor);
  sprite.drawString(String((int)speed),sx,sy-2);
  sprite.drawString(gearLBL[gear],sx2-1,sy2+2);
  sprite.setTextColor(backColor,screen);
  sprite.drawString(String(rtc.getTime()),184,118);
  sprite.unloadFont();

  //brightness bar
  for(int i=0;i<brightnes;i++)
  sprite.fillRect(6+(i*4),6,2,6,TFT_ORANGE);

  StickCP2.Display.pushImage(0,0,240,135,(uint16_t*)sprite.getPointer()); 
}

void loop() {
  StickCP2.update();
  
  acc=map((int)rpm,0,260,0,4);

   if(digitalRead(37)==0)
   {if(rpm<260)rpm=rpm+accR[acc];;
   if(speed<gearMax[gear]) speed=speed+0.4;
   }else {
    if(rpm>0) rpm=rpm-4;
    if(rpm<0) rpm=0;
    if(speed>0) speed=speed-1;
    if(speed<0) {speed=0; gear=0;}}

    if(digitalRead(39)==0)
    {
    if(deb==0){ deb=1;if(gear<6) gear++;}
    }else deb=0;
      

  if(rightD){ rightBlink++; if(rightBlink>20) rightBlink=0;}
  if(leftD){ leftBlink++; if(leftBlink>20) leftBlink=0;}
  
  draw();

}
