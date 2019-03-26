// ============================================================================
// This header file contains the declaration of the neighbor class. 
// ============================================================================

#ifndef NEIGHBOR_HEADER
#define NEIGHBOR_HEADER

class Neighbor
{
public:
    int GetName() const { return name; }
    double GetDistance() const { return distanceToTarget; }
    double GetXCoord() const { return xCoord; }
    double GetYCoord() const { return yCoord; }
    double GetZCoord() const { return zCoord; }
    void SetName(const int n) { name = n; }
    void SetDistance(const double d) { distanceToTarget = d; }
    void SetXCoord(const double x) { xCoord = x; }
    void SetYCoord(const double y) { yCoord = y; }
    void SetZCoord(const double z) { zCoord = z; }
private:
    int         name;
    double      distanceToTarget;
    double      xCoord;
    double      yCoord;
    double      zCoord;
};

#endif //NEIGHBOR_HEADER
