#ifndef SMART_HUB_REG_CMD_H
#define SMART_HUB_REG_CMD_H

#define USB7252C_MSB 0xff00  //  MSB Helper
#define USB7252C_LSB 0x00ff  //  LSB Helper

#define USB7252C_I2C_ADDR 0x2D         //  I2C Address for USB7252C hub
#define USB7252C_I2C_SECOND_ADDR 0x2C  //  I2C Address for USB7252C hub
#define USB7252C_I2C_DISABLE_ADDR 0x0
#define USB7252C_MAX_DATALEN 128  //  Maximum length for a data chunk

#define VENDOR_ID_MICROCHIP 0x0424  //  default ID after Reset

#define USB7252C_RAM_START 0x00     //  RAM Address start
#define USB7252C_RAM_ADDR_OFF 0x06  //  RAM Read memory offset

#define USB7252C_READ_CONF_REG_LEN 8             //  Length of read command
#define USB7252C_WRITE_CONF_REG_LEN 9            //  Length of write command
#define USB7252C_WRITE_CONF_REG_CMD_BLOCK_LEN 6  //  Length of write command

#define USB7252C_PORT1 (1 << 1)  //  Offset for USB port 1
#define USB7252C_PORT2 (1 << 2)  //  Offset for USB port 2
#define USB7252C_PORT3 (1 << 3)  //  Offset for USB port 3
#define USB7252C_PORT4 (1 << 4)  //  Offset for USB port 4
#define USB7252C_PORT5 (1 << 5)  //  Offset for USB port 5
#define USB7252C_PORT6 (1 << 6)  //  Offset for USB port 6
#define USB7252C_PORT7 (1 << 7)  //  Offset for USB port 7
// ------------------------------------------------------------------------------
// CONFIGURATION REGISTERS MEMORY MAP
//  Table1 from datasheet
#define USB7252C_BASEADDR0 0xBF800000

#define USB7252C_DEV_REV 0x0000 + USB7252C_BASEADDR0
#define USB7252C_USB2_SYS_CONFIG 0x0808 + USB7252C_BASEADDR0
#define USB7252C_USB3_SYS_CONFIG 0x0828 + USB7252C_BASEADDR0
#define USB7252C_VENDOR_ID 0x3000 + USB7252C_BASEADDR0
#define USB7252C_PROD_ID 0x3002 + USB7252C_BASEADDR0
#define USB7252C_DEV_ID 0x3004 + USB7252C_BASEADDR0
#define USB7252C_HUB_CFG 0x3006 + USB7252C_BASEADDR0

#define USB7252C_USB2_LINK_STATE0_3 0x3100 + USB7252C_BASEADDR0
#define USB7252C_USB2_LINK_STATE4_7 0x3101 + USB7252C_BASEADDR0
#define USB7252C_USB2_VER_BCD 0x3108 + USB7252C_BASEADDR0

// i2c address default value 0x2D
#define USB7252C_SMBUS_PRIMAIRY_ADR 0xC804 + USB7252C_BASEADDR0
// i2c address default value 0x2C, disable with val 0x00
#define USB7252C_SMBUS_SECOND_ADR 0xC805 + USB7252C_BASEADDR0
// PORT DISABLE – SELF POWERED
#define USB7252C_PORT_DIS_SELF 0x300a
// PORT DISABLE – BUS POWERED
#define USB7252C_PORT_DIS_BUS 0x300b
// ------------------------------------------------------------------------------
// Table2 from datasheet
#define USB7252C_BASEADDR1 0xBFD20000
#define USB7252C_REG_I2S_FEAT_SEL 0x3412 + USB7252C_BASEADDR1
#define USB7252C_FLEX_FEATURE_REG 0x3440 + USB7252C_BASEADDR1

// ------------------------------------------------------------------------------
// Special Commands

// Upstream port 0
#define USB7252C_FLEXPORT0 0x0
// Upstream port 1
#define USB7252C_FLEXPORT1 0x1

// Data offset from a read command
#define USB7252C_READ_DATA_LEN_OFFSET 0
// Data offset from a read command
#define USB7252C_READ_DATA_OFFSET 1

#endif