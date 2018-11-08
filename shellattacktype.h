#ifndef SHELLATTACKTYPE_H
#define SHELLATTACKTYPE_H

class ShellAttackType
{
public:
    enum type { FireBall, AutoTarget, FirstTarget, SingleTarget, MultipleTarget, MassAddEffect };
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
        valueVector.reserve(6);
        valueVector.push_back(FireBall);
        valueVector.push_back(AutoTarget);
        valueVector.push_back(FirstTarget);
        valueVector.push_back(SingleTarget);
        valueVector.push_back(MultipleTarget);
        valueVector.push_back(MassAddEffect);
        return valueVector;
    }
    static std::map<type,std::string> _make_enum_strings_map()
    {
        std::map<type,std::string> enumStringsMap;
        enumStringsMap.insert(std::make_pair(FireBall, "FireBall"));
        enumStringsMap.insert(std::make_pair(AutoTarget, "AutoTarget"));
        enumStringsMap.insert(std::make_pair(FirstTarget, "FirstTarget"));
        enumStringsMap.insert(std::make_pair(SingleTarget, "SingleTarget"));
        enumStringsMap.insert(std::make_pair(MultipleTarget, "MultipleTarget"));
        enumStringsMap.insert(std::make_pair(MassAddEffect, "MassAddEffect"));
        return enumStringsMap;
    }
    static std::map<std::string,type> _make_strings_enum_map()
    {
        std::map<std::string,type> stringsEnumMap;
        stringsEnumMap.insert(std::make_pair("FireBall", FireBall));
        stringsEnumMap.insert(std::make_pair("AutoTarget", AutoTarget));
        stringsEnumMap.insert(std::make_pair("FirstTarget", FirstTarget));
        stringsEnumMap.insert(std::make_pair("SingleTarget", SingleTarget));
        stringsEnumMap.insert(std::make_pair("MultipleTarget", MultipleTarget));
        stringsEnumMap.insert(std::make_pair("MassAddEffect", MassAddEffect));
        return stringsEnumMap;
    }
};

#endif // SHELLATTACKTYPE_H
