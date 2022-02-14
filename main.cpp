
#include <bits/stdc++.h>
using namespace std;

struct XY
{

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
