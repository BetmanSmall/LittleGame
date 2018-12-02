#ifndef TILELAYERCELL_H
#define TILELAYERCELL_H

#include "tile.h"

/** @brief represents a cell in a TiledLayer: Tile, flip and rotation properties. */
class TileLayerCell {
    Tile *tile;
    bool flipHorizontally;
    bool flipVertically;
    int rotation;
public:
    static const int ROTATE_0 = 0;
    static const int ROTATE_90 = 1;
    static const int ROTATE_180 = 2;
    static const int ROTATE_270 = 3;
    TileLayerCell();
    /** @return The tile currently assigned to this cell. */
    Tile *getTile () {
        return tile;
    }

    /** Sets the tile to be used for this cell.
     *
     * @param tile the {@link Tile} to use for this cell. */
    void setTile(Tile *tile) {
        this->tile = tile;
    }

    /** @return Whether the tile should be flipped horizontally. */
    bool getFlipHorizontally () {
        return flipHorizontally;
    }

    /** Sets whether to flip the tile horizontally.
     *
     * @param flipHorizontally whether or not to flip the tile horizontally. */
    void setFlipHorizontally (bool flipHorizontally) {
        this->flipHorizontally = flipHorizontally;
    }

    /** @return Whether the tile should be flipped vertically. */
    bool getFlipVertically () {
        return flipVertically;
    }

    /** Sets whether to flip the tile vertically.
     *
     * @param flipVertically whether or not this tile should be flipped vertically. */
    void setFlipVertically (bool flipVertically) {
        this->flipVertically = flipVertically;
    }

    /** @return The rotation of this cell, in degrees. */
    int getRotation () {
        return rotation;
    }

    /** Sets the rotation of this cell, in degrees.
     *
     * @param rotation the rotation in degrees. */
    void setRotation (int rotation) {
        this->rotation = rotation;
    }
};

#endif // TILELAYERCELL_H
