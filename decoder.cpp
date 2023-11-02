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

    pair<int, int> last = start; // to keep track of the furthest point

    while (!bfsQueue.isEmpty()) {
        pair<int, int> current = bfsQueue.peek();
        bfsQueue.dequeue();

        for (pair<int, int> neighbor : neighbors(current)) {
            if (good(visited, distance, current, neighbor)) {

                visited[neighbor.first][neighbor.second] = true;
                distance[neighbor.first][neighbor.second] = distance[current.first][current.second] + 1;
                predecessor[neighbor.first][neighbor.second] = current;
                bfsQueue.enqueue(neighbor);
                last = neighbor; // update the furthest point.
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

    vector<vector<bool>> v(mazeImg.width(), vector<bool>(mazeImg.height(), false));
    vector<vector<int>> distance(mazeImg.width(), vector<int>(mazeImg.height(), 0));

    Queue<pair<int,int>> q;

    q.enqueue(start);
    v[start.first][start.second] = true;
    distance[start.first][start.second] = 0;

    while(!q.isEmpty()) {
        pair<int, int> curr = q.dequeue();
        vector<pair<int, int>> n = neighbors(curr);

        for(auto p : n) {
            if(good(v, distance, curr, p)) {
                setGrey(mazeImg, p);
                distance[p.first][p.second] = distance[curr.first][curr.second] + 1;
                v[p.first][p.second] = true;
                 q.enqueue(p);
            }
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
   pixel->r = 2* (pixel->r/4);  
   pixel->g = 2* (pixel->g/4);
   pixel->b = 2* (pixel->b/4);
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
    neighborList.push_back(make_pair(curr.first, curr.second+1));   // below (down)
    neighborList.push_back(make_pair(curr.first+1, curr.second));   // right
    neighborList.push_back(make_pair(curr.first, curr.second-1));   // above (up)
    
    return neighborList;
}



bool decoder::compare(RGBAPixel p, int d){
    int value = (p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4);  // convert lower order bits to an integer between 0 and 63.
    return (value - d) % 64 == 1 || (d - value) % 64 == 1;  // the distance should be 1 either way.
}



bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next) {
    // boundary Check
    if (next.first < 0 || next.second < 0 || next.first >= mapImg.width() || next.second >= mapImg.height()) {
        return false;
    }
    // visited Check
    if (v[next.first][next.second]) {
        return false;
    }
    // maze-distance check
    RGBAPixel& currPixel = *mapImg.getPixel(curr.first, curr.second);
    RGBAPixel& nextPixel = *mapImg.getPixel(next.first, next.second);
    int mazeValueCurr = (currPixel.r % 4) * 16 + (currPixel.g % 4) * 4 + currPixel.b % 4;
    int mazeValueNext = (nextPixel.r % 4) * 16 + (nextPixel.g % 4) * 4 + nextPixel.b % 4;
    int distance = (mazeValueNext - mazeValueCurr + 64) % 64; // +64 and %64 ensure positive modulo

    return distance == 1;
}



