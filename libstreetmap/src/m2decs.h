/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2decs.h
 * Author: khokha73
 *
 * Created on March 12, 2021, 10:19 PM
 */

#ifndef M2DECS_H
#define M2DECS_H

#include "globals.h"

void draw_main_canvas(ezgl::renderer *g);
void act_on_mouse_click(ezgl::application *app, GdkEventButton *event, double x, double y);
void draw_arrows(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd, bool oneWay);
void draw_arrows_curved_segments(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd, bool oneWay);
void on_dialog_response(GtkDialog *dialog, gint response_id);
void initial_setup(ezgl::application *application, bool new_window);
void find_button(GtkWidget * /*widget*/, ezgl::application *application);
void find_cancel_button(GtkWidget * /*widget*/, ezgl::application *application);
void path_find_button(GtkWidget * /*widget*/, ezgl::application *application);
void dark_mode_button(GtkWidget * /*widget*/, ezgl::application *application);
void strokesButton(GtkWidget * /*widget*/, ezgl::application *application);
void oneWay_button(GtkWidget * /*widget*/, ezgl::application *application);
void help_button(GtkWidget * /*widget*/, ezgl::application *application);
void zoom_to_intersection_button(GtkWidget * /*widget*/, ezgl::application *application);
void intersectionsHighlight(ezgl::application *app, double x, double y);
void draw_features(ezgl::renderer *g);
void draw_intersections(ezgl::renderer *g);
void draw_parks(ezgl::renderer *g, FeatureIdx id);
void draw_beaches(ezgl::renderer *g, FeatureIdx id);
void draw_lakes(ezgl::renderer *g, FeatureIdx id);
void draw_rivers(ezgl::renderer *g);
void draw_islands(ezgl::renderer *g, FeatureIdx id);
void draw_buildings(ezgl::renderer *g, FeatureIdx id);
void draw_greenspaces(ezgl::renderer *g, FeatureIdx id);
void draw_golfcourses(ezgl::renderer *g, FeatureIdx id);
void draw_streams(ezgl::renderer *g);
void draw_POIs(ezgl::renderer *g);
void draw_streets(ezgl::renderer *g, std::vector<StreetSegmentIdx> streetsegs, int width, ezgl::color line_colour);
void drawPOInames(ezgl::renderer *g);
void draw_street_names(ezgl::renderer *g, std::vector<StreetSegmentIdx> streetsegs, int offset);
void draw_feature_name(ezgl::renderer *g);
void draw_highways(ezgl::renderer *g, int scale);
void draw_major_roads(ezgl::renderer *g, int scale);
void draw_minor_roads(ezgl::renderer *g);
void checking_type_of_street(ezgl::renderer *g);
void draw_common_inters(ezgl::renderer *g);
void poiPopUpBox(ezgl::application *app, double x, double y);
void pathByClick(ezgl::application *app, double x, double y);
void pathByClick_button(GtkWidget * /*widget*/, ezgl::application *application);
void pathCancel_button(GtkWidget * /*widget*/, ezgl::application *application);
void pathArrows(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd);
void pathArrowsCurve(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd);
float lon_from_x(float x);
float lat_from_y(float y);

void multipleIntersectionsToFound(ezgl::application *application, std::vector<int> inters);
void multipleIntersectionsFromFound(ezgl::application *application, std::vector<int> inters);
void mulIntersectPick(ezgl::application *app, double x, double y);
void invalidIntersection(ezgl::application *application, int inputNumber);
void multipleIntersection(ezgl::application *application, int inputNumber);
void popUpBox(ezgl::application *application, std::string title, std::string display);
IntersectionIdx partialStreetSearch(std::vector<StreetIdx> vectorA, std::vector<StreetIdx> vectorB);

#endif /* M2DECS_H */
