#include "template.h"

Template::Template() {
//    qDebug() << "Template::Template(); -- ";
    templateName = "NULL";
//    properties = new ObjectMap<String, String>();
//    tiles = new ObjectMap<Integer, TiledMapTile>();
//    animatedTiles = new ObjectMap<Integer, AnimatedTiledMapTile>();
}

Template::~Template() {
    qDebug() << "Template::~Template(); -- ";
}

void Template::loadBasicTemplate(QString templateFile) {
    qDebug() << "Template::loadBasicTemplate(); -- templateFile:" << templateFile;
    QDomDocument* domDocument = MapLoader::loadDomDocument(templateFile);
    QDomElement templateORtileset = domDocument->documentElement();
    this->templateName = templateORtileset.attribute("name", NULL);

    int firstgid = templateORtileset.attribute("firstgid", "0").toInt();
    int tilewidth = templateORtileset.attribute("tilewidth", "0").toInt();
    int tileheight = templateORtileset.attribute("tileheight", "0").toInt();
    int spacing = templateORtileset.attribute("spacing", "0").toInt();
    int margin = templateORtileset.attribute("margin", "0").toInt();

    MapLoader::loadProperties(getProperties(), templateORtileset.firstChildElement("properties"));
    QDomElement imageElement = templateORtileset.firstChildElement("image");
    QString source = imageElement.attribute("source", NULL);
    QString textureFile = MapLoader::findFile(templateFile, source);
    QPixmap texture(textureFile);
    qDebug() << "Template::loadBasicTemplate(); -- textureFile:" << textureFile;
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
    QDomNodeList tileElements = templateORtileset.elementsByTagName("tile");
    for (int t = 0; t < tileElements.size(); t++) {
        QDomElement tileElement = tileElements.item(t).toElement();
        int localtid = tileElement.attribute("id", "0").toInt();
        Tile* tile = tiles.value(localtid);
        if (tile != NULL) {
            MapLoader::loadProperties(tile->getProperties(), tileElement.firstChildElement("properties"));
            QDomElement animationElement = tileElement.firstChildElement("animation");
            if (!animationElement.isNull()) {
                QVector<StaticTile*> staticTiles;
                QVector<int> intervals;
                QDomNodeList framesNodeList = animationElement.elementsByTagName("frame");
                for (int f = 0; f < framesNodeList.length(); f++) {
                    QDomElement frameElement = framesNodeList.item(f).toElement();
                    staticTiles.append( (StaticTile*)(tiles.value(frameElement.attribute("tileid").toInt()/*+1*/)) );
                    intervals.append(frameElement.attribute("duration").toInt());
                }
                AnimatedTile* animatedTile = new AnimatedTile(intervals, staticTiles);
                animatedTile->setId(tile->getId());
                animatedTile->addProperties(*(tile->getProperties()));
                animatedTiles.insert(animatedTile->getId(), animatedTile);
//                delete tile;
//                delete tiles[localtid];
//                tile = animatedTile;
//                tiles.insert(animatedTile->getId(), animatedTile); // not good mb?
            }
        }
    }
}

void Template::basicValidate() {
    if (properties.contains("templateName") || templateName.size()) {
        qDebug() << "Template::basicValidate(); -- templateName:" << templateName;
        qDebug() << "Template::basicValidate(); -- tiles:" << tiles.size();
        qDebug() << "Template::basicValidate(); -- animatedTiles:" << animatedTiles.size();
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
    sb += ",tiles.size():" + QString::number(tiles.size());
    sb += ",animatedTiles.size():" + QString::number(animatedTiles.size());
    sb += "]";
    return sb;
}
