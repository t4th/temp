// temp.cpp : Defines the entry point for the application.
//

#include "TemperatureControl.h"

constexpr Celsius SYSTEM_MAX_TEMP{25};
constexpr Celsius SYSTEM_MIN_TEMP{10};

TemperatureControl::TemperatureControl(
    TemperatureControlInput & input,
    TemperatureControlOutput & output,
    Celsius MinTemp,
    Celsius MaxTemp
) : mInput{input}, mOutput{output}, mMinTemp{}, mMaxTemp{}
{
    mMinTemp = (MinTemp < SYSTEM_MIN_TEMP)?SYSTEM_MIN_TEMP:MinTemp;
    mMaxTemp = (MaxTemp > SYSTEM_MAX_TEMP)?SYSTEM_MAX_TEMP:MaxTemp;
}

void TemperatureControl::Run()
{
    Celsius current{};
     
    const bool success{mInput.GetTemperature(current)};
    if (!success)
    {
        // some HW error action
        return;
    }

    // Straight forward naive solution.
    //
    // Normally you would like to average the readings
    // and use some range Delta to stay within proper range,
    // without touching true MIN and MAX depending how the
    // temperature is controller (0-1 relay or something more fancy).
    //
    // Also formula can be put as external dependency so each
    // Temp Control use some diffrent scheme.. depending how much
    // 'generic' this needs to be.. (if at all)
    if (current > mMaxTemp)
    {
        const bool success{mOutput.DecreaseTemperature()};
        if (!success)
        {
            // some HW error action
            return;
        }
    }
    else if (current < mMinTemp)
    {
        const bool success{mOutput.IncreaseTemperature()};
        if (!success)
        {
            // some HW error action
            return;
        }
    }
    else
    {
        // its within range
    }
}

bool TemperatureControl::ChangeMin(const Celsius NewMin)
{
    if ((NewMin < SYSTEM_MIN_TEMP) || (NewMin > SYSTEM_MAX_TEMP))
    {
        return false;
    }

    mMinTemp = NewMin;

    return true;
}

bool TemperatureControl::ChangeMax(const Celsius NewMax)
{
    if ((NewMax < SYSTEM_MIN_TEMP) || (NewMax > SYSTEM_MAX_TEMP))
    {
        return false;
    }

    mMaxTemp = NewMax;

    return true;
}
