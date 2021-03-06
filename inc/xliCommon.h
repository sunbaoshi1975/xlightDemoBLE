//  xlCommon.h - Xlight common definitions header

#ifndef xliCommon_h
#define xliCommon_h

#include "application.h"
#include "xliConfig.h"

#define BITTEST(var,pos)          (((var)>>(pos)) & 0x0001)
#define BITMASK(pos)              (0x0001 << (pos))
#define BITSET(var,pos)           ((var) | BITMASK(pos))
#define BITUNSET(var,pos)         ((var) & (~BITMASK(pos)))
#define _BV(x)                    (1<<(x))

//Here, common data types have been given alternative names through #define statements

// Common Data Type
#define BOOL                      boolean
#define UC                        uint8_t
#define US                        uint16_t
#define UL                        uint32_t
#define S_BYTE                    int8_t
#define SHORT                     int16_t
#define LONG                      int32_t

// Device Status
#define STATUS_OFF                0x00        // Power Off
#define STATUS_INIT               0x01        // Initializing
#define STATUS_BMW                0x02        // Basic Mode of Working
#define STATUS_DIS                0x03        // WAN Disconnected
#define STATUS_NWS                0x04        // Normal Working Status
#define STATUS_SLP                0x05        // Sleep
#define STATUS_RST                0x06        // Reseting
#define STATUS_ERR                0x07        // Fatal Error

// Serial Port Speed
#define SERIALPORT_SPEED_LOW      9600
#define SERIALPORT_SPEED_14400    14400
#define SERIALPORT_SPEED_MID      19200
#define SERIALPORT_SPEED_HC05     38400
#define SERIALPORT_SPEED_57600    57600
#define SERIALPORT_SPEED_HIGH     115200
#define SERIALPORT_SPEED_DEFAULT  SERIALPORT_SPEED_HIGH

// Row State Flags for Sync between Cloud, Flash, and Working Memory
enum OP_FLAG {GET, POST, PUT, DELETE};
enum FLASH_FLAG {UNSAVED, SAVED};
enum RUN_FLAG {UNEXECUTED, EXECUTED};

//enum values for CldJSONCommand()
enum COMMAND {CMD_SERIAL, CMD_POWER, CMD_COLOR, CMD_BRIGHTNESS, CMD_SCENARIO};

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On
#define DEVICE_SW_TOGGLE            2       // Toggle

// Update operator for set brightness & CCT command
#define OPERATOR_SET                0
#define OPERATOR_ADD                1
#define OPERATOR_SUB                2
#define OPERATOR_MUL                3
#define OPERATOR_DIV                4

// Macros for UID identifiers
#define CLS_RULE                  'r'
#define CLS_SCHEDULE              'a'
#define CLS_SCENARIO              's'
#define CLS_NOTIFICATION          'n'
#define CLS_SENSOR                'e'
#define CLS_LIGHT_STATUS          'h'
#define CLS_TOPOLOGY              't'
#define CLS_CONFIGURATION         'c'

// Device (lamp) type
#define MAX_RING_NUM                3
typedef enum
{
  devtypUnknown = 0,
  devtypCRing3,     // Color ring - Rainbow
  devtypCRing2,
  devtypCRing1,
  devtypWRing3,     // White ring - Sunny
  devtypWRing2,
  devtypWRing1,
  devtypMRing3 = 8, // Color & Motion ring - Mirage
  devtypMRing2,
  devtypMRing1,
  devtypDummy = 255
} devicetype_t;

#define RING_ID_ALL                 0
#define RING_ID_1                   1
#define RING_ID_2                   2
#define RING_ID_3                   3

// Specify system serial port, could be Serial, Serial1 or Seria2
#ifndef TheSerial
#define TheSerial       Serial
#endif

#ifndef SERIAL
#define SERIAL          TheSerial.printf
#endif

#ifndef SERIAL_LN
#define SERIAL_LN       TheSerial.printlnf
#endif

#ifndef BLEPort
#define BLEPort         Serial1             // Could be serial1 or serial2
#endif

//--------------------------------------------------
// Tools & Helpers
//--------------------------------------------------
uint8_t h2i(const char c);
char* PrintUint64(char *buf, uint64_t value, bool bHex = true);
char* PrintMacAddress(char *buf, const uint8_t *mac, char delim = ':');
uint64_t StringToUInt64(const char *strData);
inline time_t tmConvert_t(US YYYY, UC MM, UC DD, UC hh, UC mm, UC ss)  // inlined for speed
{
  struct tm t;
  t.tm_year = YYYY-1900;
  t.tm_mon = MM - 1;
  t.tm_mday = DD;
  t.tm_hour = hh;
  t.tm_min = mm;
  t.tm_sec = ss;
  t.tm_isdst = 0;  // not used
  time_t t_of_day = mktime(&t);
  return t_of_day;
};

#endif /* xliCommon_h */
