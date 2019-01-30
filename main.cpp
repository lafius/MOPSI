#include "component_tree.h"
#include<Imagine/Graphics.h>
#include <map>
using namespace Imagine;

set<int> Qnode;
set<Node> Qtree;

bool compareV(Vertex v1, Vertex v2)
{
    return(v1.m_level > v2.m_level);
}

int main(){
    const char *image_file =
    (argc > 1) ? argv[1] : srcPath("exemple.png");
    // Load image
    byte* image;
    int W, H;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image, W, H);
    // Print statistics
    cout << "Image size: " << W << "x" << H << endl;
    cout << "Number of pixels: " << W*H << endl;
    // Display image
    cout << "Image de dimension une puissance de 2" << endl;
    Window window = openWindow(W, H);
    putGreyImage(IntPoint2(0,0), image, W, H);

    // cree vecteur nodes
    vector<Node> nodes;
    for(int i=0; i<W; i++){
        for(int j=0; j<H; j++){
            Node n;
            n = MakeNode(int(image[i+W*j]));
            nodes.push_back(n);
        }
    }

    vector<Vertex> imageV;
    for(int i=0; i<W; i++){
        for(int j=0; j<H; j++){
            int level = image[i+W*j];
            Vertex v(i, j, level);
            imageV.push_back(v);
        }
    }

    // sort imageV by level decreasing
    sort(imageV.begin(), imageV.end(), compareV);
    for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++){
            std::cout << "i = " << (*it).m_i << "; " << "j = " << (*it).m_j << "; " << "level = " << (*it).m_level << std::endl;
    }

    // step 2
    int* lowestNode = new int[W*H];
    int root;
    map<int, int> M;
    vector<int> ParNode(W*H, 0);
    vector<int> ParTree(W*H, 0);
    vector<int> Rnk(W*H, 0);

    for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        int level = (*it).m_level;
        MakeSetTree(p, ParTree, Rnk);
        MakeSetNode(p, ParNode, Rnk);
        nodes[p] = MakeNode(level);
        lowestNode[p] = p;
    }

    // step 3
    vector<Vertex> already_processed;
    for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        int curTree = FindTree(p, ParTree);
        int curNode = FindNode(lowestNode[curTree], ParNode);

        for(vector<Vertex>::iterator it2=already_processed.begin(); it2!=already_processed.end(); it2++)
        {
            if (isNeighbor(*it, *it2))
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
                        nodes[curNode].m_area += nodes[adjNode].m_area;
                        nodes[curNode].m_highest = std::max(nodes[curNode].m_highest, nodes[adjNode].m_highest);
                    }
                    curTree = LinkTree(adjTree, curTree, ParTree, Rnk);
                    lowestNode[curTree] = curNode;
                }
            }
        }
    }

    // step 15
    root = lowestNode[FindTree(FindNode(0, ParNode), ParTree)];

    // step 16
    for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        M[p] = FindNode(p, ParNode);
    }

    delete [] lowestNode;
return(0);
}
