#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../TemperatureControl/TemperatureControl.h"

// Input/output interface mocks
class InputSensorMock : public TemperatureControlInput
{
public:
    MOCK_METHOD1(GetTemperature, bool(Celsius & value));
};

class OutputControlMock : public TemperatureControlOutput
{
public:
    MOCK_METHOD0(IncreaseTemperature, bool());
    MOCK_METHOD0(DecreaseTemperature, bool());
};

using ::testing::MockFunction;
using ::testing::Mock;

/* Common mocks and setup. */
class TemperatureControl_tests : public ::testing::Test
{
public:

    TemperatureControl_tests()
    {
    }

    void SetUp() override
    {
    }

    // input/output interface mocks instances
    InputSensorMock     inputSensorMock;
    OutputControlMock   outputControlMock;
};

// Initial starting temperature is within range
// expected: no action taken
TEST_F(TemperatureControl_tests, InitalStartingTempWithinRage)
 {
    using ::testing::Exactly;
    using ::testing::Return;
    using ::testing::_;
    using ::testing::SetArgReferee;
    using ::testing::DoAll;

    constexpr Celsius InitialMin{15};
    constexpr Celsius InitialMax{20};

    // Simulate input sensor readout
    Celsius current_room_temperature{17};

    TemperatureControl temperatureControl(
        inputSensorMock,
        outputControlMock,
        InitialMin,
        InitialMax);

    // Return current temperature
    EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillOnce(DoAll(SetArgReferee<0>(current_room_temperature), Return(true)));
    
    // No action should be taken
    EXPECT_CALL(outputControlMock, IncreaseTemperature()).Times(Exactly(0));
    EXPECT_CALL(outputControlMock, DecreaseTemperature()).Times(Exactly(0));
    
    // Run tested function
    temperatureControl.Run();
}

// Initial starting temperature is bigger than MAX set
// expected: cooldown is called until readout temp. is valid
TEST_F(TemperatureControl_tests, InitalStartingTempTooBig)
 {
    using ::testing::Exactly;
    using ::testing::Return;
    using ::testing::_;
    using ::testing::SetArgReferee;
    using ::testing::DoAll;
    using ::testing::Invoke;

    constexpr Celsius InitialMin{15};
    constexpr Celsius InitialMax{20};

    // Simulate input sensor readout
    Celsius current_room_temperature{22};

    TemperatureControl temperatureControl(
        inputSensorMock,
        outputControlMock,
        InitialMin,
        InitialMax);

    // Return current temperature
    EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillRepeatedly(Invoke([&](Celsius & value)-> bool
        {
            value = current_room_temperature;
            return true;
        }));

    // Increase should never been called
    EXPECT_CALL(outputControlMock, IncreaseTemperature()).Times(Exactly(0));
    
    // Decrease should be called as long current room temp. is bigger than MAX
    EXPECT_CALL(outputControlMock, DecreaseTemperature())
        .WillRepeatedly(Invoke([&]()-> bool
        {
            --current_room_temperature;
            return true;
        }));

    // Run tested function #1
    temperatureControl.Run();
    
    // Run tested function #2
    temperatureControl.Run();

    // Update mock with new current temperature
   EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillRepeatedly(DoAll(SetArgReferee<0>(current_room_temperature), Return(true)));

    // No action should be taken in this case.
    EXPECT_CALL(outputControlMock, IncreaseTemperature()).Times(Exactly(0));
    EXPECT_CALL(outputControlMock, DecreaseTemperature()).Times(Exactly(0));

    // Run tested function #3
    temperatureControl.Run();
}

// Change MIN temperature to higher value in runtime
// expected: increase temperature to new MIN
TEST_F(TemperatureControl_tests, ChangeMinTempDuringRuntime)
 {
    using ::testing::Exactly;
    using ::testing::Return;
    using ::testing::_;
    using ::testing::SetArgReferee;
    using ::testing::DoAll;
    using ::testing::Invoke;

    constexpr Celsius InitialMin{15};
    constexpr Celsius InitialMax{20};

    // Simulate input sensor readout
    Celsius current_room_temperature{15};

    TemperatureControl temperatureControl(
        inputSensorMock,
        outputControlMock,
        InitialMin,
        InitialMax);

    // Return current temperature
    EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillRepeatedly(Invoke([&](Celsius & value)-> bool
        {
            value = current_room_temperature;
            return true;
        }));

    // No action should be taken
    EXPECT_CALL(outputControlMock, IncreaseTemperature()).Times(Exactly(0));
    EXPECT_CALL(outputControlMock, DecreaseTemperature()).Times(Exactly(0));

    // Run tested function few times
    temperatureControl.Run();
    temperatureControl.Run();
    temperatureControl.Run();

    // Change Min temp.
    constexpr Celsius NewMinValue{18};

    EXPECT_EQ(temperatureControl.ChangeMin(NewMinValue), true);
    
    // Increase should be called as long current room temp. is smaller than MIN
    EXPECT_CALL(outputControlMock, IncreaseTemperature())
        .WillRepeatedly(Invoke([&]()-> bool
        {
            ++current_room_temperature;
            return true;
        }));

    // Decrease should never been called
    EXPECT_CALL(outputControlMock, DecreaseTemperature()).Times(Exactly(0));

    // Run tested function #1
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, InitialMin+1); // current temperature is going up

    // Run tested function #2
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, InitialMin+2); // current temperature is going up

    // Run tested function #3
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, InitialMin+3); // current temperature is going up

    // Update mock with new current temperature
   EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillRepeatedly(DoAll(SetArgReferee<0>(current_room_temperature), Return(true)));

    // No action should be taken in this case.
    EXPECT_CALL(outputControlMock, IncreaseTemperature()).Times(Exactly(0));
    EXPECT_CALL(outputControlMock, DecreaseTemperature()).Times(Exactly(0));

    // Run tested function #4
    temperatureControl.Run();

    // current temperature changed to new MIN value
    ASSERT_EQ(current_room_temperature, NewMinValue);
}

// Multiple small scenarios in one test
TEST_F(TemperatureControl_tests, TemperatureChangingInRuntime)
 {
    using ::testing::Exactly;
    using ::testing::Return;
    using ::testing::_;
    using ::testing::SetArgReferee;
    using ::testing::DoAll;
    using ::testing::Invoke;

    constexpr Celsius InitialMin{15};
    constexpr Celsius InitialMax{19};

    // Simulate input sensor readout
    Celsius current_room_temperature{17};

    TemperatureControl temperatureControl(
        inputSensorMock,
        outputControlMock,
        InitialMin,
        InitialMax);

    // Return current temperature
    EXPECT_CALL(inputSensorMock, GetTemperature(_))
        .WillRepeatedly(Invoke([&](Celsius & value)-> bool
        {
            value = current_room_temperature;
            return true;
        }));

    // Increase should be called as long current room temp. is smaller than MIN
    EXPECT_CALL(outputControlMock, IncreaseTemperature())
        .WillRepeatedly(Invoke([&]()-> bool
        {
            ++current_room_temperature;
            return true;
        }));

    // Decrease should be called as long current room temp. is bigger than MAX
    EXPECT_CALL(outputControlMock, DecreaseTemperature())
        .WillRepeatedly(Invoke([&]()-> bool
        {
            --current_room_temperature;
            return true;
        }));

    // Run tested function:

    // Expected: current temp. within range - no changes
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 17);
    
    // Increase input temperature
    ++current_room_temperature;

    // Expected: current temp. within range - no changes
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 18);

    // Increase input temperature
    ++current_room_temperature;

    // Expected: current temp. within range - no changes
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 19);

    // Increase input temperature
    ++current_room_temperature;

    // Expected: current temp. is out of range (20) -> decrease it to 19
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 19);

    // Change Min temp to 19
    ASSERT_EQ(temperatureControl.ChangeMin(19), true);

    // Increase input temperature
    ++current_room_temperature;

    // Expected: Current temp not changed.
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 19);

    // Dencrease input temperature
    --current_room_temperature;

    // Expected: Current temp lower than MIN - increase it
    temperatureControl.Run();

    ASSERT_EQ(current_room_temperature, 19);
}
