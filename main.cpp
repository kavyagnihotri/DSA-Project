
#include <bits/stdc++.h>
using namespace std;

// Simple coordinate object to represent points and vectors
struct XY {
    float x;
    float y;

    void pConstruct(float x, float y) {
        x = x;
        y = y;
    }
};

// Axis-aligned bounding box with half dimension and center
struct AABB {
    XY center;
    float halfDimension;

    void construct(XY center, float halfDimension) {
        center = center;
        halfDimension = halfDimension;
    }
    bool containsPoint(XY point) {
        // corners
        float x1 = center.x - halfDimension;
        float y1 = center.y - halfDimension;

        float x2 = center.x - halfDimension;
        float y2 = center.y + halfDimension;
        
        float x3 = center.x + halfDimension;
        float y3 = center.y + halfDimension;

        float x4 = center.x + halfDimension;
        float y4 = center.y - halfDimension;

        float x0 = point.x;
        float y0 = point.y;

        if(x0 > x1 && y0 > y1 && x0 > x2 && y0 < y2 && x0 < x2 && y0 < y2 && x0 < x4 && y0 > y4) 
            return true;
        else return false;
    }

    // boundary not included
    bool intersectsAABB(AABB other) {
        if(abs(center.x - other.center.x) < halfDimension + other.halfDimension 
            && abs(center.y - other.center.y) < halfDimension + other.halfDimension) 
            return true;
        else return false;
    }
};

bool QuadTrees::insert(XY p)
{
    if (!boundary.containsPoint(p))
    {
        return false;
    }
    //our QTnodeCapacity is 1 and this check if node
    // has not been subdivided yet
    if(points.size < QT_NODE_CAPACITY && northWest == null)
    {
        points.append(p);  //points is an array containing all points of type XY (see struct)
        return true;
    }

    //if node contains point
    if(northWest == null)
    {
        subdivide();
    }

    //if node is already partitioned, insert into first empty node
    if (northWest->insert(p)) return true;
    if (northEast->insert(p)) return true;
    if (southWest->insert(p)) return true;
    if (southEast->insert(p)) return true;

    return false; //should never happen


}
vector<XY> QuadTress::queryRange(AABB range){
    vector<XY> pointsInRange;
    if(!this.boundary.inersectAABB(range)){
        return pointtsInRange;
    }
    for(int p=0;p<this->points.size();p++){
        if(range.containsPoint(this->points[p])){
            pointsInRange.append(this->points[p]);
        }
    }
    
    if(northWeat==NULL)
        return pointsInRange;
    
    pointsInRange.append(northWest->queryRange(range));
    pointsInRange.append(southhWest->queryRange(range));
    pointsInRange.append(southEast->queryRange(range));
    pointsInRange.append(northEast->queryRange(range));
}
