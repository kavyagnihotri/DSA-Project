#include "bits/stdio.h"
using namespace std;

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