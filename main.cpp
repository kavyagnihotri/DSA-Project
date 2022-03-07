#include <iostream>
#include <cmath>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;
 
// Used to hold details of a point
struct Point
{
    int x; // x coordinate of point in 2d space
    int y; // y coordinate of point in 2d space
 
    Point(int _x, int _y) // constructor
    {
        x = _x;
        y = _y;
    }
 
    Point() // default constructor
    {
        x = 0;
        y = 0;
    }
};
 
// this struct is used for the range querries(you can search for points within a particular radius of a center)
struct Circle
{
    Point center; // we use the structure point to store the center of the circular region to be searched.
    float radius; // stores the radius of the circular region
    Circle(int x, int y, float rad)
    {
        center = Point(x, y);
        radius = rad;
    }
};
 
// this struct is used for the range querries(you can search for points within a rectangular region)
struct Rectangle
{
    Point center;  // stores the center of the rectangular region   
    Point topLeft;  // stores the top left corner of the rectangle
    Point botRight;  // stores the bottom right corner of the rectangle
 
    Rectangle(Point _topLeft, Point _botRight)
    {
        topLeft = _topLeft;
        botRight = _botRight;
        center = Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2);
    }
};
 
// The objects that we want stored in the quadtree. 
// it is a combination of the struct point and the data we want to store in that point.
struct Node
{
    Point pos; // stores position of data to be inserted
    int data; // stores the data we want to store there
 
    Node(Point _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    Node()
    {
        data = 0;
    }
};
 
// The main quadtree class
// https://en.wikipedia.org/wiki/Quadtree
class Quad
{
    // Hold details of the boundary of this node
    string loc; // used to hold location of the node(for example top left quad is given a value "01" and so on)
                // kind of direction to reach that node
    Point topLeft; // stores top left corner of quad
    Point botRight; // stores bottom right corner of quad
    Quad *parent; // stores the parent quad to help back trace
 
 
    Node *n; // Contains details of node
 
    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *botLeftTree;
    Quad *botRightTree;
 
public:
    Quad() 
    {
        topLeft = Point(0, 0);
        botRight = Point(0, 0);
        n = NULL;
        parent = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
        loc = "";
    }
    Quad(Point topL, Point botR, Quad *_parent, string rel_loc) //constructor that initializes the quad along with its children
    {
        parent = _parent;
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
        topLeft = topL; // top left corner of quad
        botRight = botR; // bottom right corner of quad
        if (_parent != NULL)
        {
            loc = parent->loc + rel_loc; // to store the "direction to reach this quad"
        }
        else
        {
            loc = rel_loc;
        }
    }
    void printer();
    void insert(Node *);
    Node *search(Point);
    void rangeQuery(Circle cir, vector<Node> &ans);
    void rangeQuery(Rectangle rec, vector<Node> &ans);
    void subDivide();
    bool inBoundary(Point p);
    void nearestNeighbour(Point target);
    // vector<Node> rangeQuery(Rectangle rec);
};
void Quad::printer()
{
    if(n!=NULL)
    {
        cout<<"("<<topLeft.x<<","<<topLeft.y<<") -> ("<<botRight.x<<","<<botRight.y<<")"<<endl;
        cout<<"("<<n->pos.x<<","<<n->pos.y<<") data:"<<n->data<<endl;
    }
    else if(topLeftTree!=NULL)
    {
        topLeftTree->printer();
        topRightTree->printer();
        botRightTree->printer();
        botLeftTree->printer();
    }
}
 
// Check if the point is inside the circular boundary
bool inBoundaryCircle(Point p, Circle cir)
{
    float dist = (p.x - cir.center.x) * (p.x - cir.center.x) + (p.y - cir.center.y) * (p.y - cir.center.y);
    dist = sqrt(dist);
    return dist - cir.radius < 0.00001; // < 0.00001 cause of inaccuracies due to float
}
bool inBoundaryCirRec(Circle cir,Rectangle rec)
{
    Point a1 = rec.topLeft; 
    Point a3 = rec.botRight;
    Point a2 = Point(rec.botRight.x, rec.topLeft.y);
    Point a4 = Point(rec.topLeft.x, rec.botRight.y);
    if (inBoundaryCircle(a1, cir) || inBoundaryCircle(a2, cir) || inBoundaryCircle(a3, cir) || inBoundaryCircle(a4, cir))
    {
        return true;
    }
    else
    {
        int a1=abs(cir.center.x-rec.topLeft.x);
        int a2=abs(cir.center.x-rec.botRight.x);
        int b2=abs(cir.center.y-rec.botRight.y);
        int b1=abs(cir.center.x-rec.topLeft.x);
        if(cir.center.x<rec.botRight.x &&  cir.center.x>rec.topLeft.x && cir.radius<rec.botRight.y && cir.radius>rec.topLeft.y)
        {
            return true;
        }
        if(cir.center.y<rec.botRight.y &&  cir.center.y>rec.topLeft.y && cir.radius<rec.botRight.x && cir.radius>rec.topLeft.x)
        {
            return true;
        }
 
    }
    return false;
}
 
// function to insert node into quad tree
void Quad::insert(Node *node)
{
    if (node == NULL)
    {
        return;
    }
    // Current quad cannot contain it
    if (!inBoundary(node->pos))
    {
        //cout << "Cannot insert point"<<endl;
        return;
    }
    if (n == NULL) // if node does not contain any point 
    {
        if (topLeftTree == NULL) // if quad is not subdivided yet
        {
            n = node;
            return;
        }
        else // quad has been subdivided into four quads and we have to check which quad the point belongs to
        {
            if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
            {
                // Indicates topLeftTree
                if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
                {
                    topLeftTree->insert(node); // recursive call of insert function
                }
 
                // Indicates botLeftTree
                else
                {
                    botLeftTree->insert(node);
                }
            }
            else
            {
                // Indicates topRightTree
                if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
                {
                    topRightTree->insert(node);
                }
 
                // Indicates botRightTree
                else
                {
                    botRightTree->insert(node);
                }
            }
        }
    }
    else // we subdivide if the quad already has a node in it as a quad can have maximum one node
    {
        this->subDivide(); // call to function which divides quad into four quads and re-inserts the node back into the quad
        n = NULL;
        if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
        {
            // Indicates topLeftTree
            if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
            {
                topLeftTree->insert(node);
            }
 
            // Indicates botLeftTree
            else
            {
                botLeftTree->insert(node);
            }
        }
        else
        {
            // Indicates topRightTree
            if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
            {
                topRightTree->insert(node);
            }
 
            // Indicates botRightTree
            else
            {
                botRightTree->insert(node);
            }
        }
    }
}
 
void Quad::subDivide() // divides quad into four new quads and reinserts the node already present in it
{
    botRightTree = new Quad(
        Point((topLeft.x + botRight.x) / 2,
              (topLeft.y + botRight.y) / 2),
        Point(botRight.x, botRight.y), this, "11");
 
    topRightTree = new Quad(
        Point((topLeft.x + botRight.x) / 2,
              topLeft.y),
        Point(botRight.x,
              (topLeft.y + botRight.y) / 2),
        this, "00");
 
    botLeftTree = new Quad(
        Point(topLeft.x,
              (topLeft.y + botRight.y) / 2),
        Point((topLeft.x + botRight.x) / 2,
              botRight.y),
        this, "10");
 
    topLeftTree = new Quad(
        Point(topLeft.x, topLeft.y),
        Point((topLeft.x + botRight.x) / 2,
              (topLeft.y + botRight.y) / 2),
        this, "01");
 
    if ((topLeft.x + botRight.x) / 2 >= n->pos.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= n->pos.y)
        {
            topLeftTree->insert(n);
        }
 
        // Indicates botLeftTree
        else
        {
            botLeftTree->insert(n);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= n->pos.y)
        {
            topRightTree->insert(n);
        }
 
        // Indicates botRightTree
        else
        {
            botRightTree->insert(n);
        }
    }
}
 
// Find a node in a quadtree
Node *Quad::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
    {
        return NULL;
    }
 
    if (n != NULL) // if n has a node
    {
        if (n->pos.x == p.x && n->pos.y == p.y)
        {
            return n;
        }
        else
        {
            return NULL;
        }
    }
    if ((topLeft.x + botRight.x) / 2 >= p.x) // if n does not have a node we search for the correct quad 
    {                                        //and recursively searches for the point
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(p);
        }
 
        // Indicates botLeftTree
        else
        {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(p);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(p);
        }
 
        // Indicates botRightTree
        else
        {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(p);
        }
    }
};
 
// funtion to check if point is in boundary
bool Quad::inBoundary(Point p) 
{
    return (p.x >= topLeft.x &&
            p.x <= botRight.x &&
            p.y >= topLeft.y &&
            p.y <= botRight.y);
}
 
// Check if the point is inside the rectangular boundary
bool inBoundaryRect(Point p, Rectangle rec)
{
    return (p.x >= rec.topLeft.x &&
            p.x <= rec.botRight.x &&
            p.y >= rec.topLeft.y &&
            p.y <= rec.botRight.y);
}
 
 
// overloaded function. this one checks if any point is in circlular region
void Quad::rangeQuery(Circle cir, vector<Node> &ans)
{
    if (inBoundaryCirRec(cir,Rectangle(topLeft,botRight)))
    {
        if (n == NULL)
        {
            if (topLeftTree != NULL)
            {
                topLeftTree->rangeQuery(cir, ans);
            }
            if (botRightTree != NULL)
            {
                botRightTree->rangeQuery(cir, ans);
            }
            if (topRightTree != NULL)
            {
                topRightTree->rangeQuery(cir, ans);
            }
            if (botLeftTree != NULL)
            {
                botLeftTree->rangeQuery(cir, ans);
            }
        }
        else
        {
            if (inBoundaryCircle(n->pos, cir))
            {
                ans.push_back(*n);
            }
        }
    }
}
void Quad::rangeQuery(Rectangle rec, vector<Node> &ans)
{
    Point a1 = topLeft;
    Point a3 = botRight;
    Point a2 = Point(botRight.x, topLeft.y);
    Point a4 = Point(topLeft.x, botRight.y);
    if (inBoundaryRect(a1, rec) || inBoundaryRect(a2, rec) || inBoundaryRect(a3, rec) || inBoundaryRect(a4, rec))
    {
        if (n == NULL)
        {
            if (topLeftTree != NULL)
            {
                topLeftTree->rangeQuery(rec, ans);
            }
            if (botRightTree != NULL)
            {
                botRightTree->rangeQuery(rec, ans);
            }
            if (topRightTree != NULL)
            {
                topRightTree->rangeQuery(rec, ans);
            }
            if (botLeftTree != NULL)
            {
                botLeftTree->rangeQuery(rec, ans);
            }
        }
        else
        {
            if (inBoundaryRect(n->pos, rec))
            {
                ans.push_back(*n);
            }
        }
    }
    else
    {
        if (inBoundaryRect(rec.center, Rectangle(topLeft, botRight)))
        {
            if (n == NULL)
            {
                if (topLeftTree != NULL)
                    topLeftTree->rangeQuery(rec, ans);
                if (botRightTree != NULL)
                    botRightTree->rangeQuery(rec, ans);
                if (topRightTree != NULL)
                    topRightTree->rangeQuery(rec, ans);
                if (botLeftTree != NULL)
                    botLeftTree->rangeQuery(rec, ans);
            }
            else if (inBoundaryRect(n->pos, rec))
            {
                ans.push_back(*n);
            }
        }
    }
}
void Quad::nearestNeighbour(Point target)
{
    int size = 1;
    vector<Node> ans;
    while (true)
    {
        Rectangle area(Point(target.x - size, target.y - size), Point(target.x + size, target.y + size));
        rangeQuery(area, ans);
        size++;
        if (ans.size() > 0)
        {
            break;
        }
    }
    Node *anser;
    float dist = -1;
    for (auto k : ans)
    {
        float temp_dist = (k.pos.x - target.x) * (k.pos.x - target.x) + (k.pos.y - target.y) * (k.pos.y - target.y);
        temp_dist = sqrt(temp_dist);
        cout << temp_dist << endl;
        if (dist == -1 || temp_dist < dist)
            anser = &k;
    }
    cout<<"Nearest Neighbour for point ("<<target.x<<","<<target.y<<") is "<<"("<<anser->pos.x<<","<<anser->pos.y<<")";
    // return anser;
}
// stuff to add
// range querry box and circle
// json file with all data
// nearest neighbour
// remove point
// edit file and change vars
// visuals
 
// Driver program
int main()
{ 
    Quad center(Point(0, 0), Point(9, 9),NULL, "");
    Node a(Point(5 , 2) , 1);
    Node b(Point(2 , 2) , 2);
    Node c(Point(6 , 7) , 3);
    Node d(Point(4, 3) , 4);
    Node e(Point(5 , 6) , 5);
    Node f(Point(3 , 7) , 6);
    Node g(Point(2 , 5) , 7);
    Node h(Point(5 , 1) , 8);
    center.insert(&a);
    center.insert(&b);
    center.insert(&c);
    center.insert(&d);
    center.insert(&e);
    center.insert(&f);
    center.insert(&g);
    center.insert(&h);
    center.printer();
    cout << "Node a: " <<
        center.search(Point(2, 2))->data << "\n";
    cout << "Node b: " <<
        center.search(Point(5, 6))->data << "\n";
    cout<<"Node d:" << center.search(Point(4,4));
    cout<<endl;
    cout<<"RangeQuery for rectangle"<<endl;
    vector<Node> ans;
    Rectangle rec(Point(6, 7),Point(6, 7));
    center.rangeQuery(rec,ans);
    cout<<"Number of points found = "<<ans.size()<<endl;;
    for(auto &k:ans)
    {
        cout<<"("<<k.pos.x<<", "<<k.pos.y<<") ";
    }
    cout<<endl;
    vector<Node> ans1;
    Circle cir(3,4,3);
    center.rangeQuery(cir,ans1);
    cout<<"Number of points found = "<<ans1.size()<<endl;;
    for(auto &k:ans1)
    {
        cout<<"("<<k.pos.x<<", "<<k.pos.y<<") ";
    }
    cout<<endl;
    center.nearestNeighbour(Point(4, 5));
}
