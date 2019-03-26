// ============================================================================
// This is the main file for nearest neighbor
// ============================================================================

#include <iostream>
#include "neighbor.h"
#include "cbstree.h"
#include <math.h>
#include "ctreenode.h"
using namespace std;

// global constant, number of point on the graph
const int NUM_NODE = 100;

// function prototype
void SetupCoordinate(Neighbor node[]);
void AddNodeToTree(Neighbor &root, Neighbor point[], CBSTree<Neighbor> &tree);
void DisplayNeighbor(const Neighbor &neighbor);



// === main ===================================================================
//
// ============================================================================

int main()
{
    Neighbor node[NUM_NODE];
    CBSTree<Neighbor> neighborTree;
    int center = 0;
    int numNeig = 0;
    char quit = 0;

    // Generate coordinate
    SetupCoordinate(node);
    
    // ask user for input, generate tree and get nearest neighbor
    do {
        neighborTree.DestroyTree();
        cout << "There are 100 points on the graph\n"
	     << "Enter target point (from 1 to 100): ";
	cin >> center;
	if (center > 100) // avoid out of bound
	{
	    center = 100;
	}
	cout << "How many nearest neighbor you want?: ";
	cin >> numNeig;
	if (numNeig > 99) // avoid out of bound
	{
	    numNeig = 99;
	}
	AddNodeToTree(node[center - 1], node, neighborTree);
	neighborTree.NeighborTraversal(DisplayNeighbor, numNeig);
	cout << "Enter Q to quit or anything to continue: ";
	cin >> quit;
	quit = tolower(quit);
	if (quit == 'q')
	  quit = 1;
	else
	  quit = 0;
    } while(!quit);

    return 0;
} // end of "main"



// === SetupCoordinate =========================================================
// This function will generate random 3D points
//
// Input: --node[] an array of node that represent each point on the graph
//
// Output: nothing
// ============================================================================

void SetupCoordinate(Neighbor node[])
{
    for(int index = 0; index < NUM_NODE; ++index)
    {
        node[index].SetName(index + 1);
	node[index].SetDistance(0);
	node[index].SetXCoord(rand() % 1000);
	node[index].SetYCoord(rand() % 1000);
	node[index].SetZCoord(rand() % 1000);
    }
} // end of "SetupCordinate"



// === AddNodeToTree ==========================================================
// This function will set up the tree by adding node to tree. The binary search
// tree will be set up based on the distance from the target node to other 
// node. It will apply Euclidean Distance to compute distance.
//
// Input: --root: The target node
//        --point: The array of all the point on the graph
//        --tree: Reference to the tree
// Output: nothing
// ============================================================================

void AddNodeToTree(Neighbor &root, Neighbor point[], CBSTree<Neighbor> &tree)
{
    double sum = 0;

    // add root to tree
    root.SetDistance(0);
    tree.InsertItem(root);

    // get Euclidean distance and insert to tree
    for (int index = 0; index < NUM_NODE; ++index)
    {
        sum = pow(root.GetXCoord() - point[index].GetXCoord(), 2);
        sum += pow(root.GetYCoord() - point[index].GetYCoord(), 2);
        sum += pow(root.GetZCoord() - point[index].GetZCoord(), 2);
	point[index].SetDistance(sqrt(sum));
	tree.InsertItem(point[index]);
    }
} // end of "AddNodeToTree"



// === DisplayNeighbor ========================================================
// This function will display nearest neighbors
// 
// Input: -- neighbor: target point neighbor
//
// Output: Nothing
// ============================================================================

void DisplayNeighbor(const Neighbor &neighbor)
{
    cout << "Point name: " << neighbor.GetName() <<endl;
    cout << "\t x coordinate: " << neighbor.GetXCoord() << endl;
    cout << "\t y coordinate: " << neighbor.GetYCoord() << endl;
    cout << "\t z coordinate: " << neighbor.GetZCoord() << endl;
    cout << "\t distance from target point: " << neighbor.GetDistance() 
	 << endl;
} // end of "DisplayNeighbor"
