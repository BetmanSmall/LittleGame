#ifndef DIRECTION_H
#define DIRECTION_H

class Direction
{
public:
    enum type { IDLE, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT };
    static const QString toString(type enumVal) {
        return QString(to_string(enumVal).c_str());
    }

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
        valueVector.reserve(9);
        valueVector.push_back(IDLE);
        valueVector.push_back(UP);
        valueVector.push_back(UP_RIGHT);
        valueVector.push_back(RIGHT);
        valueVector.push_back(DOWN_RIGHT);
        valueVector.push_back(DOWN);
        valueVector.push_back(DOWN_LEFT);
        valueVector.push_back(LEFT);
        valueVector.push_back(UP_LEFT);
        return valueVector;\
    }
    static std::map<type,std::string> _make_enum_strings_map()
    {
        std::map<type,std::string> enumStringsMap;
        enumStringsMap.insert(std::make_pair(IDLE, "IDLE"));
        enumStringsMap.insert(std::make_pair(UP, "UP"));
        enumStringsMap.insert(std::make_pair(UP_RIGHT, "UP_RIGHT"));
        enumStringsMap.insert(std::make_pair(RIGHT, "RIGHT"));
        enumStringsMap.insert(std::make_pair(DOWN_RIGHT, "DOWN_RIGHT"));
        enumStringsMap.insert(std::make_pair(DOWN, "DOWN"));
        enumStringsMap.insert(std::make_pair(DOWN_LEFT, "DOWN_LEFT"));
        enumStringsMap.insert(std::make_pair(LEFT, "LEFT"));
        enumStringsMap.insert(std::make_pair(UP_LEFT, "UP_LEFT"));
        return enumStringsMap;
    }
    static std::map<std::string,type> _make_strings_enum_map()
    {
        std::map<std::string,type> stringsEnumMap;
        stringsEnumMap.insert(std::make_pair("IDLE", IDLE));
        stringsEnumMap.insert(std::make_pair("UP", UP));
        stringsEnumMap.insert(std::make_pair("UP_RIGHT", UP_RIGHT));
        stringsEnumMap.insert(std::make_pair("RIGHT", RIGHT));
        stringsEnumMap.insert(std::make_pair("DOWN_RIGHT", DOWN_RIGHT));
        stringsEnumMap.insert(std::make_pair("DOWN", DOWN));
        stringsEnumMap.insert(std::make_pair("DOWN_LEFT", DOWN_LEFT));
        stringsEnumMap.insert(std::make_pair("LEFT", LEFT));
        stringsEnumMap.insert(std::make_pair("UP_LEFT", UP_LEFT));
        return stringsEnumMap;
    }
};

#endif // DIRECTION_H

