#ifndef TOWERATTACKTYPE_H
#define TOWERATTACKTYPE_H

class TowerAttackType
{
public:
    enum type { FireBall, Pit, Melee, Range, RangeFly };
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
        valueVector.reserve(5);
        valueVector.push_back(FireBall);
        valueVector.push_back(Pit);
        valueVector.push_back(Melee);
        valueVector.push_back(Range);
        valueVector.push_back(RangeFly);
        return valueVector;
    }
    static std::map<type,std::string> _make_enum_strings_map()
    {
        std::map<type,std::string> enumStringsMap;
        enumStringsMap.insert(std::make_pair(FireBall, "FireBall"));
        enumStringsMap.insert(std::make_pair(Pit, "Pit"));
        enumStringsMap.insert(std::make_pair(Melee, "Melee"));
        enumStringsMap.insert(std::make_pair(Range, "Range"));
        enumStringsMap.insert(std::make_pair(RangeFly, "RangeFly"));
        return enumStringsMap;
    }
    static std::map<std::string,type> _make_strings_enum_map()
    {
        std::map<std::string,type> stringsEnumMap;
        stringsEnumMap.insert(std::make_pair("FireBall", FireBall));
        stringsEnumMap.insert(std::make_pair("Pit", Pit));
        stringsEnumMap.insert(std::make_pair("Melee", Melee));
        stringsEnumMap.insert(std::make_pair("Range", Range));
        stringsEnumMap.insert(std::make_pair("RangeFly", RangeFly));
        return stringsEnumMap;
    }
};

#endif // TOWERATTACKTYPE_H
