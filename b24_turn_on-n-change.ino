#include <IRremote.h>

int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;
int gear_light = 6; // PWM pin
int pos_light_red = 11;
int pos_light_green = 10;
int pos_light_white = 9; //pwm pin
int pos_light_white_brightness = 25;
int beacon_light_red = 8;
int beacon_light_white = 7; // reserved
int form_light_blue = 12;

// status of lights: 0 - off. 1 - on, 2 - to be turned off
int gl_brightness = 0;
int pos_light_status = 0;
int beacon_light_status = 0;
int gear_light_status = 0;
int form_light_status = 0;
int startup_seq = 0;


void setup(){
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(gear_light, OUTPUT);
  pinMode(pos_light_red, OUTPUT);
  pinMode(pos_light_green, OUTPUT);
  pinMode(pos_light_white, OUTPUT);
  pinMode(beacon_light_red, OUTPUT);
  pinMode(beacon_light_white, OUTPUT);
  pinMode(form_light_blue, OUTPUT);
}

void loop(){
    if (irrecv.decode(&results)){

        switch(results.value){
          case 0xFF30CF: //Keypad button "1" change status of position lights
            pos_light_status = pos_light_status + 1;
            break;
          case 0xFF18E7: //Keypad button "2" change status of lights
            beacon_light_status = beacon_light_status + 1;
            break;
          case 0xFF7A85: //keypad button "3" change status of gears
            gear_light_status = gear_light_status + 1;
            break;
          case 0xFF10EF: // keypad button "4" change status of formation lights
            form_light_status = form_light_status + 1;
            break;
          case 0xFF38C7: // keypad button "5" changes the status of all lights. Needs to be reworked as currently on lights are turned off
            startup_seq = startup_seq + 1; // Change status of startup_seq
            if (startup_seq == 1) 
              {
                pos_light_status =1;
                beacon_light_status = 1;
                gear_light_status =1;
                form_light_status =1;
              };
            if (startup_seq==2)
              {
                pos_light_status = 2;
                beacon_light_status = 2;
                gear_light_status = 2;
                form_light_status = 2;
                startup_seq =0;
              }
            
            break;
           
        };

        irrecv.resume();
    };
    
     if (gear_light_status == 1)
    { if (gl_brightness <= 255) // do incremental brightness increase untill brightness is max
      do {
    analogWrite(gear_light, gl_brightness);
    gl_brightness = gl_brightness + 5; 
    delay (50);} 
    while (gl_brightness <= 255);
    gear_light_status = 1; // gear light turned on
    };
    
    if (pos_light_status == 1) {
     analogWrite(pos_light_white, pos_light_white_brightness);
     digitalWrite (pos_light_green, HIGH);
     digitalWrite (pos_light_red, HIGH);
     pos_light_status = 1; // positioning lights turned on
    };

    if (beacon_light_status == 1) {
     digitalWrite(beacon_light_red, HIGH);  
     delay(150);                       
     digitalWrite(beacon_light_red, LOW);   
     delay(150);  
     digitalWrite(beacon_light_red, HIGH);   
     delay(150);                      
     digitalWrite(beacon_light_red, LOW);
     delay(550);    
     beacon_light_status = 1; // beacon lights turned on
   };
    
    if(form_light_status==1)
    { digitalWrite (form_light_blue, HIGH);
      form_light_status=1; // formation lights turned on
    };

    if (pos_light_status == 2){
      digitalWrite (pos_light_white, LOW);
      digitalWrite (pos_light_red, LOW);
      digitalWrite (pos_light_green, LOW);
      pos_light_status = 0; // position lights turned off
    };
    
    if (beacon_light_status == 2) {
     digitalWrite(beacon_light_red, LOW);      
     beacon_light_status = 0; // beacon lights turned off
   };

    if (gear_light_status == 2) {
      analogWrite(gear_light,0);
      gl_brightness =0; // gear lights brightness set 0
      gear_light_status=0; // gear lights turned off
    }

    if (form_light_status == 2) {
      digitalWrite (form_light_blue, LOW);
      form_light_status = 0; // formation lights turned off
    }

}
