#include "smart_hub_manager.h"

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log/logger.h"

#define CMD_MEMORY_READ 0x04
#define CMD_MEMORY_WRITE 0x03
#define REGISTER_READ 1
#define REGISTER_WRITE 2

#define CTRL_TIMEOUT (5 * 1000) /* milliseconds */

static constexpr auto TAG{"SmartHubManager"};

SmartHubManager::SmartHubManager()
{
}

bool SmartHubManager::Initilize()
{
    int error = libusb_init(&ctx_);
    if (error)
    {
        LOG::Fatal(TAG, "Initilize failed {}{%s}", error, libusb_error_name(error));
        return false;
    }

    LOG::Message(TAG, "Open Usb with vid {} and pid {}", vid_, pid_);

    handle_ = libusb_open_device_with_vid_pid(ctx_, vid_, pid_);
    if (handle_ == NULL)
    {
        LOG::Fatal(TAG, "Open device failed");
        libusb_exit(ctx_);
        return false;
    }
    LOG::Message(TAG, "Open device succeeded");
    return true;
}

bool SmartHubManager::RegisterRead(uint32_t address, uint16_t length, std::vector<uint8_t> &buffer)
{
    if (handle_ == NULL)
    {
        LOG::Warn(TAG, "Handle is NULL");
        return false;
    }

    uint8_t buf[1024] = "\0";
    if (length > 1024)
    {
        LOG::Warn(TAG, "Read length is too large {}", length);
        return false;
    }
    last_error_code = libusb_control_transfer(handle_,
                                              LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR |
                                                  LIBUSB_RECIPIENT_INTERFACE,
                                              CMD_MEMORY_READ,
                                              (address & 0xFFFF),
                                              ((address & 0xFFFF0000) >> 16),
                                              buf,
                                              length,
                                              CTRL_TIMEOUT);
    if (last_error_code < 0)
    {
        LOG::Warn(TAG, "Can not read register address {}", address);
        return false;
    }

    for (uint16_t i = 1; i <= length; i++)
    {
        buffer.push_back(buf[i]);
    }
    return true;
}
bool SmartHubManager::RegisterWrite(uint32_t address, uint16_t length, const std::vector<uint8_t> &buffer)
{
    if (handle_ == NULL)
    {
        LOG::Warn(TAG, "Handle is NULL");
        return false;
    }
    uint8_t buf[1024] = "\0";
    for (int i = 0; i < buffer.size(); i++)
        buf[i] = buffer[i];

    last_error_code = libusb_control_transfer(handle_,
                                              LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR |
                                                  LIBUSB_RECIPIENT_INTERFACE,
                                              CMD_MEMORY_WRITE,
                                              (address & 0xFFFF),
                                              ((address & 0xFFFF0000) >> 16),
                                              buf,
                                              length,
                                              CTRL_TIMEOUT);
    if (last_error_code < 0)
    {
        LOG::Warn(TAG, "Can not write register address {}", address);
        return false;
    }
    return true;
}

void SmartHubManager::setVidPid(uint16_t vid, uint16_t pid)
{
    vid_ = vid;
    pid_ = pid;
}

SmartHubManager::~SmartHubManager()
{
    if (handle_ != NULL)
    {
        libusb_close(handle_);
        handle_ = NULL;
    }

    libusb_exit(ctx_);
}