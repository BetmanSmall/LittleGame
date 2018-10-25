#include "src/head/templateforunit.h"

TemplateForUnit::TemplateForUnit(QString templateFile) {
    qDebug() << "TemplateForUnit::TemplateForUnit(); -- templateFile:" << templateFile;
    QDomDocument* domDocument = MapLoader::loadDomDocument(templateFile);
    QDomElement templateORtileset = domDocument->documentElement();
    templateName = templateORtileset.attribute("name", NULL);
    qDebug() << "TemplateForUnit::TemplateForUnit(); -- templateName:" << templateName;
//    try {

        int firstgid = templateORtileset.attribute("firstgid", "0").toInt();
        int tilewidth = templateORtileset.attribute("tilewidth", "0").toInt();
        int tileheight = templateORtileset.attribute("tileheight", "0").toInt();
        int spacing = templateORtileset.attribute("spacing", "0").toInt();
        int margin = templateORtileset.attribute("margin", "0").toInt();

        QDomElement properties = templateORtileset.firstChildElement("properties");
        if (!properties.isNull()) {
            QDomNodeList propertiesNodeList = properties.elementsByTagName("property");
            for (int k = 0; k < propertiesNodeList.length(); k++) {
                QDomNode propertyNode = propertiesNodeList.item(k);
                QString key = propertyNode.toElement().attribute("name", NULL);
                QString value = propertyNode.toElement().attribute("value", NULL);
                if (!key.isNull() && !value.isNull()) {
//                if (key != NULL && value != NULL) {
                    if (key == "factionName") {
                        this->factionName = value;
                    } else if (key == "healthPoints") {
                        this->healthPoints = value.toInt();
                    } else if (key == "name") {
                        this->name = value;
                    } else if (key == "speed") {
                        this->speed = value.toFloat();
//                    } else if (key.equals("type")) {
//                        this->type = value;
                    }
                }
            }
        }
        QDomElement imageElement = templateORtileset.firstChildElement("image");
        QString source = imageElement.attribute("source", NULL);
        QString textureFile = MapLoader::findFile(templateFile, source);

        qDebug() << "TemplateForUnit::TemplateForUnit(); -- source:" << source;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- textureFile:" << textureFile;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- " << toString(true);
//        QPixmap* texture = new QPixmap(textureFile);
        QPixmap texture(textureFile);
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- texture:" << texture;

        int stopWidth = texture.width() - tilewidth;
        int stopHeight = texture.height() - tileheight;
        int id = firstgid;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- stopWidth:" << stopWidth;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- stopHeight:" << stopHeight;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- id:" << id;
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- ------";

        QMap<int, Tile*> tiles;
        for (int y = margin; y <= stopHeight; y += tileheight + spacing) {
            for (int x = margin; x <= stopWidth; x += tilewidth + spacing) {
                QPixmap tileRegion = texture.copy(x, y, tilewidth, tileheight);
                StaticTile* tile = new StaticTile(tileRegion);
                tile->setId(id);
//                tile->setOffsetX(flipX ? -offsetX : offsetX);
//                tile->setOffsetY(flipY ? -offsetY : offsetY);
                tiles.insert(id++, tile);
            }
        }
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- tiles:" << tiles;
//        exit(0);

        QDomNodeList tileElements = templateORtileset.elementsByTagName("tile");
//        animations = new ObjectMap<String, AnimatedTiledMapTile>();
        QVector<AnimatedTile*> animatedTiles;
        for (int t = 0; t < tileElements.size(); t++) {
            QDomElement tileElement = tileElements.item(t).toElement();
            int localtid = tileElement.attribute("id", "0").toInt();
            Tile* tile = tiles[localtid];
            if (tile != NULL) {
                qDebug() << "TemplateForUnit::TemplateForUnit(); -- localtid:" << localtid;
                QDomElement propertiesElement = tileElement.firstChildElement("properties");
                if (!propertiesElement.isNull()) {
                    QDomNodeList propertyNodeList = propertiesElement.elementsByTagName("property");
//                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- propertyNodeList.length():" << propertyNodeList.length();
                    for (int p = 0; p < propertyNodeList.length(); p++) {
                        QDomElement propertyElement = propertyNodeList.item(p).toElement();
                        QString name = propertyElement.attribute("name", NULL);
                        QString value = propertyElement.attribute("value", NULL);
                        if (value == NULL) {
                            value = propertyElement.text();
                        }
                        tile->getProperties()->insert(name, value);
//                        qDebug() << "TemplateForUnit::TemplateForUnit(); -- name:" << name;
//                        qDebug() << "TemplateForUnit::TemplateForUnit(); -- value:" << value;
                    }
                }
                qDebug() << "TemplateForUnit::TemplateForUnit(); -- tile->getProperties():" << *(tile->getProperties());

                QDomElement animationElement = tileElement.firstChildElement("animation");
                if (!animationElement.isNull()) {
                    QVector<StaticTile*> staticTiles;
                    QVector<int> intervals;
                    QDomNodeList framesNodeList = animationElement.elementsByTagName("frame");
                    for (int f = 0; f < framesNodeList.length(); f++) {
                        QDomElement frameElement = framesNodeList.item(f).toElement();
                        qDebug() << "TemplateForUnit::TemplateForUnit(); -- frameElement.attribute(tileid):" << frameElement.attribute("tileid");
                        qDebug() << "TemplateForUnit::TemplateForUnit(); -- tiles[frameElement.attribute(tileid).toInt():" << tiles[frameElement.attribute("tileid").toInt()]->getId();
                        staticTiles.append((StaticTile*) tiles[frameElement.attribute("tileid").toInt()/*+1*/]);
                        intervals.append(frameElement.attribute("duration").toInt());
                    }
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- staticTiles.length():" << staticTiles.length();

                    AnimatedTile* animatedTile = new AnimatedTile(intervals, staticTiles);
                    animatedTile->setId(tile->getId());
                    animatedTile->addProperties(*(tile->getProperties()));
                    animatedTiles.append(animatedTile);
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- old tile:" << tile;
                    tile = animatedTile;

                    QString actionAndDirection = tile->getProperties()->take("actionAndDirection");
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- actionAndDirection:" << actionAndDirection;
                    if(actionAndDirection != NULL) {
                        animations.insert(actionAndDirection, animatedTile);
//                        qDebug() << "TemplateForUnit::TemplateForUnit(); -- walk_Direction::UP_RIGHT:" << ("walk_" + Direction::toString(Direction::UP_RIGHT));
                        if(actionAndDirection == ("walk_" + Direction::toString(Direction::UP_RIGHT)) ) {
                            animations.insert("walk_" + Direction::toString(Direction::UP_LEFT), flipAnimatedTiledMapTile(animatedTile));
                        } else if(actionAndDirection == ("walk_" + Direction::toString(Direction::RIGHT)) ) {
                            animations.insert("walk_" + Direction::toString(Direction::LEFT), flipAnimatedTiledMapTile(animatedTile));
                        } else if(actionAndDirection == ("walk_" + Direction::toString(Direction::DOWN_RIGHT)) ) {
                            animations.insert("walk_" + Direction::toString(Direction::DOWN_LEFT), flipAnimatedTiledMapTile(animatedTile));
                        }
                        if(actionAndDirection == ("death_" + Direction::toString(Direction::UP_RIGHT)) ) {
                            animations.insert("death_" + Direction::toString(Direction::UP_LEFT), flipAnimatedTiledMapTile(animatedTile));
                        } else if(actionAndDirection == ("death_" + Direction::toString(Direction::RIGHT)) ) {
                            animations.insert("death_" + Direction::toString(Direction::LEFT), flipAnimatedTiledMapTile(animatedTile));
                        } else if(actionAndDirection == ("death_" + Direction::toString(Direction::DOWN_RIGHT)) ) {
                            animations.insert("death_" + Direction::toString(Direction::DOWN_LEFT), flipAnimatedTiledMapTile(animatedTile));
                        }
                    }
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- tile:" << tile;
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- animatedTile:" << animatedTile;
                    qDebug() << "TemplateForUnit::TemplateForUnit(); -- animations.size:" << animations.size();
                }
            }
        }
//        validate();
        qDebug() << "TemplateForUnit::TemplateForUnit(); -- animations.size:" << animations.size();
        exit(0);
//    } catch (Exception exp) {
//        Gdx.app.log("TemplateForUnit::TemplateForUnit()", "-- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//        throw new Exception("TemplateForUnit::TemplateForUnit() -- Could not load TemplateForUnit from " + templateFile.path() + " Exp:" + exp);
//    }
}

TemplateForUnit::~TemplateForUnit() {
    qDebug() << "TemplateForUnit::~TemplateForUnit(); -- ";
    clearVectors();
}

void TemplateForUnit::clearVectors() {
    idle.clear();

    walk_up.clear();
    walk_up_right.clear();
    walk_right.clear();
    walk_down_right.clear();
    walk_down.clear();
    walk_down_left.clear();
    walk_left.clear();
    walk_up_left.clear();

    attack_up.clear();
    attack_up_right.clear();
    attack_right.clear();
    attack_down_right.clear();
    attack_down.clear();
    attack_down_left.clear();
    attack_left.clear();
    attack_up_left.clear();

    death_up.clear();
    death_up_right.clear();
    death_right.clear();
    death_down_right.clear();
    death_down.clear();
    death_down_left.clear();
    death_left.clear();
    death_up_left.clear();
}

AnimatedTile* TemplateForUnit::flipAnimatedTiledMapTile(AnimatedTile* animatedTiledMapTile) {
    qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- animatedTiledMapTile:" << animatedTiledMapTile;
    QVector<StaticTile*> frames = QVector<StaticTile*>(animatedTiledMapTile->getFrameTiles());
    for (int k = 0; k < frames.length(); k++) {
        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- frames.length():" << frames.length();
        if (frames.at(k) == NULL) {
            qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- frames.at(k):" << frames.at(k);
        }
        QPixmap textureRegion = QPixmap(frames.at(k)->getPixmap());
        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -1- ";
        textureRegion = QPixmap::fromImage(textureRegion.toImage().mirrored(true, false));
        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -2- ";
        StaticTile* frame = new StaticTile(textureRegion);
        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -3- ";
        frames.replace(k, frame);
        qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -4- ";
    }
    QVector<int> intervals = QVector<int>(animatedTiledMapTile->getAnimationIntervals());
    AnimatedTile* an = new AnimatedTile(intervals, frames);
    qDebug() << "TemplateForUnit::flipAnimatedTiledMapTile(); -- an:" << an;
    return an;
}

//void TemplateForUnit::validate() {
//    // Need check range values

//    if (this.templateName != null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Load TemplateForUnit: " + this.templateName);
//    }
//    if (this.bounty == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'bounty'! Check the file");
//    } else if (this.factionName == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'factionName'! Check the file");
//    } else if (this.healthPoints == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'healthPoints'! Check the file");
//    } else if (this.name == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'name'! Check the file");
//    } else if (this.speed == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'speed'! Check the file");
//    } else if (this.type == null) {
//        Gdx.app.log("TemplateForUnit::validate()", "-- Can't get 'type'! Check the file");
//    }

////    speed = 0.1f;
////    for (String key : animations.keys()) {
////        Gdx.app.log("TemplateForUnit::validate()", "-- Dir:" + key + " Lenght:" + animations.get(key).getFrameTiles().length);
////    }
//}

QString TemplateForUnit::toString() {
    return toString(false);
}

QString TemplateForUnit::toString(bool full) {
    QString sb;
    sb.append("TemplateForUnit[");
    sb.append("templateName:" + templateName);
    if(full) {
//        sb.append("," + "bounty:" + bounty);
//        sb.append("," + "cost:" + cost);
        sb.append(",factionName:" + factionName);
        sb.append(QString(",healthPoints:%1").arg(healthPoints));
        sb.append(",name:" + name);
        sb.append(QString(",speed:%1").arg(speed));
//        sb.append(",type:%1").arg(type);
    }
    sb.append("]");
    return sb;
}
