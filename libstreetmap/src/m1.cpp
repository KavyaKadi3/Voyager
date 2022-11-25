/* 
 * Copyright 2021 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

// * GLOBAL VARIABLE DECLARATIONS

std::vector<StreetIdx> streets;
std::vector<std::vector<StreetSegmentIdx>> intersection_street_segments;
std::vector<std::vector<StreetSegmentIdx>> outgoing_street_segments;
std::vector<std::vector<StreetSegmentIdx>> streets_street_segments;
std::vector<std::vector<IntersectionIdx>> streets_intersections;
std::vector<std::vector<LatLon>> segment_curve_points;
std::vector<std::vector<LatLon>> street_curve_points;
std::vector<std::pair<std::string, StreetIdx>> sorted_names;
std::multimap<std::string, StreetIdx> names_map;
std::vector<double> seg_travel_time;
std::vector<std::vector<ezgl::point2d>> feature_points;
std::unordered_map<OSMID, const OSMWay *> ways_map;
std::unordered_map<OSMID, std::string> street_attributes;
std::vector<StreetSegmentIdx> highways;
std::vector<StreetSegmentIdx> major_roads;
std::vector<StreetSegmentIdx> minor_roads;
std::map<int, FeatureIdx> feature_areas;
std::vector<std::pair<float, float>> feature_centroids;
std::vector<IntersectionIdx> commonInters;
std::vector<intersection_data> intersections;
std::vector<POI_data> POIs;

float maxSpeedLimit = 0;
double avg_lat;
double max_lat;
double min_lat;
double max_lon;
double min_lon;

// loadMap will be called with the name of the file that stores the "layer-2"
// map data accessed through StreetsDatabaseAPI: the street and intersection
// data that is higher-level than the raw OSM data).
// This file name will always end in ".streets.bin" and you
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 (StreetsDatabase) API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the
// name of the ".osm.bin" file that matches your map -- just change
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.

bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    std::string OSMDB_filename = std::regex_replace(map_streets_database_filename, std::regex(".streets"), ".osm");
    loadOSMDatabaseBIN(OSMDB_filename);
    if (!load_successful)
        return load_successful;
    //Indicates whether the map has loaded successfully from the database file,
    //returns false if it was not.

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    // Pre-loads all data structures

    loadStreets();

    loadIntersStreetSegs();
    loadIntersDataVec();
    loadOutStreetSegs();
    loadStreetsStreetSegs();
    loadSegmentCurves();
    loadStreetCurves();
    loadStreetsInters();
    loadFeaturePoints();

    loadNamesVec();
    loadNamesMap();
    loadSegTime();
    loadWaysMaps();
    loadAreaMap();
    loadFeatureCentroids();
    categorizeStreetSegs();

    for (int i = 0; i < NUM_SEGS; i++) {
        auto speedLim = getStreetSegmentInfo(i).speedLimit;
        if (speedLim > maxSpeedLimit) {
            maxSpeedLimit = speedLim;
        }
    }

    return load_successful;
}

void closeMap() {
    //Clean-up your map related data structures here
    streets.clear();

    intersection_street_segments.clear();
    streets_street_segments.clear();
    streets_intersections.clear();

    segment_curve_points.clear();
    street_curve_points.clear();

    sorted_names.clear();
    names_map.clear();
    feature_points.clear();

    seg_travel_time.clear();
    intersections.clear();
    POIs.clear();
    ways_map.clear();
    street_attributes.clear();
    highways.clear();
    major_roads.clear();
    minor_roads.clear();
    feature_centroids.clear();
    outgoing_street_segments.clear();
    //interNodes.clear();

    closeOSMDatabase();
    closeStreetDatabase();
}

std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id) {
    return intersection_street_segments[intersection_id];
    //intersection_street_segments is a vector that contains the street segments connected to each intersection
    //we use this vector with the intersection ID we are interested in to find the street segments
}

std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id) {
    std::vector<StreetSegmentIdx> curr_inters = intersection_street_segments[intersection_id];

    std::vector<std::string> names;

    for (int num = 0; num < getNumIntersectionStreetSegment(intersection_id); num++) {
        std::string streetName = getStreetName(getStreetSegmentInfo(curr_inters[num]).streetID);
        names.push_back(streetName);
    }

    return names;
}

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points) {
    double firstLat = (points.first.latitude()) * (kDegreeToRadian); //Retrieving the latitude of first point, convert into radians
    double secondLat = points.second.latitude() * (kDegreeToRadian); //Retrieving the latitude of second point, convert into radians
    double firstLong = (points.first.longitude() * (kDegreeToRadian)); //Retrieving the longitude of first point, convert into radians
    double secondLong = (points.second.longitude() * (kDegreeToRadian)); //Retrieving the longitude of second point, convert into radians

    double firstCoordX = (firstLong * (cos((firstLat + secondLat) / 2))); //Calculating the first x-coordinate of the first point
    double secondCoordX = (secondLong * (cos((firstLat + secondLat) / 2))); //Calculating the second x-coordinate of the first point

    double distance = kEarthRadiusInMeters * (sqrt(((secondLat - firstLat) * (secondLat - firstLat)) + ((secondCoordX - firstCoordX) * (secondCoordX - firstCoordX))));

    return distance;
}

double findStreetSegmentLength(StreetSegmentIdx street_segment_id) {
    StreetSegmentInfo segmentInfo = getStreetSegmentInfo(street_segment_id);
    double streetSegmentLength = 0;

    //initializing starting and ending points of street segment
    LatLon startingP, endingP;
    startingP = getIntersectionPosition(segmentInfo.from);
    endingP = getIntersectionPosition(segmentInfo.to);
    int numOfCurves = segmentInfo.numCurvePoints;
    LatLon pointOfCurve;

    //scenario where there is minimum 1 curve point in segment
    if (numOfCurves != 0) {
        for (int i = 0; i < numOfCurves; i++) {
            //obtaining the curve point in segment
            pointOfCurve = getStreetSegmentCurvePoint(street_segment_id, i);

            //startToCurve length = previous segment length + distance between old segment and curve
            std::pair<LatLon, LatLon> startToCurve(startingP, pointOfCurve);
            streetSegmentLength += findDistanceBetweenTwoPoints(startToCurve);

            //curve point becomes the new starting point
            startingP = pointOfCurve;
        }
        std::pair<LatLon, LatLon> curveToEnd(pointOfCurve, endingP);
        //summing up segment length and distance between curve and last point
        streetSegmentLength += findDistanceBetweenTwoPoints(curveToEnd);
        return streetSegmentLength;
    }
        //scenario where there are 0 curves in segment
    else {
        //segment length = distance between starting and ending points
        streetSegmentLength = findDistanceBetweenTwoPoints(std::make_pair(startingP, endingP));
        return streetSegmentLength;
    }
}

double findStreetLength(StreetIdx street_id) {
    std::vector<StreetSegmentIdx> totalStreetSegments;
    //defining totalStreetSegments as total street segments in a certain street
    totalStreetSegments = streets_street_segments[street_id];
    double streetLength = 0;
    //finding total number of street segments in a street by checking the streetSegments vector size
    int totalNumOfSegments = streets_street_segments[street_id].size();
    //iterating through total number of street segments to cumulatively add one street segment length to another
    //in one street until you find entire street length
    for (int j = 0; j < totalNumOfSegments; j++) {
        streetLength += findStreetSegmentLength(totalStreetSegments[j]);
    }
    return streetLength;
}

double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id) {
    return seg_travel_time[street_segment_id];
}

IntersectionIdx findClosestIntersection(LatLon my_position) {
    double closestIntersectionDistance = 100000000;
    int intersectionID = 0;
    // searching through all intersections
    for (int i = 0; i < NUM_INTS; i++) {
        //getting distance between my position and intersection position
        double distanceBetween = findDistanceBetweenTwoPoints(std::make_pair(my_position, getIntersectionPosition(i)));
        //comparing the distance found to the previous closest distance to see if it is closer
        if (distanceBetween < closestIntersectionDistance) {
            //if condition is satisfied, the distance found will become the new closest distance
            closestIntersectionDistance = distanceBetween;
            intersectionID = i;
        }
    }
    return intersectionID;
}

POIIdx findClosestPOI(LatLon my_position, std::string POIname) {
    double closestDistance = 10000000;
    int closestPOIid = 0;
    // searching through all points of interests
    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        //checks to see if the POI is the one we are looking for
        if (getPOIName(i) == POIname) {
            LatLon POIposition = getPOIPosition(i);
            double distanceBetween = findDistanceBetweenTwoPoints(std::make_pair(my_position, POIposition));
            //getting distance between my position and POI
            if (distanceBetween < closestDistance) {
                closestDistance = distanceBetween;
                closestPOIid = i;
            }
        }
    }
    return closestPOIid;
}

std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id) {
    //vector returns all intersections containing the street id specified
    return streets_intersections[street_id];
}

std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids) {

    // Creates vectors from the streets_intersections vector
    std::vector<int> firstStreet = streets_intersections[street_ids.first];
    std::vector<int> secondStreet = streets_intersections[street_ids.second];

    std::vector<int> twoStreetIntersections;

    // Searches through the second vector for any entries matching the first.
    // Any found matches is pushed into the twoStreetIntersections vector.
    for (std::vector<int>::iterator it = firstStreet.begin(); it != firstStreet.end(); ++it) {
        if (std::find(secondStreet.begin(), secondStreet.end(), *it) != secondStreet.end()) {
            twoStreetIntersections.push_back(*it);
        }
    }

    return twoStreetIntersections;
    //returning the vector that contains all the intersections where the two streets meet
}

LatLonBounds findStreetBoundingBox(StreetIdx street_id) {
    LatLonBounds boundingBox;
    float minLat = 1000000000;
    float minLong = 1000000000;
    float maxLat = -1000000000;
    float maxLong = -1000000000;

    for (int i = 0; i < streets_intersections[street_id].size(); i++) {
        //parsing through all intersections that contain the given street ID
        // to find the intersections with the highest/lowest longitude and latitude
        if (getIntersectionPosition(streets_intersections[street_id][i]).latitude() < minLat) {
            minLat = getIntersectionPosition(streets_intersections[street_id][i]).latitude();
        }
        if (getIntersectionPosition(streets_intersections[street_id][i]).longitude() < minLong) {
            minLong = getIntersectionPosition(streets_intersections[street_id][i]).longitude();
        }
        if (getIntersectionPosition(streets_intersections[street_id][i]).latitude() > maxLat) {
            maxLat = getIntersectionPosition(streets_intersections[street_id][i]).latitude();
        }
        if (getIntersectionPosition(streets_intersections[street_id][i]).longitude() > maxLong) {
            maxLong = getIntersectionPosition(streets_intersections[street_id][i]).longitude();
        }
    }
    std::vector<StreetSegmentIdx> StreetSegmentsOfTheStreet;
    StreetSegmentsOfTheStreet = streets_street_segments[street_id];

    for (int a = 0; a < StreetSegmentsOfTheStreet.size(); a++) {

        StreetSegmentIdx ss_id = StreetSegmentsOfTheStreet[a];

        for (int b = 0; b < getStreetSegmentInfo(ss_id).numCurvePoints; b++) {
            //going through all of the curve points in each street segment with the given street ID
            //looking for any that may have a higher/lower longitude and latitude than the values found for the intersections
            LatLon specific_curve_point = getStreetSegmentCurvePoint(ss_id, b);

            if (specific_curve_point.latitude() < minLat) {
                minLat = specific_curve_point.latitude();
            }
            if (specific_curve_point.latitude() > maxLat) {
                maxLat = specific_curve_point.latitude();
            }
            if (specific_curve_point.longitude() < minLong) {
                minLong = specific_curve_point.longitude();
            }
            if (specific_curve_point.longitude() > maxLong) {
                maxLong = specific_curve_point.longitude();
            }
        }
    }
    boundingBox.min = LatLon(minLat, minLong); //setting the minimum bound to be the lowest lat and lon found
    boundingBox.max = LatLon(maxLat, maxLong); //setting the maximum bound to be the highest lat and lon found

    return boundingBox;
}

double findFeatureArea(FeatureIdx feature_id) {
    //initializing feature area and obtaining total number of feature points
    double fArea = 0;
    int numOfFeaturePoints = getNumFeaturePoints(feature_id);
    //point 1 is start and point 2 is end of line being drawn to form polygon (if not forming just a line)
    LatLon point1, point2;
    point1 = getFeaturePoint(feature_id, 0);
    point2 = getFeaturePoint(feature_id, numOfFeaturePoints - 1);
    //initializing x and y coordinate vectors (after conversion) & average latitude vector
    std::vector<double> xCoord;
    std::vector<double> yCoord;
    std::vector<double> averageLat;
    //initializing feature point
    LatLon featurePoint;

    //checking if polygon being drawn is just a line (if start and end point are same, then it is not a closed shape)
    //Pseudo-Code: if point1 != point2
    if (point1.longitude() != point2.longitude() || point1.latitude() != point2.latitude()) {
        fArea = 0;
        return fArea;
    } else {
        //find the average value of all the the latitudes (x average)
        for (int i = 0; i < numOfFeaturePoints; i++) {
            featurePoint = getFeaturePoint(feature_id, i);
            double fPointLat = featurePoint.latitude();
            averageLat.push_back(fPointLat);
        }
        //calculating average of latitude vector using formula found on stackoverflow
        //Link to website: https://stackoverflow.com/questions/28574346/find-average-of-input-to-vector-c/28574413
        double avgLatValue = std::accumulate(averageLat.begin(), averageLat.end(), 0.0) / averageLat.size();

        //converting into x and y coordinates
        for (int i = 0; i < numOfFeaturePoints; i++) {
            featurePoint = getFeaturePoint(feature_id, i);
            double m = cos(avgLatValue * kDegreeToRadian);
            double fPointLon = featurePoint.longitude();
            double fPointLat = featurePoint.latitude();
            xCoord.push_back(m * kDegreeToRadian * fPointLon);
            yCoord.push_back(kDegreeToRadian * fPointLat);
        }
    }
    //calculating area of polygon using formula found on geeksforgeeks website
    //Link to website: https://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/
    int j = xCoord.size() - 1;
    for (int i = 0; i < xCoord.size(); i++) {
        //multiply by m*m to get unit of m^2 (SI unit for area)
        fArea += kEarthRadiusInMeters * kEarthRadiusInMeters * (xCoord[i] + xCoord[j]) * (-yCoord[i] + yCoord[j]);
        j = i;
    }
    fArea = abs(fArea * 0.5);
    return fArea;
}

std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id) {

    std::vector<IntersectionIdx> adjacentInters;
    std::vector<StreetSegmentIdx> segments = intersection_street_segments[intersection_id];
    IntersectionIdx adjInt;

    bool unique = true;
    for (int i = 0; i < segments.size(); i++) {
        StreetSegmentInfo seg_info = getStreetSegmentInfo(segments[i]);
        if (seg_info.to == intersection_id) {
            if (!seg_info.oneWay) {
                adjInt = seg_info.from;
            } else {
                continue;
            }
        } else
            adjInt = seg_info.to;

        if (!adjacentInters.empty()) {
            for (int j = 0; j < adjacentInters.size(); j++) {
                if (adjInt == adjacentInters[j]) {
                    unique = false;
                    break;
                } else
                    unique = true;
            }
        }
        if (unique == true) {
            adjacentInters.push_back(adjInt);
        }
    }
    return adjacentInters;
}

std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix) {

    std::vector<StreetIdx> streetsFromPrefix;
    if (street_prefix.length() == 0) {
        streetsFromPrefix.push_back(-1);
        return streetsFromPrefix;
    }

    // Removes all whitespace from the given prefix and sets all the characters
    // in the string to their lowercase alternatives.

    std::string editedName = removeWhiteSpace(street_prefix);
    std::transform(editedName.begin(), editedName.end(), editedName.begin(), [](unsigned char c) {
        return std::tolower(c); });

    int prefixSize = editedName.length();
    std::string prefixLow = editedName;
    std::string prefixHigh = editedName;
    prefixLow[prefixSize - 1]--;
    prefixHigh[prefixSize - 1]++;

    std::multimap<std::string, StreetIdx>::iterator itlow, ithigh;

    itlow = names_map.lower_bound(prefixLow);
    ithigh = names_map.upper_bound(prefixHigh);

    if (itlow != names_map.end() || ithigh != names_map.end()) {
        for (auto it = itlow; it != ithigh; it++) {
            if ((it->first).substr(0, prefixSize) == editedName) {
                streetsFromPrefix.push_back(it->second);
            }
        }
    }
    return streetsFromPrefix;
}

void loadStreets() {
    streets.resize(NUM_STREETS);
    for (StreetIdx num_streets = 0; num_streets < NUM_STREETS; num_streets++) {
        streets[num_streets] = num_streets;
    }
}

void loadIntersStreetSegs() {
    intersection_street_segments.resize(NUM_INTS);

    for (int inters = 0; inters < NUM_INTS; inters++) {
        int numSegs = getNumIntersectionStreetSegment(inters);
        for (int segment = 0; segment < numSegs; segment++) {
            StreetSegmentIdx seg_id = getIntersectionStreetSegment(inters, segment);
            intersection_street_segments[inters].push_back(seg_id);
        }
    }
}
void loadOutStreetSegs() {
    outgoing_street_segments.resize(NUM_INTS);
    for(int inters = 0; inters < NUM_INTS; inters++){
        auto numSegs = getNumIntersectionStreetSegment(inters);
        for(int streetSeg = 0; streetSeg < numSegs; streetSeg++){
            auto segment_info = getStreetSegmentInfo(streetSeg);
            if(segment_info.oneWay){
                if(segment_info.from != inters){
                    continue;
                }
            }
            int to_ID = segment_info.from == inters ? segment_info.to : segment_info.from;
            outgoing_street_segments[inters].push_back(to_ID);
        }
    }

}
void loadIntersDataVec() {
    max_lat = getIntersectionPosition(0).latitude();
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).longitude();
    min_lon = max_lon;
    intersections.resize(NUM_INTS);

    int id;
    for (id = 0; id < NUM_INTS; ++id) {
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);

        max_lat = std::max(max_lat, intersections[id].position.latitude());
        min_lat = std::min(min_lat, intersections[id].position.latitude());
        max_lon = std::max(max_lon, intersections[id].position.longitude());
        min_lon = std::min(min_lon, intersections[id].position.longitude());
    }

    POIs.resize(getNumPointsOfInterest());
    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        POIs[i].name = getPOIName(i);
        POIs[i].type = getPOIType(i);
        POIs[i].position = getPOIPosition(i);
    }
    avg_lat = (min_lat + max_lat) / 2;
}

void loadStreetsStreetSegs() {
    streets_street_segments.resize(NUM_STREETS);
    for (StreetSegmentIdx seg = 0; seg < NUM_SEGS; seg++) {
        StreetIdx seg_streetid = getStreetSegmentInfo(seg).streetID;
        streets_street_segments[seg_streetid].push_back(seg);
    }
}

void loadSegmentCurves() {
    segment_curve_points.resize(NUM_SEGS);
    for (int seg = 0; seg < NUM_SEGS; seg++) {

        StreetSegmentInfo info = getStreetSegmentInfo(seg);

        std::vector<LatLon> curve_points;
        for (int curves = 0; curves < info.numCurvePoints; curves++) {
            curve_points.push_back(getStreetSegmentCurvePoint(seg, curves));
        }
        segment_curve_points[seg] = curve_points;
    }
}

void loadStreetCurves() {
    street_curve_points.resize(NUM_STREETS);
    for (int street = 0; street < NUM_STREETS; street++) {
        std::vector<StreetSegmentIdx> segments = streets_street_segments[street];
        for (int seg = 0; seg < segments.size(); seg++) {
            std::vector<LatLon> curve_points = segment_curve_points[seg];
            auto scp_end = street_curve_points[street].end();
            street_curve_points[street].insert(scp_end, curve_points.begin(), curve_points.end());
        }
    }
}

void loadStreetsInters() {
    streets_intersections.resize(NUM_STREETS);
    for (StreetIdx street_id = 0; street_id < streets.size(); street_id++) {
        std::vector<StreetSegmentIdx> segments = streets_street_segments[street_id];
        std::unordered_set<IntersectionIdx> inters_streets;

        for (int i = 0; i < segments.size(); i++) {
            StreetSegmentInfo info = getStreetSegmentInfo(segments[i]);
            inters_streets.insert(info.from);
            inters_streets.insert(info.to);
        }
        std::vector<IntersectionIdx> intersVec(inters_streets.begin(), inters_streets.end());
        sort(intersVec.begin(), intersVec.end());
        streets_intersections[street_id] = intersVec;
    }
}

void loadNamesVec() {
    sorted_names.resize(NUM_STREETS);
    for (int i = 0; i < NUM_STREETS; i++) {
        std::string editedStreetName = removeWhiteSpace(getStreetName(i));
        std::transform(editedStreetName.begin(), editedStreetName.end(), editedStreetName.begin(), [](unsigned char c) {
            return std::tolower(c); });
        sorted_names[i] = std::pair<std::string, int>(editedStreetName, i);
    }
    std::sort(sorted_names.begin(), sorted_names.end());
}

void loadNamesMap() {
    for (int str = 0; str < NUM_STREETS; str++) {
        names_map.emplace(sorted_names[str].first, sorted_names[str].second);
    }
}

void loadSegTime() {
    seg_travel_time.resize(NUM_SEGS);
    for (int i = 0; i < NUM_SEGS; i++) {
        StreetSegmentInfo segmentInfo = getStreetSegmentInfo(i);
        double segmentSpeed = segmentInfo.speedLimit;
        double segmentDistance = findStreetSegmentLength(i);

        //travel time = distance/speed
        double streetSegmentTravelTime = segmentDistance / segmentSpeed;
        seg_travel_time[i] = streetSegmentTravelTime;
    }
}

void loadFeaturePoints() {
    feature_points.resize(getNumFeatures());
    for (int feature = 0; feature < getNumFeatures(); feature++) {

        std::vector<ezgl::point2d> points;

        for (int pointNum = 0; pointNum < getNumFeaturePoints(feature); pointNum++) {

            LatLon latlonpoint = getFeaturePoint(feature, pointNum);

            double x = x_from_lon(latlonpoint.longitude());
            double y = y_from_lat(latlonpoint.latitude());

            points.push_back({x, y});
        }

        feature_points[feature] = points;
    }
}

void loadWaysMaps() {

    for (int i = 0; i < getNumberOfWays(); i++) {
        //Get each OSMWay for analysis and construction of OSMWays vector.
        const OSMWay *currWay = getWayByIndex(i);

        OSMID wayID = currWay->id();

        std::pair<OSMID, const OSMWay *> OSMPair;

        OSMPair = std::make_pair(wayID, currWay);

        ways_map.insert(OSMPair);
        for (int j = 0; j < getTagCount(currWay); j++) {
            std::pair<std::string, std::string> tagPair = getTagPair(currWay, j);

            if (tagPair.first == "highway") {
                //                if( tagPair.second == "motorway"
                //                   ||tagPair.second == "trunk"
                //                   ||tagPair.second == "primary"
                //                   ||tagPair.second == "secondary"
                //                   ||tagPair.second == "tertiary"
                //                   ||tagPair.second == "unclassified"
                //                   ||tagPair.second == "residential"){

                std::pair<OSMID, std::string> way_attr;
                way_attr = std::make_pair(wayID, tagPair.second);
                //std::cout<<way_attr.first<<", "<<way_attr.second<<"\n";

                street_attributes.insert(way_attr);
                //                 }
            }
        }
    }
}

void loadAreaMap() {
    for (int i = 0; i < getNumFeatures(); i++) {
        int area = findFeatureArea(i);
        std::pair<int, FeatureIdx> areaPair;
        areaPair = std::make_pair(area, i);
        feature_areas.insert(areaPair);
    }
}

//void loadInterNodes(){
//    for(int i = 0; i < NUM_INTS; i++){
//        Node intNode(i, NO_EDGE);
//        interNodes.push_back(intNode);
//    }
//}

void loadFeatureCentroids() {
    feature_centroids.resize(getNumFeatures());
    for (int i = 0; i < getNumFeatures(); i++) {
        auto points = feature_points[i];
        int numP = getNumFeaturePoints(i);
        auto sumx = 0;
        auto sumy = 0;
        for (int j = 0; j < numP; j++) {
            sumx += feature_points[i][j].x;
            sumy += feature_points[i][j].y;
        }
        auto averagex = sumx / numP;
        auto averagey = sumy / numP;
        feature_centroids[i] = {averagex, averagey};
        //ezgl::point2d Point(averagex, averagey);
        //std::pair<FeatureIdx, ezgl::point2d> featCentroid;
    }
}

void categorizeStreetSegs() {
    for (int segID = 0; segID < NUM_SEGS; segID++) {
        OSMID osm_id = getStreetSegmentInfo(segID).wayOSMID;

        if (street_attributes.find(osm_id) != street_attributes.end() && segID != NUM_SEGS - 1) {
            auto itstreet = street_attributes.find(osm_id);
            //std::cout<<segID<<", "<<itstreet->first<<", "<<itstreet->second<<"\n";

            //std::cout<<segID<<", "<<itway->first<<", "<<itway->second<<"\n";
            //
            std::string attribute = itstreet->second;

            if (attribute == "motorway") {
                highways.push_back(segID);
            } else if (attribute == "trunk" || attribute == "primary" || attribute == "secondary") {
                major_roads.push_back(segID);
            } else {
                minor_roads.push_back(segID);
            }
        }
    }
}