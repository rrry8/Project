#pragma once
#include<list>
#include"Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollisionM = std::pair<Vector2Int, Vector2Int>;
using CollList = std::list<Collision>;

//first:位置 second:yの方向
using CollisionList = std::pair<Vector2, Vector2>;
