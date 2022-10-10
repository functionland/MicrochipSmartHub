#ifndef SMART_HUB_REG_CMD_H
#define SMART_HUB_REG_CMD_H

#define MSB 0xff00  //  MSB Helper
#define LSB 0x00ff  //  LSB Helper

#define I2C_ADDR 0x2D         //  I2C Address for USB7252C hub
#define I2C_SECOND_ADDR 0x2C  //  I2C Address for USB7252C hub
#define I2C_DISABLE_ADDR 0x0
#define MAX_DATALEN 128  //  Maximum length for a data chunk

#define VENDOR_ID_MICROCHIP 0x0424  //  default ID after Reset

#define RAM_START 0x00     //  RAM Address start
#define RAM_ADDR_OFF 0x06  //  RAM Read memory offset

#define READ_CONF_REG_LEN 8             //  Length of read command
#define WRITE_CONF_REG_LEN 9            //  Length of write command
#define WRITE_CONF_REG_CMD_BLOCK_LEN 6  //  Length of write command

#define PORT1 (1 << 1)  //  Offset for USB port 1
#define PORT2 (1 << 2)  //  Offset for USB port 2
#define PORT3 (1 << 3)  //  Offset for USB port 3
#define PORT4 (1 << 4)  //  Offset for USB port 4
#define PORT5 (1 << 5)  //  Offset for USB port 5
#define PORT6 (1 << 6)  //  Offset for USB port 6
#define PORT7 (1 << 7)  //  Offset for USB port 7
// ------------------------------------------------------------------------------
// CONFIGURATION REGISTERS MEMORY MAP
//  Table1 from datasheet
#define BASEADDR0 0xBF800000

#define DEV_REV 0x0000 + BASEADDR0
#define USB2_SYS_CONFIG 0x0808 + BASEADDR0
#define USB3_SYS_CONFIG 0x0828 + BASEADDR0
#define VENDOR_ID 0x3000 + BASEADDR0
#define PROD_ID 0x3002 + BASEADDR0
#define DEV_ID 0x3004 + BASEADDR0
#define HUB_CFG 0x3006 + BASEADDR0

#define USB2_LINK_STATE0_3 0x3100 + BASEADDR0
#define USB2_LINK_STATE4_7 0x3101 + BASEADDR0
#define USB2_VER_BCD 0x3108 + BASEADDR0

// i2c address default value 0x2D
#define SMBUS_PRIMAIRY_ADR 0xC804 + BASEADDR0
// i2c address default value 0x2C, disable with val 0x00
#define SMBUS_SECOND_ADR 0xC805 + BASEADDR0
// PORT DISABLE – SELF POWERED
#define PORT_DIS_SELF 0x300a
// PORT DISABLE – BUS POWERED
#define PORT_DIS_BUS 0x300b
// ------------------------------------------------------------------------------
// Table2 from datasheet
#define BASEADDR1 0xBFD20000
#define REG_I2S_FEAT_SEL 0x3412 + BASEADDR1
#define FLEX_FEATURE_REG 0x3440 + BASEADDR1

// ------------------------------------------------------------------------------
// Special Commands

// Upstream port 0
#define FLEXPORT0 0x0
// Upstream port 1
#define FLEXPORT1 0x1

// Data offset from a read command
#define READ_DATA_LEN_OFFSET 0
// Data offset from a read command
#define READ_DATA_OFFSET 1

#endif