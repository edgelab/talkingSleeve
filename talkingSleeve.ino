#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"

#include <CapSense.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

#define DEBOUNCE 5  // button debouncer
CapSense   cs_4_2 = CapSense(32,22);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapSense   cs_5_2 = CapSense(32,24);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapSense   cs_6_2 = CapSense(32,26);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapSense   cs_7_2 = CapSense(32,28);
CapSense   cs_8_2 = CapSense(32,30);

long total1;
long total2;
long total3;
long total4;
long total5;

int page = 0;
int buttonPress = 0;
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}



void setup() {
  byte i;

  // set up serial port
  Serial.begin(9600);


  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  cs_5_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  cs_6_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  cs_7_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_8_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.println("Startup!");  // Something went wrong, lets print out why
  // pin13 LED
  pinMode(13, OUTPUT);

  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
   // Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
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
    //Serial.println("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }

  /* // Lets tell the user about what we found
   putstring("Using partition ");
   Serial.print(part, DEC);
   putstring(", type is FAT");
   Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
   */
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    //Serial.println("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }

  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

 // Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;


}

SIGNAL(TIMER2_OVF_vect) {

  check_switches();
}


void check_switches(){
 


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




