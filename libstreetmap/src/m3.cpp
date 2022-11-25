/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "globals.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m1decs.h"
#include "m2decs.h"
#include "m3decs.h"
std::string beginningpath;
std::string directions = "";
std::string directionsPart = "";
std::string pastHolder = "";


bool straightOntoRepeat = false;

double computePathTravelTime(const std::vector<StreetSegmentIdx> &path, const double turn_penalty) {
    double pathTravelTime = 0.0;
    double sizeOfPath = path.size();
    int current_street_ID = getStreetSegmentInfo(path[0]).streetID;

    // in the scenario that the path is empty
    if (sizeOfPath == 0) {
        return pathTravelTime;
    }// in the scenario that the path is not empty
    else {
        for (int i = 0; i < sizeOfPath; i++) {
            int upcoming_street_ID = getStreetSegmentInfo(path[i]).streetID;

            if (current_street_ID != upcoming_street_ID) {
                current_street_ID = upcoming_street_ID;
                pathTravelTime = pathTravelTime + turn_penalty;
            }
            pathTravelTime += findStreetSegmentTravelTime(path[i]);
        }
    }
    return pathTravelTime;
}

void travel_directions(const std::vector<StreetSegmentIdx>& path, ezgl::application *application) {
    double pathTime = computePathTravelTime(path, 15);
    directions = "";
    directionsPart = "";
    double sizeOfPath = path.size();
    int current_street_ID;
    std::string current_street_name;
    int upcoming_street_ID;
    std::string upcoming_street_name;

    // in the scenario that the user chose the same starting and ending points, creating no path
    if (sizeOfPath == 0) {
        directions = "Insufficient path specified. Starting point is same as end point.";
        std::cout << "Insufficient path specified. Starting point is same as end point." << std::endl;
    }        // in the scenario that the user did not select another intersection, hence creating an insufficient path length
    else if (sizeOfPath == 1) {
        std::cout << " " << std::endl;
        std::cout << "Travel Directions:" << std::endl;
        std::cout << " " << std::endl;
        current_street_ID = getStreetSegmentInfo(path[0]).streetID;
        std::string starting_street_name = getStreetName(current_street_ID);

        directions = "Continue straight on " + starting_street_name + " until you arrive at your final destination.";
        std::cout << "Continue straight on " << starting_street_name << " until you arrive at your final destination." << std::endl;
    } else {
        std::cout << " " << std::endl;
        std::cout << "Travel Directions:" << std::endl;
        std::cout << " " << std::endl;

        for (int i = 0; i < (sizeOfPath - 1); i++) {

            // specifying current and upcoming street segment
            auto current_street = path[i];
            auto upcoming_street = path[i + 1];

            // accounting for last segment of path since for loop runs until "sizeOfPath-1" by updating current and upcoming street ID
            if (i == (sizeOfPath - 2)) {
                current_street_ID = path[i + 1];
                upcoming_street_ID = path[i + 2];
            }

            // identifying name and ID of current street segment
            StreetSegmentInfo current_street_info = getStreetSegmentInfo(current_street);
            current_street_ID = getStreetSegmentInfo(current_street).streetID;
            current_street_name = getStreetName(current_street_ID);

            //identifying name and ID of upcoming street segment
            StreetSegmentInfo upcoming_street_info = getStreetSegmentInfo(upcoming_street);
            upcoming_street_ID = getStreetSegmentInfo(upcoming_street).streetID;
            upcoming_street_name = getStreetName(upcoming_street_ID);

            // identifying number of curves in each street segment
            int segment1_curves, segment2_curves;
            segment1_curves = current_street_info.numCurvePoints;
            segment2_curves = upcoming_street_info.numCurvePoints;

            double segment1_curve_bounds = segment1_curves - 1;
            double segment2_curve_bounds = segment2_curves - 1;

            LatLon segment_starting_location, segment_ending_location, streets_intersection;

            IntersectionIdx current_street_start = current_street_info.from;
            IntersectionIdx current_street_end = current_street_info.to;
            IntersectionIdx upcoming_street_start = upcoming_street_info.from;
            IntersectionIdx upcoming_street_end = upcoming_street_info.to;


            ////////////// ENDING OF 1ST SEG = STARTING OF 2ND SEG //////////////        
            if (current_street_end == upcoming_street_start) {

                if (segment1_curves == 0) {
                    segment_starting_location = getIntersectionPosition(current_street_start);
                    streets_intersection = getIntersectionPosition(current_street_end);
                } else {
                    segment_starting_location = getStreetSegmentCurvePoint(current_street, segment1_curve_bounds);
                    streets_intersection = getIntersectionPosition(current_street_end);
                }
                if (segment2_curves == 0) {
                    segment_ending_location = getIntersectionPosition(upcoming_street_end);
                    streets_intersection = getIntersectionPosition(current_street_end);
                } else {
                    segment_ending_location = getStreetSegmentCurvePoint(upcoming_street, 0);
                    streets_intersection = getIntersectionPosition(current_street_end);
                }
            }
                ////////////// STARTING OF 1ST SEG = ENDING OF 2ND SEG //////////////        
            else if (current_street_start == upcoming_street_end) {

                if (segment1_curves == 0) {
                    segment_starting_location = getIntersectionPosition(current_street_end);
                    streets_intersection = getIntersectionPosition(current_street_start);
                } else {
                    segment_starting_location = getStreetSegmentCurvePoint(current_street, 0);
                    streets_intersection = getIntersectionPosition(current_street_start);
                }
                if (segment2_curves == 0) {
                    segment_ending_location = getIntersectionPosition(upcoming_street_start);
                    streets_intersection = getIntersectionPosition(current_street_start);
                } else {
                    segment_ending_location = getStreetSegmentCurvePoint(upcoming_street, segment2_curve_bounds);
                    streets_intersection = getIntersectionPosition(current_street_start);
                }
            }
                ////////////// STARTING OF 1ST SEG = STARTING OF 2ND SEG //////////////        
            else if (current_street_start == upcoming_street_start) {

                if (segment1_curves == 0) {
                    segment_starting_location = getIntersectionPosition(current_street_end);
                    streets_intersection = getIntersectionPosition(current_street_start);
                } else {
                    segment_starting_location = getStreetSegmentCurvePoint(current_street, 0);
                    streets_intersection = getIntersectionPosition(current_street_start);
                }
                if (segment2_curves == 0) {
                    segment_ending_location = getIntersectionPosition(upcoming_street_end);
                    streets_intersection = getIntersectionPosition(current_street_start);
                } else {
                    segment_ending_location = getStreetSegmentCurvePoint(upcoming_street, 0);
                    streets_intersection = getIntersectionPosition(current_street_start);
                }
            }
                ////////////// ENDING OF 1ST SEG = ENDING OF 2ND SEG //////////////        
            else if (current_street_end == upcoming_street_end) {

                if (segment1_curves == 0) {
                    segment_starting_location = getIntersectionPosition(current_street_start);
                    streets_intersection = getIntersectionPosition(current_street_end);
                } else {
                    segment_starting_location = getStreetSegmentCurvePoint(current_street, segment1_curve_bounds);
                    streets_intersection = getIntersectionPosition(current_street_end);
                }
                if (segment2_curves == 0) {
                    segment_ending_location = getIntersectionPosition(upcoming_street_start);
                    streets_intersection = getIntersectionPosition(current_street_end);
                } else {
                    segment_ending_location = getStreetSegmentCurvePoint(upcoming_street, segment2_curve_bounds);
                    streets_intersection = getIntersectionPosition(current_street_end);
                }
            }
            // finding angle between street segments using findAngleBetweenStreets function
            double angle = findAngleBetweenStreets(streets_intersection, segment_starting_location, segment_ending_location);

            // in the scenario that the user needs to keep going straight
            if ((angle >= -190 && angle <= -170) || (angle >= 170 && angle <= 190) || (angle <= 10 && angle >= -10) || (angle >= 350 && angle <= 360) || (angle <= -350 && angle >= -360)) {
                if (i == 0) {
                    directionAngle(streets_intersection, segment_starting_location);
                    directionsPart = "onto " + current_street_name;
                    std::cout << " onto " << current_street_name << std::endl; //Head Onto
                } else if (current_street_ID != upcoming_street_ID) {
                    directionsPart = "Keep continuing straight on " + upcoming_street_name;
                    std::cout << "Keep continuing straight on " << upcoming_street_name << std::endl;

                } else {
                    if (straightOntoRepeat == true) {
                    } else {
                        directionsPart = "Keep continuing straight on " + upcoming_street_name;
                        std::cout << "Keep continuing straight on " << upcoming_street_name << std::endl;
                        straightOntoRepeat = true;
                    }
                }
            }                // defining angle for right turns
            else if ((-350 < angle && angle < -190) || (10 < angle && angle < 170)) {
                if (i == 0) {
                    directionAngle(streets_intersection, segment_starting_location);
                    std::cout << " onto " << current_street_name << std::endl;
                }
                if (current_street_ID != upcoming_street_ID) {
                    directionsPart = "Make a right turn onto " + upcoming_street_name;
                    std::cout << "Make a right turn onto " << upcoming_street_name << std::endl;
                    straightOntoRepeat = false;
                }
            }                // defining angle for left turns
            else if ((-170 < angle && angle < -10) || (190 < angle && angle < 350)) {
                if (i == 0) {
                    directionAngle(streets_intersection, segment_starting_location);
                    std::cout << " onto " << current_street_name << std::endl;
                }
                if (current_street_ID != upcoming_street_ID) {
                    directionsPart = "Make a left turn onto " + upcoming_street_name;
                    std::cout << "Make a left turn onto " << upcoming_street_name << std::endl;
                    straightOntoRepeat = false;
                }
            }
            if (pastHolder == directionsPart) {

            } else {
                directions = directions + "\n" + directionsPart;
            }
            pastHolder = directionsPart;
        }
        std::cout << "You have arrived at your destination!" << std::endl;
        std::cout << "Path Travel Time: " << pathTime << " s" << std::endl;

    }
    pastHolder = "\n\nPath Travel Time: " + std::to_string(pathTime) + " s";
    directions = beginningpath + directions + pastHolder;
    popUpBox(application, "Directions:", directions);
    directions = "";
    directionsPart = "";
    pastHolder = "";
}

double findAngleBetweenStreets(LatLon streets_intersection, LatLon segment_starting_location, LatLon segment_ending_location) {

    double street1_angle, street2_angle, streets_angle, street1_x, street1_y, street2_x, street2_y;

    // streets intersection information 
    double streets_intersection_x = streets_intersection.longitude();
    double streets_intersection_y = streets_intersection.latitude();

    // street 1 information
    double street1_start_x = segment_starting_location.longitude();
    double street1_start_y = segment_starting_location.latitude();

    // street 2 information
    double street2_end_x = segment_ending_location.longitude();
    double street2_end_y = segment_ending_location.latitude();

    // from intersection to each street (1 or 2) respectively
    street1_x = streets_intersection_x - street1_start_x;
    street1_y = streets_intersection_y - street1_start_y;
    street2_x = street2_end_x - streets_intersection_x;
    street2_y = street2_end_y - streets_intersection_y;

    // calculating angle of individual streets
    street1_angle = atan2(street1_y, street1_x);
    double street1_angle_degrees = ((street1_angle)*(180 / M_PI));
    street2_angle = atan2(street2_y, street2_x);
    double street2_angle_degrees = ((street2_angle)*(180 / M_PI));

    // calculating angle between street segments
    streets_angle = (street1_angle_degrees - street2_angle_degrees);

    return streets_angle;
}

void directionAngle(LatLon streets_intersection, LatLon segment_starting_location) {
    double street1_angle, street1_x, street1_y;

    double streets_intersection_x = streets_intersection.longitude();
    double streets_intersection_y = streets_intersection.latitude();

    double street1_start_x = segment_starting_location.longitude();
    double street1_start_y = segment_starting_location.latitude();

    street1_x = streets_intersection_x - street1_start_x;
    street1_y = streets_intersection_y - street1_start_y;

    // calculating angle of starting segment (i.e. street 1)
    street1_angle = atan2(street1_y, street1_x);

    // converting street 1 angle from radians to degrees
    double street1_degrees = ((street1_angle)*(180 / M_PI));

    std::string dirAngle;

    // in the scenario when street 1 angle is positive; defining angle bounds for N, E, W, S
    if (street1_degrees > 0) {
        if ((street1_degrees >= 0 && street1_degrees <= 45) || (street1_degrees > 315 && street1_degrees <= 360)) {
            dirAngle = "East";
        } else if (street1_degrees > 45 && street1_degrees <= 135) {
            dirAngle = "North";
        } else if (street1_degrees > 135 && street1_degrees <= 225) {
            dirAngle = "West";
        } else if (street1_degrees > 225 && street1_degrees <= 315) {
            dirAngle = "South";
        }
    }        // in the scenario when street 1 angle is negative; defining angle bounds for N, E, W, S
    else {
        if ((street1_degrees <= 0 && street1_degrees >= -45) || (street1_degrees < -315 && street1_degrees >= -360)) {
            dirAngle = "East";
        } else if (street1_degrees < -45 && street1_degrees >= -135) {
            dirAngle = "South";
        } else if (street1_degrees < -135 && street1_degrees >= -225) {
            dirAngle = "West";
        } else if (street1_degrees < -225 && street1_degrees >= -315) {
            dirAngle = "North";
        }
        beginningpath = "Head " + dirAngle;
        std::cout << "Head " << dirAngle;
    }
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(
        const IntersectionIdx intersect_id_start,
        const IntersectionIdx intersect_id_destination,
        const double turn_penalty) {
    // Custom comparison sets node with smallest time at the top of the queue.
    std::priority_queue<WaveElem, std::vector<WaveElem>, decltype(&Compare) > waveFront(Compare);
    WaveElem elem(intersect_id_start, 0);
    waveFront.emplace(elem);

    /* Initializing a vector of nodes to be used solely within the scope of
     * this function. Originally, we tried using a global variable, which
     * necessitated resetting the vector after a path was found. This way, no
     * resetting needs to occur. */
    std::vector<Node> nodes;
    nodes.resize(NUM_INTS);

    nodes[intersect_id_start].reachTime = 0;

    while (!waveFront.empty()) {

        WaveElem curr = waveFront.top();
        waveFront.pop();
        int prev_ID = curr.waveID;

        // node->found is used to evaluate whether or not a street has already
        // been checked. The top of the wavefront is always going to have the
        // best time out of those nodes.
        Node *node = &nodes[prev_ID];
        if (prev_ID == intersect_id_destination) {
            node->found = true;
            break;
        }

        if (node->found) {
            continue;
        }
        node->found = true;
        for (int streetSeg : intersection_street_segments[prev_ID]) {
            auto segment_info = getStreetSegmentInfo(streetSeg);
            if (segment_info.oneWay) {
                if (segment_info.from != prev_ID) {
                    continue; //Invalid street, skip.
                }
            }
            int to_ID = segment_info.from == prev_ID ? segment_info.to : segment_info.from;
            Node *curr_seg = &nodes[to_ID];
            if (curr_seg->found) {
                continue; // Already seen, skip.
            }
            double cost = node->reachTime + seg_travel_time[streetSeg];
            if (node->reachingEdge != NO_EDGE) {
                if (getStreetSegmentInfo(node->reachingEdge).streetID != segment_info.streetID) {
                    cost += turn_penalty;
                }
            }
            LatLon currPoint = getIntersectionPosition(to_ID);
            LatLon endPoint = getIntersectionPosition(intersect_id_destination);
            double curr_end_dist = findDistanceBetweenTwoPoints({currPoint, endPoint});
            if (cost < curr_seg->reachTime) {
                double heurPrio = cost + (curr_end_dist / maxSpeedLimit);
                WaveElem tempElem(to_ID, heurPrio);
                waveFront.emplace(tempElem);
                curr_seg->reachTime = cost;
                curr_seg->reachingEdge = streetSeg;
                curr_seg->intID = prev_ID;
            }
        }
    }

    std::vector<StreetSegmentIdx> path;
    Node *node = &nodes[intersect_id_destination];

    while (node->reachingEdge != NO_EDGE) {
        path.push_back(node->reachingEdge);
        node = &nodes[node->intID];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
bool Compare(WaveElem a, WaveElem b) {
    if (a.time > b.time) {
        return true;
    } else {
        return false;
    }
}
