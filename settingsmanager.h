#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include<QtCore>

class SettingsParameter;

namespace nsSettings
{
    enum id
    {
        firstParameter = 0
    };
    enum type
    {
        paramInt,
        paramDouble,
        paramString,
    };
}



class SettingsManager
{
public:
    void addParameter(SettingsParameter * settingsParamter);
    SettingsParameter * getParameter(nsSettings::id id);

private:
    SettingsManager();
    SettingsManager& getInstance();
    void initialize();
    QVector<SettingsParameter*> paramVector;
};

class SettingsParameter
{
public:
    nsSettings::id getId()
    {
        return myId;
    };
    nsSettings::type getType()
    {
      return myType;
    };

protected:
   nsSettings::type myType;
   nsSettings::id myId;
};

class settingsParameterInt : SettingsParameter
{
private:
    int myParam;
public:
    settingsParameterInt(nsSettings::id id, int param)
    {
      myType = nsSettings::paramInt;
      myId = id;
      myParam = param;
    };
    void setParameter(int param)
    {
        param = myParam;
    };
    int getParameter()
    {
        return myParam;
    };
};

class settingsParameterDouble : SettingsParameter
{
private:
    double myParam;
public:
    settingsParameterDouble(nsSettings::id id, double param)
    {
      myType = nsSettings::paramDouble;
      myId = id;
      myParam = param;
    };
    void setParameter(double param)
    {
        param = myParam;
    };
    double getParameter()
    {
        return myParam;
    };
};

class settingsParameterString : SettingsParameter
{
private:
    QString myParam;
    settingsParameterString(nsSettings::id id, QString param)
    {
      myType = nsSettings::paramString;
      myId = id;
      myParam = param;
    };
    void setParameter(QString param)
    {
        param = myParam;
    };
    QString getParameter()
    {
        return myParam;
    };
};



#endif // SETTINGSMANAGER_H
