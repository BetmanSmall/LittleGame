//#include "tileset.h"

//#include "map.h"

//TileSet::TileSet(QDomElement tileSetElement, QString mapPath) {
//    properties = new QHash<QString, QString>;
//    QDomNamedNodeMap attributes = tileSetElement.attributes();
//    for(int k = 0; k < attributes.length(); k++) {
//        QDomNode node = attributes.item(k);
//        if(node.isAttr()) {
//            QDomAttr attr = node.toAttr();
//            properties->insert(attr.name(), attr.value());
//        }
//    }

//    QDomNode propertiesNode = tileSetElement.namedItem("properties");
//    if(!propertiesNode .isNull()) {
//        QDomElement propertyNode = propertiesNode .firstChildElement();
//        while (!propertyNode.isNull()) {
//            QString tagName = propertyNode.tagName();
//            if(tagName == "property") {
//                QString name = propertyNode.attribute("name");
//                QString value = propertyNode.attribute("value");
//                if(name != NULL && value != NULL) {
//                    properties->insert(name, value);
//                }
//            }
//            propertyNode = propertyNode.nextSiblingElement();
//        }
//    }

//    QDomNode imageNode = tileSetElement.namedItem("image");
//    QString imageSource = imageNode.toElement().attribute("source");
//    imageSource = findFile(imageSource, mapPath);

//    if(!image.load(imageSource)) {
//        qDebug() << "Failed to load tile sheet.";
//        return;
//    }

//    int margin = properties->value("margin", "0").toInt();
//    int spacing = properties->value("spacing", "0").toInt();
//    int tileWidth = properties->value("tilewidth", "64").toInt();
//    int tileHeight = properties->value("tileheight", "32").toInt();

//    int columns = image.width() / tileWidth;
//    int rows = image.height() / tileHeight;
//    for(int y = 0; y < rows; y++) {
//        for(int x = 0; x < columns; x++) {
//            Tile tile;
//            QRect rect(margin + (spacing * x) + (x * tileWidth), margin + (spacing * y) + (y * tileHeight), tileWidth, tileHeight);
//            tile.pixmap = image.copy(rect);
//            tiles.append(tile);
//        }
//    }

//    QDomNodeList tilesNodeList = tileSetElement.elementsByTagName("tile");
//    for(int k = 0; k < tilesNodeList.length(); k++) {
//        QDomNode tileNode = tilesNodeList.item(k);
//        QDomNode propertiesNode = tileNode.namedItem("properties");
//        if(!propertiesNode.isNull()) {
//            QString id = tileNode.toElement().attribute("id");
//            Tile tile = tiles.at(id.toInt());
//            QDomNodeList propertysNodeList = propertiesNode.toElement().elementsByTagName("property");
//            for(int l = 0; l < propertysNodeList.length(); l++) {
//                QDomNode propertyNode = propertysNodeList.item(l);
//                QString name = propertyNode.toElement().attribute("name");
//                QString value = propertyNode.toElement().attribute("value");
//                tile.properties.insert(name, value);
//            }
//        }
//    }
//}

//QString *TileSet::toString() {
//    QString* str = new QString();
//    str->append("[TileSet:");
//    str->append(properties->value("name"));
//    str->append(properties->value("fractionName"));
//    str->append("]");
//    return str;
//}

//QString TileSet::findFile(QString filePath, QString mapPath) {
//    qDebug() << "TileSet::filePath(" << filePath << ", " << mapPath << ");";
//    QString result = mapPath.left(mapPath.lastIndexOf("/"));
//    QString currDir = filePath.left(filePath.indexOf("/"));
//    filePath = filePath.mid(filePath.indexOf("/")+1);
//    while (currDir != NULL) {
//        if(currDir == "..") {

//        } else {
//            result.append("/" + currDir);
//            currDir = filePath.left(filePath.indexOf("/"));
//            filePath = filePath.right(filePath.indexOf("/")+1);
//        }
//    }
//    return result;
//}
