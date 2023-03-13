// Control module used to control temperature to stay within selected range.
#pragma once

// Alias for Celsius temperature type.
typedef int Celsius;

// Abstract Input interface.
class TemperatureControlInput
{
public:
    virtual bool GetTemperature(Celsius & value) = 0;
};

// Abstract Output interface.
class TemperatureControlOutput
{
public:
    virtual bool IncreaseTemperature() = 0;
    virtual bool DecreaseTemperature() = 0;
};

// TemperatureControl Class definition used to control temperature to
// stay within a selected range <MIN:MAX>.
class TemperatureControl
{
public:
    TemperatureControl() = delete;

    // Class constructor used to provide Inputs, Outputs implementation
    // and initial MIN, MAX temperatures.
    TemperatureControl(
        TemperatureControlInput & input,
        TemperatureControlOutput & output,
        Celsius MinTemp,
        Celsius MaxTemp
    );
    
    // Function used to execute class logic in a non-blocking way.
    // Rate of the calling should be dependant on precison and
    // hardware sensor/control used.
    void Run();

    // Function used to change MIN temperature.
    bool ChangeMin(const Celsius NewMin);

    // Function used to change MAX temperature.
    bool ChangeMax(const Celsius NewMax);

private:
    // Reference to Input implementation.
    TemperatureControlInput &     mInput;

    // Reference to Outoutimplementation.
    TemperatureControlOutput &    mOutput;

    // Viariable holding MIN temperature of the selected range.
    Celsius mMinTemp;

    // Viariable holding MAX temperature of the selected range.
    Celsius mMaxTemp;
};