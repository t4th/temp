// temp.cpp : Defines the entry point for the application.
//

#include "TemperatureControl.h"

// module/project global
// can also be put as parameter
constexpr Celsius SYSTEM_MAX_TEMP{25};
constexpr Celsius SYSTEM_MIN_TEMP{10};

TemperatureControl::TemperatureControl(
    TemperatureControlInput & input,
    TemperatureControlOutput & output,
    Celsius MinTemp,
    Celsius MaxTemp
) : mInput{input}, mOutput{output}, mMinTemp{}, mMaxTemp{}
{
    // On invalid parameters use default settings.
    // it would be nicer to be done as an assert.
    if (MinTemp <= MaxTemp)
    {
        mMinTemp = (MinTemp < SYSTEM_MIN_TEMP)?SYSTEM_MIN_TEMP:MinTemp;
        mMaxTemp = (MaxTemp > SYSTEM_MAX_TEMP)?SYSTEM_MAX_TEMP:MaxTemp;
    }
    else
    {
        mMinTemp = SYSTEM_MIN_TEMP;
        mMaxTemp = SYSTEM_MAX_TEMP;
    }
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
    // Check range.
    if ((NewMin < SYSTEM_MIN_TEMP) || (NewMin > SYSTEM_MAX_TEMP))
    {
        return false;
    }

    // Be sure Min is never bigger than Max.
    if (NewMin > mMaxTemp)
    {
        return false;
    }

    mMinTemp = NewMin;

    return true;
}

bool TemperatureControl::ChangeMax(const Celsius NewMax)
{
    // Check range.
    if ((NewMax < SYSTEM_MIN_TEMP) || (NewMax > SYSTEM_MAX_TEMP))
    {
        return false;
    }

    // Be sure Max is never smaller than Min.
    if (NewMax < mMinTemp)
    {
        return false;
    }

    mMaxTemp = NewMax;

    return true;
}
