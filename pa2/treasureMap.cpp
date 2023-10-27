#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    start = s;
    base = baseim;
    maze = mazeim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

    RGBAPixel * p = im.getPixel(loc.first, loc.second);

    p->r = 2 * (p->r / 4);
    p->g = 2 * (p->g / 4);
    p->b = 2 * (p->b / 4);


}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    RGBAPixel * p = im.getPixel(loc.first, loc.second);
    int dmod = d % 64;

    p->r = (p->r & 0xFC) | ((dmod & 0x30) >> 4);
    p->g = (p->g & 0xFC) | ((dmod & 0x0C) >> 2);
    p->b = (p->b & 0xFC) | (dmod & 0x03);

}

PNG treasureMap::renderMap(){

    PNG out(base);
    vector<vector<bool>> visited(maze.height(), vector<bool>(maze.width(), false));
    vector<vector<int>> distance(maze.height(), vector<int>(maze.width(), -1));
    
    Queue<pair<int, int>> q;
    q.enqueue(start);
    visited[start.second][start.first] = true;
    distance[start.second][start.first] = 0;
    setLOB(out, start, 0);

    while (!q.isEmpty()) {
        pair<int, int> curr = q.peek();
        q.dequeue();

        for (pair<int, int> neighbor : neighbors(curr)) {
            if (good(visited, curr, neighbor)) {
                visited[neighbor.second][neighbor.first] = true;
                distance[neighbor.second][neighbor.first] = distance[curr.second][curr.first] + 1;
                setLOB(out, neighbor, distance[neighbor.second][neighbor.first]);
                q.enqueue(neighbor);
            }
        }
    }

    return out;

}


PNG treasureMap::renderMaze(){
    PNG out(base);
    vector<vector<bool>> visited(maze.height(), vector<bool>(maze.width(), false));
    
    Queue<pair<int, int>> q;
    q.enqueue(start);
    visited[start.second][start.first] = true;
    setGrey(out, start);

    while (!q.isEmpty()) {
        pair<int, int> curr = q.peek();
        q.dequeue();

        for (pair<int, int> neighbor : neighbors(curr)) {
            if (good(visited, curr, neighbor)) {
                visited[neighbor.second][neighbor.first] = true;
                setGrey(out, neighbor);
                q.enqueue(neighbor);
            }
        }
    }

    // drawing the red square around the start
    for (int i = -3; i <= 3; ++i) {
        for (int j = -3; j <= 3; ++j) {
            if (start.first + i >= 0 && start.first + i < out.width() && start.second + j >= 0 && start.second + j < out.height()) {
                RGBAPixel * p = out.getPixel(start.first + i, start.second + j);
                p->r = 255;
                p->g = 0;
                p->b = 0;
            }
        }
    }

    return out;
}


bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
     // check bounds
    if (next.first < 0 || next.first >= maze.width() || next.second < 0 || next.second >= maze.height()) {
        return false;
    }
    // check if visited
    if (v[next.second][next.first]) {
        return false;
    }
    // check color
    if (*maze.getPixel(curr.first, curr.second) != *maze.getPixel(next.first, next.second)) {
        return false;
    }

    return true;

}


vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> n;
    n.push_back(make_pair(curr.first-1, curr.second));   // left
    n.push_back(make_pair(curr.first, curr.second+1));   // below
    n.push_back(make_pair(curr.first+1, curr.second));   // right
    n.push_back(make_pair(curr.first, curr.second-1));   // above

    return n;

}

