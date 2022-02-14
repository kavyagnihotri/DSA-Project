#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define nl "\n"
#define REP(i,a,b) for (int i = a; i < b; i++)
#define F first
#define S second
#define PB push_back
#define MP make_pair


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
