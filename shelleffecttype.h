#ifndef SHELLEFFECTTYPE_H
#define SHELLEFFECTTYPE_H

#include <QDebug>

class ShellEffectType
{
public:
    enum ShellEffectEnum { FreezeEffect, FireEffect };
    static const QString toString(ShellEffectEnum enumVal) {
        return QString(to_string(enumVal).c_str());
    }

public:
    ShellEffectEnum shellEffectEnum;
    float time;
    float elapsedTime;
    float damage;
    float speed;
    bool used = false;

public:
    ShellEffectType(ShellEffectEnum shellEffectEnum) {
        this->shellEffectEnum = shellEffectEnum;
        this->elapsedTime = 0;
    }

    ShellEffectType(ShellEffectType* shellEffectType) {
        this->shellEffectEnum = shellEffectType->shellEffectEnum;
        this->time = shellEffectType->time;
        this->elapsedTime = shellEffectType->elapsedTime;
        this->damage = shellEffectType->damage;
        this->speed = shellEffectType->speed;
    }

//    @Override
//    boolean equals(Object object) {
//        if(object instanceof ShellEffectType) {
//            ShellEffectType shellEffectType = (ShellEffectType) object;
//            if (this.shellEffectEnum.equals(shellEffectType.shellEffectEnum)) {
//                return true;
//            } else {
//                return false;
//            }
//        }
//        return false;
//    }

    QString toString() {
        QString sb("ShellEffectType[");
        sb.append(",shellEffectEnum:" + ShellEffectType::toString(shellEffectEnum));
        sb.append(",time:" + QString::number(time));
        sb.append(",elapsedTime:" + QString::number(elapsedTime));
        sb.append(",damage:" + QString::number(damage));
        sb.append(",speed:" + QString::number(speed));
        sb.append(",used:" + used);
        sb.append("]");
        return sb;
    }

    static const std::string &to_string( ShellEffectEnum enumVal )
    {
        static const std::map<ShellEffectEnum,std::string> enumStringsMap = _make_enum_strings_map();
        auto it = enumStringsMap.find(enumVal);
        static std::string emptyString;
        if(it==enumStringsMap.end())
            return emptyString;
        return it->second;
    }
    static ShellEffectEnum from_string(const std::string &value)
    {
        static const std::map<std::string,ShellEffectEnum> stringsEnumMap = _make_strings_enum_map();
        std::map<std::string,ShellEffectEnum>::const_iterator it = stringsEnumMap.find(value);
        if(it==stringsEnumMap.end())
            return (ShellEffectEnum)0;
        return it->second;
    }
    static const std::vector<ShellEffectEnum>& values()
    {
        static const std::vector<ShellEffectEnum> valueVector = _make_values();
        return valueVector;
    }
private:
    static const std::vector<ShellEffectEnum> _make_values()
    {
        std::vector<ShellEffectEnum> valueVector;
        valueVector.reserve(2);
        valueVector.push_back(FreezeEffect);
        valueVector.push_back(FireEffect);
        return valueVector;
    }
    static std::map<ShellEffectEnum,std::string> _make_enum_strings_map()
    {
        std::map<ShellEffectEnum,std::string> enumStringsMap;
        enumStringsMap.insert(std::make_pair(FreezeEffect, "FreezeEffect"));
        enumStringsMap.insert(std::make_pair(FireEffect, "FireEffect"));
        return enumStringsMap;
    }
    static std::map<std::string,ShellEffectEnum> _make_strings_enum_map()
    {
        std::map<std::string,ShellEffectEnum> stringsEnumMap;
        stringsEnumMap.insert(std::make_pair("FreezeEffect", FreezeEffect));
        stringsEnumMap.insert(std::make_pair("FireEffect", FireEffect));
        return stringsEnumMap;
    }
};

#endif // SHELLEFFECTTYPE_H
