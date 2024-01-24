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
	void ListString()
	{
		for (std::string i : list)
		{
			list_str.append(i);
			list_str.push_back('\0');
		}
		list_str.push_back('\0');
	}
};

class CommunicateMethodComboConfig : public ComboConfig
{
public:
    enum CommunicateMethod
    {
        COMMUNICATE_SERIAL = 0,
        COMMUNICATE_TCP_SERVER,
        COMMUNICATE_TCP_CLIENT,
        COMMUNICATE_UDP,
        COMMUNICATE_NUM
    };

    CommunicateMethodComboConfig() : ComboConfig("COM_METHOD")
    {
        const char* method_list[COMMUNICATE_NUM] = { "SERIAL", "TCP_SERVER", "TCP_CLIENT", "COMMUNICATE_UDP" };
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
    enum BaudRate
    {
        BAUD_RATE_9600 = 0,
        BAUD_RATE_19200,
        BAUD_RATE_38400,
        BAUD_RATE_115200,
        BAUD_RATE_128000,
        BAUD_RATE_256000,
        BAUD_RATE_512000,
        BAUD_RATE_921600,
        BAUD_RATE_NUM
    };

    BaudRateComboConfig() : ComboConfig("BAUD_RATE")
    {
        const char* method_list[BAUD_RATE_NUM] = { "9600", "19200", "38400", "115200", "128000", "256000" , "512000" , "921600" };
        for (int i = 0; i < BAUD_RATE_NUM; i++)
        {
            list.emplace_back(std::string(method_list[i]));
        }
        ListString();
    }
};


void ShowUartConfig();