////////////////////////////////////////////////////////////////////////////
//  FileName:     lcd.h
//  Author:       Ethan Terrill
//  Date:         25.07.2025
//
//  Description:  Header file used for interfacing with the 
//                ESP32-s3 lcd
//////////////////////////////////////////////////////////////////////





// ============================== Macros ============================== //
#define LCD_CLEAR 0x01
#define LCD_RESET_CURSOR 0x80



// ============================ ProtoTypes ============================ //




/**
* @brief sends a command to the LCD display.
*  
* @param cmd the binary value of the command
*          to be sent.
*/
inline void send_cmd(uint8_t cmd);


/**
* @brief writes a character to the LCD  
* @param c 
*   the character to be written to the LCD
*/

inline void write_char(char c);

/**
*  @brief  writes a string to the output of an LCD
*  @param  c The string intened to be written as 
*          ouptut to the lcd
*/
inline void write_char_p(const char* c);



/**
*  @brief  writes a string to the output of an LCD
*  @param  S The string intened to be written as 
*          ouptut to the lcd
*/
inline void write_string(String s);


/**
*  @brief  writes a string to the output of an LCD
*  @param  c The string intened to be written as 
*/

inline void clear_lcd(uint8_t lcd);
