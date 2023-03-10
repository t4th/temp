// Seome 'generic' Temperature control.
#pragma once

typedef int Celsius;

// Some generic input interface
class TemperatureControlInput
{
public:
    virtual bool GetTemperature(Celsius & value) = 0;
};

// Some generic output interface
class TemperatureControlOutput
{
public:
    virtual bool IncreaseTemperature() = 0;
    virtual bool DecreaseTemperature() = 0;
};

class TemperatureControl
{
public:
    TemperatureControl() = delete;

    // Some fancy generic dependency inversion.
    TemperatureControl(
        TemperatureControlInput & input,
        TemperatureControlOutput & output,
        Celsius MinTemp,
        Celsius MaxTemp
    );
    
    // Run the logic itself.
    // You want to call this function as often the precision
    // is required.
    // 
    // Timing of the control can also be implemented withing
    // this class.
    void Run();

    bool ChangeMin(const Celsius NewMin);
    bool ChangeMax(const Celsius NewMax);

private:
    TemperatureControlInput &     mInput;
    TemperatureControlOutput &    mOutput;
    Celsius mMinTemp;
    Celsius mMaxTemp;
};