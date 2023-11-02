#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;


}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){


    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    int r = pixel->r;
    int g = pixel->g;
    int b = pixel->b;
    int d_mod_64 = d % 64;

    pixel->r = (r & 0xFC) | ((d_mod_64 >> 4) & 0x3);
    pixel->g = (g & 0xFC) | ((d_mod_64 >> 2) & 0x3);
    pixel->b = (b & 0xFC) | (d_mod_64  & 0x3);




}

PNG treasureMap::renderMap() {

    PNG treasureMap = base; 

    //Initialize working vectors
    vector<vector<bool>> visited(maze.width(), vector<bool>(maze.height(), false));


    vector<vector<int>> distance(maze.width(), vector<int>(maze.height(), 0));

    // Initialize a queue
    Queue<pair<int,int>> q;
    
  

    // For the start location
    visited[start.first][start.second] = true;
    distance[start.first][start.second] = 0;
    setLOB(treasureMap, start, 0); 
    q.enqueue(start);

    // While the queue is not empty
    while (!q.isEmpty()) {
        pair<int,int> curr = q.peek();
         q.dequeue();

        vector<pair<int,int>> adj = neighbors(curr); 

        for (pair<int,int> p : adj) {
            if (good(visited, curr, p)) { 
                visited[p.first][p.second] = true;
                distance[p.first][p.second] = distance[curr.first][curr.second] + 1;
                setLOB(treasureMap, p, distance[p.first][p.second]);
                q.enqueue(p);
            }
        }
    }

    // return the image
    return treasureMap;
}

PNG treasureMap::renderMaze() {
 
    PNG mazeImage = base;

    // Initialize working vectors
    vector<vector<bool>> visited(maze.width(), vector<bool>(maze.height(), false));

    // Initialize a queue
    Queue<pair<int,int>> q;

    // Mark the start location as visited and enqueue it
    visited[start.first][start.second] = true;
    q.enqueue(start);
    while(!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> nextTo = neighbors(curr);
        setGrey(mazeImage, curr);

        for(auto p : nextTo) {
            if(good(visited, curr, p)) {
                 visited[p.first][p.second] = true;
                q.enqueue(p);
            }
        }
    }  // Draw a red 7px x 7px square at the start location
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            int x = start.first + i;
            int y = start.second + j;

            if (x >= 0 && x < mazeImage.width() && y >= 0 && y < mazeImage.height()) {
                RGBAPixel* pixel = mazeImage.getPixel(x, y);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }

    return mazeImage;
}


bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

    // check if 'next' is within the image boundaries
    if (next.first < 0 || next.first >= (int)maze.width() ||
        next.second < 0 || next.second >= (int)maze.height()) {
        return false;
    }

    // check if 'next' has been visited
    if (v[next.first][next.second]) {
        return false;
    }

    // check if the color of 'next' in the 'maze' matches the color of 'curr' in the 'base'
    RGBAPixel * currPixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel * nextPixel = maze.getPixel(next.first, next.second);

    if (*currPixel != *nextPixel) {
        return false;
    }

    // all checks passed, 'next' is a good pixel
    return true;


}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> n;
    n.push_back(make_pair(curr.first, curr.second - 1));   // north
    n.push_back(make_pair(curr.first + 1, curr.second));   // east
    n.push_back(make_pair(curr.first, curr.second + 1));   // south
    n.push_back(make_pair(curr.first - 1, curr.second));   // west
    return n;



}