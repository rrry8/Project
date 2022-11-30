#pragma once
#include<list>
#include"../../Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollList = std::list<Collision>;

//first:�ʒu second:y�̕���
using CollisionFIRST = std::pair<Vector2, Vector2>;

//second:���肷�邩�ǂ����t���O
using Collision2 = std::pair<CollisionFIRST, bool>;

//��L��list�ɂ���
using DrumCollList = std::list<Collision2>;
