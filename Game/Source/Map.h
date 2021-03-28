#ifndef __MAP_H__
#define __MAP_H__

#include "SString.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

struct AnimatedTiles
{
	eastl::list<int> frames;
	int maxTime;
	int timer;
	bool hasChanged;
};

// Holds the information of the tileset
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;
	eastl::list<AnimatedTiles*> animatedTiles;

	// Receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: TODO 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};
	
	~Properties()
	{
		list.clear();
	}

	// Ask the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;

	eastl::list<Property*> list;
};


struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// Get the value of x and y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}

	inline uint ChangeTile(int x, int y, int newValue)
	{
		data[(y * width) + x] = newValue;
		return data[(y * width) + x];
	}
};

// Holds the information of Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	eastl::list<TileSet*> tilesets;

	eastl::list<MapLayer*> layers;
};

class Map
{
public:

	Map();

	// Destructor
	~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Update(float dt);

	// Called each loop iteration
	void Draw(bool showColliders);

	void DrawLayer(Render* render, MapLayer* layer);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, Textures* tex);

	// Translates x and y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// Translates x and y coordinates from world positions to map positions
	iPoint WorldToMap(int x, int y) const;

	SDL_Rect GetTilemapRec(int x, int y) const;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	// Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set, Textures* tex);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	TileSet* GetTilesetFromTileId(int id) const;

	void UpdateTiles();

public:

	MapData data;

private:

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;

	uint32 scale;
	iPoint camOffset;

	SString name;
};
#endif // __MAP_H__