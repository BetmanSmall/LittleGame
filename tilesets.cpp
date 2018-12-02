#include "tilesets.h"

TileSets::TileSets() {

}

void TileSets::addTileSet(TileSet tileSet) {
    this->tileSets.append(tileSet);
}

TileSet *TileSets::getTileSet(int index) {
    return &(tileSets[index]);
}

QList<TileSet> TileSets::getTileSets() {
    return this->tileSets;
}

/** @param id id of the {@link Tile} to get.
 * @return tile with matching id, null if it doesn't exist */
Tile *TileSets::getTile(int id) {
    // The purpose of backward iteration here is to maintain backwards compatibility
    // with maps created with earlier versions of a shared tileset.  The assumption
    // is that the tilesets are in order of ascending firstgid, and by backward
    // iterating precedence for conflicts is given to later tilesets in the list,
    // which are likely to be the earlier version of any given gid.
    // See TiledMapModifiedExternalTilesetTest for example of this issue.
    id++;
    for (int i = tileSets.length()-1; i >= 0; i--) {
//    for (int i = 0; i < tileSets.length(); i++) {
        TileSet tileset = tileSets[i];
        if(tileset.firshgid >= id)
            continue;
        Tile *tile = tileset.getLocalTile(id-tileset.firshgid);
        if (tile != NULL) {
            return tile;
        }
    }
    return NULL;
}

int TileSets::size() {
    return tileSets.size();
}
