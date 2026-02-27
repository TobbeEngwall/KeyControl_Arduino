// 🧱 Struktur som håller både MIDI-inställningar och färgdata
// Används för att spara/ladda allt i ett svep till/från minnet
struct SavedData {
  MIDISetting matrix_midisetting_list[TOT_ROWS][TOT_COLUMNS][20];  // 🎚️ MIDI-typ per knapp och page
  int matrix_color_list[TOT_ROWS][TOT_COLUMNS][20];                // 🎨 Färgindex per knapp och page
};



// 💾 Sparar globala matriser till NVS (non-volatile storage)
void saveMatrixData() {
  SavedData data_to_save;  // 📦 Tillfällig behållare för datan som ska sparas

  // 🔄 Kopiera MIDI-inställningar till strukturen
  memcpy(&data_to_save.matrix_midisetting_list, matrix_midisetting_list, sizeof(matrix_midisetting_list));

  // 🔄 Kopiera färginställningar till strukturen
  memcpy(&data_to_save.matrix_color_list, matrix_color_list, sizeof(matrix_color_list));

  // 📂 Öppna NVS-minnet i skrivläge under namespace "storage"
  prefs.begin("storage", false);

  // 🧬 Skriv hela strukturen som bytearray till nyckeln "matrixdata"
  prefs.putBytes("matrixdata", &data_to_save, sizeof(data_to_save));

  // 🧹 Stäng minnesåtkomst – bra vana för att spara resurser
  prefs.end();
  
  Serial.println("✅ Data saved");
}



// 🔁 Läser in sparade matriser från NVS (non-volatile storage)
void loadMatrixData() {

  // 📂 Öppna NVS-minnet i läsläge under namespace "storage"
  prefs.begin("storage", true);  // true = läs-läge

  // 📏 Förväntad storlek på den sparade strukturen
  size_t expected_size = sizeof(SavedData);

  // 📦 Faktisk storlek på datan som finns sparad under nyckeln "matrixdata"
  size_t actual_size = prefs.getBytesLength("matrixdata");

  // ✅ Om storlekerna matchar – läs in data och kopiera till globala matriser
  if (actual_size == expected_size) {
    SavedData loaded_data;  // 📥 Temporär behållare för inläst data

    // 🧬 Läs hela strukturen från NVS till RAM
    prefs.getBytes("matrixdata", &loaded_data, expected_size);

    // ⬅️ Kopiera MIDI-inställningar från strukturen till globala listan
    memcpy(matrix_midisetting_list, &loaded_data.matrix_midisetting_list, sizeof(matrix_midisetting_list));

    // ⬅️ Kopiera färginställningar från strukturen till globala listan
    memcpy(matrix_color_list, &loaded_data.matrix_color_list, sizeof(matrix_color_list));
  }

  // 🧹 Stäng minnesåtkomst – bra vana för att frigöra resurser
  prefs.end();
}
