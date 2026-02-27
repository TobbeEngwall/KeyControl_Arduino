// ╔══════════════════════╗
// ║     SHIFT BUTTON     ║
// ╚══════════════════════╝

void read_shift() {

  if (1-digitalRead(SHIFT) == HIGH) {     // Om Shift trycks ned
    shift_state = HOLD;
  }
  else {
    shift_state = OFF;
  }
}




// ╔══════════════════════╗
// ║    POTENTIOMETERS    ║
// ╚══════════════════════╝

void read_pots() {

  Control_Surface.loop();     // Använder CCPotentiometer för auto-midi. De ska ändå inte byta page
  
}



// ╔═══════════════════════╗
// ║     BUTTON MATRIX     ║
// ╚═══════════════════════╝

void read_matrix() {

  for (i_col = 0; i_col <= TOT_COLUMNS-1; i_col++) {      // Sätter en kolumn till 3.3V i taget
  digitalWrite(MATRIX_COLUMN[i_col], HIGH);
    for (i_row = 0; i_row <= TOT_ROWS-1; i_row++) {       // Läser en rad i taget
      if (digitalRead(MATRIX_ROW[i_row]) == HIGH) {       // Om vi hittar 3.3V...

        if (matrix_value_list[i_row][i_col] == OFF) {             // OFF --> PRESS
          matrix_value_list[i_row][i_col] = PRESS;
        }
        else if (matrix_value_list[i_row][i_col] == PRESS) {      // PRESS --> HOLD
          matrix_value_list[i_row][i_col] = HOLD;
        }
      }
      
      else if(digitalRead(MATRIX_ROW[i_row]) == LOW) {    // Om vi hittar 0V...

        if (matrix_value_list[i_row][i_col] == PRESS) {           // PRESS --> RELEASE
          matrix_value_list[i_row][i_col] = RELEASE;
        }
        else if (matrix_value_list[i_row][i_col] == HOLD) {       // HOLD --> RELEASE
          matrix_value_list[i_row][i_col] = RELEASE;
        }
        else {
          matrix_value_list[i_row][i_col] = OFF;                  // RELEASE --> OFF
        }
      }
    }
  digitalWrite(MATRIX_COLUMN[i_col], LOW);                // Stänger av 3.3V på kolumnen
  }
}



