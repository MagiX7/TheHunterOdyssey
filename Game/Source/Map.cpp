#include "App.h"
#include "Render.h"
#include "Textures.h"
#include"AssetsManager.h"
#include "Map.h"
#include"Player.h"
#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map()
{
	//name.Create("map");

	folder.Create("Maps/");

	scale = 1;
	mapLoaded = false;
}

// Destructor
Map::~Map()
{
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
eastl::list<Property*>::iterator prop = list.begin().mpNode;

while (prop != list.end())
{
	if (strcmp(value, (*prop)->name.GetString()) == 0)
	{
		return (*prop)->value;
	}
	++prop;
}

return defaultValue;
}

bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	return ret;
}

bool Map::Update(float dt)
{
	eastl::list<TileSet*>::iterator item = data.tilesets.begin();
	eastl::list<TileSet*>::iterator itEnd = data.tilesets.end();
	for (; item != itEnd; ++item)
	{
		TileSet* tSet = (*item);
		eastl::list<AnimatedTiles*>::iterator tiles = tSet->animatedTiles.begin();
		eastl::list<AnimatedTiles*>::iterator tileEnd = tSet->animatedTiles.end();
		for (; tiles != tileEnd; ++tiles)
		{
			AnimatedTiles* animTiles = (*tiles);
			if (animTiles->timer >= animTiles->maxTime)
			{
				animTiles->timer = 0;
			}
			animTiles->timer++;
		}
	}

	UpdateTiles();

	return true;
}

// Draw the map (all required layers)
void Map::Draw(bool showColliders, EntityManager* manager, Player* current)
{
	if (mapLoaded == false) return;
	//entityList.
	eastl::list<MapLayer*>::iterator item = data.layers.begin();
	eastl::list<MapLayer*>::iterator itEnd = data.layers.end();
	MapLayer* layer;
	MapLayer* auxiliar = nullptr;
	MapLayer* auxiliar2 = nullptr;
	for (; item != itEnd; ++item)
	{
		layer = (*item);
		if (layer->name != "Bordes") 
		{
			if ((layer->properties.GetProperty("draw", 1) != 0) || showColliders) DrawLayer(app->render, layer);
		}
		else
		{
			auxiliar2 = layer;
		}
		if (layer->name == "Edificios" || layer->name == "Muebles") 
		{
			auxiliar = layer;
		}
		
	}
	if (auxiliar != nullptr)DrawAllWithMap(showColliders, app->render, manager, current, auxiliar);
	if (auxiliar2 != nullptr) DrawLayer(app->render, auxiliar2);

}

void Map::DrawAllWithMap(bool showColliders, Render* render, EntityManager* manager, Player* current, MapLayer* layer)
{
	SDL_Rect rec;
	iPoint tileMapPosition;
	TileSet* tileset;
	iPoint points[4];
	int point[4] = { -1,-1,-1,-1 };
	eastl::list<Entity*>::iterator item = manager->entities.begin();
	manager->Draw(showColliders);
	current->Draw(showColliders);
	//combine  player and npc, npc and npc
	for (; item != manager->entities.end(); item++)
	{
		if ((*item)->type == EntityType::NPC_WIZARD || (*item)->type == EntityType::RAY || (*item)->type == EntityType::TABERN || (*item)->type == EntityType::TOWN)
		{
			if ((current->bounds.x < ((*item)->bounds.x + (*item)->bounds.w) && (current->bounds.x + current->bounds.w) >(*item)->bounds.x) &&
				(current->bounds.y < ((*item)->bounds.y + (*item)->bounds.h) && (current->bounds.y + current->bounds.h) >(*item)->bounds.y))
			{
				if ((*item)->bounds.y + (*item)->bounds.h > current->bounds.y + current->bounds.h)
				{
					(*item)->Draw(showColliders);
				}
			}
			points[0] = WorldToMap((*item)->bounds.x, (*item)->bounds.y);
			points[1] = WorldToMap((*item)->bounds.x + (*item)->bounds.w, (*item)->bounds.y);
			points[2] = WorldToMap((*item)->bounds.x, (*item)->bounds.y + (*item)->bounds.h);
			points[3] = WorldToMap((*item)->bounds.x + (*item)->bounds.w, (*item)->bounds.y + (*item)->bounds.h);
			for (int a = 0; a < 4; a++)
			{
				point[a] = layer->Get(points[a].x, points[a].y);
				tileMapPosition = MapToWorld(points[a].x, points[a].y);
				if (point[a] > 0 && tileMapPosition.y < (*item)->bounds.y + (*item)->bounds.w + 32)
				{
					tileset = GetTilesetFromTileId(point[a]);
					rec = tileset->GetTileRect(point[a]);
					render->DrawTexture(tileset->texture, tileMapPosition.x, tileMapPosition.y, &rec);
				}
				point[a] = -1;
			}
		}
	}

	points[0] = WorldToMap(current->bounds.x, current->bounds.y);
	points[1] = WorldToMap(current->bounds.x + current->bounds.w, current->bounds.y);
	points[2] = WorldToMap(current->bounds.x, current->bounds.y + current->bounds.h);
	points[3] = WorldToMap(current->bounds.x + current->bounds.w, current->bounds.y + current->bounds.h);
	for (int a = 0; a < 4; a++)
	{
		point[a] = layer->Get(points[a].x, points[a].y);
		tileMapPosition = MapToWorld(points[a].x, points[a].y);
		if (point[a] > 0 && current->bounds.y + current->bounds.w <tileMapPosition.y+32)
		{
			tileset = GetTilesetFromTileId(point[a]);
			rec=tileset->GetTileRect(point[a]);
			render->DrawTexture(tileset->texture, tileMapPosition.x, tileMapPosition.y, &rec);

			point[a] = layer->Get(points[a].x, points[a].y - 1);
			if (point[a] > 0) {
				tileMapPosition = MapToWorld(points[a].x, points[a].y-1);
				rec = tileset->GetTileRect(point[a]);
				render->DrawTexture(tileset->texture, tileMapPosition.x, tileMapPosition.y, &rec);
			}
		}
	}

}

void Map::DrawLayer(Render* render, MapLayer* layer)
{
	iPoint startTile = WorldToMap(-render->camera.x, -render->camera.y);
	iPoint finalTile = WorldToMap(-render->camera.x + render->camera.w + 32, -render->camera.y + render->camera.h + 32);
	for (int y = startTile.y; y < finalTile.y; ++y)
	{
		for (int x = startTile.x; x < finalTile.x; ++x)
		{
			
			int tileId = layer->Get(x, y);

			if (tileId > 0)
			{
				// L04: DONE 9: Complete the draw function
				TileSet* tileset = GetTilesetFromTileId(tileId); 
				eastl::list<AnimatedTiles*>::iterator anim;
				eastl::list<AnimatedTiles*>::iterator animEnd;

				anim = tileset->animatedTiles.begin();
				animEnd = tileset->animatedTiles.end();

				for (; anim != animEnd; ++anim)
				{
					AnimatedTiles* tile = (*anim);
					if (tile->timer >= tile->maxTime)
					{
						eastl::list<int>::iterator frames = tile->frames.begin();
						if (layer->Get(x, y) == (*frames))
						{
							layer->ChangeTile(x, y, (*frames.next()));
							tile->hasChanged = true;
							break;
						}
					}
				}

				SDL_Rect rec = tileset->GetTileRect(tileId);
				iPoint pos = MapToWorld(x, y);

				render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, &rec);
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect Map::GetTilemapRec(int x, int y) const
{
	iPoint pos = MapToWorld(x, y);
	SDL_Rect rec = { pos.x * scale, pos.y * scale, data.tileWidth * scale, data.tileHeight * scale };

	return rec;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	eastl::list<TileSet*>::iterator item = data.tilesets.begin().mpNode;
	TileSet* set = (*item);

	while (item != data.tilesets.end())
	{
		if (id < (*item)->firstgid)
		{
			set = (*item.prev());
			break;
		}
		set = (*item);
		++item;
	}

	return set;
}

void Map::UpdateTiles()
{
	eastl::list<AnimatedTiles*>::iterator anim;
	eastl::list<AnimatedTiles*>::iterator animEnd;
	eastl::list<TileSet*>::iterator tile = data.tilesets.begin();
	eastl::list<TileSet*>::iterator tileEnd = data.tilesets.end();
	
	for (; tile != tileEnd; ++tile)
	{
		TileSet* set = (*tile);
		anim = set->animatedTiles.begin();
		animEnd = set->animatedTiles.end();
		for (; anim != animEnd; ++anim)
		{
			AnimatedTiles* animTiles = (*anim);
			if (animTiles->hasChanged)
			{
				animTiles->hasChanged = false;
				int aux = (*animTiles->frames.begin());
				animTiles->frames.pop_front();
				animTiles->frames.push_back(aux);
			}
		}
	}
}

bool Map::IsTileOnCamera(SDL_Rect rect, SDL_Rect cam)
{
	if (rect.x + rect.w >= -cam.x && rect.x <= -cam.x + cam.w &&
		rect.y + rect.h >= -cam.y && rect.y <= -cam.y + cam.h) return true;

	return false;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	eastl::list<TileSet*>::iterator item = data.tilesets.begin();

	while (item != data.tilesets.end())
	{
		// Important to unload the texture
		eastl::list<AnimatedTiles*>::iterator it = (*item)->animatedTiles.begin();

		for (; it != (*item)->animatedTiles.end(); ++it)
		{
			(*it)->frames.clear();
			RELEASE((*it));
		}
		(*item)->animatedTiles.clear();

		app->tex->UnLoad((*item)->texture);
		RELEASE((*item));
		++item;
	}
	data.tilesets.clear();

	// Remove all layers along with the properties
	eastl::list<MapLayer*>::iterator itemLayer = data.layers.begin();

	while (itemLayer != data.layers.end())
	{
		(*itemLayer)->properties.list.clear();
		RELEASE((*itemLayer));
		++itemLayer;
	}
	data.layers.clear();


	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename, Textures* tex)
{
	bool ret = true;
	
	name.Create(filename);

	SString tmp("%s%s", folder.GetString(), filename);
	int size = app->assetsManager->MakeLoad(tmp.GetString());
	pugi::xml_parse_result result = mapFile.load_buffer(app->assetsManager->GetLastBuffer(), size);
	app->assetsManager->DeleteBuffer();

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if(ret == true)	ret = LoadMap();

	// Calls a private function to load the tileset values
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set, tex);

		data.tilesets.push_back(set);
	}

	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		// Create new tileset
		MapLayer* lay = new MapLayer(); 

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.push_back(lay); 
	}
	
	if(ret == true)
	{
		LOG("--------------------------------------------------------------------------");
		LOG("<< MAP DATA >>");
		LOG("Width=%d", data.width);
		LOG("Height=%d", data.height);
		LOG("TileWidth=%d", data.tileWidth);
		LOG("TileHeight=%d", data.tileHeight);
		LOG("<< END DATA >>\n");

		// L04: TODO 4: LOG the info for each loaded layer
		eastl::list<MapLayer*>::iterator layerList;
		layerList = data.layers.begin();
		while (layerList != data.layers.end())
		{
			LOG("<< LAYER >>");
			LOG("Name=%s", (*layerList)->name.GetString());
			LOG("Width=%d", (*layerList)->width);
			LOG("Height=%d", (*layerList)->height);
			LOG("<< END LAYER >>\n");
			++layerList;
		}
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		LOG("Filling Map info");

		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);

		SString tmp("%s", map.attribute("orientation").as_string());
		if (tmp == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (tmp == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (tmp == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

// Loads tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;

	LOG("Filling TilesetDetails");
	set->firstgid = tilesetNode.attribute("firstgid").as_int(0);
	set->name = tilesetNode.attribute("name").as_string("no");
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int(0);
	set->tileHeight = tilesetNode.attribute("tileheight").as_int(0);
	set->spacing = tilesetNode.attribute("spacing").as_int(0);
	set->margin = tilesetNode.attribute("margin").as_int(0);
	set->texWidth = tilesetNode.child("image").attribute("width").as_int(0);
	set->texHeight = tilesetNode.child("image").attribute("height").as_int(0);
	set->numTilesWidth = set->texWidth / set->tileWidth;
	set->numTilesHeight = set->texHeight / set->tileHeight;

	for (pugi::xml_node tile = tilesetNode.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		AnimatedTiles* anim = new AnimatedTiles();
		for (pugi::xml_node frame = tile.child("animation").child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			anim->frames.push_back(frame.attribute("tileid").as_uint() + set->firstgid);
			anim->maxTime = 20;
		}
		set->animatedTiles.push_back(anim);
	}

	return ret;
}

// Loads Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set, Textures* tex)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string("s"));
		set->texture = tex->Load(tmp.GetString());
	}

	return ret;
}

// Create the definition for a function that loads the layers
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string("no");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);

	layer->data = new unsigned int[layer->width * layer->height];

	memset(layer->data, 0, layer->width * layer->height * sizeof(unsigned int));

	int i = 0;

	for (pugi::xml_node tile = node.child("data").first_child(); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_uint(0);
		++i;
	}

	LoadProperties(node, layer->properties);

	return ret;
}

// Load a group of properties from a node and fills a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	for (pugi::xml_node proper = node.child("properties").first_child(); proper; proper = proper.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property;
		prop->name = proper.attribute("name").as_string();
		prop->value = proper.attribute("value").as_bool();

		properties.list.push_back(prop);
	}

	return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	eastl::list<MapLayer*>::iterator item;

	for (item = data.layers.begin().mpNode; item != data.layers.end(); ++item)
	{
		MapLayer* layer = (*item);

		if (layer->properties.GetProperty("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}