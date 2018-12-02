#include "maplayers.h"

MapLayers::MapLayers() {
}

int MapLayers::size() {
    return layers.length();
}

void MapLayers::add(Layer *layer) {
    this->layers.append(layer);
}

Layer *MapLayers::get(int index) {
    return layers[index];
}

Layer *MapLayers::get(QString name) {
    for (int i = 0, n = layers.length(); i < n; i++) {
        Layer *layer = layers[i];
        if (name == layer->getName()) {
            return layer;
        }
    }
    return NULL;
}

int MapLayers::getIndex(QString name) {
    return getIndex(get(name));
}

int MapLayers::getIndex(Layer *layer) {
    return layers.indexOf(layer, true);
}

void MapLayers::remove(int index) {
    layers.remove(index);
}

void MapLayers::remove(Layer *layer) {
    layers.removeOne(layer);
}

void MapLayers::turnRight() {
    foreach (Layer* layer, layers) {
        layer->turnRight();
    }
}

void MapLayers::turnLeft() {
    foreach (Layer* layer, layers) {
        layer->turnLeft();
    }
}

void MapLayers::flipX() {
    foreach (Layer* layer, layers) {
        layer->flipX();
    }
}

void MapLayers::flipY() {
    foreach (Layer* layer, layers) {
        layer->flipY();
    }
}
