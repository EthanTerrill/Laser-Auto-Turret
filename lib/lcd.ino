////////////////////////////////////////////////////////////////////////////
//  FileName:     LCD.ino
//  Author:       Ethan Terrill
//  Date:         27.07.2025
//
//  Description:  implementation of functions declared in LCD.h
//////////////////////////////////////////////////////////////////////



// ============================== Function declarations ============================== //
inline void write_char(char c) {

  // send first half of char
  // 0xD for writing as data with backlight on
  Wire.write(0xD | ((c << 0) & 0xF0));
  Wire.write(0x09);

  // send second half
  Wire.write(0xD | ((c << 4) & 0xF0));
  Wire.write(0x09);

  // add small delay for data to 
  // finish processing before it gets sent again
  delay(2);
}

inline void write_string(const char* c) {
  for(int i =0; i < strlen(c); i++) {
    write_char(c[i]);
  }
}




inline void send_cmd(uint8_t cmd) {
  // send first half of cmd
  // 0xC for writing as commandd with backlight on
  Wire.write(0xC | ((cmd << 0) & 0xF0));
  Wire.write(0x08);

  // send second half
  Wire.write(0xC | ((cmd << 4) & 0xF0));
  Wire.write(0x08);

  // small delay for 
  delay(2);
}





inline void clear_lcd(uint8_t lcd) {
  Wire.beginTransmission(lcd);
  send_cmd(LCD_CLEAR);
  Wire.endTransmission(lcd);

}
