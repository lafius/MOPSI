#include "component_tree.h"
#include<Imagine/Graphics.h>

using namespace Imagine;

set<int> Qnode;
set<Node> Qtree;
vector<int> ParNode;
vector<int> ParTree;
vector<int> Rnk;

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

return(0);
}
