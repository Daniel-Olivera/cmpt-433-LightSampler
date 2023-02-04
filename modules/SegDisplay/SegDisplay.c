#include "../tools/tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define SEG_LEFT_VAL "/sys/class/gpio/gpio61/value"
#define SEG_RIGHT_VAL "/sys/class/gpio/gpio44/value"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

typedef struct SegDisplayValue{
    unsigned char top_half_val;
    unsigned char bot_half_val;
} SegDisplayValue;

//The index in the array will correspond to number to display
SegDisplayValue displayVals[] = 
{
    [0].top_half_val = 0x86, [0].bot_half_val = 0xa1, // values for 0
    [1].top_half_val = 0x12, [1].bot_half_val = 0x80, //            1
    [2].top_half_val = 0x0e, [2].bot_half_val = 0x31, //            2
    [3].top_half_val = 0x06, [3].bot_half_val = 0xb0, //            3
    [4].top_half_val = 0x8a, [4].bot_half_val = 0x90, //            4
    [5].top_half_val = 0x8c, [5].bot_half_val = 0xb0, //            5
    [6].top_half_val = 0x8c, [6].bot_half_val = 0xb1, //            6
    [7].top_half_val = 0x14, [7].bot_half_val = 0x04, //            7
    [8].top_half_val = 0x8e, [8].bot_half_val = 0xb1, //            8
    [9].top_half_val = 0x8e, [9].bot_half_val = 0x90, //            9
};

enum ON_OR_OFF {OFF, ON};

static int openI2CBus(char* bus, int address);
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
// static FILE* openValueFile(char* fileName);
// static void setDisplaySideOnOrOff(char* displaySide, char mode[]);


void init()
{
    // runCommand("config-pin P9_18 i2c");
    // runCommand("config-pin P9_17 i2c");

    int i2cFileDesc = openI2CBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

    close(i2cFileDesc);

}

static int openI2CBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if(result < 0){
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }

    return i2cFileDesc;

}

void setDisplayValue(int value)
{
    int i2cFileDesc = openI2CBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    writeI2cReg(i2cFileDesc, REG_OUTB, displayVals[value].top_half_val);
    writeI2cReg(i2cFileDesc, REG_OUTA, displayVals[value].bot_half_val);

    close(i2cFileDesc);
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int result = write(i2cFileDesc, buff, 2);
    if(result != 2){
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

// static FILE* openValueFile(char* fileName)
// {
//     FILE* valueFile = fopen(fileName, "w");

//     if(valueFile == NULL){
//         perror("I2C: Unable to open value file.");
//         exit(1);
//     }

//     return valueFile;
// }

// static void setDisplaySideOnOrOff(char* displaySide, char mode[])
// {
//     FILE* valueFile = openValueFile(displaySide);

//     int charWritten = fprintf(valueFile, mode);
//     if(charWritten <= 0){
//         perror("I2C: Error writing data to.");
//         exit(1);
//     }

//     fclose(valueFile);
// }