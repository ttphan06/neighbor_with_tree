// ============================================================================
// This header file contains the declaration of the neighbor class. 
// ============================================================================

#ifndef NEIGHBOR_HEADER
#define NEIGHBOR_HEADER

const int DIMENSIONAL = 3;

class Neighbor
{
public:
    int GetName() const { return name; }
    double GetDistance() const { return distanceToTarget; }
    double GetXCoord() const { return coord[0]; }
    double GetYCoord() const { return coord[1]; }
    double GetZCoord() const { return coord[2]; }
    void SetName(const int n) { name = n; }
    void SetDistance(const double d) { distanceToTarget = d; }
    void SetXCoord(const double x) { coord[0] = x; }
    void SetYCoord(const double y) { coord[1] = y; }
    void SetZCoord(const double z) { coord[2] = z; }
private:
    int         name;
    double      distanceToTarget;
    double      coord[DIMENSIONAL]; // point store x,y,z... base on dimension
};

#endif //NEIGHBOR_HEADER
