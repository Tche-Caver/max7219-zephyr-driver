
#define DT_DRV_COMPAT adi_max7219

#include <errno.h>
#include <zephyr/logging/log.h>

#include "max7219.h"

//Enable logging at CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(max7219);


//Function declarations------------------------------------------------------

static int max7219_reg_write(const struct device *dev, uint8_t reg, uint8_t data);
static int max7219_init (const struct device *dev, uint32_t intensity);
static int led_matrix_test (const struct device *dev, uint8_t mode);
static int led_matrix_write (const struct device *dev, uint8_t *data);


//Private functions (init function)------------------------------------------

static int max7219_init(const struct device *dev, uint32_t intensity){

const struct max7219_config * cfg = (const struct max7219_config *)dev->config;
const struct spi_dt_spec * led_drv = &cfg->bus;

if (!spi_is_ready_dt(led_drv)){
    LOG_ERR("Error initializing MAX7219\n");
    return -1;
}

uint8_t bright = (uint8_t)((cfg->intensity) & 0x000F);

max7219_reg_write(dev, REG_DECODE, VAL_DECODE_NONE);        //Decode register (0xX9) -> 0x00 (no decode: each bit corresponds to an LED/segment)
max7219_reg_write(dev, REG_SHUTDOWN, VAL_SHUTDOWN_ON);      //Shutdown register (0xXC) -> 0xX1 (enable)
max7219_reg_write(dev, REG_TEST, VAL_TEST_DISABLE);          //Test mode enabled, all LEDs on (0xXF) -> 0xX1 
max7219_reg_write(dev, REG_INTENSITY, bright);   //Intensity register, duty cycle (0xXA) -> 0xXF (31/32 duty cycle, full intensity)
max7219_reg_write(dev, REG_SCAN, VAL_SCAN_ALL_ON);           //Scan register (0xXB) -> 0xX7 (All digits on)

//Set all output values to 0 (no LEDs on)
for(int i = 0; i < 8; i++){
    max7219_reg_write(dev, (REG_LEDROW_BASE + i), 0x00);  
    }


printk("\nMAX7219 initialization complete.\n");

}


static int max7219_reg_write(const struct device *dev, uint8_t reg, uint8_t data){
    
    int ret = 0;

    const struct max7219_config * cfg = (const struct max7219_config *)dev->config;
    const struct spi_dt_spec * led_drv = &cfg->bus;

    //Combine register and data into SPI packet for device
    uint8_t packet[2] = {(reg & 0x0F), data};

    //uint8_t packet[2] = {0xAA, 0xAA};

    //Zephyr SPI API structures
    const struct spi_buf tx_buf = {.buf = packet, .len = sizeof(packet)};
    const struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1};
    
    //Perform SPI transaction, return value is ret
    ret= spi_write_dt(led_drv, &tx);

    //Error handling
    if (ret){
        LOG_DBG("SPI transaction failed with code %d\n", ret);
    };

    return ret;

}


//Public functions (LED Matrix API)--------------------------------------------

static int led_matrix_test (const struct device *dev, uint8_t mode){

    //const struct max7219_config * cfg = (const struct max7219_config *)dev->config;

    if(mode){
       max7219_reg_write(dev, REG_TEST, VAL_TEST_ENABLE);  //Test mode enabled, all LEDs on (0xXF) -> 0xX1  
        printk("\nMAX7219 test mode enabled (all LEDs on).\n");
    }
    else{
        max7219_reg_write(dev, REG_TEST, VAL_TEST_DISABLE);  //Test mode enabled, all LEDs off (0xXF) -> 0xX0
        printk("\nMAX7219 test mode disabled (all LEDs off).\n");
    }
}



static int led_matrix_write (const struct device *dev, uint8_t *data){

    //const struct max7219_config * cfg = (const struct max7219_config *)dev->config;
    
    for(int i = 0; i < 8; i++){
    max7219_reg_write(dev, (REG_LEDROW_BASE + i), *(data + i));  
    }

    printk("\nFinished LED matrix write.\n");

    return 0;
}



//Devicetree ------------------------------------------------------------------------


//Define public functions of LED Matrix API

//Assigning private led_matrix_test and led_matrix_write functions to public API (test, write)
static const struct led_matrix_api led_matrix_api_functions = {
    .test = led_matrix_test,
    .write = led_matrix_write,
};

//MAX7219 instances

    //Config struct instances               
    //Zephyr will populate instance value, create config struct variable for each instance initialized in Devicetree

#define MAX7219_DEFINE(inst)                \
                                            \
    static const struct max7219_config max7219_config_##inst = {    \
        .bus = SPI_DT_SPEC_INST_GET(inst,   \
                    SPI_WORD_SET(8) |       \
                    SPI_TRANSFER_MSB,       \
                    0),                     \
        .intensity = DT_INST_PROP(inst, intensity)                   \
    };                            \
                                                                    \
                                                                    \
                                                                    \
/*Device instance created from devicetree node, register init function*/ \
DEVICE_DT_INST_DEFINE(inst,                         \
                      &max7219_init,                \
                      NULL,                         \
                      NULL,                         \
                      &max7219_config_##inst,       \
                      POST_KERNEL,                  \
                      CONFIG_GPIO_INIT_PRIORITY,     \
                      &led_matrix_api_functions);   
                                                                    
DT_INST_FOREACH_STATUS_OKAY(MAX7219_DEFINE)



