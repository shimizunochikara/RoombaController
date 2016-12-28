
#include "sensorpacketproperty.h"
#include <QtCore/QDebug>

/*************************************************/
/* define sensorPacketProperty method            */
/*************************************************/
sensorPacketProperty::sensorPacketProperty()
{
    // initialize List.
    for(int i = 0; i < end; i++)
    {
        dataSize[i]        = noExistedValue;
        minimumValue[i]    = noExistedValue;
        maximumValue[i]    = noExistedValue;
        name[i]            = "";
        unit[i]            = "";
    }
    initData();
}

sensorPacketProperty::~sensorPacketProperty()
{

}

void sensorPacketProperty::setIdData(int id, int size, int minValue, int maxValue, QString dataname, QString dataunit)
{
    dataSize[id]        = size;
    minimumValue[id]    = minValue;
    maximumValue[id]    = maxValue;
    name[id]            = dataname;
    unit[id]            = dataunit;
}

void sensorPacketProperty::initData()
{
    //ID, dataSize, minimumValue, maximumValue, name, unit
    setIdData(BumpsWheeldrops,        1,        0,       15,              "Bumps Wheel drops",    "");
    setIdData(Wall,                   1,        0,       1,              "Wall",                  "");
    setIdData(CliffLeft,              1,        0,       1,              "Cliff Left",            "");
    setIdData(CliffFrontLeft,         1,        0,       1,              "Cliff Front Left",      "");
    setIdData(CliffFrontRight,        1,        0,       1,              "CliffFrontRight",       "");
    setIdData(CliffRight,             1,        0,       1,              "CliffRight",            "");
    setIdData(VirtualWall,            1,        0,       1,              "Virtual Wall",          "");
    setIdData(Overcurrents,           1,        0,       29,              "Wheel Overcurrents",   "");
    setIdData(DirtDetect,             1,        0,      255,              "Dirt Detect",          "");
    setIdData(Unused_1,               1,        0,      255,              "Unused",               "");

    setIdData(IrOpcode,               1,        0,      255,            "Ir Opcode",              "");
    setIdData(Buttons,                1,        0,      255,            "Buttons",                "");
    setIdData(Distance,               2,   -32768,    32767,            "Distance",             "mm");
    setIdData(Angle,                  2,   -32768,    32767,                "Angle",       "degrees");
    setIdData(ChargingState,          1,        0,        6,        "ChargingState",              "");
    setIdData(Voltage,                2,        0,    65535,              "Voltage",            "mV");
    setIdData(Current,                2,   -32768,    32767,              "Current",            "mA");
    setIdData(Temperature,            1,     -128,      127,          "Temperature",             "C");
    setIdData(BatteryCharge,          2 ,       0,    65535,        "BatteryCharge",           "mAh");
    setIdData(BatteryCapacity,        2,        0,    65535,      "BatteryCapacity",           "mAh");

    setIdData(WallSignal,             2,        0,     1023,           "WallSignal",              "");
    setIdData(CliffLeftSignal,        2,        0,     4095,      "CliffLeftSignal",              "");
    setIdData(CliffFrontLeftSignal,   2,        0,     4095, "CliffFrontLeftSignal",              "");
    setIdData(CliffFrontRightSignal,  2,        0,     4095,"CliffFrontRightSignal",              "");
    setIdData(CliffRightSignal,       2,        0,     4095,     "CliffRightSignal",              "");
    setIdData(Unused_2,               1,        0,      255,             "Unused_2",              "");
    setIdData(Unused_3,               2,        0,    65535,             "Unused_3",              "");
    setIdData(ChargerAvailable,       1,        0,        3,     "ChargerAvailable",              "");
    setIdData(OpenInterfaceMode,      1,        0,        3,    "OpenInterfaceMode",              "");
    setIdData(SongNumber,             1,        0,        4,           "SongNumber",              "");

    setIdData(SongPlaying,            1,        0,        1,          "SongPlaying",              "");
    setIdData(OiStreamNumPackets,     1,        0,      108,   "OiStreamNumPackets",              "");
    setIdData(Velocity,               2,     -500,      500,             "Velocity",          "mm/s");
    setIdData(Radius,                 2,   -32768,    32767,               "Radius",            "mm");
    setIdData(VelocityRight,          2,     -500,      500,        "VelocityRight",          "mm/s");
    setIdData(VelocityLeft,           2,     -500,      500,         "VelocityLeft",          "mm/s");
    setIdData(EncoderCountsLeft,      2,        0,    65535,   "EncoderCountsLeft",               "");
    setIdData(EncoderCountsRight,     2,        0,    65535,   "EncoderCountsRight",              "");
    setIdData(LightBumper,            1,        0,      127,   "LightBumper",                     "");
    setIdData(LightBumpLeft,          2,        0,     4095,   "LightBumpLeft",                   "");

    setIdData(LightBumpFrontLeft,     2,        0,     4095,   "LightBumpFrontLeft",              "");
    setIdData(LightBumpCenterLeft,    2,        0,     4095,   "LightBumpCenterLeft",             "");
    setIdData(LightBumpCenterRight,   2,        0,     4095,   "LightBumpCenterRight",            "");
    setIdData(LightBumpFrontRight,    2,        0,     4095,   "LightBumpFrontRight",             "");
    setIdData(LightBumpRight,         2,        0,     4095,   "LightBumpRight",                  "");
    setIdData(IrOpcodeLeft,           1,        0,      255,   "IrOpcodeLeft",                    "");
    setIdData(IrOpcodeRight,          1,        0,      255,   "IrOpcodeRight",                   "");
    setIdData(LeftMotorCurrent,       2,   -32768,    32767,   "LeftMotorCurrent",              "mA");
    setIdData(RightMotorCurrent,      2,   -32768,    32767,   "RightMotorCurren",                "");
    setIdData(MainBrushCurrent,       2,   -32768,    32767,   "MainBrushCurrent",                "");

    setIdData(SideBrushCurrent,       2,   -32768,    32767,   "SideBrushCurrent",                "");
    setIdData(Stasis,                 1,        0,        3,              "Stasis",               "");


}

int sensorPacketProperty::getDataSize(int id)
{
    if(begin > id || end < id)
    {
        qDebug("getDataSize Error: id = %d", id);
        return fetchError;
    }
    else if(dataSize[id] == noExistedValue)
    {
        qDebug("getDataSize Error: id = %d", id);
        return fetchError;
    }
    else
    {
        return dataSize[id];
    }
}

int sensorPacketProperty::getMinimumValue(int id)
{
    if(begin > id || end < id)
    {
        qDebug("getMinimumValue Error: id = %d", id);
        return fetchError;
    }
    else if(dataSize[id] == noExistedValue)
    {
        qDebug("getMinimumValue: id = %d", id);
        return fetchError;
    }
    else
    {
        return minimumValue[id];
    }
}

int sensorPacketProperty::getMaximumValue(int id)
{
    if(begin > id || end < id)
    {
        qDebug("getMaximumValue Error: id = %d", id);
        return fetchError;
    }
    else if(dataSize[id] == noExistedValue)
    {
        qDebug("getMaximumValue: id = %d", id);
        return fetchError;
    }
    else
    {
        return maximumValue[id];
    }
}

QString sensorPacketProperty::getName(int id)
{
    if(begin > id || end < id)
    {
        qDebug("getName Error: id = %d", id);
        return fetchError;
    }
    else
    {
        return name[id];
    }
}

QString sensorPacketProperty::getUnit(int id)
{
    if(begin > id || end < id)
    {
        qDebug("getUnit Error: id = %d", id);
        return fetchError;
    }
    else
    {
        return unit[id];
    }
}

/*************************************************/
/* define sensorPacketData method                */
/*************************************************/

void sensorPacketData::setInterval(int interval)
{
    request_interval = interval;
}

int sensorPacketData::getInterval()
{
    return request_interval;
}

void sensorPacketData::setRequestDataList(QByteArray requestList)
{
    requestDataList = requestList;
}

QByteArray sensorPacketData::getRequestDataList()
{
    return requestDataList;
}

void sensorPacketData::setArrivedData(QByteArray data)
{
    arrivedData = data;
}

QByteArray sensorPacketData::getArrivedData()
{
    return arrivedData;
}
