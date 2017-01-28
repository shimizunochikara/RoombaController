#include "settingsmanager.h"

SettingsManager::SettingsManager()
{

}

SettingsManager & SettingsManager::getInstance()
{
    static SettingsManager settingsInstance;
    return settingsInstance;
}

void SettingsManager::addParameter(SettingsParameter * settingsParamter)
{
    paramVector.append(settingsParamter);
}

SettingsParameter * SettingsManager::getParameter(nsSettings::id id)
{
    SettingsParameter * answer = NULL;
    bool findFlag = false;
    for(int i = 0; i < paramVector.length(); i++)
    {
        if(paramVector[i]->getId() == id)
        {
            findFlag = true;
            answer = paramVector[i];
            break;
        }
    }
    return answer;

}

void SettingsManager::initialize()
{

}

