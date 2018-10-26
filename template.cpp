#include "template.h"

//Template::Template() {
//    qDebug() << "Template::Template(); -- ";
//}

//Template::~Template() {
//    qDebug() << "Template::~Template(); -- ";
//}

void Template::loadBasicTemplate(QString templateFile) {
    qDebug() << "Template::loadBasicTemplate(); -- templateFile:" << templateFile;
    QDomDocument* domDocument = MapLoader::loadDomDocument(templateFile);
    QDomElement templateORtileset = domDocument->documentElement();
    this->templateName = templateORtileset.attribute("name", NULL);
//    qDebug() << "Template::loadBasicTemplate(); -- templateName:" << templateName;

    int firstgid = templateORtileset.attribute("firstgid", "0").toInt();
    int tilewidth = templateORtileset.attribute("tilewidth", "0").toInt();
    int tileheight = templateORtileset.attribute("tileheight", "0").toInt();
    int spacing = templateORtileset.attribute("spacing", "0").toInt();
    int margin = templateORtileset.attribute("margin", "0").toInt();

    MapLoader::loadProperties(getProperties(), templateORtileset.firstChildElement("properties"));
//    qDebug() << "Template::loadBasicTemplate(); --" << toString(true).toStdString().c_str();

    QDomElement imageElement = templateORtileset.firstChildElement("image");
    QString source = imageElement.attribute("source", NULL);
    QString textureFile = MapLoader::findFile(templateFile, source);
    qDebug() << "Template::loadBasicTemplate(); -- textureFile:" << textureFile;
    QPixmap texture(textureFile);
    qDebug() << "Template::loadBasicTemplate(); -- texture:" << texture;

    int stopWidth = texture.width() - tilewidth;
    int stopHeight = texture.height() - tileheight;
    int id = firstgid;
    for (int y = margin; y <= stopHeight; y += tileheight + spacing) {
        for (int x = margin; x <= stopWidth; x += tilewidth + spacing) {
            QPixmap tileRegion = texture.copy(x, y, tilewidth, tileheight);
            StaticTile* tile = new StaticTile(tileRegion);
            tile->setId(id);
            tiles.insert(id++, tile);
        }
    }
//    qDebug() << "Template::loadBasicTemplate(); -- tiles:" << tiles;
//    qDebug() << "Template::loadBasicTemplate(); --" << toString(true).toStdString().c_str();

    QDomNodeList tileElements = templateORtileset.elementsByTagName("tile");
    for (int t = 0; t < tileElements.size(); t++) {
        QDomElement tileElement = tileElements.item(t).toElement();
        int localtid = tileElement.attribute("id", "0").toInt();
        Tile* tile = tiles.value(localtid);
        if (tile != NULL) {
            MapLoader::loadProperties(tile->getProperties(), tileElement.firstChildElement("properties"));
//            qDebug() << "TemplateForUnit::TemplateForUnit(); -- localtid:" << localtid;
//            qDebug() << "TemplateForUnit::TemplateForUnit(); -- tile->getProperties():" << *(tile->getProperties());

            QDomElement animationElement = tileElement.firstChildElement("animation");
            if (!animationElement.isNull()) {
                QVector<StaticTile*> staticTiles;
                QVector<int> intervals;
                QDomNodeList framesNodeList = animationElement.elementsByTagName("frame");
                for (int f = 0; f < framesNodeList.length(); f++) {
                    QDomElement frameElement = framesNodeList.item(f).toElement();
//                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- frameElement.attribute(tileid):" << frameElement.attribute("tileid");
//                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- tiles[frameElement.attribute(tileid).toInt():" << tiles[frameElement.attribute("tileid").toInt()]->getId();
                    staticTiles.append( (StaticTile*)(tiles.value(frameElement.attribute("tileid").toInt()/*+1*/)) );
                    intervals.append(frameElement.attribute("duration").toInt());
                }
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- staticTiles.length():" << staticTiles.length();

                AnimatedTile* animatedTile = new AnimatedTile(intervals, staticTiles);
                animatedTile->setId(tile->getId());
                animatedTile->addProperties(*(tile->getProperties()));
                animatedTiles.insert(animatedTile->getId(), animatedTile);
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- localtid:" << localtid;
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old1 tile->getId():" << tile->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old1 tile:" << tile;
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old1 tiles[localtid]->getId():" << tiles[localtid]->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old1 tiles[localtid]:" << tiles[localtid];
//                delete tile;
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old2 tile->getId():" << tile->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old2 tile:" << tile;
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old2 tiles[localtid]->getId():" << tiles[localtid]->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old2 tiles[localtid]:" << tiles[localtid];
//                delete tiles[localtid];
//                tile = animatedTile;
//                tiles.insert(animatedTile->getId(), animatedTile); // not good mb?
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old3 tile->getId():" << tile->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old3 tile:" << tile;
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old3 tiles[localtid]->getId():" << tiles[localtid]->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- old3 tiles[localtid]:" << tiles[localtid];
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- new animatedTile->getId():" << animatedTile->getId();
//                qDebug() << "TemplateForUnit::TemplateForUnit(); -- new animatedTile:" << animatedTile;
            }
        }
    }
    qDebug() << "Template::loadBasicTemplate(); -- tiles:" << tiles;
    qDebug() << "Template::loadBasicTemplate(); -- animatedTiles:" << animatedTiles;
}

void Template::basicValidate() {
    if (properties.contains("templateName")) {
        qDebug() << "Template::basicValidate(); -- templateName:" << templateName;
    }
    if (!properties.contains("factionName")) {
        qDebug() << "Template::basicValidate(); -- Can't get 'factionName'! Check the file";
    } else {
        factionName = properties.value("factionName");
    }
    if (!properties.contains("name")) {
        qDebug() << "Template::basicValidate(); -- Can't get 'name'! Check the file";
    } else {
        name = properties.value("name");
    }
    if (!properties.contains("type")) {
        qDebug() << "TemplateForUnit::validate(); -- Can't get 'type'! Check the file";
    } else {
        type = properties.value("type");
    }
}

QMap<QString, QString>* Template::getProperties() {
    return &properties;
}

QString Template::toStringProperties() {
    QString sb("Properties:[");
    foreach (QString key, properties.keys()) {
        sb.append(key + ":" + properties.value(key) + ",");
    }
    sb = sb.left(sb.length()-1);
    sb.append("]");
    return sb;
}

QString Template::toStringBasicParam() {
    QString sb("Template:[");
    sb += "templateName:" + templateName;
//    sb += ",faction:" + faction;
    sb += ",factionName:" + factionName;
    sb += ",name:" + name;
    sb += ",type:" + type;
    sb += "]";
    return sb;
}
