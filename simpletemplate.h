#ifndef SIMPLETEMPLATE_H
#define SIMPLETEMPLATE_H

#include "template.h"

class SimpleTemplate : public Template
{
public:
    SimpleTemplate(QString templateFile);

    QString toString();
    QString toString(bool full);
};

#endif // SIMPLETEMPLATE_H
