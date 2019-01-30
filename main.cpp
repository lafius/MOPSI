#include "component_tree.h"
#include<Imagine/Graphics.h>
#include <map>
using namespace Imagine;

//set<int> Qnode;
//set<Node> Qtree;

bool compareV(Vertex v1, Vertex v2)
{
    return(v1.m_level > v2.m_level);
}

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
    cout << "Image de dimension une puissance de 2" << endl;
    Window window = openWindow(W, H);
    putGreyImage(IntPoint2(0,0), image, W, H);
    //click();

    // cree vecteur nodes
    vector<Node> nodes;
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            Node n = MakeNode(int(image[i+W*j]));
            nodes.push_back(n);
        }
    }

    vector<Vertex> imageV;
    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            int level = image[i+W*j];
            Vertex v(i, j, level);
            imageV.push_back(v);
        }
    }

    // sort imageV by level decreasing
    sort(imageV.begin(), imageV.end(), compareV);
    /*for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++){
            std::cout << "i = " << (*it).m_i << "; " << "j = " << (*it).m_j << "; " << "level = " << (*it).m_level << std::endl;
    }*/

    // step 2
    int* lowestNode = new int[W*H];
    int root;
    int* M = new int[W*H];
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
    cout << "lowestNode :" << endl;

    for(int j=0; j<H; j++){
        for(int i=0; i<W; i++){
            cout << lowestNode[i+W*j] << " ";
        }
        cout << endl;
    }

    // step 15
    root = lowestNode[FindTree(FindNode(0, ParNode), ParTree)];
    delete [] lowestNode;

    // step 16
    for(vector<Vertex>::iterator it=imageV.begin(); it!=imageV.end(); it++)
    {
        int p = (*it).m_i + W*(*it).m_j;
        M[p] = FindNode(p, ParNode);
        //cout << M[p] << endl;
    }

    cout << endl;
    cout << "arbre des composantes connexes :" << endl;
    nodes[root].display("* ", " ");
    cout << endl << endl;

    cout << "volume :" << endl;
    cout << ComputeVolume(nodes[root]) << endl;
    cout << endl << endl;

    delete [] M;





//    // Keep Lobes

//    const char *image_file2 =
//    (argc > 1) ? argv[1] : srcPath("lobes.png");
//    // Load image
//    byte* image2;
//    int w2, h2;
//    cout << "Loading image: " << image_file2 << endl;
//    loadGreyImage(image_file2, image2, w2, h2);
//    // Print statistics
//    cout << "Image size: " << w2 << "x" << h2 << endl;
//    cout << "Number of pixels: " << w2*h2 << endl;
//    // Display image
//    cout << "Image de dimension une puissance de 2" << endl;
//    Window window2 = openWindow(w2, h2);
//    putGreyImage(IntPoint2(0,0), image2, w2, h2);
//    click();

//    // cree vecteur nodes
//    vector<Node> nodes2;
//    for(int j=0; j<H; j++){
//        for(int i=0; i<W; i++){
//            Node n = MakeNode(int(image2[i+W*j]));
//            nodes2.push_back(n);
//        }
//    }

//    vector<Vertex> image2V;
//    for(int j=0; j<H; j++){
//        for(int i=0; i<W; i++){
//            int level = image2[i+W*j];
//            Vertex v(i, j, level);
//            image2V.push_back(v);
//        }
//    }

//    // sort imageV by level decreasing
//    sort(image2V.begin(), image2V.end(), compareV);

//    // step 2
//    int* lowestNode2 = new int[w2*h2];
//    int root2;
//    int* M2 = new int[w2*h2];
//    vector<int> ParNode2(w2*h2, 0);
//    vector<int> ParTree2(w2*h2, 0);
//    vector<int> Rnk2(w2*h2, 0);

//    for(vector<Vertex>::iterator it=image2V.begin(); it!=image2V.end(); it++)
//    {
//        int p = (*it).m_i + w2*(*it).m_j;
//        int level = (*it).m_level;
//        MakeSetTree(p, ParTree2, Rnk2);
//        MakeSetNode(p, ParNode2, Rnk2);
//        nodes2[p] = MakeNode(level);
//        lowestNode2[p] = p;
//    }

//    // step 3
//    vector<Vertex> already_processed2;

//    for(vector<Vertex>::iterator it=image2V.begin(); it!=image2V.end(); it++)
//    {
//        int p = (*it).m_i + w2*(*it).m_j;
//        int curTree = FindTree(p, ParTree2);
//        int curNode = FindNode(lowestNode2[curTree], ParNode2);

//        for(vector<Vertex>::iterator it2=already_processed2.begin(); it2!=already_processed2.end(); it2++)
//        {
//            if (isNeighbor(*it, *it2) && (*it2).m_level >= (*it).m_level)
//            {
//                int q = (*it2).m_i + w2*(*it2).m_j;
//                int adjTree = FindTree(q, ParTree2);
//                int adjNode = FindNode(lowestNode2[adjTree], ParNode2);

//                if (curNode != adjNode)
//                {
//                    if (nodes[curNode].m_level == nodes[adjNode].m_level)
//                    {
//                        curNode = MergeNodes(adjNode, curNode, nodes2, ParNode2, Rnk2);
//                    }
//                    else
//                    {
//                        nodes2[curNode].addChild(nodes2[adjNode]);
//                        nodes2[curNode].m_area += nodes2[adjNode].m_area;
//                        nodes2[curNode].m_highest = std::max(nodes2[curNode].m_highest, nodes2[adjNode].m_highest);
//                    }
//                    curTree = LinkTree(adjTree, curTree, ParTree2, Rnk2);
//                    lowestNode2[curTree] = curNode;
//                }
//            }
//        }

//        already_processed2.push_back(*it);
//    }


//    // step 15
//    root2 = lowestNode2[FindTree(FindNode(0, ParNode2), ParTree2)];
//    delete [] lowestNode2;

//    // step 16
//    for(vector<Vertex>::iterator it=image2V.begin(); it!=image2V.end(); it++)
//    {
//        int p = (*it).m_i + w2*(*it).m_j;
//        M2[p] = FindNode(p, ParNode2);
//        //cout << M2[p] << endl;
//    }

//    cout << endl;
//    cout << "arbre des composantes connexes :" << endl;
//    nodes2[root2].display("* ", " ");
//    cout << endl << endl;

//    delete [] M2;

    return(0);
}
