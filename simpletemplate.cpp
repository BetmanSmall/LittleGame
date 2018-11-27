#include "simpletemplate.h"

SimpleTemplate::SimpleTemplate(QString templateFile) {
    loadBasicTemplate(templateFile);
    basicValidate();
}

QString SimpleTemplate::toString() {
    return toString(true);
}

QString SimpleTemplate::toString(bool full) {
    QString sb("SimpleTemplate[");
    if(full) {
        sb.append(toStringProperties());
        sb.append(toStringBasicParam());
    }
    sb.append("]");
    return sb;
}
