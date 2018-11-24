#ifndef GAMETYPE
#define GAMETYPE

#include <QDebug>
//#include <QString>
//#include <stdio.h>
//#include <iostream>
//#include <stdlib.h>

class GameType
{
public:
    enum type { LittleGame, TowerDefence, OrthogonalityTowerDefence };
//    static const QString toString(type enumVal) {
//        return QString(to_string(enumVal).c_str());
//    }

    static const std::string &to_string( type enumVal )
    {
        static const std::map<type,std::string> enumStringsMap = _make_enum_strings_map();
        auto it = enumStringsMap.find(enumVal);
        static std::string emptyString;
        if(it==enumStringsMap.end())
            return emptyString;
        return it->second;
    }
    static type from_string(const std::string &value)
    {
        static const std::map<std::string,type> stringsEnumMap = _make_strings_enum_map();
        std::map<std::string,type>::const_iterator it = stringsEnumMap.find(value);
        if(it==stringsEnumMap.end())
            return (type)0;
        return it->second;
    }
    static const std::vector<type>& values()
    {
        static const std::vector<type> valueVector = _make_values();
        return valueVector;
    }
private:
    static const std::vector<type> _make_values()
    {
        std::vector<type> valueVector;
        valueVector.reserve(3);
        valueVector.push_back(LittleGame);
        valueVector.push_back(TowerDefence);
        valueVector.push_back(OrthogonalityTowerDefence);
        return valueVector;
    }
    static std::map<type,std::string> _make_enum_strings_map()
    {
        std::map<type,std::string> enumStringsMap;
        enumStringsMap.insert(std::make_pair(LittleGame, "LittleGame"));
        enumStringsMap.insert(std::make_pair(TowerDefence, "TowerDefence"));
        enumStringsMap.insert(std::make_pair(OrthogonalityTowerDefence, "OrthogonalityTowerDefence"));
        return enumStringsMap;
    }
    static std::map<std::string,type> _make_strings_enum_map()
    {
        std::map<std::string,type> stringsEnumMap;
        stringsEnumMap.insert(std::make_pair("LittleGame", LittleGame));
        stringsEnumMap.insert(std::make_pair("TowerDefence", TowerDefence));
        stringsEnumMap.insert(std::make_pair("OrthogonalityTowerDefence", OrthogonalityTowerDefence));
        return stringsEnumMap;
    }
};

#endif // GAMETYPE
