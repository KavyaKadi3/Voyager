/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m1decs.h
 * Author: khokha73
 *
 * Created on March 12, 2021, 10:22 PM
 */

#ifndef M1DECS_H
#define M1DECS_H

#include "globals.h"

float x_from_lon(float lon);
float y_from_lat(float lat);
void loadStreets();
void loadIntersStreetSegs();
void loadOutStreetSegs();
void loadIntersDataVec();
void loadStreetsStreetSegs();
void loadSegmentCurves();
void loadStreetCurves();
void loadStreetsInters();
void loadNamesVec();
void loadNamesMap();
void loadSegTime();
void loadFeaturePoints();
void loadWaysMaps();
void loadAreaMap();
void loadInterNodes();
void loadFeatureCentroids();
void categorizeStreetSegs();

#endif /* M1DECS_H */
