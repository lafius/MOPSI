#pragma once

#include<vector>
#include<iostream>
#include<set>
#include "math.h"
#include <map>
using namespace std;

#include<Imagine/Graphics.h>
using namespace Imagine;


class Node
{
public:
    int m_area;
    int m_level;
    int m_highest;
    int m_volume;
    bool m_mark;
    vector<Node> m_sons;
    int m_parent;
    int m_nbChildren;
    int m_position;
    int m_canonic;

    Node();
    void addChild(Node son);
    void display(string prefix, string indent);
    int getAttribute(string str);
    bool operator<(const Node n) const
        {return m_level < n.m_level;}
};

bool isLeaf(Node n, vector<Node> nodes);
int MergeNodes(int node1, int node2, vector<Node>& nodes, vector<int>& ParNode, vector<int>& Rnk);
Node MakeNode(int position, int level);
void exchange(int& x, int& y);

class less_than_key
{
    string m_key;
public:

    less_than_key(string attribute)
    {
        m_key = attribute;
    }
    bool operator() (Node n1, Node n2)
    {
        return (n1.getAttribute(m_key) < n2.getAttribute(m_key));
    }
};

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
int ComputeVolume(Node n);

bool compareVertex(Vertex v1, Vertex v2);

int BuildingComponentTree(byte* image, vector<Node>& Nodes, int*& M, int W, int H);
void RebuildImage(vector<Node> nodes, int W, int H, int* M, const char* str);

int RemoveLobe(int n, vector<Node>& nodes);
Node RemoveLobe2(Node &n, vector<Node>& nodes);
int nbLeaf(Node n);
byte* KeepNLobes(vector<Node>& nodes, Node root, int W, int H, int* M, int NbLobes, string attribute);
