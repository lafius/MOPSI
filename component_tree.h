#pragma once

#include<vector>
#include<iostream>
#include<set>

using namespace std;

class Node
{
public:
    int m_area;
    int m_level;
    int m_highest;
    vector<Node> m_sons;

    Node();
    int MergeNodes(int node1, int node2, vector<Node> nodes, vector<int> ParNode, vector<int> Rnk);
};

Node MakeNode(int level);

class Vertex
{
public:
    int m_i;
    int m_j;
    int m_level;
    Vertex();
    Vertex(int i, int j, int level);
};

