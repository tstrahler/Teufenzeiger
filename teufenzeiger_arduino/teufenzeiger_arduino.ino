#define PIN_WD A1 // Watchdog
#define PIN_MR A3 // Master reset

/* Data pins */
#define PIN_D0 A13
#define PIN_D1 A14
#define PIN_D2 A15
#define PIN_D3 52
#define PIN_D4 53
#define PIN_D5 50
#define PIN_D6 51
#define PIN_D7 48
int dataPin[] = {PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7};

#define ROW_0 A12
#define ROW_1 A11
#define ROW_2 A10
#define ROW_3 A9
#define ROW_4 A8
#define ROW_5 A7
#define ROW_6 A6
#define ROW_7 A5
int rowPin[] = {ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7};

#define PIN_LD_ADDR 49

#define PIN_EN_DEMUX_1 A0
#define PIN_EN_DEMUX_2 A4

#define PIN_WR 46



#define ADDR_CTRL 0b000



#include <string.h>

/* Custom functions */
void setData(unsigned char addr){
  for(int i = 0; i < 8; i++)
    digitalWrite(dataPin[i], (addr >> i) & 1);
}

void setRow(unsigned char row, unsigned char level){
  digitalWrite(rowPin[row], level);
}

void loadAddr(){
  digitalWrite(PIN_LD_ADDR, 1);
  delayMicroseconds(10);
  digitalWrite(PIN_LD_ADDR, 0);
}

void setMatrixData(int matrix, unsigned char addr, unsigned char data){
  setData( (1 << 7) | ( ((5 + matrix) << 3) & 0b111000 ) | (addr & 0b111) );
  loadAddr();
  digitalWrite(PIN_WR, 0);
  setData(data);
  digitalWrite(PIN_WR, 1);
}

void setChar(int matrix, int digit, unsigned char character){
  unsigned char addr = (1 << 2) | (digit & 0b11);
  setMatrixData(matrix, addr, character);
}

void setBrightness(int matrix, unsigned char brightness){
  setMatrixData(matrix, ADDR_CTRL, brightness);
}

void setString(char string[]){
  int len = strlen(string);
  for(int i = 0; i < len; i++){
    int matrix = (int)floor(i / 4.0);
    int digit = i % 4;
    setChar(matrix, digit, *(string + len - i - 1));
  }
}

void clearMatrix(int matrix){
    setMatrixData(matrix, ADDR_CTRL, 0b10000000);
    setMatrixData(matrix, ADDR_CTRL, 0b00000000);
    setBrightness(matrix, 3);
}

void displayTime(unsigned char hour, unsigned char minutes, unsigned char seconds){
  char text[12];
  sprintf(text, "  %02d:%02d:%02d  ", hour, minutes, seconds);
  setString(text);
}

void displayTeufe(float teufe){
  char text[12];
  sprintf(text, "+%6d.%02d m", (int)teufe, (int)(teufe*100)%100); 
  setString(text);
}


void setup(){
  // Setup watchdog
  pinMode(PIN_WD, OUTPUT);
  analogWrite(PIN_WD, 127);
  
  // Set Master Reset
  pinMode(PIN_MR, OUTPUT);
  digitalWrite(PIN_MR, 1);
 
  // Set Data pin modes
  for(int i = 0; i < 8; i++)
    pinMode(dataPin[i], OUTPUT);
    
  // Set Load Address Pin
  pinMode(PIN_LD_ADDR, OUTPUT);
  digitalWrite(PIN_LD_ADDR, 0);
  
  // Set DEMUX Pins
  pinMode(PIN_EN_DEMUX_1, OUTPUT);
  pinMode(PIN_EN_DEMUX_2, OUTPUT);
  
  digitalWrite(PIN_EN_DEMUX_1, 0);
  digitalWrite(PIN_EN_DEMUX_2, 1);
  
  // Set WR Prin Dir
  pinMode(PIN_WR, OUTPUT);
  digitalWrite(PIN_WR, 1);
    

  // Init Matrix Displays
  setBrightness(1, 3);
  setBrightness(2, 3);
  setBrightness(3, 3);
  
  for(int i = 0; i < 3; i++)
    clearMatrix(i);
    
  
}

void loop(){
  displayTime(1,2, 3);
}
