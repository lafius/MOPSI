#include "component_tree.h"

Node MakeNode(int position, int level)
{
    Node n = Node();
    n.m_level = level;
    n.m_area = 1;
    n.m_highest = level;
    n.m_position = position;
    return(n);
}

Node::Node()
{
    m_level = 0;
    m_area = 0;
    m_highest = 0;
    m_volume = 0;
    m_mark = false;
    m_parent = -1;
    m_nbChildren = 0;
    m_position = -1;
}

int Node::getAttribute(string str){
    if(str == "level"){
        return m_level;
    }
    if(str == "area"){
        return m_area;
    }
    if(str == "highest"){
        return m_highest;
    }
    if(str == "volume"){
        return m_volume;
    }
}

void Node::addChild(Node son)
{
    m_sons.push_back(son);
    m_nbChildren += 1;
}

void Node::display(string prefix, string indent){
    cout << prefix << m_level;
    for(vector<Node>::iterator it=m_sons.begin(); it!=m_sons.end(); it++){
        cout << endl;
        (*it).display(prefix + indent, indent);
    }
}

bool isLeaf(Node n, vector<Node> nodes)
{
    bool is_leaf = false;
    if (n.m_sons.size() == 0 && nodes[n.m_canonic].m_sons.size() == 0)
    {
        is_leaf = true;
    }
    return(is_leaf);
}

void exchange(int& x, int& y)
{
    int temp=x;
    x=y;
    y=temp;
}

void MakeSetNode(int x, vector<int>& ParNode, vector<int>& Rnk){
    ParNode[x] = x;
    Rnk[x] = 0;
}

void MakeSetTree(int x, vector<int>& ParTree, vector<int>& Rnk){
    ParTree[x] = x;
    Rnk[x] = 0;
}

int FindNode(int x, vector<int>& ParNode){
    if(ParNode[x] != x){
        ParNode[x] = FindNode(ParNode[x], ParNode);
    }
    return ParNode[x];
}

int FindTree(int x, vector<int>& ParTree){
    if(ParTree[x] != x){
        ParTree[x] = FindTree(ParTree[x], ParTree);
    }
    return ParTree[x];
}

int LinkNode(int x, int y, vector<int>& ParNode, vector<int>& Rnk){
    if(Rnk[x] > Rnk[y]){
        exchange(x, y);
    }
    if(Rnk[x] == Rnk[y]){
        Rnk[y]+=1;
    }
    ParNode[x] = y;
    return y;
}

int LinkTree(int x, int y, vector<int>& ParTree, vector<int>& Rnk){
    if(Rnk[x] > Rnk[y]){
        exchange(x, y);
    }
    if(Rnk[x] == Rnk[y]){
        Rnk[y]+=1;
    }
    ParTree[x] = y;
    return y;
}

int MergeNodes(int node1, int node2, vector<Node>& nodes, vector<int>& ParNode, vector<int>& Rnk)
{
    int tmpNode = LinkNode(node1, node2, ParNode, Rnk);
    int tmpNode2;
    if(tmpNode == node2)
    {
        for(vector<Node>::iterator it=nodes[node1].m_sons.begin(); it!=nodes[node1].m_sons.end(); it++)
        {
            (*it).m_parent = node2;
            nodes[node2].addChild(*it);
        }
        tmpNode2 = node1;
    }
    else
    {
        for(vector<Node>::iterator it=nodes[node2].m_sons.begin(); it!=nodes[node2].m_sons.end(); it++)
        {
            (*it).m_parent = node1;
            nodes[node1].addChild(*it);
        }
        tmpNode2 = node2;
    }
    nodes[tmpNode2].m_canonic = tmpNode;
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

bool isNeighbor(Vertex p, Vertex q)
{
    bool isNeighbor = false;
    if (abs(p.m_i-q.m_i)+abs(p.m_j-q.m_j) == 1)
    {
        isNeighbor = true;
    }
    return(isNeighbor);
}

int ComputeVolume(Node n)
{
    int vol = n.m_area;
    vector<Node> sons = n.m_sons;
    for(vector<Node>::iterator it=sons.begin(); it!=sons.end(); it++)
    {
        vol += ComputeVolume(*it) + (*it).m_area*((*it).m_level - n.m_level);
    }
    n.m_volume = vol;
    return(vol);
}

bool compareVertex(Vertex v1, Vertex v2)
{
    return(v1.m_level > v2.m_level);
}

int BuildingComponentTree(byte* image, vector<Node>& nodes, int*& M, int W, int H)
{
    vector<Vertex> imageDec;
    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            int level = image[i+W*j];
            Vertex v(i, j, level);
            imageDec.push_back(v);
        }
    }

    // sort imageDec by level decreasing
    sort(imageDec.begin(), imageDec.end(), compareVertex);

    // step 2
    int* lowestNode = new int[W*H];
    int root;
    vector<int> ParNode(W*H, 0);
    vector<int> ParTree(W*H, 0);
    vector<int> Rnk(W*H, 0);

    for(vector<Vertex>::iterator it=imageDec.begin(); it!=imageDec.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        int level = (*it).m_level;
        MakeSetTree(p, ParTree, Rnk);
        MakeSetNode(p, ParNode, Rnk);
        nodes[p] = MakeNode(p, level);
        lowestNode[p] = p;
    }

    // step 3
    vector<Vertex> already_processed;

    for(vector<Vertex>::iterator it=imageDec.begin(); it!=imageDec.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        int curTree = FindTree(p, ParTree);
        int curNode = FindNode(lowestNode[curTree], ParNode);

        for(vector<Vertex>::iterator it2=already_processed.begin(); it2!=already_processed.end(); it2++)
        {
            if (isNeighbor(*it, *it2) && (*it2).m_level >= (*it).m_level)
            {
                int q = (*it2).m_i + W*(*it2).m_j;
                int adjTree = FindTree(q, ParTree);
                int adjNode = FindNode(lowestNode[adjTree], ParNode);

                if (curNode != adjNode)
                {
                    if (nodes[curNode].m_level == nodes[adjNode].m_level)
                    {
                        curNode = MergeNodes(adjNode, curNode, nodes, ParNode, Rnk);
                    }
                    else
                    {
                        nodes[curNode].addChild(nodes[adjNode]);
                        nodes[adjNode].m_parent = curNode;
                        nodes[curNode].m_area += nodes[adjNode].m_area;
                        nodes[curNode].m_highest = std::max(nodes[curNode].m_highest, nodes[adjNode].m_highest);
                    }
                    curTree = LinkTree(adjTree, curTree, ParTree, Rnk);
                    lowestNode[curTree] = curNode;
                }
            }
        }

        already_processed.push_back(*it);
    }

    cout << endl;
    cout << "ParNode :" << endl;
    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            cout << ParNode[i+W*j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "ParTree :" << endl;

    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            cout << ParTree[i+W*j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "LowestNode :" << endl;

    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            cout << lowestNode[i+W*j] << " ";
        }
        cout << endl;
    }


    // step 15
    root = lowestNode[FindTree(FindNode(0, ParNode), ParTree)];
    delete [] lowestNode;

    // step 16
    for(vector<Vertex>::iterator it=imageDec.begin(); it!=imageDec.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        M[p] = FindNode(p, ParNode);
    }


    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            int p = i+W*j;
            if (nodes[p].m_parent == -1)
            {
                nodes[p].m_parent = FindNode(p, ParNode);
                if (nodes[nodes[p].m_parent].m_level == nodes[p].m_level)
                {
                    nodes[p].m_parent = nodes[nodes[p].m_parent].m_parent;
                }
            }
        }
    }

    cout << "Parent :" << endl;

    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            cout << nodes[i+W*j].m_parent << " ";
        }
        cout << endl;
    }

    return root;
}

void RebuildImage(vector<Node> nodes, int W, int H, int* M, const char* str)
{
    byte* imageRec = new byte[W*H];
    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            imageRec[i+W*j] = byte(nodes[M[i+W*j]].m_level);
        }
    }

    saveGreyImage(str, imageRec, W, H);

    delete[] imageRec;
}

int RemoveLobe(int n, vector<Node>& nodes)
{
    if(nodes[n].m_mark == true)
    {
        nodes[n] = nodes[RemoveLobe(nodes[n].m_parent, nodes)];
        cout << "removeLobe" << endl;
    }
    return(n);
}

Node RemoveLobe2(Node& n, vector<Node>& nodes)
{
    if(n.m_mark == true)
    {
        n = RemoveLobe2(nodes[n.m_parent], nodes);
        cout << "removeLobe2" << endl;
    }
    return(n);
}

int nbLeaf(Node n)
{
    int nb_leaf = 0;
    if (n.m_sons.size() == 0)
    {
        nb_leaf += 1;
    }
    else
    {
        for(vector<Node>::iterator it=n.m_sons.begin(); it!=n.m_sons.end(); it++)
        {
            nb_leaf += nbLeaf(*it);
        }
    }
    return(nb_leaf);
}

byte* KeepNLobes(vector<Node>& nodes, Node root, int W, int H, int* M, int NbLobes, string attribute)
{
    set<Node> Q;
    byte* F = new byte[W*H];
    cout << "nodes not sorted : " << endl;
    for(vector<Node>::iterator it=nodes.begin(); it!=nodes.end(); it++)
    {
         cout << (*it).getAttribute(attribute) << endl;
    }
    cout << endl;
    vector<Node> nodes_sorted = nodes;
    sort(nodes_sorted.begin(), nodes_sorted.end(), less_than_key(attribute));
    cout << "nodes sorted by increasing " << attribute << " : " << endl;
    for(vector<Node>::iterator it=nodes_sorted.begin(); it!=nodes_sorted.end(); it++)
    {
         cout << (*it).getAttribute(attribute) << endl;
    }

    int L = nbLeaf(root);
    cout << endl;
    cout << "nbLeaf = " << L << endl;
    cout << endl;

    while(L > NbLobes)
    {
        cout << "L = " << L << endl;
        for(vector<Node>::iterator it=nodes_sorted.begin(); it!=nodes_sorted.end(); it++)
        {
            if (isLeaf(*it, nodes) && (*it).m_mark == false)
            {
                cout << (*it).m_level << endl;
                int p = (*it).m_parent;
                cout << p << endl;
                nodes[p].m_nbChildren =  nodes[p].m_sons.size();
                nodes[p].m_nbChildren -= 1;
                if (nodes[p].m_nbChildren > 0)
                {
                    cout << "yes!" << endl;
                    L = L- 1;
                }
                (*it).m_mark = true;
                nodes[(*it).m_position].m_mark = true;
                Q.insert(*it);
                break;

//                int c = (*it).m_position;
//                int p = nodes[c].m_parent;
//                nodes[p].m_nbChildren -= 1;
//                if (nodes[p].m_nbChildren > 0)
//                {
//                    L = L-1;
//                }
//                nodes[c].m_mark = true;
//                Q.insert(c);
            }

        }
    }

    cout << endl;

    while (Q.empty() == false)
    {
        cout << "Q in not empty" << endl;
        set<Node>::iterator it = Q.begin();
        Q.erase(*it);
        //cout << "*it.m_position = " << (*it).m_position << endl;
        RemoveLobe((*it).m_position, nodes); //nodes_sorted ?

//        set<int>::iterator it = Q.begin();
//        Q.erase(*it);
//        RemoveLobe(*it, nodes);
    }

    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            int p = i + W*j;
            F[p] = nodes[M[p]].m_level;
        }
    }

    return(F);
}
