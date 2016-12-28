#ifndef SENSORPACKECTPROPERTY
#define SENSORPACKECTPROPERTY

#include <QList>


#define MAXIMUM_ID_NUMBER 256

enum packetId{
    begin                   = 7,
    BumpsWheeldrops          = 7,
    Wall                     = 8,
    CliffLeft                = 9,
    CliffFrontLeft          = 10,
    CliffFrontRight         = 11,
    CliffRight              = 12,
    VirtualWall             = 13,
    Overcurrents            = 14,
    DirtDetect              = 15,
    Unused_1                = 16,
    IrOpcode                = 17,
    Buttons                 = 18,
    Distance                = 19,
    Angle                   = 20,
    ChargingState           = 21,
    Voltage                 = 22,
    Current                 = 23,
    Temperature             = 24,
    BatteryCharge           = 25,
    BatteryCapacity         = 26,
    WallSignal              = 27,
    CliffLeftSignal         = 28,
    CliffFrontLeftSignal    = 29,
    CliffFrontRightSignal   = 30,
    CliffRightSignal        = 31,
    Unused_2                = 32,
    Unused_3                = 33,
    ChargerAvailable        = 34,
    OpenInterfaceMode       = 35,
    SongNumber              = 36,
    SongPlaying             = 37,
    OiStreamNumPackets      = 38,
    Velocity                = 39,
    Radius                  = 40,
    VelocityRight           = 41,
    VelocityLeft            = 42,
    EncoderCountsLeft       = 43,
    EncoderCountsRight      = 44,
    LightBumper             = 45,
    LightBumpLeft           = 46,
    LightBumpFrontLeft      = 47,
    LightBumpCenterLeft     = 48,
    LightBumpCenterRight    = 49,
    LightBumpFrontRight     = 50,
    LightBumpRight          = 51,
    IrOpcodeLeft            = 52,
    IrOpcodeRight           = 53,
    LeftMotorCurrent        = 54,
    RightMotorCurrent        = 55,
    MainBrushCurrent        = 56,
    SideBrushCurrent        = 57,
    Stasis                  = 58,
    end                     = Stasis
};

class sensorPacketProperty
{
public:
    // this value are returned when id is not exist.
    static const int fetchError = -100000;
    //this is no existed value
    static const int noExistedValue = -100000;

    sensorPacketProperty();
    ~sensorPacketProperty();
    int getDataSize(int id);
    int getMinimumValue(int id);
    int getMaximumValue(int id);
    QString getName(int id);
    QString getUnit(int id);

private:
    void setIdData(int id, int dataSize, int minimumValue, int maximumValue, QString name, QString unit);
    void initData();

private:
    int dataSize[MAXIMUM_ID_NUMBER];
    int minimumValue[MAXIMUM_ID_NUMBER];
    int maximumValue[MAXIMUM_ID_NUMBER];
    QString name[MAXIMUM_ID_NUMBER];
    QString unit[MAXIMUM_ID_NUMBER];
};

class sensorPacketData
{
public:
    void setInterval(int interval);
    int getInterval();
    void setRequestDataList(QByteArray requestList);
    QByteArray getRequestDataList();
    void setArrivedData(QByteArray data);
    QByteArray getArrivedData();
private:
    int request_interval;
    QByteArray requestDataList;
    QByteArray arrivedData;
};

#endif // SENSORPACKECTPROPERTY
