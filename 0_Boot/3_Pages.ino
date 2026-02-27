// ╔═══════════════════════╗
// ║         SHIFT         ║
// ╚═══════════════════════╝

void select_page() {
  
  for (i_row = 0; i_row <= TOT_ROWS-1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      leds[i_col + i_row*5] = CRGB::Black;
      if (matrix_value_list[i_row][i_col] == PRESS) {     // Om den hittar en knapptryckning
        if (i_row >= 2 && mode == PIANO) {                              // Egen pagefunktion för pianomode
          if (i_col + i_row*5 - 10 <= 4) {
            page_piano = i_col + i_row*5 - 10;
          }
        }
        else if (i_row >= 2) {                            // Väljer page
          page = i_col + i_row*5 - 10;
          channel = i_row - 2;
        }
        else if (i_col + i_row*5 == 0) {                  // Väljer SHOW
          mode = SHOW;
          saveMatrixData();
        }
        else if (i_col + i_row*5 == 1) {                  // Väljer PROG
          mode = PROG;
        }
        else if (i_col + i_row*5 == 2) {                  // Väljer COLOR
          mode = COLOR;
        }
        else if (i_col + i_row*5 == 3) {                  // Väljer PIANO
          mode = PIANO;
        }
        else if (i_col + i_row*5 == 9) {                  // Ändrar ljusstyrkan
          brightness_level++;
          if (brightness_level >= sizeof(BRIGHTNESS)/sizeof(BRIGHTNESS[0])) {   // Om vi nått maxnivån, reset till lägsta
            brightness_level = 0;
          }
        }
      }
    }
  }

  FastLED.setBrightness(BRIGHTNESS[brightness_level]);
  leds[0] = C_SHIFT_SHOW;
  leds[1] = C_SHIFT_PROG;
  leds[2] = C_SHIFT_COLOR;
  leds[3] = C_SHIFT_PIANO;

  // Blinkar valda modet
  if (millis() % (2*BLINK_SPEED_MS) <= BLINK_SPEED_MS) {
    leds[mode] = CRGB::Black;
  }
  else {
    if (mode == SHOW) {
      leds[mode] = C_SHIFT_SHOW;
    }
    else if (mode == PROG) {
      leds[mode] = C_SHIFT_PROG;
    }
    else if (mode == COLOR) {
      leds[mode] = C_SHIFT_COLOR;
    }
    else if (mode == PIANO) {
      leds[mode] = C_SHIFT_PIANO;
    }
  }

  leds[9] = C_SHIFT_BRIGHTNESS;

  if (mode == PIANO) {                  // Egen pagefunktion för pianomode
    leds[10 + page_piano] = CRGB::Magenta;
  }
  else {
    leds[10 + page] = C_SHIFT_PAGE;
  }
}



// ╔══════════════════════╗
// ║         SHOW         ║
// ╚══════════════════════╝

void update_leds_show() {

  for (i_row = 0; i_row <= 1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {

      switch (matrix_color_list[i_row][i_col][0]) {
        case 0: {
          leds[i_col + 5*i_row] = CRGB::Black;
          break;
        }
        case 1: {
          leds[i_col + 5*i_row] = CRGB::Red;
          break;
        }
        case 2: {
          leds[i_col + 5*i_row] = CRGB::Green;
          break;
        }
        case 3: {
          leds[i_col + 5*i_row] = CRGB::Blue;
          break;
        }
        case 4: {
          leds[i_col + 5*i_row] = CRGB::Cyan;
          break;
        }
        case 5: {
          leds[i_col + 5*i_row] = CRGB::Magenta;
          break;
        }
        case 6: {
          leds[i_col + 5*i_row] = CRGB::Yellow;
          break;
        }
        case 7: {
          leds[i_col + 5*i_row] = CRGB::White;
          break;
        }
      }

      if (matrix_midisetting_list[i_row][i_col][0] == MOMENTARY && matrix_value_list[i_row][i_col] == HOLD) {   // Om knappen är MOMENTARY och trycks ned
        leds[i_col + 5*i_row] = CRGB::White;
        if (matrix_color_list[i_row][i_col][0] == 7) {
          leds[i_col + 5*i_row] = CRGB::Black;
        }
      }
      else if (matrix_midisetting_list[i_row][i_col][0] == TOGGLE && toggle_activated_list[i_row][i_col][0] == true) {   // Om knappen är TOGGLE och aktiverad
        if (millis() % (2*BLINK_SPEED_MS) <= BLINK_SPEED_MS) {
          leds[i_col + 5*i_row] = CRGB::Black;
        }
        else {
          if (matrix_color_list[i_row][i_col][0] == 0) {
            leds[i_col + 5*i_row] = CRGB::White;
          }
        }
      }
    }
  }

  // raderna 3-6 anpassar sig efter pages
  for (i_row = 2; i_row <= TOT_ROWS-1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {

      switch (matrix_color_list[i_row][i_col][page]) {
        case 0: {
          leds[i_col + 5*i_row] = CRGB::Black;
          break;
        }
        case 1: {
          leds[i_col + 5*i_row] = CRGB::Red;
          break;
        }
        case 2: {
          leds[i_col + 5*i_row] = CRGB::Green;
          break;
        }
        case 3: {
          leds[i_col + 5*i_row] = CRGB::Blue;
          break;
        }
        case 4: {
          leds[i_col + 5*i_row] = CRGB::Cyan;
          break;
        }
        case 5: {
          leds[i_col + 5*i_row] = CRGB::Magenta;
          break;
        }
        case 6: {
          leds[i_col + 5*i_row] = CRGB::Yellow;
          break;
        }
        case 7: {
          leds[i_col + 5*i_row] = CRGB::White;
          break;
        }
      }

      if (matrix_midisetting_list[i_row][i_col][page] == MOMENTARY && matrix_value_list[i_row][i_col] == HOLD) {   // Om knappen är MOMENTARY och trycks ned
      leds[i_col + 5*i_row] = CRGB::White;
        if (matrix_color_list[i_row][i_col][page] == 7) {
          leds[i_col + 5*i_row] = CRGB::Black;
        }
      }
      else if (matrix_midisetting_list[i_row][i_col][page] == TOGGLE && toggle_activated_list[i_row][i_col][page] == true) {   // Om knappen är TOGGLE och aktiverad
        if (millis() % (2*BLINK_SPEED_MS) <= BLINK_SPEED_MS) {
          leds[i_col + 5*i_row] = CRGB::Black;
        }
        else {
          if (matrix_color_list[i_row][i_col][0] == 0) {
            leds[i_col + 5*i_row] = CRGB::White;
          }
        }
      }
    }
  }
}



// ╔═══════════════════════╗
// ║   SEND MIDI (SHOW)    ║
// ╚═══════════════════════╝

void send_midi_show() {

  // Första 2 raderna låsta på CHANNEL_1
  for (i_row = 0; i_row <= 1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      if (matrix_value_list[i_row][i_col] == PRESS) {
        if (matrix_midisetting_list[i_row][i_col][0] == MOMENTARY) {   // Om knappen är FLASH   OFF --> ON
          MIDIAddress addr = {i_row*5 + i_col, Channel(0)};
          Control_Surface.sendNoteOn(addr, velocity);
        }

        else if (matrix_midisetting_list[i_row][i_col][0] == TOGGLE) {    // Om knappen är GO
          if (toggle_activated_list[i_row][i_col][0] == false) {     // OFF --> ON
            MIDIAddress addr = {i_row*5 + i_col, Channel(0)};
            Control_Surface.sendNoteOn(addr, velocity);
            toggle_activated_list[i_row][i_col][0] = true;
          }
          else if (toggle_activated_list[i_row][i_col][0] == true) {    // ON --> OFF
            MIDIAddress addr = {i_row*5 + i_col, Channel(0)};
            Control_Surface.sendNoteOff(addr, velocity);
            toggle_activated_list[i_row][i_col][0] = false;
          }
        }
      }

      else if (matrix_value_list[i_row][i_col] == RELEASE && matrix_midisetting_list[i_row][i_col][0] == MOMENTARY) {    //ON --> OFF
        MIDIAddress addr = {i_row*5 + i_col, Channel(0)};
        Control_Surface.sendNoteOff(addr, velocity);
      }
    }
  }



  // raderna 3-6 anpassar sig efter pages
  for (i_row = 2; i_row <= TOT_ROWS-1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      if (matrix_value_list[i_row][i_col] == PRESS) {
        if (matrix_midisetting_list[i_row][i_col][page] == MOMENTARY) {   // Om knappen är FLASH   OFF --> ON
          MIDIAddress addr = {20*(page % 5) + i_row*5 + i_col, Channel(channel)};
          Control_Surface.sendNoteOn(addr, velocity);
        }

        else if (matrix_midisetting_list[i_row][i_col][page] == TOGGLE) {    // Om knappen är GO
          if (toggle_activated_list[i_row][i_col][page] == false) {     // OFF --> ON
            MIDIAddress addr = {20*(page % 5) + i_row*5 + i_col, Channel(channel)};
            Control_Surface.sendNoteOn(addr, velocity);
            toggle_activated_list[i_row][i_col][page] = true;
          }
          else if (toggle_activated_list[i_row][i_col][page] == true) {    // ON --> OFF
            MIDIAddress addr = {20*(page % 5) + i_row*5 + i_col, Channel(channel)};
            Control_Surface.sendNoteOff(addr, velocity);
            toggle_activated_list[i_row][i_col][page] = false;
          }
        }
      }

      else if (matrix_value_list[i_row][i_col] == RELEASE && matrix_midisetting_list[i_row][i_col][page] == MOMENTARY) {    //ON --> OFF
        MIDIAddress addr = {20*(page % 5) + i_row*5 + i_col, Channel(channel)};
        Control_Surface.sendNoteOff(addr, velocity);
      }
    }
  }
}



// ╔══════════════════════╗
// ║         PROG         ║
// ╚══════════════════════╝

void update_leds_prog() {

  for (i_row = 0; i_row <= 1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
    
      if (matrix_midisetting_list[i_row][i_col][0] == MOMENTARY) {   // Om knappen är MOMENTARY
        leds[i_col + 5*i_row] = C_PROG_MOMENTARY;
        if (matrix_value_list[i_row][i_col] == PRESS) {
          matrix_midisetting_list[i_row][i_col][0] = TOGGLE;
        }
      }

      else {                                                         // Om knappen är TOGGLE
        leds[i_col + 5*i_row] = C_PROG_TOGGLE;
        if (matrix_value_list[i_row][i_col] == PRESS) {
          matrix_midisetting_list[i_row][i_col][0] = MOMENTARY;
          if (toggle_activated_list[i_row][i_col][0] == true) {     // Skickar midi OFF om TOGGLE är aktiverad
            MIDIAddress addr = {i_row*5 + i_col, Channel(0)};
            Control_Surface.sendNoteOff(addr, velocity);
            toggle_activated_list[i_row][i_col][0] = false;
          }
        }
      }

    }
  }

  // raderna 3-6 anpassar sig efter pages
  for (i_row = 2; i_row <= TOT_ROWS-1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
    
      if (matrix_midisetting_list[i_row][i_col][page] == MOMENTARY) {   // Om knappen är MOMENTARY
        leds[i_col + 5*i_row] = C_PROG_MOMENTARY;
        if (matrix_value_list[i_row][i_col] == PRESS) {
          matrix_midisetting_list[i_row][i_col][page] = TOGGLE;
        }
      }

      else {                                                         // Om knappen är TOGGLE
        leds[i_col + 5*i_row] = C_PROG_TOGGLE;
        if (matrix_value_list[i_row][i_col] == PRESS) {
          matrix_midisetting_list[i_row][i_col][page] = MOMENTARY;
          if (toggle_activated_list[i_row][i_col][page] == true) {    // Skickar midi OFF om TOGGLE är aktiverad
            MIDIAddress addr = {i_row*5 + i_col, Channel(page)};
            Control_Surface.sendNoteOff(addr, velocity);
            toggle_activated_list[i_row][i_col][page] = false;
          }
        }
      }

    }
  }

}



// ╔═════════════════════╗
// ║        COLOR        ║
// ╚═════════════════════╝

void update_leds_color() {

  for (i_row = 0; i_row <= 1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      
      if (matrix_value_list[i_row][i_col] == PRESS) {
        matrix_color_list[i_row][i_col][0] = (matrix_color_list[i_row][i_col][0] + 1) % 8;
      }

      switch (matrix_color_list[i_row][i_col][0]) {
        case 0: {
          leds[i_col + 5*i_row] = CRGB::Black;
          break;
        }
        case 1: {
          leds[i_col + 5*i_row] = CRGB::Red;
          break;
        }
        case 2: {
          leds[i_col + 5*i_row] = CRGB::Green;
          break;
        }
        case 3: {
          leds[i_col + 5*i_row] = CRGB::Blue;
          break;
        }
        case 4: {
          leds[i_col + 5*i_row] = CRGB::Cyan;
          break;
        }
        case 5: {
          leds[i_col + 5*i_row] = CRGB::Magenta;
          break;
        }
        case 6: {
          leds[i_col + 5*i_row] = CRGB::Yellow;
          break;
        }
        case 7: {
          leds[i_col + 5*i_row] = CRGB::White;
          break;
        }
      }

    }
  }

  // raderna 3-6 anpassar sig efter pages
  for (i_row = 2; i_row <= TOT_ROWS-1; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      
      if (matrix_value_list[i_row][i_col] == PRESS) {
        matrix_color_list[i_row][i_col][page] = (matrix_color_list[i_row][i_col][page] + 1) % 8;
      }

      switch (matrix_color_list[i_row][i_col][page]) {
        case 0: {
          leds[i_col + 5*i_row] = CRGB::Black;
          break;
        }
        case 1: {
          leds[i_col + 5*i_row] = CRGB::Red;
          break;
        }
        case 2: {
          leds[i_col + 5*i_row] = CRGB::Green;
          break;
        }
        case 3: {
          leds[i_col + 5*i_row] = CRGB::Blue;
          break;
        }
        case 4: {
          leds[i_col + 5*i_row] = CRGB::Cyan;
          break;
        }
        case 5: {
          leds[i_col + 5*i_row] = CRGB::Magenta;
          break;
        }
        case 6: {
          leds[i_col + 5*i_row] = CRGB::Yellow;
          break;
        }
        case 7: {
          leds[i_col + 5*i_row] = CRGB::White;
          break;
        }
      }

    }
  }


  if (millis() % (PULSE_FREQ_MS) <= BLINK_SPEED_MS) {
    FastLED.setBrightness(map(millis() % (PULSE_FREQ_MS), 0, 1*BLINK_SPEED_MS, BRIGHTNESS[brightness_level], 0));
  }
  else if (millis() % (PULSE_FREQ_MS) <= 3*BLINK_SPEED_MS) {
    FastLED.setBrightness(map(millis() % (PULSE_FREQ_MS), BLINK_SPEED_MS, 3*BLINK_SPEED_MS, 0, BRIGHTNESS[brightness_level]));
  }
  else {
    FastLED.setBrightness(BRIGHTNESS[brightness_level]);
  }
}



// ╔═══════════════════════╗
// ║         PIANO         ║
// ╚═══════════════════════╝

void update_leds_piano() {

  for (i_row = 0; i_row <= 5; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      int index = i_col + 5*i_row;
      
      // Vanligt Piano
      if (page_piano == 0) {
        if (index == 0 || index == 1 ||                 index == 3 || index == 4 ||       // Färgar alla sharps vita
          index == 10 || index == 11 || index == 12 ||                index == 14 ||
          index == 20 ||                index == 22 || index == 23) {
          leds[index] = CRGB::White;
        }
        else if (i_row == 1 || i_row == 3 || i_row == 5) {                                // Färgar alla vanliga noter magenta
          leds[index] = CRGB::Magenta;
        }
        else {
          leds[index] = CRGB::Black;                                                      // Färgar "döda" knappar svart
        }
      }

      // Resterande pages
      else {
        if (i_row == 1 || i_row == 3 || i_row == 5) {
          leds[index] = CRGB::Magenta;
        }
        else {
          leds[index] = CRGB(255, 120, 255);
        }
      }

      if (matrix_value_list[i_row][i_col] == HOLD) {     // Om den hittar en knapptryckning -> släck knappen
        leds[index] = CRGB::Black;
      }
    }
  }
}



// ╔═══════════════════════╗
// ║   SEND MIDI (PIANO)   ║
// ╚═══════════════════════╝

void send_midi_piano() {

  // Första 2 raderna låsta på CHANNEL_1
  for (i_row = 0; i_row <= 5; i_row++) {
    for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {
      
      if (matrix_value_list[i_row][i_col] == PRESS && midi_map[page_piano][i_row][i_col] != -1) {
        MIDIAddress addr = {midi_map[page_piano][i_row][i_col], Channel(0)};
        Control_Surface.sendNoteOn(addr, velocity);
        
        Serial.print("Page = ");
        Serial.print(page_piano);
        Serial.print("     MIDI = ");
        Serial.println(midi_map[page][i_row][i_col]);
      }
      else if (matrix_value_list[i_row][i_col] == RELEASE && midi_map[page_piano][i_row][i_col] != -1) {
        MIDIAddress addr = {midi_map[page_piano][i_row][i_col], Channel(0)};
        Control_Surface.sendNoteOff(addr, velocity);
      }
    }
  }
}
