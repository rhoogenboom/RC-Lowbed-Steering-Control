// EEPROM defines
#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }

// Useful functions
#define MIN(x,y) ( x > y ? y : x )
#define MAX(x,y) ( x > y ? x : y )

// Debugging
#define BAUDRATE 9600
#define DEBUG_INPUT false         
#define DEBUG_OUTPUT false         

// TIMER
#define TRANSMISSION_INTERVAL 100  

//receiver items and model inputs
#define POTMETER_PIN A7 // 5th wheel Mega: A1/pin96 - Nano: A7/

