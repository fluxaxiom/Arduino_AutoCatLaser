/* Automatic Cat Laser

   This sketch runs one of 12 functions at random. The functions vary -
   The range restriction controls the extent of both servos in each direction, making it adjustable to the room.
   0 - 180 is the full range on a standard servo.
   The warpSpeed variable controls the speed. It is a divisor of the delay so lower value is slower, 1.0 is normal speed. 

   created Jan 2016
   by Frank Appio
   http://fluxaxiom.com/
   Feel free to modify or use as you wish
*/

int rangeRestrictX[] {50,170}; 
int rangeRestrictY[] {65,160}; 

float warpSpeed = 1.0;





#include <Servo.h> // Using the standard Arduino servo library to keep things simple. 

// Attach servos and laser
Servo servoX; 
Servo servoY;
const int laser=3;

// Initialize some variables
int posX = 0;
int posY = 0;
int lastPosX = 0;
int lastPosY = 0;

int rangeX = rangeRestrictX[1]-rangeRestrictX[0];
int rangeY = rangeRestrictY[1]-rangeRestrictY[0];



void setup() {
    pinMode(laser, OUTPUT);
    servoX.attach(4);
    servoY.attach(8); 
    
    // Shuffle the random algorithm with an unconnected pin
    randomSeed(analogRead(0)); 
}



void loop() {
    digitalWrite(laser,HIGH);
    float angle = 0;
    int sPosX = posX;
    int sPosY = posY;
    
    // Random variables
    int selectRandom = random(0,12); 
    int randomDelay = random(10,51)/warpSpeed;
    int randomDelay2 = random(10,101)/warpSpeed;
    int randomDelay3 = random(20,31)/warpSpeed;
    int randomSteps = random(1,4);
    int randomSteps2 = random(1,5);
    bool luck = random(0,2);
    int circleXcenter = random(80,101);
    int circleYcenter = random(80,101);

    // Keep things in bounds - needed for the unrestricted functions or you will wind up on the ceiling.
    if (posX<rangeRestrictX[0] || posX>rangeRestrictX[1]) { 
      posX=rangeRestrictX[1]-rangeX/2;
      servoX.write(posX); }
    if (posY<rangeRestrictY[0] || posY>rangeRestrictY[1]) {
      posY=rangeRestrictY[1]-rangeY/2;
      servoY.write(posY); }

    
    switch (selectRandom) { 

// Taunt      
      case 0:     
                for (int i=0; i<6; i++) {
                  posX=random(rangeRestrictX[0]+rangeX/4,rangeRestrictX[1]-rangeX/4);
                  posY=random(rangeRestrictY[0]+rangeY/4,rangeRestrictY[1]-rangeY/4);
                  servoX.write(posX);
                  servoY.write(posY);
                  delay(randomDelay3*30/warpSpeed);
                }
        break;

// Wobble      
      case 1: 
                for (int rad = 5; rad < 15; rad++) {
                  for (int i = 5; i > 0; i--) {
                    angle = i*2*3.14/10;
                    lastPosX=posX;
                    lastPosY=posY;
                    posX = circleXcenter + (cos(angle) * rad);
                    posY = circleYcenter + (sin(angle) * rad);
                    if (posX>lastPosX) { // Slow things down
                      posX=lastPosX+1;
                    }
                    else {
                      posX=lastPosX-1;
                    }
                    if (posY>lastPosY) {
                      posY=lastPosY+1;
                    }
                    else {
                      posY=lastPosY-1;
                    }
                    servoX.write(posX);
                    servoY.write(posY);
                    
                    if (luck) {
                      if (sPosX>rangeX/2) {
                        circleXcenter-=1;
                      }
                      else {
                        circleXcenter+=1;
                      }
                    }
                    else {
                      if (sPosY>rangeY/2) {
                        circleYcenter-=1;
                      }
                      else {
                        circleYcenter+=1;
                      }
                    }
                    delay(randomDelay*2); }
                }                              
        break;

// Scan        
      case 2:    
                posY=random(rangeRestrictY[0]+rangeY/10,rangeRestrictY[1]-rangeY/4);
                servoY.write(posY);
                for (posX = rangeRestrictX[0]; posX <= rangeRestrictX[1]; posX += 1) { 
                  servoX.write(posX);              
                  delay(randomDelay);                       
                }
                for (posX = rangeRestrictX[1]; posX >= rangeRestrictX[0]; posX -= 1) { 
                  servoX.write(posX);              
                  delay(randomDelay2);                      
                }
        break;

// Zip        
      case 3:    
                for (int i=0; i<3; i++) {
                  posX=random(rangeRestrictX[0],rangeRestrictX[1]);
                  posY=random(rangeRestrictY[0],rangeRestrictY[1]);
                  servoX.write(posX);
                  servoY.write(posY);
                  delay(randomDelay2*10);
                }
        break;

// Boomerang        
      case 4: 
                lastPosX=posX;
                sPosX=random(rangeRestrictX[0],rangeRestrictX[1]);
                if (posX<sPosX) {
                  for (lastPosX; posX <= sPosX; posX++) {
                    servoX.write(posX);
                    delay(randomDelay3);
                  }
                }                
                else {
                  for (lastPosX; posX >= sPosX; posX--) {
                    servoX.write(posX);
                    delay(randomDelay3);
                  }
                } 
                for (posY; posY >= rangeRestrictY[0]; posY--) {
                  posY-=1;
                  servoY.write(posY);
                  delay(randomDelay3);
                }
                for (posY = rangeRestrictY[0]; posY <= rangeRestrictY[1]-rangeY/5; posY += 1) { 
                  if (posY%2){ // Wobble on the throw
                    posX+=1;
                    servoX.write(posX);  
                  }
                  else {
                    posX-=1;
                    servoX.write(posX);  
                  }
                  servoY.write(posY);              
                  delay(randomDelay);                       
                }
                if (luck>0) { // If we have no luck, the boomerang doesn't come back
                  for (posY = rangeRestrictY[1]-rangeY/5; posY >= rangeRestrictY[0]; posY -= 1) { 
                    if (posY>rangeRestrictY[1]-rangeY/2){ // Curve on return
                      if(posY%2){
                        posX+=1;
                        servoX.write(posX);  
                      }
                    }
                    else {
                      if(posY%2){
                        posX-=1;  
                        servoX.write(posX); 
                      } 
                    }
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }}
        break;

// Creep        
      case 5: 
                lastPosX=posX;
                sPosX=(posX+(rangeRestrictX[1]-rangeX/2))/2;
                if (posX<sPosX) {
                  for (lastPosX; posX <= sPosX; posX++) {
                    servoX.write(posX);
                    delay(randomDelay3);
                  }
                }                
                else {
                  for (lastPosX; posX >= sPosX; posX--) {
                    servoX.write(posX);
                    delay(randomDelay3);
                  }
                } 
                lastPosY=posY;
                sPosY=(posY+(rangeRestrictY[1]-rangeY/2))/2;
                if (posY<sPosY) {
                  for (lastPosY; posY <= sPosY; posY++) {
                    servoY.write(posY);
                    delay(randomDelay3);
                  }
                }                
                else {
                  for (lastPosY; posY >= sPosY; posY--) {
                    servoY.write(posY);
                    delay(randomDelay3);
                  }
                }                       
                for (int i=0; i<20; i++){
                  if (luck) {  
                    posX+=randomSteps;
                    posY+=randomSteps;
                  }
                  else {
                    posX-=randomSteps;
                    posY-=randomSteps;  
                  }
                  servoX.write(posX);              
                  servoY.write(posY);   
                  delay(randomDelay3*15/warpSpeed);           
                }
        break; 

// Squiggle        
      case 6: 
                for (int i=0; i<120; i++){
                  if (i%2) { 
                    posX+=randomSteps2;
                    posY+=randomSteps;
                  }
                  else {
                    posX-=randomSteps2;
                    posY-=randomSteps;  
                  }
                  servoX.write(posX);
                  servoY.write(posY);
                  delay(randomSteps*5/warpSpeed);
                }
        break;

// Blink        
      case 7: 
                for (int i=0; i<10; i++) {
                  digitalWrite(laser,LOW);
                  delay(randomDelay+randomDelay2+(20/warpSpeed));
                  digitalWrite(laser,HIGH);
                  delay(randomDelay+randomDelay2+(20/warpSpeed));
                }
        break; 

// Circle                    
      case 8: 
                if (luck){
                for (int rad = 5; rad < 20; rad++) {
                  for (int i = 0; i < 10; i++) {
                    angle = i*2*3.14/10;
                    posX = circleXcenter + (cos(angle) * rad);
                    posY = circleYcenter + (sin(angle) * rad);     
                    servoX.write(posX);
                    servoY.write(posY);
                    delay(randomSteps2*6/warpSpeed); }}
                }
                else {
                for (int rad = 20; rad > 5; rad--) {
                  for (int i = 10; i > 0; i--) {
                    angle = i*2*3.14/10;
                    posX = circleXcenter + (cos(angle) * rad);
                    posY = circleYcenter + (sin(angle) * rad);
                    servoX.write(posX);
                    servoY.write(posY);
                    delay(randomSteps2*6/warpSpeed); }}
                }   
        break;

// ZigZag
      case 9:   
                if (luck) {
                  for (posX = rangeRestrictX[0]; posX <= rangeRestrictX[0]+rangeX/10; posX += 1) { 
                    posY+=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                  for (posX = rangeRestrictX[0]+rangeX/10; posX <= rangeRestrictX[0]+rangeX/5; posX += 1) { 
                    posY-=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                  for (posX = rangeRestrictX[0]+rangeX/5; posX <= rangeRestrictX[0]+rangeX/3.3; posX += 1) { 
                    posY+=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                  for (posX = rangeRestrictX[0]+rangeX/3.3; posX <= rangeRestrictX[0]+rangeX/2.5; posX += 1) { 
                    posY-=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                  for (posX = rangeRestrictX[0]+rangeX/2.5; posX <= rangeRestrictX[0]+rangeX/2; posX += 1) { 
                    posY+=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                  for (posX = rangeRestrictX[0]+rangeX/2; posX <= rangeRestrictX[0]+rangeX/1.65; posX += 1) { 
                    posY-=randomSteps;
                    servoX.write(posX); 
                    servoY.write(posY);             
                    delay(randomDelay);                       
                  }
                }
                else { 
                  for (posY = rangeRestrictY[1]-rangeY/10; posY >= rangeRestrictY[1]-rangeY/5; posY -= 1) { 
                    posX-=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }
                  for (posY = rangeRestrictY[1]-rangeY/5; posY >= rangeRestrictY[1]-rangeY/3.3; posY -= 1) { 
                    posX+=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }                  
                  for (posY = rangeRestrictY[1]-rangeY/3.3; posY >= rangeRestrictY[1]-rangeY/2.5; posY -= 1) { 
                    posX-=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }
                  for (posY = rangeRestrictY[1]-rangeY/2.5; posY >= rangeRestrictY[1]-rangeY/2; posY -= 1) { 
                    posX+=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }                    
                  for (posY = rangeRestrictY[1]-rangeY/2; posY >= rangeRestrictY[1]-rangeY/1.65; posY -= 1) { 
                    posX-=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }  
                  for (posY = rangeRestrictY[1]-rangeY/1.65; posY >= rangeRestrictY[1]-rangeY/1.425; posY -= 1) { 
                    posX+=randomSteps2;
                    servoX.write(posX);
                    servoY.write(posY);              
                    delay(randomDelay2);                      
                  }                                                      
                }
        break;

// Chase
      case 10:  
                posY=random(rangeRestrictY[0]+rangeY/10,rangeRestrictY[1]-rangeY/4);
                servoY.write(posY); 
                posX=rangeRestrictX[1]-rangeX/2;
                servoX.write(posX);     
                for (int i = 40; i > 0; i--) {
                  luck = random(0,2); // Making our own luck
                  if (luck) {
                    posX+=randomSteps2*2;
                    servoX.write(posX);
                  }
                  else {
                    posX-=randomSteps*2;
                    servoX.write(posX);
                  }
                  delay(randomDelay2); 
                }
        break;
        
// Hide
      case 11:    
                if (luck) {
                  digitalWrite(laser,LOW); // Laser off
                  delay(randomSteps2*1000/warpSpeed);
                }
        break;
      } // End switch
                
} // End loop
