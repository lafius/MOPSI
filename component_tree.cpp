#include "component_tree.h"

Node MakeNode(int level)
{
    Node n = Node();
    n.m_level = level;
    n.m_area = 1;
    n.m_highest = level;
    return(n);
}

Node::Node()
{
    m_level = 0;
    m_area = 1;
    m_highest = 0;
}

void MakeSetNode(int x, vector<int> ParNode, vector<int> Rnk){
    ParNode[x] = x;
    Rnk[x] = 0;
}

void MakeSetTree(int x, vector<int> ParTree, vector<int> Rnk){
    ParTree[x] = x;
    Rnk[x] = 0;
}

int FindNode(int x, vector<int> ParNode){
    if(ParNode[x] != x){
        ParNode[x] = FindNode(ParNode[x], ParNode);
    }
    return ParNode[x];
}

int LinkNode(int x, int y, vector<int> ParNode, vector<int> Rnk){
    if(Rnk[x] > Rnk[y]){
        LinkNode(y, x, ParNode, Rnk);
    }
    if(Rnk[x] == Rnk[y]){
        Rnk[y]+=1;
    }
    ParNode[x] = y;
    return y;
}

int Node::MergeNodes(int node1, int node2, vector<Node> nodes, vector<int> ParNode, vector<int> Rnk)
{
    int tmpNode = LinkNode(node1, node2, ParNode, Rnk);
    int tmpNode2;
    if(tmpNode == node2){
        for(vector<Node>::iterator it=nodes[node1].m_sons.begin(); it!=nodes[node1].m_sons.end(); it++){
            nodes[node2].m_sons.push_back(*it);
        }
        tmpNode2 = node1;
    } else {
        for(vector<Node>::iterator it=nodes[node2].m_sons.begin(); it!=nodes[node2].m_sons.end(); it++){
            nodes[node1].m_sons.push_back(*it);
        }
        tmpNode2 = node2;
    }
    nodes[tmpNode].m_area = nodes[tmpNode].m_area + nodes[tmpNode2].m_area;
    nodes[tmpNode].m_highest = max(nodes[tmpNode].m_highest, nodes[tmpNode2].m_highest);
    return tmpNode;
}

Vertex::Vertex()
{
    m_level = 0;
}

Vertex::Vertex(int i, int j, int level)
{
    m_i = i;
    m_j = j;
    m_level = level;
}
