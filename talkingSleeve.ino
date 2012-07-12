#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"

#include <CapSense.h>

SdReader card;    
FatVolume vol;    
FatReader root;   
FatReader f;      

WaveHC wave;      


CapSense   cs_4_2 = CapSense(32,22);        
CapSense   cs_5_2 = CapSense(32,24);        
CapSense   cs_6_2 = CapSense(32,26);        
CapSense   cs_7_2 = CapSense(32,28);
CapSense   cs_8_2 = CapSense(32,30);

long total1;
long total2;
long total3;
long total4;
long total5;

int page = 0;
int buttonPress = 0;


void setup() {
  byte i;

  // set up serial port
  Serial.begin(9600);


  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);    
  cs_5_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     
  cs_6_2.set_CS_AutocaL_Millis(0xFFFFFFFF);    
  cs_7_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_8_2.set_CS_AutocaL_Millis(0xFFFFFFFF);


  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);


  pinMode(13, OUTPUT);


  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }

  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    Serial.println("No valid FAT partition!");
    while(1);                            // then 'halt' - do nothing!
  }


  if (!root.openRoot(vol)) {
    Serial.println("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }


}


void loop() {

  total1 =  cs_4_2.capSense(30);
  total2 =  cs_5_2.capSense(30);
  total3 =  cs_6_2.capSense(30);
  total4 =  cs_7_2.capSense(30);
  total5 =  cs_8_2.capSense(30);
  long start = millis();



  if (total1 > 100 && buttonPress != 1){

    buttonPress = 1;

  }
  else if (total2 > 100&& buttonPress != 2){

    buttonPress = 2;
  }
  else if (total3 > 100&& buttonPress != 3){

    buttonPress =3;
  } 
  else if (total4 > 100&& buttonPress != 4){

    buttonPress =4;
  }
  else if (total5 > 100&& buttonPress != 5){

    buttonPress =5;
  }

  if (buttonPress == 1) {
    if (page == 1){

      playfile("ZOE.WAV");
    }
    if (page == 2){
      playfile("MOM.WAV");
    }
    if (page == 3){
      playfile("YES.WAV");
    }
    if (page == 4){
      playfile("SAD.WAV");
    }

  }
  if (buttonPress == 2) {
    if (page == 1){

      playfile("HAPPY.WAV");
    }
    if (page == 2){
      playfile("DAD.WAV");
    }
    if (page == 3){
      playfile("NO.WAV");
    }
    if (page == 4){
      playfile("KISS.WAV");
    }

  }
  if (buttonPress == 3) {
    if (page == 1){

      playfile("HUNGRY.WAV");
    }
    if (page == 2){
      playfile("PLAY.WAV");
    }
    if (page == 3){
      playfile("TIRED.WAV");
    }
    if (page == 4){
      playfile("EAT.WAV");
    }
  }
  if (buttonPress == 4 && page != 3) {
    page = page + 1;
    buttonPress = 0;
    delay(500);
  }
  if (buttonPress == 5 && page != 1) {
    page = page - 1;
    buttonPress = 0;
    delay(500);
  }



  Serial.println(page);
}


// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  Serial.println("PLAY");
  while (wave.isplaying) {
    // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    // putstring("Couldn't open file "); 
    // Serial.print(name); 
    return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    // Serial.println("Not a valid WAV"); 
    return;
  }

  // ok time to play! start playback
  wave.play();
  buttonPress = 0;
}





