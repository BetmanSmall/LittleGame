#include "simpletemplate.h"

SimpleTemplate::SimpleTemplate(QString templateFile) {
    loadBasicTemplate(templateFile);
}

QString SimpleTemplate::toString() {
    return toString(false);
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
