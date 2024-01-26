#pragma once
#include "common.h"
#include "language.h"

class ComboConfig
{
public:
    std::string label;
    std::vector<std::string> list;
    int choice = 0;
    std::string list_str;

    ComboConfig(std::string label_)
    {
        label = GetText(label_);
    }

protected:
    void ListString();
};

class CommunicateMethodComboConfig : public ComboConfig
{
public:
    enum CommunicateMethod_e
    {
        COMMUNICATE_SERIAL = 0,
        COMMUNICATE_TCP_SERVER,
        COMMUNICATE_TCP_CLIENT,
        COMMUNICATE_UDP,
        COMMUNICATE_NUM
    };

    CommunicateMethodComboConfig() : ComboConfig("COM_METHOD")
    {
        const char *method_list[COMMUNICATE_NUM] = {"SERIAL", "TCP_SERVER", "TCP_CLIENT", "UDP"};
        for (int i = 0; i < COMMUNICATE_NUM; i++)
        {
            list.emplace_back(std::string(GetText(method_list[i])));
        }
        ListString();
    }
};

class BaudRateComboConfig : public ComboConfig
{
public:
    enum BaudRate_e
    {
        BAUD_RATE_9600 = 0,
        BAUD_RATE_19200,
        BAUD_RATE_38400,
        BAUD_RATE_57600,
        BAUD_RATE_115200,
        BAUD_RATE_128000,
        BAUD_RATE_256000,
        BAUD_RATE_512000,
        BAUD_RATE_921600,
        BAUD_RATE_1500000,
        BAUD_RATE_NUM
    };

    BaudRateComboConfig() : ComboConfig("BAUD_RATE")
    {
        const char *baud_rate_list[BAUD_RATE_NUM] = {"9600", "19200", "38400", "57600", "115200", "128000", "256000", "512000", "921600", "1500000"};
        for (int i = 0; i < BAUD_RATE_NUM; i++)
        {
            list.emplace_back(std::string(baud_rate_list[i]));
        }
        ListString();
    }
};

class DataBitsComboConfig : public ComboConfig
{
public:
    enum DataBits_e
    {
        DATA_BITS_5 = 0,
        DATA_BITS_6,
        DATA_BITS_7,
        DATA_BITS_8,
        DATA_BITS_NUM
    };

    DataBitsComboConfig() : ComboConfig("DATA_BITS")
    {
        const char *data_bits_list[DATA_BITS_NUM] = {"5", "6", "7", "8"};
        for (int i = 0; i < DATA_BITS_NUM; i++)
        {
            list.emplace_back(std::string(data_bits_list[i]));
        }
        ListString();
    }
};

class ParityComboConfig : public ComboConfig
{
public:
    enum Parity_e
    {
        PARITY_NONE = 0,
        PARITY_ODD,
        PARITY_EVEN,
        PARITY_NUM
    };

    ParityComboConfig() : ComboConfig("PARITY")
    {
        const char *parity_list[PARITY_NUM] = {"None", "Odd", "Even"};
        for (int i = 0; i < PARITY_NUM; i++)
        {
            list.emplace_back(std::string(parity_list[i]));
        }
        ListString();
    }
};

class StopBitsComboConfig : public ComboConfig
{
public:
    enum StopBits_e
    {
        STOP_BITS_1 = 0,
        STOP_BITS_1_5,
        STOP_BITS_2,
        STOP_BITS_NUM
    };

    StopBitsComboConfig() : ComboConfig("STOP_BITS")
    {
        const char *stop_bits_list[STOP_BITS_NUM] = {"1", "1.5", "2"};
        for (int i = 0; i < STOP_BITS_NUM; i++)
        {
            list.emplace_back(std::string(stop_bits_list[i]));
        }
        ListString();
    }
};

void ShowUartConfig();
void ShowTextWindow();
void ShowPlottingWindow();
void ShowProfessionalWindow();
void ShowHelpWindow();