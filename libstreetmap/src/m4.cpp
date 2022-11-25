/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "globals.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "m1decs.h"
#include "m2decs.h"
#include "m3decs.h"
#include "m4decs.h"
#include <fstream>
#include <algorithm>

int depots_start;
std::vector<IntersectionIdx> deliveryInts;
std::vector<IntersectionIdx> pickUpsVec;
std::unordered_map<IntersectionIdx, IntersectionIdx> pickUps;
std::unordered_map<IntersectionIdx, std::pair<FoundPickUps, TotalPickUps>> dropOffs;

double **costMatrix = nullptr;

void initDelivData(const std::vector<DeliveryInf> &deliveries, const std::vector<IntersectionIdx> &depots)
{
    
    
    for (int i = 0; i < deliveries.size(); i++)
    {
        deliveryInts.push_back(deliveries[i].pickUp);
        deliveryInts.push_back(deliveries[i].dropOff);
        pickUpsVec.push_back(deliveries[i].pickUp);
    }
    depots_start = deliveryInts.size();
    for(int i = 0; i < depots.size(); i++){
        deliveryInts.push_back(depots[i]);
    }
    for (int i = 0; i < deliveries.size(); i++)
    {

        // Inserts pickUps into its map, value of each element is the
        // corresponding dropOff.
        pickUps.emplace(deliveries[i].pickUp, deliveries[i].dropOff);

        // Checks if the drop-off of the current DeliveryInf object has already
        // been inserted.
        auto dropLoc = dropOffs.find(deliveries[i].dropOff);
        if (dropLoc == dropOffs.end())
        {
            // If the dropOff hasn't been inserted yet, initialize with FoundPickUps = 0  and TotalPickUps = 1.
            std::pair<int, int> temp_pair;
            temp_pair = std::make_pair(0, 1);
            dropOffs.emplace(deliveries[i].dropOff, temp_pair);
        }
        else
        {

            // If the dropOff location has already been inserted, this
            // updates the TotalPickUps.
            (dropLoc->second).second = (dropLoc->second).second + 1;
        }
    }
    auto numCritInts = deliveryInts.size();
    double **matrix = 0;
    matrix = new double *[numCritInts];

    // Matrix initialized to 0s.

    /*
    * Since this multi dimensional array has been created to the dimensions
    * of the deliveryInts vector, the indices correspond to the entries in it!
    * as a result. 
   */
    for (int i = 0; i < numCritInts; i++)
    {
        matrix[i] = new double[numCritInts];
        for (int j = 0; j < numCritInts; j++)
        {
            matrix[i][j] = 0;
        }
    }
    costMatrix = matrix;
}
std::unordered_map<IntersectionIdx, int> createDelivMap(){
    std::unordered_map<IntersectionIdx, int> deliveryMap;
    for (int i = 0; i < deliveryInts.size(); i++)
    {
        deliveryMap.emplace(deliveryInts[i], i);
    }
    return deliveryMap;
}
void multiDestDijkstra(const IntersectionIdx source_int, const double turn_penalty, std::unordered_map<IntersectionIdx, int> deliveryMap )
{
    // Custom comparison sets node with smallest time at the top of the queue.
    std::priority_queue<WaveElem, std::vector<WaveElem>, decltype(&Compare)> waveFront(Compare);
    WaveElem elem(source_int, 0);
    waveFront.emplace(elem);

    /*
   * Initializing a vector of nodes to be used solely within the scope of
   * this function. Originally, we tried using a global variable, which
   * necessitated resetting the vector after a path was found. This way, no
   * resetting needs to occur.
   */
    std::vector<Node> nodes;
    nodes.resize(NUM_INTS);

    nodes[source_int].reachTime = 0;
    int source_idx = deliveryMap.find(source_int)->second;
    int numDel = 0;
    
    // 314, 167, 243, 62, 96, 326, 295, 324, 273,
    while (!waveFront.empty())
    {
        WaveElem curr = waveFront.top();
        waveFront.pop();
        int prev_ID = curr.waveID;
        
        // node->found is used to evaluate whether or not a street has already
        // been checked. The top of the wavefront is always going to have the
        // best time out of those nodes.
        Node *node = &nodes[prev_ID];
        if (deliveryMap.find(prev_ID) != deliveryMap.end())
        {
            int index = deliveryMap.find(prev_ID)->second;
            costMatrix[source_idx][index] = node->reachTime;
            numDel++;
        }
        if (numDel == deliveryInts.size())
        {
            break;
        }

        if (node->found)
        {
            continue;
        }
        node->found = true;
        
        for (int streetSeg : intersection_street_segments[prev_ID])
        {

            auto segment_info = getStreetSegmentInfo(streetSeg);
            if (segment_info.oneWay)
            {
                if (segment_info.from != prev_ID)
                {
                    continue; //Invalid street, skip.
                }
            }
            int to_ID = segment_info.from == prev_ID ? segment_info.to : segment_info.from;
            Node *curr_seg = &nodes[to_ID];
            if (curr_seg->found)
            {
                continue; // Already seen, skip.
            }
            double cost = node->reachTime + seg_travel_time[streetSeg];
            if (node->reachingEdge != NO_EDGE)
            {
                if (getStreetSegmentInfo(node->reachingEdge).streetID != segment_info.streetID)
                {
                    cost += turn_penalty;
                }
            }
            if (cost < curr_seg->reachTime)
            {
                WaveElem tempElem(to_ID, cost);
                waveFront.emplace(tempElem);
                curr_seg->reachTime = cost;
                curr_seg->reachingEdge = streetSeg;
                curr_seg->intID = prev_ID;
            }
        }
    }
}

/*
   * Organize pickups and drop offs into their own data structures,
   * store drop offs into a map, second field is a vector of pointers to bools,
   * when a pickup location is visited, the bool corresponding to it is updated
   * and whenever a drop off is encountered, perform bitwise AND, if this is 
   * not true, then skip this intersection.
   */

std::vector<IntersectionIdx> findDeliveryOrder(const std::vector<DeliveryInf> &deliveries, const std::vector<IntersectionIdx> &depots, const double turn_penalty)
{
    initDelivData(deliveries, depots);
    std::unordered_map<IntersectionIdx, int> deliveryMap = createDelivMap();
    for (int i = 0; i < deliveryInts.size(); i++)
    {
        multiDestDijkstra(deliveryInts[i], turn_penalty, deliveryMap);
    }
    std::vector<IntersectionIdx> deliveryOrder;
    std::vector<IntersectionIdx> to_visit = pickUpsVec;
    IntersectionIdx curr_loc = depots[0];
    deliveryOrder.push_back(curr_loc);
    IntersectionIdx curr_best = curr_loc;
    int cb_idx = 0;
    while(to_visit.size() != 0){
        double min_dist = MAX_TIME;

        for(int i = 0; i < to_visit.size(); i++){
            int curr_idx = deliveryMap.find(curr_loc)->second;
            int temp_idx = deliveryMap.find(to_visit[i])->second;
            auto dist = costMatrix[curr_idx][temp_idx];
            if( dist < min_dist){
                min_dist = dist;
                curr_best = to_visit[i];
                cb_idx = i;
                
            }
            // auto curridx = deliveryMap.find()
            
            // if(dist < min_dist){
            //     min_dist = dist;
            //     curr_best = to_visit[i];
            //     cb_idx = i;
            // }    
            
        }
        curr_loc = curr_best;
        // std::cout<<"CURRLOC CURRBEST "<<curr_loc<<" "<<curr_best<<std::endl;
        // std::cout<<"TOVISITBEGIN+ "<< *(to_visit.begin() + cb_idx) <<std::endl;
        to_visit.erase(to_visit.begin()+cb_idx);
                deliveryOrder.push_back(curr_loc);

        if(pickUps.find(curr_loc) != pickUps.end()){
            auto dropOffID = pickUps.find(curr_loc)->second;
            auto dropLoc = dropOffs.find(dropOffID);
            
            ++(dropLoc->second).first;
            to_visit.push_back(dropLoc->first);
            std::cout<<"PICKUP: "<<curr_loc<<" DROPOFF: "<<dropLoc->first<<std::endl;
                   
        }
    }


    // Last point is now the end of the second last subPath, need to 
    // go back to a depot - find the closest one

    // auto last = deliveryOrder[deliveryOrder.size() - 1 ];
    // int last_idx = deliveryMap.find(last)->second;
    auto end_loc = depots[0];
    deliveryOrder.push_back(end_loc);
    // Depots start at depots_start index in Matrix
    
    // double min_dist = MAX_TIME;
    // int end_dep;
    // for(int i = 0; i < depots.size(); i++){
    //     int dep_idx = i + depots_start;

    //     if(costMatrix[last_idx][dep_idx] < min_dist){
    //         min_dist = costMatrix[last_idx][dep_idx];
    //         end_dep = depots[dep_idx];
    //     }

    // }
    // deliveryOrder.push_back(end_dep);

    return deliveryOrder;
}

// std::vector<CourierSubPath> travelingCourier(
//     const std::vector<DeliveryInf> &deliveries,
//     const std::vector<int> &depots,
//     const float turn_penalty)
// {
//     for(int i = 0; i < deliveries.size(); i++){
//         std::cout<<deliveries[i].pickUp<<", "<<deliveries[i].dropOff<<std::endl;
//     }
//     for(int i = 0; i < depots.size(); i++){
//         std::cout<<depots[i]<<std::endl;
//     }
//     std::vector<IntersectionIdx> ordered = findDeliveryOrder(deliveries, depots, turn_penalty);
//     std::vector<CourierSubPath> cspVec;
//     for(int i = 0; i < ordered.size() - 1; i++){
//         CourierSubPath tempCSP;

//         tempCSP.start_intersection = ordered[i];
//         tempCSP.end_intersection = ordered[i+1];
//         tempCSP.subpath = findPathBetweenIntersections(tempCSP.start_intersection, tempCSP.end_intersection, turn_penalty);

//         cspVec.push_back(tempCSP);
//     }
//     std::cout<<"DONE"<<std::endl;
//     for(int i = 0; i < deliveryInts.size(); i++){
//         delete [] costMatrix[i];
//     }
//     delete [] costMatrix;
    
//     return cspVec;
// }
std::vector<CourierSubPath> travelingCourier(const std::vector<DeliveryInf>& deliveries, const std::vector<int>& depots,const float turn_penalty){
    
 //   std::vector<int> pathway;
    std::vector<DeliveryInf> deliverSpots;
    int delivPickup;
    int depotPickup;
    int delivFrom;
    int delivTo;
    double distanceBetween;
    double shortestDistance = MAX_TIME;
    int depotChosen = 1;
    int deliveryChosen = 1;
    std::vector<CourierSubPath> totalPath;
    deliverSpots = deliveries;
    CourierSubPath test;
    #pragma omp parallel for
    for (int i = 0; i < depots.size(); i++){
           depotPickup = depots[i];
        #pragma omp parallel for
        for(int j= 0; j < deliverSpots.size(); j++){
          
           delivPickup = deliverSpots[j].pickUp;
  
           distanceBetween = findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(depotPickup),getIntersectionPosition(delivPickup)));
          
          if (distanceBetween < shortestDistance){
              shortestDistance = distanceBetween;
              depotChosen = i;
              deliveryChosen= j;
          }
        }
    }
    
       test.start_intersection = depots[depotChosen];
       test.end_intersection = deliverSpots[deliveryChosen].pickUp;
       test.subpath= findPathBetweenIntersections(depots[depotChosen],deliverSpots[deliveryChosen].pickUp, turn_penalty);
       totalPath.push_back(test);
       
       test.start_intersection = deliverSpots[deliveryChosen].pickUp;
       test.end_intersection = deliverSpots[deliveryChosen].dropOff;
       test.subpath= findPathBetweenIntersections(deliverSpots[deliveryChosen].pickUp,deliverSpots[deliveryChosen].dropOff, turn_penalty);
       totalPath.push_back(test);
        
//     pathway.push_back(depots[depotChosen]);
//     pathway.push_back(deliverSpots[deliveryChosen].pickUp);
//     pathway.push_back(deliverSpots[deliveryChosen].dropOff);
     delivFrom = deliverSpots[deliveryChosen].dropOff;
     deliverSpots.erase(deliverSpots.begin()+(deliveryChosen));

     
     while(deliverSpots.size() > 0){         
        shortestDistance = MAX_TIME;
        #pragma omp parallel for
         for(int a = 0; a < deliverSpots.size(); a++){    
             
             delivTo = deliverSpots[a].pickUp; 
             distanceBetween = findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(delivFrom),getIntersectionPosition(delivTo)));
            
             if(distanceBetween < shortestDistance){
              shortestDistance = distanceBetween;
              deliveryChosen= a;  
             }
         }
//             pathway.push_back(deliverSpots[deliveryChosen].pickUp);
//             pathway.push_back(deliverSpots[deliveryChosen].dropOff);
        
       test.start_intersection = delivFrom;
       test.end_intersection = deliverSpots[deliveryChosen].pickUp;
       test.subpath= findPathBetweenIntersections(delivFrom,deliverSpots[deliveryChosen].pickUp, turn_penalty);
       totalPath.push_back(test);
       
       test.start_intersection = deliverSpots[deliveryChosen].pickUp;
       test.end_intersection = deliverSpots[deliveryChosen].dropOff;
       test.subpath= findPathBetweenIntersections(deliverSpots[deliveryChosen].pickUp,deliverSpots[deliveryChosen].dropOff, turn_penalty);
       totalPath.push_back(test);
        
             delivFrom = deliverSpots[deliveryChosen].dropOff;
             deliverSpots.erase(deliverSpots.begin()+(deliveryChosen));
    }
        shortestDistance = MAX_TIME;
        #pragma omp parallel for
         for (int i = 0; i < depots.size(); i++){
           depotPickup = depots[i];
          
           delivPickup = delivFrom;
  
           distanceBetween = findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(delivPickup),getIntersectionPosition(depotPickup)));
          
          if (distanceBetween < shortestDistance){
              shortestDistance = distanceBetween;
              depotChosen = i;
          }
    }
     
     
     
     
      test.start_intersection = delivFrom;
       test.end_intersection = depots[depotChosen];
       test.subpath= findPathBetweenIntersections(delivFrom,depots[depotChosen], turn_penalty);
       totalPath.push_back(test);
     
//    int lastSlot = pathway.size()-1;
// 
//     for(int g = 0; g < lastSlot; g++){
//       CourierSubPath test;
//       test.start_intersection = pathway[g];
//       test.end_intersection = pathway[g+1];
//       test.subpath= findPathBetweenIntersections(pathway[g],pathway[g+1], turn_penalty);
//       totalPath.push_back(test);
//     }
//    CourierSubPath lastPath; 
//    lastPath.start_intersection = pathway[lastSlot];
//    lastPath.end_intersection = depots[0];
//    lastPath.subpath = findPathBetweenIntersections(pathway[lastSlot],depots[0], turn_penalty);
//    totalPath.push_back(lastPath);
    
    
    return totalPath;
     }