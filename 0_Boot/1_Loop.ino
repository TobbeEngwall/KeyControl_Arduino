void read_pots();
void read_matrix();
void read_shift();

void select_page();
void update_leds_show();
void update_leds_prog();
void update_leds_color();

void send_midi();


// ╔══════════════════════╗
// ║         LOOP         ║
// ╚══════════════════════╝

void loop() {
  
  read_shift();
  read_pots();
  read_matrix();

  

  if(shift_state == HOLD) {
    select_page();
  }

  else {
    switch (mode) {
      case SHOW:
        update_leds_show();
        send_midi_show();
        break;

      case PROG:
        update_leds_prog();
        break;

      case COLOR:
        update_leds_color();
        break;
      
      case PIANO:
        update_leds_piano();
        send_midi_piano();
        break;
    }
  }


  midi.update();
  FastLED.show();

  delay(5);    // Denna förhindrar LED glitchar från FastLED. Vettefan varför men det funkar skitbra
}