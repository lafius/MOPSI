#include "component_tree.h"



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
    //Window window = openWindow(W, H);
    //putGreyImage(IntPoint2(0,0), image, W, H);
    ;
    // Initialisation du vecteur nodes
    vector<Node> nodes;
    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)
        {
            int p = i+W*j;
            Node n = MakeNode(p, int(image[p]));
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

    byte* keepLobes = KeepNLobes(nodes, W, H, M, 1, "area");
    //RebuildImage(nodes, W, H, M, "keepLobes.png");

    delete[] M;

    return(0);
}
