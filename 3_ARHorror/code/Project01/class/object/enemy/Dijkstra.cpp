#include<fstream>
#include<iostream>
#include<string>
#include<time.h>
#include "Dijkstra.h"
#include"Node.h"
#include "../../../_debug/_DebugConOut.h"
#include"../../tmx/TmxObj.h"

constexpr float HUNT_COEFFICIENT = 2.5f;    //�ǐՎ��̈ړ��W��
constexpr float MOVE_SPEED = 25.0f;	        //�ړ��X�s�[�h

Dijkstra::Dijkstra()
{
    Init();
}

Dijkstra::~Dijkstra()
{
    Clear();
}

void Dijkstra::Init(void)
{
    startNode_ = nullptr;
    goalNode_ = nullptr;
    retrurnNode_ = nullptr;
    nodeNum_ = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    ImportNodeData("./resource/data/enemy/node.tsx");

    enemyNode_ = startNode_;
    enemyPos_ = enemyNode_->pos;
    SearchRoot();
}

Vector3 Dijkstra::Update(bool huntFlag,Vector3 playerPos)
{
    //�G�̃m�[�h��null�ł͂Ȃ��ꍇ�̓S�[���Ɍ������m�[�h�Ɉړ�����
    if (enemyNode_->toGoal != nullptr)
    {
        float moveCoefficient = huntFlag ? HUNT_COEFFICIENT : 1.0f;
        //�m�[�h�̃x�N�g���Ɉړ�
        auto vecN = (enemyNode_->toGoal->pos - enemyNode_->pos).Normalized();
        enemyPos_ += vecN * (MOVE_SPEED * moveCoefficient);
        
        //�ړ��X�s�[�h��菬�����ꍇ�͎��̃m�[�h�̈ʒu�ֈړ�
        if ((enemyNode_->toGoal->pos - enemyPos_).Magnitude() < MOVE_SPEED * moveCoefficient)
        {
            //���̃m�[�h
            enemyPos_ = enemyNode_->toGoal->pos;
            enemyNode_ = enemyNode_->toGoal;        
        }
        if (enemyNode_->toGoal == nullptr)
        {
            SetGoalProcess(huntFlag,playerPos);
        }
    }
    else
    {
        //�ی��Ƃ���nullptr�������ꍇ�̓S�[���̔z�u��������
        SetStartAndGoal(goalNode_, GetNodeFromIndex(rand() % basicNodeNum_));
    }
    return enemyPos_;
}

void Dijkstra::Draw(void)
{
    //�f�o�b�O�\��
    VECTOR sPos{};
    VECTOR ePos{};
    int num = 0;
    //�_�`��
    for (auto& node : node_)
    {
        auto pos = node->pos.toVECTOR();
        pos.y += 300.0f;

        DrawSphere3D(pos, 50.0f, 20, 0xff0000, 0xff0000, true);
        VECTOR screenPos = ConvWorldPosToScreenPos(pos);
        if (!CheckCameraViewClip(pos))
        {
            DrawFormatStringF(screenPos.x, screenPos.y, 0xffffff, "%d", num);
        }

        //�S�Ă̌o�H
        for (auto& c : node->connectNode)
        {
            sPos = pos;
            ePos = c.node->pos.toVECTOR();
            ePos.y += 300.0f;

            DrawLine3D(sPos, ePos, 0xffdddd);
        }
        num++;
    }

    //�X�^�[�g����S�[���ւ̌o�H
    if (startNode_ && startNode_->cost != -1)
    {
        SharedNode curr = startNode_;
        while (curr->toGoal)
        {
            sPos = curr->pos.toVECTOR();
            ePos = curr->toGoal->pos.toVECTOR();
            sPos.y += 300.0f;
            ePos.y += 300.0f;
            DrawLine3D(sPos, ePos, 0xff0000);
            curr = curr->toGoal;
        }
    }
}

void Dijkstra::Clear(void)
{
    node_.clear();
    startNode_ = nullptr;
    goalNode_ = nullptr;
    //startNode_ = nullptr;
    //goalNode_ = nullptr;
}

void Dijkstra::SetStartAndGoal(SharedNode startNode, SharedNode goalNode)
{
    SetStart(startNode);
    SetGoal(goalNode);
    SearchRoot();
}

void Dijkstra::AddHuntNode(Vector3 playerPos, Vector3 enemyPos)
{
    //�G�̈ʒu�̃m�[�h
    AddNode(enemyPos);
    //�v���C���[�̈ʒu�̃m�[�h
    AddNode(playerPos);

    auto enemyNode = GetNodeFromIndex(static_cast<int>(node_.size()) - 2);
    auto playerNode = GetNodeFromIndex(static_cast<int>(node_.size()) - 1);

    ConnectNode(playerNode, enemyNode);

    float minDis = 99999.9f;
    SharedNode minNode = nullptr;

    //�G�̈ʒu�̃m�[�h�Ƌ߂��̃m�[�h���q����
    for (int n = 0; n < basicNodeNum_; n++)
    {
        const auto node = GetNodeFromIndex(n);
        //�������g�̏ꍇ�͕Ԃ�(�ی�)
        if (node == enemyNode || node == playerNode)
        {
            continue;
        }

        if (abs(node->pos.y - enemyNode->pos.y) > 0.1f)
        {
            continue;
        }

        float dis = QueryCost(enemyNode, node);//(newNode->pos - n->pos).Magnitude();

        if (minDis > dis)
        {
            minDis = dis;
            minNode = node;
        }
    }

    //���������Ƀm�[�h���Ȃ��ꍇ�͈�ԋ߂��m�[�h�Ɍq����
    if (minNode == nullptr)
    {
        for (int n = 0; n < basicNodeNum_; n++)
        {
            const auto node = GetNodeFromIndex(n);
            if (node == enemyNode || node == playerNode)
            {
                continue;
            }

            float dis = QueryCost(enemyNode, node);;

            if (minDis > dis)
            {
                minDis = dis;
                minNode = node;
            }
        }
    }

    ConnectNode(enemyNode, minNode);
    SetGoal(playerNode);
    enemyNode_ = enemyNode;
    SearchRoot();
}

void Dijkstra::ConnectNearNode(Vector3 enemyPos)
{
    float minDis = 99999.9f;
    SharedNode minNode = nullptr;

    //�S�[���m�[�h�Ƌ߂��̃m�[�h���q����
    for (int n = 0; n < basicNodeNum_; n++)
    {
        auto node = GetNodeFromIndex(n);
        if (goalNode_ == node)
        {
            continue;
        }

        if (abs(node->pos.y - goalNode_->pos.y) > 1.0f)
        {
            continue;
        }

        float dis = QueryCost(goalNode_, node);

        if (minDis > dis)
        {
            minDis = dis;
            minNode = node;
        }
    }

    if (minNode == nullptr)
    {
        return;
    }

    retrurnNode_ = minNode;

    ConnectNode(goalNode_, minNode);
    SetGoal(minNode);
    SearchRoot();
}

bool Dijkstra::ImportNodeData(std::string fileName)
{
    std::string name;

    Clear();

    rapidxml::file<> file = fileName.c_str();
    rapidxml::xml_document<> document;
    document.parse<0>(file.data());
    rapidxml::xml_node<>* posNode = document.first_node("NodePos");

    //�m�[�h�̈ʒu��ǂݍ���
    int num = 1;
    name = posNode->first_attribute("name")->value();
    if (name == "NodePos")
    {
        for (auto node = posNode->first_node("Pos1"); node != nullptr; node = node->next_sibling())
        {
            name = node->first_attribute("name")->value();
            if (name == "Pos" + std::to_string(num))
            {
                Vector3 pos = Vector3(
                    static_cast<float>(atof(node->first_attribute("posX")->value())),
                    static_cast<float>(atof(node->first_attribute("posY")->value())),
                    static_cast<float>(atof(node->first_attribute("posZ")->value()))
                );
                AddNode(pos);
            }
            num++;
        }
    }

    nodeNum_ = num - 1;
    basicNodeNum_ = nodeNum_;

    //�X�^�[�g�ʒu��ǂݍ���
    auto startPosNode = posNode->next_sibling();
    name = startPosNode->first_attribute("name")->value();
    if (name == "StartNode")
    {
        startNode_ = GetNodeFromIndex(atoi(startPosNode->first_attribute("index")->value()));
    }

    //�S�[���ʒu��ǂݍ���
    auto goalPosNode = startPosNode->next_sibling();
    name = goalPosNode->first_attribute("name")->value();
    if (name == "GoalNode")
    {
        goalNode_ = GetNodeFromIndex(atoi(goalPosNode->first_attribute("index")->value()));
    }

    //�ڑ��m�[�h��ǂݍ���
    num = 1;
    auto connectNode = goalPosNode->next_sibling();
    name = connectNode->first_attribute("name")->value();
    if (name == "ConnectNode")
    {
        for (auto node = connectNode->first_node("Connect1"); node != nullptr; node = node->next_sibling())
        {
            name = node->first_attribute("name")->value();
            if (name == "Connect" + std::to_string(num))
            {
                ConnectNode(
                    GetNodeFromIndex(atoi(node->first_attribute("index1")->value())),
                    GetNodeFromIndex(atoi(node->first_attribute("index2")->value()))
                );
            }
            num++;
        }
    }

    return true;
}

void Dijkstra::AddNode(Vector3 initPos)
{
    for (auto& n : node_)
    {
        if (n->pos == initPos)
        {
            return;
        }
    }
    node_.emplace_back(std::make_shared<Node>(initPos));
    nodeNum_++;
}

void Dijkstra::RemoveNode(SharedNode node)
{
    for (auto& n : node->connectNode)
    {
        n.node->RemoveConnect(node);
    }

    node_.remove(node);
    //shared��delete����ɂ� reset?
    node.reset();
}

void Dijkstra::RemoveHuntNode(void)
{
    if (retrurnNode_ != nullptr)
    {
        int removeNum = nodeNum_ - basicNodeNum_;
        for (int n = 0; n < removeNum; n++)
        {
            auto itr = node_.end();
            itr--;
            RemoveNode(*itr);
        }
        retrurnNode_ = nullptr;
        nodeNum_ = basicNodeNum_;
    }
}

void Dijkstra::ConnectNode(SharedNode n1, SharedNode n2)
{
    //�o�^�ς݂̏ꍇ�͕Ԃ�
    for (auto& cn : n1->connectNode)
    {
        if (cn.node == n2)
        {
            return;
        }
    }

    //�����̌v�Z
    auto cost = QueryCost(n1, n2);

    //�ڑ�
    n1->connectNode.emplace_back(NodeConnection(n2, cost));
    n2->connectNode.emplace_back(NodeConnection(n1, cost));
}

void Dijkstra::SearchRoot(void)
{
    //���Z�b�g�E����
    ResetNode();

    if (!startNode_ || !goalNode_)
    {
        return;
    }

    std::list<SharedNode> work1;
    std::list<SharedNode> work2;

    std::list<SharedNode>* nowLevel = &work1;
    std::list<SharedNode>* nextLevel = &work2;
    std::list<SharedNode>* swapLevel = nullptr;

    //�S�[������v�Z
    goalNode_->cost = 0;    //�S�[���ɃR�X�g0�ɂ���
    nowLevel->emplace_back(goalNode_);

    float nodeCost = 0.0f;

    while (nowLevel->size())
    {
        for (const auto& level : *nowLevel)
        {
            for (auto& c : level->connectNode)
            {
                nodeCost = level->cost + c.cost;

                if (c.node->cost == -1 || nodeCost < c.node->cost)
                {
                    //���T���m�[�h���邢�͍ŒZ���[�g���X�V�ł���ꍇ
                    //�o�H�R�X�g�ƃS�[���֌��������߂̃m�[�h���Z�b�g
                    c.node->cost = nodeCost;
                    c.node->toGoal = level;
                }
                else
                {
                    continue;
                }
                nextLevel->push_back(c.node);
            }
        }

        //���X�g�����ւ��Ď��̊K�w��T������
        swapLevel = nowLevel;
        nowLevel = nextLevel;
        nextLevel = swapLevel;
        nextLevel->clear();
    }
}

void Dijkstra::SetStart(SharedNode node)
{
    startNode_ = node;
}

void Dijkstra::SetGoalProcess(const bool huntFlag, const Vector3 playerPos)
{
    //�S�[���ɒH�蒅�����ꍇ
    if (!huntFlag)
    {
        //�ǐՃt���O���Ȃ��ꍇ�͕��ʂɈړ�
        //�ǐՃm�[�h�폜
        RemoveHuntNode();
        //�V�����S�[���𗐐��Ō��߂�
        SetStartAndGoal(goalNode_, GetNodeFromIndex(rand() % basicNodeNum_));
    }
    else
    {
        //�ǐՃt���O������ꍇ�̓v���C���[�̈ʒu�Ƀm�[�h��ǉ����Č��݂̃m�[�h�ƌq����
        AddNode(playerPos);
        ConnectNode(enemyNode_, GetNodeFromIndex(static_cast<int>(node_.size()) - 1));
        enemyNode_ = goalNode_;
        SetGoal(GetNodeFromIndex(static_cast<int>(node_.size()) - 1));
        SearchRoot();
    }
    enemyPos_ = enemyNode_->pos;
}

void Dijkstra::SetGoal(SharedNode node)
{
    goalNode_ = node;
}

float Dijkstra::QueryCost(SharedNode n1, SharedNode n2)
{
    return (n1->pos - n2->pos).Magnitude();
}

void Dijkstra::ResetNode(void)
{
    for (auto& node : node_)
    {
        node->Reset();
    }
}

int Dijkstra::SearchIndex(SharedNode node)
{
    int ret = 0;
    for (auto& n : node_)
    {
        if (n == node)
        {
            return ret;
        }
        ret++;
    }
    return -1;
}

SharedNode Dijkstra::GetNodeFromIndex(int idx)
{
    int ret = 0;
    
    for (auto& itr : node_)
    {
        if (ret == idx)
        {
            return itr;
        }
        ret++;
    }
    return nullptr;
}
