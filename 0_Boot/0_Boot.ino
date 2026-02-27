// ╔════════════════════════╗
// ║     UPLOAD SETTINGS    ║
// ╚════════════════════════╝

// Board = ESP32S3 Dev Module
// (Tools) USB CDC On Boot = Enabled
// (Tools) Flash Size = 16Mb
// (Tools) Upload Mode = USB-OTG CDC (TinyUSB)
// (Tools) USB Mode = USB-OTG (TinyUSB)



// ╔════════════════════════╗
// ║       INITIATING       ║
// ╚════════════════════════╝

// Libraries
#include <FastLED.h>
#include <Control_Surface.h>
#include <Preferences.h>  // Bibliotek för att spara till NVS (Non-Volatile Storage)

Preferences prefs;

// Define pins
const int POT[] = {15, 16, 17, 12, 13, 14};
const int MATRIX_ROW[] = {48, 45, 38, 37, 36, 35};
const int MATRIX_COLUMN[] = {44, 42, 41, 40, 39};
const int SHIFT = 43;
const int LED_SIGNAL = 18;

// General
#define C_SHIFT_SHOW              CRGB(0, 255, 255)   // Cyan
#define C_SHIFT_PROG              CRGB(255, 0, 0)     // Röd
#define C_SHIFT_COLOR             CRGB(255, 255, 0)   // Gul 
#define C_SHIFT_PIANO             CRGB(255, 0, 255)   // Magenta
#define C_SHIFT_BRIGHTNESS        CRGB(255, 255, 255) // Vit
#define C_SHIFT_PAGE              CRGB(0, 255, 0)     // Grön
#define C_PROG_MOMENTARY          CRGB(255, 0, 0)     // Röd
#define C_PROG_MOMENTARY_PRESSED  CRGB(255, 255, 255) // Vit
#define C_PROG_TOGGLE             CRGB(0, 0, 255)     // Blå

#define BLINK_SPEED_MS 300
#define PULSE_FREQ_MS 4000



// Shift button
enum ButtonState { OFF, PRESS, HOLD, RELEASE };
ButtonState shift_state = OFF;
enum ControllerMode { SHOW, PROG, COLOR, PIANO };
ControllerMode mode = SHOW;
unsigned long shift_click_time;
int page = 0;
int i_prog = 0;
#define PROG_CLICK_TIME_MS 400         // Max tillåten tid mellan shiftklick

// Potentiometers
#define TOT_POTS 6
int i_pot = 0;
float pot_value = 0; 
float pot_value_list[TOT_POTS] = {0};
float pot_analyzed_list[TOT_POTS] = {0};

// Button matrix
#define TOT_ROWS 6
#define TOT_COLUMNS 5
int i_col = 0;
int i_row = 0;
ButtonState matrix_value_list[TOT_ROWS][TOT_COLUMNS] = {};
enum MIDISetting { MOMENTARY, TOGGLE };
MIDISetting matrix_midisetting_list[TOT_ROWS][TOT_COLUMNS][20] = {};  // 20 pages
bool toggle_activated_list[TOT_ROWS][TOT_COLUMNS][20] = {};    // För att komma ihåg om den är igång eller ej

// LED control
int matrix_color_list[TOT_ROWS][TOT_COLUMNS][20] = {};  // 20 pages
#define TOT_LEDS 30
int brightness_level = 2;   // Ljusstyrkan vid startup
const int BRIGHTNESS[] = {5, 30, 70, 255};
unsigned long last_millis = 0;
CRGB leds[TOT_LEDS];

// PIANO
int page_piano = 0;
int piano_sharps[] = {0, 1, 3, 4, 10, 11, 12, 14, 20, 22, 23};
int midi_map[5][6][5] = {                         // Hårdkodar alla midivärden
  { // ---------- PAGE 0 (Vanlig) ----------
    {49, 51, -1, 54, 56},
    {48, 50, 52, 53, 55},
    {66, 68, 70, -1, 73},
    {65, 67, 69, 71, 72},
    {58, -1, 61, 63, -1},
    {57, 59, 60, 62, 64}
  },
  { // ---------- PAGE 1 (Major) ----------
    {52, 53, 55, 57, 59},
    {43, 45, 47, 48, 50},
    {86, 88, 89, 91, 93},
    {77, 79, 81, 83, 84},
    {69, 71, 72, 74, 76},
    {60, 62, 64, 65, 67}
  },
  { // ---------- PAGE 2 (Pentatonisk) ----------
    {48, 50, 52, 55, 57},
    {36, 38, 40, 43, 45},
    {96, 98, 100, 103, 105},
    {84, 86, 88, 91, 93},
    {72, 74, 76, 79, 81},
    {60, 62, 64, 67, 69}
  },
  { // ---------- PAGE 3 (Blues) ----------
    {51, 53, 54, 55, 58},
    {41, 42, 43, 46, 48},
    {90, 91, 93, 96, 99},
    {79, 82, 84, 87, 89},
    {70, 72, 75, 77, 78},
    {60, 63, 65, 66, 67}
  },
  { // ---------- PAGE 4 (Arabisk) ----------
    {52, 53, 55, 56, 59},
    {43, 44, 47, 48, 49},
    {85, 88, 89, 91, 92},
    {77, 79, 80, 83, 84},
    {68, 71, 72, 73, 76},
    {60, 61, 64, 65, 67}
  }
};



// MIDI Communication
USBMIDI_Interface midi;     // Initialize the MIDI interface
const int velocity = 127;   // 127 is maximum velocity
int channel = 0;

CCPotentiometer pot0 {POT[0], {1, Channel(channel)}};   // Analog pin, CC number 1 (modulation wheel), MIDI channel 1
CCPotentiometer pot1 {POT[1], {2, Channel(channel)}};
CCPotentiometer pot2 {POT[2], {3, Channel(channel)}};
CCPotentiometer pot3 {POT[3], {4, Channel(channel)}};
CCPotentiometer pot4 {POT[4], {5, Channel(channel)}};
CCPotentiometer pot5 {POT[5], {6, Channel(channel)}};


int i;int j;
int knapp = 0;
int temp_1 = 0;
unsigned long last = millis();
int counter = 0;




// ╔════════════════════════╗
// ║          SETUP         ║
// ╚════════════════════════╝

// Funktioner som defenieras senare
void saveMatrixData();
void loadMatrixData();



void setup() {
  Serial.begin(115200);
 
  // Shift button
  pinMode(SHIFT, INPUT_PULLUP);

  // Potentiometers
  for(i_pot = 0; i_pot <= 5; i_pot++){
    pinMode(POT[i_pot], INPUT);
  }

  // Button matrix
  for (int i_col = 0; i_col <= 4; i_col++) {
    pinMode(MATRIX_COLUMN[i_col], OUTPUT);
    digitalWrite(MATRIX_COLUMN[i_col], LOW);  // Starta avstängda
  }
  for (int i_row = 0; i_row <= 5; i_row++) {
    pinMode(MATRIX_ROW[i_row], INPUT_PULLDOWN);
  }

  // LEDs
  FastLED.addLeds<SK6812, LED_SIGNAL, GRB>(leds, TOT_LEDS);
  FastLED.setBrightness(BRIGHTNESS[brightness_level]);

  // MIDI output
  Control_Surface.begin();

  pot0.invert();
  pot1.invert();
  pot2.invert();
  pot3.invert();
  pot4.invert();
  pot5.invert();

  fill_solid(leds, TOT_LEDS, CRGB::Black);
  FastLED.show();

  loadMatrixData();
}