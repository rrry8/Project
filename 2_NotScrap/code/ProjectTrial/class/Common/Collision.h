#pragma once
#include<list>
#include"Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollisionM = std::pair<Vector2Int, Vector2Int>;
using CollList = std::list<Collision>;

//first:�ʒu second:y�̕���
using CollisionList = std::pair<Vector2, Vector2>;
