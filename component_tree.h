#pragma once

#include<vector>
#include<iostream>
#include<set>
#include "math.h"

using namespace std;

class Node
{
public:
    int m_area;
    int m_level;
    int m_highest;
    vector<Node> m_sons;

    Node();
    void addChild(Node son);
};

int MergeNodes(int node1, int node2, vector<Node> nodes, vector<int>& ParNode, vector<int>& Rnk);
Node MakeNode(int level);
void exchange(int& x, int& y);

class Vertex
{
public:
    int m_i;
    int m_j;
    int m_level;
    Vertex();
    Vertex(int i, int j, int level);
};

void MakeSetNode(int x, vector<int>& ParNode, vector<int>& Rnk);
void MakeSetTree(int x, vector<int>& ParTree, vector<int>& Rnk);
int FindNode(int x, vector<int>& ParNode);
int FindTree(int x, vector<int>& ParTree);
int LinkNode(int x, int y, vector<int>& ParNode, vector<int>& Rnk);
int LinkTree(int x, int y, vector<int>& ParTree, vector<int>& Rnk);
bool isNeighbor(Vertex p, Vertex q);
