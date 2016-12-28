#include "dataprocessor.h"
#include "roombaserial.h"

dataProcessor::dataProcessor()
{

}

dataProcessor::~dataProcessor()
{

}

void dataProcessor::setRoombaSerial(roombaSerial* _rs)
{
    rs = _rs;
}

unsigned int dataProcessor::getCurrentBattery()
{
    return rs->fetchPacket(BatteryCharge);
}

unsigned int dataProcessor::getBatteryCapacity()
{
    return rs->fetchPacket(BatteryCapacity);
}

unsigned int dataProcessor::getOpenInterfaceMode()
{
    return rs->fetchPacket(OpenInterfaceMode);
}

unsigned int dataProcessor::getEncoderCountsLeft()
{
    return rs->fetchPacket(EncoderCountsLeft);
}

unsigned int dataProcessor::getEncoderCountsRight()
{
    return rs->fetchPacket(EncoderCountsRight);
}

short dataProcessor::getLightBumpCenterLeft()
{
    return rs->fetchPacket(LightBumpCenterLeft);
}

short dataProcessor::getLightBumpCenterRight()
{
    return rs->fetchPacket(LightBumpCenterRight);
}

short dataProcessor::getDirtDetect()
{
    return rs->fetchPacket(DirtDetect);
}



void dataProcessor::dataReceived()
{
    /*
    getCurrentBattery();
    getBatteryCapacity();
    getOpenInterfaceMode();
    getEncoderCountsLeft();
    getEncoderCountsRight();
    */
    getLightBumpCenterLeft();
    getLightBumpCenterRight();
    //getDirtDetect();
}
