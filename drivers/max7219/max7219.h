#include <zephyr/drivers/spi.h>
#include <zephyr/device.h>


//Device registers
#define REG_SHUTDOWN 0x0C
#define REG_DECODE 0x09
#define REG_INTENSITY 0x0A
#define REG_SCAN 0x0B
#define REG_TEST 0x0F
#define REG_LEDROW_BASE 0x01

//Initialization values
#define VAL_SHUTDOWN_OFF 0x01
#define VAL_SHUTDOWN_ON 0x01
#define VAL_DECODE_NONE 0x00
#define VAL_INTENSITY_MAX 0x0F
#define VAL_SCAN_ALL_ON 0x07
#define VAL_TEST_ENABLE  0x01
#define VAL_TEST_DISABLE 0x00


//Custom LED matrix driver API for MAX7219 with 8x8 LED matrix
struct led_matrix_api{
    int (*test)(const struct device *dev, uint8_t mode);     //Test mode that will turn all LEDs on/off depending on mode variable
    int (*write)(const struct device *dev, uint8_t *data);    //8-element array of unsigned 8-bit int passed into data, bits of each element control each row of LEDs
    
};

struct max7219_config {
    struct spi_dt_spec bus;
    uint32_t intensity;
};
