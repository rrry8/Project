#pragma once

#include<string>
#include<map>
#include<vector>
#include<list>
#include"../Geometry.h"
#include"rapidxml.hpp"
#include"rapidxml_utils.hpp"
#include"../class/Other/Collision.h"

//stringはname・intはcsvのデータ・
using MapData = std::map <std::string, std::vector<int>>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string FileName);
	bool LoadTmx(std::string FileName);
	bool SetMap(void);

	const Vector2& GetWorldArea(void);
	const Vector2& GetTileSize(void);
	const unsigned int GetLayerSize(void);
	const unsigned int GetFirstGID(void);

	const MapData& GetMapData(void);
	const int GetMapData(std::string layer, Vector2 pos);	//posは座標
	const int GetMapData(std::string layer, int x, int y);	//x,yは升目
	CollList& GetCollList(void);
	CollList& GetCollTrap(void);
	CollList& GetCollCheck(void);
	DrumCollList& GetCollDrum(void);

private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);

	std::map<std::string, int> version_;

	//TMX
	rapidxml::xml_document<> tmxDoc_;
	unsigned int firstGID_;		//GID
	unsigned int layerSize_;	//nextlayerID-1
	Vector2 worldArea_;			//width
	Vector2 tileSize_;			//tilesize
	MapData mapData_;

	//TSX
	rapidxml::xml_document<> tsxDoc_;

	//当たり判定
	CollList collList_;
	CollList collTrap_;
	CollList collCheck_;
	DrumCollList collDrum;

};

