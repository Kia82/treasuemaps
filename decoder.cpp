#include "decoder.h"
#include "queue.h"
#include "stack.h"
#include "algorithm"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {
    vector<vector<bool>> visited(tm.width(), vector<bool>(tm.height(), false));
    vector<vector<pair<int, int>>> predecessor(tm.width(), vector<pair<int, int>>(tm.height(), {-1, -1}));
    vector<vector<int>> distance(tm.width(), vector<int>(tm.height(), -1));

    Queue<pair<int, int>> bfsQueue;
    bfsQueue.enqueue(start);

    visited[start.first][start.second] = true;
    distance[start.first][start.second] = 0;

    pair<int, int> last = start; // furthest point

    while (!bfsQueue.isEmpty()) {
        pair<int, int> current = bfsQueue.peek();
        bfsQueue.dequeue();

        for (pair<int, int> neighbor : neighbors(current)) {
            if (good(visited, distance, current, neighbor)) {
                visited[neighbor.first][neighbor.second] = true;
                distance[neighbor.first][neighbor.second] = distance[current.first][current.second] + 1;
                predecessor[neighbor.first][neighbor.second] = current;
                bfsQueue.enqueue(neighbor);
                last = neighbor; // update the furthest point
            }
        }
    }

    // backtrack from the furthest point to start using the predecessor matrix to populate pathPts
    while (last != start) {
        pathPts.push_back(last);
        last = predecessor[last.first][last.second];
    }
    pathPts.push_back(start);
    reverse(pathPts.begin(), pathPts.end());
}

    


PNG decoder::renderSolution(){
   PNG solutionImg = mapImg;  

   for (pair<int,int> point : pathPts) {
      RGBAPixel* pixel = solutionImg.getPixel(point.first, point.second);
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
    }

    return solutionImg;
}


PNG decoder::renderMaze(){
   PNG mazeImg = mapImg; 

    // darken pixels
    for (unsigned int x = 0; x < mazeImg.width(); x++) {
        for (unsigned int y = 0; y < mazeImg.height(); y++) {
            setGrey(mazeImg, make_pair(x, y));
        }
    }

    // 7x7 red square 
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            if (start.first + i >= 0 && start.first + i < mazeImg.width() &&
                start.second + j >= 0 && start.second + j < mazeImg.height()) {
                RGBAPixel* pixel = mazeImg.getPixel(start.first + i, start.second + j);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }

    return mazeImg;
}


void decoder::setGrey(PNG & im, pair<int,int> loc){
   RGBAPixel* pixel = im.getPixel(loc.first, loc.second);
   pixel->r = pixel->r * 0.5;
   pixel->g = pixel->g * 0.5;
   pixel->b = pixel->b * 0.5;
}



pair<int,int> decoder::findSpot(){
   return pathPts.back();
}


int decoder::pathLength(){
   return pathPts.size();
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
   vector<pair<int,int>> neighborList;
    neighborList.push_back(make_pair(curr.first-1, curr.second));   // left
    neighborList.push_back(make_pair(curr.first+1, curr.second));   // right
    neighborList.push_back(make_pair(curr.first, curr.second-1));   // up
    neighborList.push_back(make_pair(curr.first, curr.second+1));   // down
    return neighborList;
}



bool decoder::compare(RGBAPixel p, int d){
    int value = (p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4);  // convert lower order bits to an integer between 0 and 63.
    return (value - d) % 64 == 1 || (d - value) % 64 == 1;  // the distance should be 1 either way.
}



bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
    if (next.first < 0 || next.second < 0 || next.first >= mapImg.width() || next.second >= mapImg.height()) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;  // if the location already visited.
    }

    RGBAPixel& currPixel = *mapImg.getPixel(curr.first, curr.second);
    RGBAPixel& nextPixel = *mapImg.getPixel(next.first, next.second);

    int currValue = d[curr.first][curr.second]; 
    int nextValue = (nextPixel.r % 4) * 16 + (nextPixel.g % 4) * 4 + nextPixel.b % 4;

    int distance = abs(currValue - nextValue);

    if (distance == 1 || distance == 63) {  // valid maze distance
        return true;
    }
    
    return false;
}


