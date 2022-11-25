#include "globals.h"

bool Compare(WaveElem a, WaveElem b);

double crossProductCalculation(LatLon seg1_start, LatLon seg1_end, LatLon seg2_start, LatLon seg2_end);

void find_turn_type(const std::vector<StreetSegmentIdx> &path);

void travel_directions(const std::vector<StreetSegmentIdx> &path,ezgl::application *application);

double findAngleBetweenStreets(LatLon streets_intersection, LatLon segment_starting_location, LatLon segment_ending_location);

void directionAngle(LatLon streets_intersection, LatLon segment_starting_location);
