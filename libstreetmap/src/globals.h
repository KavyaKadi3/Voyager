/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2decs.h
 * Author: khokha73
 *
 * Created on April 3rd, 2021, 11:34 AM
 */

#pragma once

#include <iostream>
#include <limits>
#include <vector>
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "stringOp.h"
#include "OSMDatabaseAPI.h"
#include <cmath>
#include <fstream>
#include <math.h>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <map>
#include <unordered_map>
#include <string>
#include "ezgl/point.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/color.hpp"
#include "application.hpp"
#include "color.hpp"
#include <regex>

//* GLOBAL CONSTANTS

#define NUM_STREETS getNumStreets()
#define NUM_INTS getNumIntersections()
#define NUM_SEGS getNumStreetSegments()
#define NUM_POIS getNumPointsOfInterest()
#define NUM_FEATS getNumFeatures()
#define NO_EDGE -1
#define MAX_TIME std::numeric_limits<double>::max()

//! STRUCT/CLASS DEFINITIONS
    /*
    * Structs/Classes: Data structures defined to contain custom data points. 
    * These are used (often in conjunction with one of the above data
    * structures) to better represent and contain information relevant to a
    * specific purpose. For instance, this block contains the nodes and Wave-
    * Front elements used in our pathfinding algorithm.
    */
        
    struct intersection_data{
        LatLon position; //maybe convert pt tox y in load map to make program fasater
        std::string name;
        bool highlight = false;
        bool start = false;
        bool end = false;
        bool FIND_highlight = false;
    
    };

    struct POI_data{
        LatLon position;
        std::string name;
        std::string type;
    };

    /* PATHFINDING DATA STRUCTURES */

    struct Node{
        IntersectionIdx intID;
        StreetSegmentIdx reachingEdge = NO_EDGE;
        bool found = false;
        double reachTime = MAX_TIME; // Shortest time found to this node so far - initialized to max
    };

    struct WaveElem{
        //Node* node;
        //StreetSegmentIdx edgeID;
        double time; // Travel time of the segment taken to reach this intersection
        IntersectionIdx waveID;
        WaveElem(IntersectionIdx id, double t){waveID = id; time = t;}
    };


//! GLOBAL VARIABLES 

    /* 
    * Map data structures: Used in pre-computations often used in drawing 
    * graphics, but not limited to such.
    */
    extern std::vector<StreetIdx> streets;
    extern std::vector<std::vector<StreetSegmentIdx>> intersection_street_segments;
    extern std::vector<std::vector<StreetSegmentIdx>> outgoing_street_segments;
    extern std::vector<std::vector<StreetSegmentIdx>> streets_street_segments;
    extern std::vector<std::vector<IntersectionIdx>> streets_intersections;
    extern std::vector<std::vector<LatLon>> segment_curve_points;
    extern std::vector<std::vector<LatLon>> street_curve_points;
    extern std::vector< std::pair<std::string, StreetIdx> > sorted_names;
    extern std::multimap<std::string, StreetIdx> names_map;
    extern std::vector<double> seg_travel_time;
    extern std::vector<std::vector<ezgl::point2d>> feature_points; 
    extern std::unordered_map<OSMID, const OSMWay*> ways_map;
    extern std::unordered_map<OSMID, std::string> street_attributes;
    extern std::vector<StreetSegmentIdx> highways;
    extern std::vector<StreetSegmentIdx> major_roads;
    extern std::vector<StreetSegmentIdx> minor_roads;
    extern std::map<int, FeatureIdx> feature_areas;
    extern std::vector<std::pair<float, float>> feature_centroids;
    extern std::vector<StreetSegmentIdx> pathWay;
    extern std::vector<IntersectionIdx> commonInters;
    extern std::vector<intersection_data> intersections;
    extern std::vector<POI_data> POIs;
    
    /*
    * General global variables: Used mostly when data needs to be shared
    * between varying scopes. Used in more limited scopes compared to the 
    * data structures above.
    */
    extern bool intersection_zoom;
    extern bool oneWayButton;
    extern bool darkMode;
    extern IntersectionIdx chosenPath;
    extern IntersectionIdx intersectionFrom;
    extern IntersectionIdx intersectionTo;
    extern bool searchByClick;
    extern bool mulIntersect1;
    extern bool mulIntersect2;
    extern bool mapClicked;
    extern IntersectionIdx partial_intersection;
    extern IntersectionIdx intersectionOne;
    extern IntersectionIdx intersectionTwo;
    extern IntersectionIdx intA;
    extern IntersectionIdx intB;
    
    extern double avg_lat;
    extern double max_lat;
    extern double min_lat;
    extern double max_lon;
    extern double min_lon;
    extern float maxSpeedLimit;
