#pragma once

#include "globals.h"
#include "m4.h"
#define FoundPickUps int
#define TotalPickUps int
// The second field of every deliveryMap element is the index of it in the deliveryInts vector.
extern std::vector<IntersectionIdx> deliveryInts;

extern double **costMatrix;

extern std::unordered_map<IntersectionIdx, IntersectionIdx> pickUps;

extern std::unordered_map<IntersectionIdx, std::pair<FoundPickUps, TotalPickUps>> dropOffs;




void fillDeliveryVec(const std::vector<DeliveryInf> &deliveries);

void initDelivData(const std::vector<DeliveryInf> &deliveries, const std::vector<IntersectionIdx> &depots);

std::unordered_map<IntersectionIdx, int> createDelivMap();

void multiDestDijkstra(const IntersectionIdx source_int, const double turn_penalty, std::unordered_map<IntersectionIdx, int> deliveryMap);

std::vector<IntersectionIdx> findDeliveryOrder(const std::vector<DeliveryInf> &deliveries, const std::vector<IntersectionIdx> &depots, const double turn_penalty);

std::unordered_map<IntersectionIdx, int> createDelivMap(const std::vector<IntersectionIdx> &deliveryInts);
