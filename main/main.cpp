
#include <iostream>
#include <cstdio>

#include "../TemperatureControl/TemperatureControl.h"

// test global variable
// current room temperature
static Celsius current_temp{23};

// Some input interface
class InputSensor : public TemperatureControlInput
{
    bool GetTemperature(Celsius & value) final
    {
        value = current_temp;
        return true;
    }
};

// Some output interface
class OutputControl : public TemperatureControlOutput
{
public:
    bool IncreaseTemperature() final
    {
        std::cout << "heating" << std::endl;
        ++current_temp;
        return true;
    }

    bool DecreaseTemperature() final
    {
        std::cout << "cooling" << std::endl;
        --current_temp;
        return true;
    }
};
int main()
{
    InputSensor inputSensor;
    OutputControl OutputControl;

    constexpr Celsius InitialMin{18};
    constexpr Celsius InitialMax{20};

    TemperatureControl room1control(
        inputSensor,
        OutputControl,
        InitialMin,
        InitialMax
    );

    std::cout << "Temperature range: " << InitialMin << " - " << InitialMax << ", starting temp: " << current_temp << std::endl;
    std::cout << "Press '+' to increse temp, '-' to decrease temp, <empty> - normal run, 'q' - quit" << std::endl;
    
    bool exit{false};

    while(!exit)
    {
        room1control.Run();

        auto key{getchar()};
    
        if ('+' == key)
        {
            ++current_temp;
            std::cout << "current_temp + : " << current_temp << std::endl;
        }
        else if ('-' == key)
        {
            --current_temp;
            std::cout << "current_temp -: " << current_temp << std::endl;
        }
        else if ('q' == key)
        {
            exit = true;
        }
        else
        {
            std::cout << "current_temp : " << current_temp << std::endl;
        }
    }

    return 0;
}