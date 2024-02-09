#include "serial_config.h"
#include "serial/serial.h"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::vector;

static serial::Serial *my_serial = nullptr;

void SerialEnumeratePorts(vector<string> &ret)
{
    vector<serial::PortInfo> devices_found = serial::list_ports();
    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    SDL_Log("SerialEnumeratePorts");

    ret.clear();
    while (iter != devices_found.end())
    {
        serial::PortInfo device = *iter++;
        ret.emplace_back(device.port); // + " | " + device.description
        SDL_Log("(%s, %s, %s)", device.port.c_str(), device.description.c_str(),
                device.hardware_id.c_str());
    }
}

bool SerialStart(string port, unsigned long baud,
                 serial::bytesize_t byte_size,
                 serial::parity_t parity,
                 serial::stopbits_t stopbits)
{
    SDL_Log("SerialStart, port:%s, baud:%lu", port.c_str(), baud);
    try
    {
        my_serial = new serial::Serial(port, baud, serial::Timeout::simpleTimeout(1000),
                                       byte_size, parity, stopbits);
        my_serial->open();
    }
    catch (const std::exception &e)
    {
        SDL_Log("%s", e.what());
        my_serial = nullptr;
        return false;
    }

    return true;
}

bool SerialStop()
{
    SDL_Log("SerialStop");
    if (my_serial != nullptr)
    {
        try
        {
            my_serial->close();
            delete my_serial;
            my_serial = nullptr;
        }
        catch (const std::exception &e)
        {
            SDL_Log("%s", e.what());
            return false;
        }
    }
    return true;
}

bool SerialIsOpen()
{
    if (my_serial != nullptr)
    {
        return my_serial->isOpen();
    }
    return false;
}
