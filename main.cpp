#include "component_tree.h"
#include<Imagine/Graphics.h>
#include <map>
using namespace Imagine;

/*
 *
 * Definition de l'Algorithme
 *
 *
*/

bool compareVertex(Vertex v1, Vertex v2)
{
    return(v1.m_level > v2.m_level);
}

bool compareNode(Node n1, Node n2, string attribute)
{
    return(n1.getAttribute(attribute) < n2.getAttribute(attribute));
}

int BuildingComponentTree(byte* image, vector<Node>& Nodes, int*& M, int W, int H){
    vector<Vertex> imageDec;
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
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
        Nodes[p] = MakeNode(level);
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
                    if (Nodes[curNode].m_level == Nodes[adjNode].m_level)
                    {
                        curNode = MergeNodes(adjNode, curNode, Nodes, ParNode, Rnk);
                    }
                    else
                    {
                        Nodes[curNode].addChild(Nodes[adjNode]);
                        Nodes[adjNode].m_parent = curNode;
                        Nodes[curNode].m_area += Nodes[adjNode].m_area;
                        Nodes[curNode].m_highest = std::max(Nodes[curNode].m_highest, Nodes[adjNode].m_highest);
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
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            cout << ParNode[i+W*j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "ParTree :" << endl;

    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            cout << ParTree[i+W*j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "LowestNode :" << endl;

    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            cout << lowestNode[i+W*j] << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "Parent :" << endl;

    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            cout << Nodes[i+W*j].m_parent << " ";
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

    return root;
}

void RebuildImage(vector<Node> nodes, int W, int H, int* M, const char* str)
{
    byte* imageRec = new byte[W*H];
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            imageRec[i+W*j] = byte(nodes[M[i+W*j]].m_level);
        }
    }

    saveGreyImage(str, imageRec, W, H);

    delete[] imageRec;
}

//int RemoveLobe(int n, int*& M)
//{
//    if(nodes[n].m_mark == true){
//        M[n] = M[];
//    }
//}

//byte* KeepNLobes(vector<Node> nodes, int W, int H, int* M, int NbLobes)
//{
//    sort(nodes.begin(), nodes.end(), compareNode);
//    while()
//}


/*
 *
 * MAIN
 *
 *
*/

int main(){
    const char *image_file =
    (argc > 1) ? argv[1] : srcPath("article.png");
    // Load image
    byte* image;
    int W, H;
    cout << "Loading image: " << image_file << endl;
    loadGreyImage(image_file, image, W, H);
    // Print statistics
    cout << "Image size: " << W << "x" << H << endl;
    cout << "Number of pixels: " << W*H << endl;
    // Display image
    Window window = openWindow(W, H);
    putGreyImage(IntPoint2(0,0), image, W, H);

    // Initialisation du vecteur nodes
    vector<Node> nodes;
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            Node n = MakeNode(int(image[i+W*j]));
            nodes.push_back(n);
        }
    }

    // on initialise M
    int* M = new int[W*H];
    int root = BuildingComponentTree(image, nodes, M, W, H);

    cout << endl;
    cout << "Affichage de l'arbre des composantes connexes :" << endl;
    nodes[root].display("* ", " ");
    cout << endl << endl;

    cout << "Volume :" << endl;
    cout << ComputeVolume(nodes[root]) << endl;
    cout << endl << endl;

    // Reconstruire l'image
    RebuildImage(nodes, W, H, M, "out2.png");

    // Keep N Lobes

    delete[] M;

    return(0);
}
