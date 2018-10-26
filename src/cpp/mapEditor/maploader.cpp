#include "src/head/mapEditor/maploader.h"

MapLoader::MapLoader() {

}

Map* MapLoader::load(QString mapPath) {
    QDomDocument *domDocument = loadDomDocument(mapPath);

    QDomElement mapElement = domDocument->documentElement();
    QMap<QString, QPixmap> textures;
    QVector<QString> textureFiles = loadTilesets(mapElement, mapPath);
    qDebug() << textureFiles;
    textureFiles += loadImages(mapElement, mapPath);
    qDebug() << textureFiles;

    foreach (QString pixFilePath, textureFiles) {
        QPixmap pix(pixFilePath);
        if(!pix.isNull()) {
            textures.insert(pixFilePath, pix);
        } else {
            qDebug() << "MapLoader::load() -- Can't load QPixmap: " << pixFilePath;
            return NULL;
        }
    }

    Map *map = loadMap(mapElement, mapPath, textures);
    return map;
}

QDomDocument* MapLoader::loadDomDocument(QString xmlFile) {
    QString xmlName = xmlFile.mid(xmlFile.lastIndexOf("/")+1);
//    qDebug() << "MapLoader::loadDomDocument() -- Try load xmlName:" << xmlName;
    QDomDocument *domDocument = new QDomDocument(xmlName);
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "MapLoader::loadDomDocument() -- Can't load xmlFile:" << xmlFile;
        return NULL;
    }
    if (!domDocument->setContent(&file)) {
        qDebug() << "MapLoader::loadDomDocument() -- Could not convert to xml format:" << xmlFile;
        file.close();
        return NULL;
    }
    file.close();
//    qDebug() << "MapLoader::loadDomDocument() -- Load xmlFile: " << xmlFile;
    return domDocument;
}

QVector<QString> MapLoader::loadTilesets(QDomElement mapElement, QString mapPath) {
    QVector<QString> images;
    QDomNodeList tileSetsNodeList = mapElement.elementsByTagName("tileset");
    for(int k = 0; k < tileSetsNodeList.length(); k++) {
        QDomNode tileSetNode = tileSetsNodeList.item(k);
        QString source = tileSetNode.toElement().attribute("source");
        if(source != NULL) {
            QString tsxFile = findFile(mapPath, source);
            tileSetNode = loadDomDocument(tsxFile)->documentElement();
            QDomElement imageElement = tileSetNode.firstChildElement("image");
            if(!imageElement.isNull()) {
                QString imageSource = imageElement.attribute("source");
                QString imageFile = findFile(tsxFile, imageSource);
                images.append(imageFile);
            } else {
                QDomNodeList tilesNodeList = tileSetNode.toElement().elementsByTagName("tile");
                for(int l = 0; l < tilesNodeList.length(); l++) {
                    QDomNode tileNode = tilesNodeList.item(l);
                    QString imageSource = tileNode.firstChildElement("image").attribute("source");
                    QString imageFile = findFile(tsxFile, imageSource);
                    images.append(imageFile);
                }
            }
        } else {
            QDomElement imageElement = tileSetNode.firstChildElement("image");
            if(!imageElement.isNull()) {
                QString imageSource = imageElement.attribute("source");
                QString imageFile = findFile(mapPath, imageSource);
                images.append(imageFile);
            } else {
                QDomNodeList tilesNodeList = tileSetNode.toElement().elementsByTagName("tile");
                for(int l = 0; l < tilesNodeList.length(); l++) {
                    QDomNode tileNode = tilesNodeList.item(l);
                    QString imageSource = tileNode.firstChildElement("image").attribute("source");
                    QString imageFile = findFile(mapPath, imageSource);
                    images.append(imageFile);
                }
            }
        }
    }
    return images;
}

QVector<QString> MapLoader::loadImages(QDomElement mapElement, QString mapPath) {
    QVector<QString> images;
    QDomNodeList imageLaysersNodeList = mapElement.elementsByTagName("imagelayer");
    for(int k = 0; k < imageLaysersNodeList.length(); k++) {
        QDomNode imageLayerNode = imageLaysersNodeList.item(k);
        QDomElement imageElement = imageLayerNode.firstChildElement("image");
        QString source = imageElement.attribute("source", NULL);
        if(source != NULL) {
            QString imageFile = findFile(mapPath, source);
            if(!images.contains(imageFile)) {
                images.append(imageFile);
            }
        }
    }
    return images;
}

Map *MapLoader::loadMap(QDomElement mapElement, QString mapPath, QMap<QString, QPixmap> textures) {
    Map *map = new Map();

    QString mapOrientation = mapElement.attribute("orientation", NULL);
    int mapWidth = mapElement.attribute("width", "0").toInt();
    int mapHeight = mapElement.attribute("height", "0").toInt();
    int tileWidth = mapElement.attribute("tilewidth", "0").toInt();
    int tileHeight = mapElement.attribute("tileheight", "0").toInt();
    int hexSideLength = mapElement.attribute("hexsidelength", "0").toInt();
    QString staggerAxis = mapElement.attribute("staggeraxis", NULL);
    QString staggerIndex = mapElement.attribute("staggerindex", NULL);
    QString mapBackgroundColor = mapElement.attribute("backgroundcolor", NULL);

    QMap<QString, QString> *mapProperties = map->getProperties();
    if(mapOrientation != NULL) {
        mapProperties->insert("orientation", mapOrientation);
    }
    mapProperties->insert("width", QString::number(mapWidth));
    mapProperties->insert("height", QString::number(mapHeight));
    mapProperties->insert("tilewidth", QString::number(tileWidth));
    mapProperties->insert("tileheight", QString::number(tileHeight));
    mapProperties->insert("hexsidelength", QString::number(hexSideLength));
    if (staggerAxis != NULL) {
        mapProperties->insert("staggeraxis", staggerAxis);
    }
    if (staggerIndex != NULL) {
        mapProperties->insert("staggerindex", staggerIndex);
    }
    if (mapBackgroundColor != NULL) {
        mapProperties->insert("backgroundcolor", mapBackgroundColor);
    }

    mapTileWidth = tileWidth;
    mapTileHeight = tileHeight;
    mapWidthInPixels = mapWidth * tileWidth;
    mapHeightInPixels = mapHeight * tileHeight;

    if (mapOrientation != NULL) {
        if (mapOrientation == "staggered") {
            if (mapHeight > 1) {
                mapWidthInPixels += tileWidth / 2;
                mapHeightInPixels = mapHeightInPixels / 2 + tileHeight / 2;
            }
        }
    }

    QDomElement propertiesElement = mapElement.firstChildElement("properties");
    if (!propertiesElement.isNull()) {
        loadProperties(map->getProperties(), propertiesElement);
    }
    qDebug() << *(map->getProperties());

    QDomNodeList tileSetsNodeList = mapElement.elementsByTagName("tileset");
    for(int k = 0; k < tileSetsNodeList.length(); k++) {
        QDomNode tileSetNode = tileSetsNodeList.item(k);
        loadTileSet(map, tileSetNode.toElement(), mapPath, textures);
    }
    QDomNode node = mapElement.firstChild();
    while(!node.isNull()) {
        QString name = node.nodeName();
        if(name == "layer") {
            loadTileLayer(map, node.toElement());
//        } else if (name == "objectgroup") {
//            loadObjectGroup(map, node.toElement());
//        } else if (name == "imagelayer") {
//            loadImageLayer(map, node.toElement(), tmxFile, imageResolver);
        }
        node = node.nextSibling();
    }
    map->width = mapWidth;
    map->height = mapHeight;
    return map;
}

void MapLoader::loadProperties(QMap<QString, QString> *properties, QDomElement propertiesElement) {
    if (propertiesElement.isNull()) {
        return;
    }
    if (propertiesElement.nodeName() == "properties") {
        QDomNodeList propertyNodeList = propertiesElement.elementsByTagName("property");
        for (int p = 0; p < propertyNodeList.length(); p++) {
            QDomElement propertyElement = propertyNodeList.item(p).toElement();
            QString name = propertyElement.attribute("name", "NULL");
            QString value = propertyElement.attribute("value", NULL);
            if (value == NULL) {
                value = propertyElement.nodeValue();
            }
            properties->insert(name, value);
        }
    }
}

void MapLoader::loadTileSet(Map *map, QDomElement tileSetElement, QString mapPath, QMap<QString, QPixmap> textures) {
    if(tileSetElement.nodeName() == "tileset") {
        QString name = tileSetElement.attribute("name", NULL);
        int firstgid = tileSetElement.attribute("firstgid", "1").toInt();
        int tilewidth = tileSetElement.attribute("tilewidth", "0").toInt();
        int tileheight = tileSetElement.attribute("tileheight", "0").toInt();
        int spacing = tileSetElement.attribute("spacing", "0").toInt();
        int margin = tileSetElement.attribute("margin", "0").toInt();
        int columns = tileSetElement.attribute("columns", "0").toInt();
        QString source = tileSetElement.attribute("source", NULL);

        int offsetX = 0;
        int offsetY = 0;

        QString imageSource = "";
        int imageWidth = 0, imageHeight = 0;

        QString imagePath = "";
        if (source != NULL) {
            QString tsx = findFile(mapPath, source);
//            try {
                tileSetElement = loadDomDocument(tsx)->documentElement();
                name = tileSetElement.attribute("name", name);
                tilewidth = tileSetElement.attribute("tilewidth", "0").toInt();
                tileheight = tileSetElement.attribute("tileheight", "0").toInt();
                spacing = tileSetElement.attribute("spacing", "0").toInt();
                margin = tileSetElement.attribute("margin", "0").toInt();
                columns = tileSetElement.attribute("columns", "0").toInt();

                QDomElement offsetElement = tileSetElement.firstChildElement("tileoffset");
                if (!offsetElement.isNull()) {
                    offsetX = offsetElement.attribute("x", "0").toInt();
                    offsetY = offsetElement.attribute("y", "0").toInt();
                }
                QDomElement imageElement = tileSetElement.firstChildElement("image");
                if (!imageElement.isNull()) {
                    imageSource = imageElement.attribute("source");
                    imageWidth = imageElement.attribute("width", "0").toInt();
                    imageHeight = imageElement.attribute("height", "0").toInt();
                    imagePath = findFile(tsx, imageSource);
                }
//            } catch (IOException e) {
//                throw new GdxRuntimeException("Error parsing external tileset.");
//            }
        } else {
            QDomElement offsetElement = tileSetElement.firstChildElement("tileoffset");
            if (!offsetElement.isNull()) {
                offsetX = offsetElement.attribute("x", "0").toInt();
                offsetY = offsetElement.attribute("y", "0").toInt();
            }
            QDomElement imageElement = tileSetElement.firstChildElement("image");
            if (!imageElement.isNull()) {
                imageSource = imageElement.attribute("source");
                imageWidth = imageElement.attribute("width", "0").toInt();
                imageHeight = imageElement.attribute("height", "0").toInt();
                imagePath = findFile(mapPath, imageSource);
            }
        }

        TileSet tileset(name);
//        tileset.setName(name);
        tileset.getProperties()->insert("firstgid", QString::number(firstgid));
        tileset.firshgid = firstgid;
        if (imagePath != "") {
            QPixmap texture = textures.value(imagePath);

            QMap<QString, QString> *props = tileset.getProperties();
            props->insert("imagesource", imageSource);
            props->insert("imagewidth", QString::number(imageWidth));
            props->insert("imageheight", QString::number(imageHeight));
            props->insert("tilewidth", QString::number(tilewidth));
            props->insert("tileheight", QString::number(tileheight));
            props->insert("margin", QString::number(margin));
            props->insert("spacing", QString::number(spacing));
            props->insert("columns", QString::number(columns));

            int stopWidth = texture.width() - tilewidth;
            int stopHeight = texture.height() - tileheight;

            int id = 1;

            for (int y = margin; y <= stopHeight; y += tileheight + spacing) {
                for (int x = margin; x <= stopWidth; x += tilewidth + spacing) {
                    QPixmap tilePix = texture.copy(x, y, tilewidth, tileheight);
                    StaticTile *tile = new StaticTile(tilePix);
                    tile->setId(id);
                    tile->setOffsetX(flipX ? -offsetX : offsetX);
                    tile->setOffsetY(flipY ? -offsetY : offsetY);
                    tileset.putTile(id++, tile);
                }
            }
        } else {
            QDomNodeList tilesNodeList = tileSetElement.elementsByTagName("tile");
            for(int k = 0; k < tilesNodeList.length(); k++) {
                QDomNode tileNode = tilesNodeList.item(k);
                QDomElement imageElement = tileNode.firstChildElement("image");
                if (!imageElement.isNull()) {
                    imageSource = imageElement.attribute("source");
                    imageWidth = imageElement.attribute("width", "0").toInt();
                    imageHeight = imageElement.attribute("height", "0").toInt();
                    imagePath = findFile(mapPath, imageSource);
                }
                QPixmap texture = textures.value(imagePath);
                StaticTile *tile = new StaticTile(texture);
                tile->setId(firstgid + tileNode.toElement().attribute("id").toInt());
                tile->setOffsetX(flipX ? -offsetX : offsetX);
                tile->setOffsetY(flipY ? -offsetY : offsetY);
                tileset.putTile(tile->getId(), tile);
            }
        }

        QVector<AnimatedTile*> animatedTiles;
        QDomNodeList tilesNodeList = tileSetElement.elementsByTagName("tile");
        for(int k = 0; k < tilesNodeList.length(); k++) {
            QDomElement tileElement = tilesNodeList.item(k).toElement();
            int localtid = tileElement.attribute("id", "0").toInt();
            Tile* tile = tileset.getLocalTile(localtid+1);
            if (tile != NULL) {
                QDomElement animationElement = tileElement.firstChildElement("animation");
                if (!animationElement.isNull()) {
                    QVector<StaticTile*> staticTiles;
                    QVector<int> intervals;
                    QDomNodeList framesNodeList = animationElement.elementsByTagName("frame");
                    for (int f = 0; f < framesNodeList.length(); f++) {
                        QDomElement frameElement = framesNodeList.item(f).toElement();
                        staticTiles.append((StaticTile*) tileset.getLocalTile(frameElement.attribute("tileid").toInt()+1));
                        intervals.append(frameElement.attribute("duration").toInt());
                    }

                    AnimatedTile *animatedTile = new AnimatedTile(intervals, staticTiles);
                    animatedTile->setId(tile->getId());
                    animatedTiles.append(animatedTile);
                    tile = animatedTile;
                }

                QString terrain = tileElement.attribute("terrain", "");
                if (terrain != "") {
                    tile->getProperties()->insert("terrain", terrain);
                }
                QString probability = tileElement.attribute("probability", "");
                if (probability != "") {
                    tile->getProperties()->insert("probability", probability);
                }
                QDomElement properties = tileElement.firstChildElement("properties");
                if (!properties.isNull()) {
                    loadProperties(tile->getProperties(), properties);
                }
            }
        }

        foreach (AnimatedTile *tile, animatedTiles) {
            tileset.putTile(tile->getId(), tile);
        }

        QDomElement properties = tileSetElement.firstChildElement("properties");
        if (!properties.isNull()) {
            loadProperties(tileset.getProperties(), properties);
        }
        map->getTileSets()->addTileSet(tileset);
    }
}

QString MapLoader::findFile(QString parentPath, QString filePath) {
    QString result = parentPath.left(parentPath.lastIndexOf("/"));
    bool finished = false;
    do {
        int slashIndex = filePath.indexOf("/");
        if(slashIndex == -1) {
            result = result.append("/" + filePath);
            finished = true;
        } else {
            QString token = filePath.left(slashIndex);
            filePath = filePath.mid(slashIndex+1);
            if(token == "..") {
                result = result.left(result.lastIndexOf("/"));
            } else {
                result = result.append("/" + token);
            }
        }
    } while (!finished);
    return result;
}

void MapLoader::loadTileLayer(Map *map, QDomElement element) {
    if (element.nodeName() == "layer") {
        int width = element.attribute("width", "0").toInt();
        int height = element.attribute("height", "0").toInt();
        int tileWidth = map->getProperties()->value("tilewidth").toInt();
        int tileHeight = map->getProperties()->value("tileheight").toInt();
        Layer *layer = new Layer(width, height, tileWidth, tileHeight);

        loadBasicLayerInfo(layer, element);

        int *ids = getTileIds(element, width, height);
        TileSets *tilesets = map->getTileSets();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int id = ids[y * width + x];
                bool flipHorizontally = ((id & FLAG_FLIP_HORIZONTALLY) != 0);
                bool flipVertically = ((id & FLAG_FLIP_VERTICALLY) != 0);
                bool flipDiagonally = ((id & FLAG_FLIP_DIAGONALLY) != 0);

                Tile *tile = tilesets->getTile(id & ~MASK_CLEAR);
                if (tile != NULL) {
                    TileLayerCell cell = createTileLayerCell(flipHorizontally, flipVertically, flipDiagonally);
                    cell.setTile(tile);
                    layer->setCell((flipX)?(width-1-x):(x), (flipY)?(height-1-y):(y), cell);
                }
            }
        }

        QDomElement properties = element.firstChildElement("properties");
        if (!properties.isNull()) {
            loadProperties(layer->getProperties(), properties);
        }
        map->getMapLayers()->add(layer);
    }
}

void MapLoader::loadBasicLayerInfo(Layer *layer, QDomElement element) {
    QString name = element.attribute("name", NULL);
    float opacity = element.attribute("opacity", "1.0").toFloat();
    bool visible = element.attribute("visible", "1") == "1";

    layer->setName(name);
    layer->setOpacity(opacity);
    layer->setVisible(visible);
}

int *MapLoader::getTileIds(QDomElement element, int width, int height) {
    QDomElement data = element.firstChildElement("data");
    QString encoding = data.attribute("encoding", NULL);
    if (encoding == NULL) { // no 'encoding' attribute means that the encoding is XML
        qWarning() << "Unsupported encoding (XML) for TMX Layer Data";
//        throw new GdxRuntimeException("Unsupported encoding (XML) for TMX Layer Data");
    }
    int *ids = new int[width * height];
    if (encoding == "csv") {
        QStringList array = data.text().split(",");
        for (int i = 0; i < array.length(); i++) {
            ids[i] = array[i].trimmed().toInt();
        }
    } else {
//        if (encoding == "base64") {
//            InputStream is = null;
//            try {
//                QString compression = data.attribute("compression", NULL);
//                byte[] bytes = Base64Coder.decode(data.getText());
//                if (compression == NULL)
//                    is = new ByteArrayInputStream(bytes);
//                else if (compression.equals("gzip"))
//                    is = new BufferedInputStream(new GZIPInputStream(new ByteArrayInputStream(bytes), bytes.length));
//                else if (compression.equals("zlib"))
//                    is = new BufferedInputStream(new InflaterInputStream(new ByteArrayInputStream(bytes)));
//                else
//                    throw new GdxRuntimeException("Unrecognised compression (" + compression + ") for TMX Layer Data");

//                byte[] temp = new byte[4];
//                for (int y = 0; y < height; y++) {
//                    for (int x = 0; x < width; x++) {
//                        int read = is.read(temp);
//                        while (read < temp.length) {
//                            int curr = is.read(temp, read, temp.length - read);
//                            if (curr == -1) break;
//                            read += curr;
//                        }
//                        if (read != temp.length)
//                            throw new GdxRuntimeException("Error Reading TMX Layer Data: Premature end of tile data");
//                        ids[y * width + x] = unsignedByteToInt(temp[0]) | unsignedByteToInt(temp[1]) << 8
//                            | unsignedByteToInt(temp[2]) << 16 | unsignedByteToInt(temp[3]) << 24;
//                    }
//                }
//            } catch (IOException e) {
//                throw new GdxRuntimeException("Error Reading TMX Layer Data - IOException: " + e.getMessage());
//            } finally {
//                StreamUtils.closeQuietly(is);
//            }
//        } else {
//            // any other value of 'encoding' is one we're not aware of, probably a feature of a future version of Tiled
//            // or another editor
//            throw new GdxRuntimeException("Unrecognised encoding (" + encoding + ") for TMX Layer Data");
//        }
    }
    return ids;
}

TileLayerCell MapLoader::createTileLayerCell(bool flipHorizontally, bool flipVertically, bool flipDiagonally) {
    TileLayerCell cell;
    if (flipDiagonally) {
        if (flipHorizontally && flipVertically) {
            cell.setFlipHorizontally(true);
            cell.setRotation(TileLayerCell::ROTATE_270);
        } else if (flipHorizontally) {
            cell.setRotation(TileLayerCell::ROTATE_270);
        } else if (flipVertically) {
            cell.setRotation(TileLayerCell::ROTATE_90);
        } else {
            cell.setFlipVertically(true);
            cell.setRotation(TileLayerCell::ROTATE_270);
        }
    } else {
        cell.setFlipHorizontally(flipHorizontally);
        cell.setFlipVertically(flipVertically);
    }
    return cell;
}
