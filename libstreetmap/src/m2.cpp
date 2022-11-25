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

// * GLOBAL VARIABLE DECLARATIONS

bool intersection_zoom = false;
bool oneWayButton = false;
bool darkMode = false;
IntersectionIdx chosenPath = 0;
IntersectionIdx intersectionFrom = 0;
IntersectionIdx intersectionTo = 0;
bool searchByClick = false;
bool mulIntersect1 = false;
bool mulIntersect2 = false;
bool mapClicked = false;
IntersectionIdx partial_intersection = 0;
IntersectionIdx intersectionOne = 0;
IntersectionIdx intersectionTwo = 0;
IntersectionIdx intA = 0;
IntersectionIdx intB = 0;
std::vector<StreetSegmentIdx> pathWay;

void drawMap() {

    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},
    {
        x_from_lon(max_lon), y_from_lat(max_lat)
    });
    //Displaying the entirety of the map with the min/max lat and lon coordinates
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, ezgl::LIGHT_GREY);

    application.run(initial_setup, act_on_mouse_click, NULL, NULL);


}

void on_dialog_response(GtkDialog *dialog, gint response_id) {

    switch (response_id) {
        case GTK_RESPONSE_ACCEPT:
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            break;
        case GTK_RESPONSE_REJECT:
            break;
        default:
            break;
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));
}
//*********** ALL BUTTON FUNCTIONS ***********//

void zoom_to_intersection_button(GtkWidget */*widget*/, ezgl::application *application) {
    intersection_zoom = true; //determines tat the button is clicked, and so the next intersection clicked will be zoomed in
    application->refresh_drawing();
    application->update_message("Zoom to intersection is now activated. Click anywhere on the map to zoom right into the closest intersection.");

}

void oneWay_button(GtkWidget */*widget*/, ezgl::application *application) {
    if (oneWayButton == true) { //if the button is clicked after the one way streets are already displayed,
        oneWayButton = false; //the one way streets arrows will be removed
    } else if (oneWayButton == false) {
        oneWayButton = true; //activates the one way arrows onto the map
        application->update_message("One way streets are now being shown. Click on the 'Show One Way Streets' button again to remove them from the map.");

    }
    application->refresh_drawing();
}

void dark_mode_button(GtkWidget */*widget*/, ezgl::application *application) {
    if (darkMode == false) { //turning on dark mode
        application->update_message(" 'Dark Mode' is now activated. Click on the 'Dark Mode' button again to switch back into Light Mode.");
        darkMode = true;
    } else if (darkMode == true) { //turning off dark mode 
        application->update_message("Light Mode reactivated.");
        darkMode = false;
    }
    application->refresh_drawing();
}

void help_button(GtkWidget */*widget*/, ezgl::application *application) { //the button that allows instructions to be shown to aid users wjile using the map

    application->update_message(" Information about the map is now being displayed.");


    std::string popUpBoxTitle = "More Information";
    std::string displayMessage = "\nUse the arrows and zoom buttons on the right side of your screen to navigate through the map."
            "\n\nYou may also click directly onto the map at any point to locate the closest intersection to the point clicked."
            "\nInformation such as the streets that intersect there,and its exact latitude and longitude coordinates will be"
            "\ndisplayed at the bottom of the screen."
            "\n\nClick 'Zoom to Intersection' and then any spot on the map, to zoom directly onto the area of that intersection."
            "\n\nThe 'Show One Way Streets' button displays all streets that are classified as one way(and its one way direction)."
            "\nClick the button again to remove them from the map."
            "\n\nWhen the Point of Interest (POI) locations are displayed with the orange indication dots, click on any of the "
            "\nPOI spots to receive more information about them."
            "\n\nTo find the path between any two intersections, you have two options:\n\n"
            "1:   Write the two streets intersecting at each intersection (separated by a comma) in its corresponding search box,\n"
            "      and click on the 'Path Find: Search' button once you are done."
            "\nExample of an intersection: King Street West, Dowling Avenue\n"
            "2:   Click on the 'Path Find: Click' button, then any two intersections on the map, to display the path between them.\n"
            "\n\nClick on Path Find: Cancel at any time to clear the path displayed on the map.\n"
            " Otherwise, the map will automatically clear the path when a new one is inputted. ";
    popUpBox(application, popUpBoxTitle, displayMessage);
    application->refresh_drawing();
}

void initial_setup(ezgl::application *application, bool /*new_window*/) {

    //creating all the buttons and setting the appropriate order
    application->create_button("Help", 10, help_button);
    application->create_button("Zoom to intersection", 8, zoom_to_intersection_button);
    application->create_button("Show One Way Streets", 9, oneWay_button);
    application->create_button("Path Find: Search", 0, path_find_button);
    application->create_button("Path Find: Click", 0, pathByClick_button);
    application->create_button("Path Find: Cancel", 0, pathCancel_button);
    application->create_button("Dark Mode", 0, dark_mode_button);
}
//*********** ACTS WHEN MOUSE IS CLICKED ***********//

void act_on_mouse_click(ezgl::application* app, GdkEventButton* /*event*/, double x, double y) {
    //determining what needs to be done when the map is clicked
    if (app->get_renderer()->get_visible_world().area() <= (900000)) { //activate the ability of the POI pop up boxes to be shown if the map is zoomed in enough
        poiPopUpBox(app, x, y); //the 'zoomed in' amount is the same as when the POI indication dots are shown
    }
    if (searchByClick == true) { // Path Find: Click is activated, and the points clicked are going to be used to find a path between
        pathByClick(app, x, y);
    }
    intersectionsHighlight(app, x, y); //the intersection closest to a point on the map clicked will be highlighted
    app->refresh_drawing();
}




//*********** DRAWING MAIN CANVAS ***********//

void draw_main_canvas(ezgl::renderer *g) { //drawing the features of the map in its appropriate stacking order, and dependant on the map zoom level
    auto visible = g->get_visible_world();

    darkMode ? g->set_color(ezgl::dm_CANV) : g->set_color(ezgl::LIGHT_GREY); //The colour of the canvas depending on dark mode
    g->fill_rectangle(visible);
    g->set_line_cap(ezgl::line_cap::round);

    float visible_area = g->get_visible_world().area();
    draw_features(g);
    if (visible_area <= 900000) draw_POIs(g);

    if (visible_area <= (4e+06)) draw_minor_roads(g);
    if (visible_area >= 7e+06) {
        draw_major_roads(g, 3);
        draw_highways(g, 6);
    } else if (visible_area >= 900000) {
        draw_major_roads(g, 9.5);
        draw_highways(g, 15);
    } else {
        draw_major_roads(g, 14);
        draw_highways(g, 25);
    }

    draw_intersections(g);
    draw_common_inters(g);
    draw_streets(g, pathWay, 7, ezgl::RED);

    if (pathWay.size() != 0) { //drawing the path between selected intersections

        float xA = x_from_lon(intersections[intA].position.longitude()); //intersection #1 x-coord
        float yA = y_from_lat(intersections[intA].position.latitude()); //intersection #1 y-coord


        float xB = x_from_lon(intersections[intB].position.longitude()); //intersection #2 x-coord
        float yB = y_from_lat(intersections[intB].position.latitude()); //intersection #2 y-coord
        intersections[intB].highlight = false; //removing the highlight block of the clicked on inters

        if (visible_area <= (4e+06)) { //checking the area to render the amount path arrows displayed
            for (int pathWaySeg = 0; pathWaySeg < (pathWay.size() - 1); pathWaySeg++) {

                double xStart = x_from_lon(intersections[getStreetSegmentInfo(pathWay[pathWaySeg ]).from].position.longitude());
                double yStart = y_from_lat(intersections[getStreetSegmentInfo(pathWay[pathWaySeg ]).from].position.latitude());
                double xEnd = x_from_lon(intersections[getStreetSegmentInfo(pathWay[pathWaySeg + 1]).from].position.longitude());
                double yEnd = y_from_lat(intersections[getStreetSegmentInfo(pathWay[pathWaySeg + 1]).from].position.latitude());

                if ((getStreetSegmentInfo(pathWay[pathWaySeg]).numCurvePoints) == 0) {
                    pathArrows(g, xStart, yStart, xEnd, yEnd);
                } else {
                    pathArrowsCurve(g, xStart, yStart, xEnd, yEnd);
                }
            }
        } else {
            for (int pathWaySeg = 0; pathWaySeg < (pathWay.size() - 1); pathWaySeg += 4) {

                double xStart = x_from_lon(intersections[getStreetSegmentInfo(pathWay[pathWaySeg ]).from].position.longitude());
                double yStart = y_from_lat(intersections[getStreetSegmentInfo(pathWay[pathWaySeg ]).from].position.latitude());
                double xEnd = x_from_lon(intersections[getStreetSegmentInfo(pathWay[pathWaySeg + 1]).from].position.longitude());
                double yEnd = y_from_lat(intersections[getStreetSegmentInfo(pathWay[pathWaySeg + 1]).from].position.latitude());

                if ((getStreetSegmentInfo(pathWay[pathWaySeg]).numCurvePoints) == 0) {
                    pathArrows(g, xStart, yStart, xEnd, yEnd);
                } else {
                    pathArrowsCurve(g, xStart, yStart, xEnd, yEnd);
                }
            }
        }
        g->set_font_size(20);
        darkMode ? g->set_color(ezgl::WHITE) : g->set_color(ezgl::BLACK);
        g->draw_text({xA, yA + 30}, "START");
        g->draw_text({xB, yB + 30}, "END");

    }

    if (visible_area <= 1e+06) draw_street_names(g, minor_roads, 1);
    if (visible_area <= 2e+07) {
        draw_street_names(g, major_roads, 2);

    } else if (visible_area <= 7e+06) draw_street_names(g, major_roads, 3);
    if (visible_area <= 7e+07) draw_street_names(g, highways, 4);

    if (visible_area <= (350000)) {
        drawPOInames(g);
        draw_feature_name(g);
    }

}

void draw_street_names(ezgl::renderer *g, std::vector<StreetSegmentIdx> streetsegs, int offset) {
    int textSize = 10;
    if (streetsegs == highways) textSize = 14;
    auto world = g->get_visible_world();
    double angleOfRotation = 0;

    int size = streetsegs.size();

    for (int j = 0; j < size; j += (4 + offset)) {
        std::string streetName = getStreetName(getStreetSegmentInfo(streetsegs[j]).streetID);
        if (streetName != "<unknown>") {

            IntersectionIdx startInter = getStreetSegmentInfo(streetsegs[j]).from;
            IntersectionIdx endInter = getStreetSegmentInfo(streetsegs[j]).to;

            LatLon start = getIntersectionPosition(startInter);
            LatLon end = getIntersectionPosition(endInter);

            double xStart = x_from_lon(start.longitude());
            double yStart = y_from_lat(start.latitude());

            double xEnd = x_from_lon(end.longitude());
            double yEnd = y_from_lat(end.latitude());

            double xDistance = xEnd - xStart;
            double yDistance = yEnd - yStart;

            if (xDistance != 0) {
                angleOfRotation = atan(yDistance / xDistance) / kDegreeToRadian;
            } else {
                angleOfRotation = 90;
            }

            double xPosition = (xStart + xEnd) / 2;
            double yPosition = (yStart + yEnd) / 2;
            g->set_text_rotation(angleOfRotation);
            if (world.contains({xPosition, yPosition})) {
            darkMode ? g->set_color(ezgl::CUSTBLUE) : g->set_color(ezgl::BLACK);

            g->set_font_size(textSize);

            g->draw_text({xPosition + 3, yPosition + 3}, streetName);
        }

        }
    }

    g->set_text_rotation(0);

}

//*********** DRAWING ALL INTERSECTIONS ***********//

void draw_intersections(ezgl::renderer *g) {
    size_t i;
    float x;
    float y;
    float width = 0;
    float height = 0;
    for (i = 0; i < intersections.size(); ++i) { //parse through all intersections looking for the ones that need to be highlighted
        x = x_from_lon(intersections[i].position.longitude());
        y = y_from_lat(intersections[i].position.latitude());


        if (intersections[i].highlight) {
            if (i == intA) {
                g->set_color(ezgl::BLACK);
                g->draw_text({x, y}, "START");
            } else {
                darkMode ? g->set_color(ezgl::dm_INTER) : g->set_color(ezgl::BLACK);

                if (g->get_visible_world().area() >= 2e+08) {
                    width = 250;
                    height = width;
                } else if (g->get_visible_world().area() < 2e+08 && g->get_visible_world().area() >= 4.5e+06) {
                    width = 70;
                    height = width;
                } else if (g->get_visible_world().area() < 4.5e+06 && g->get_visible_world().area() >= 1.7e+06) {
                    width = 40;
                    height = width;
                } else if (g->get_visible_world().area() < 1.7e+06 && g->get_visible_world().area() >= 600000) {
                    width = 30;
                    height = width;
                } else if (g->get_visible_world().area() < 600000 && g->get_visible_world().area() >= 300000) {
                    width = 20;
                    height = width;

                } else if (g->get_visible_world().area() < 300000 && g->get_visible_world().area() >= 80000) {

                    width = 10;
                    height = width;
                } else if (g->get_visible_world().area() < 80000) {
                    width = 5;
                    height = width;
                }

            }
        } else {
            continue;
        }
        g->fill_rectangle({x - width / 2, y - height / 2},
        {
            x + width / 2, y + height / 2
        }); //size of the rectangle highlight

    }

}

void draw_common_inters(ezgl::renderer *g) {
    float width = 0;
    float height = 0;
    for (int i = 0; i < commonInters.size(); i++) {
        float x = x_from_lon(getIntersectionPosition(commonInters[i]).longitude());
        float y = y_from_lat(getIntersectionPosition(commonInters[i]).latitude());
        darkMode ? g->set_color(ezgl::dm_findINTER) : g->set_color(ezgl::RED);

        if (g->get_visible_world().area() >= 2e+08) {
            width = 250;
            height = width;
        } else if (g->get_visible_world().area() < 2e+08 && g->get_visible_world().area() >= 4.5e+06) {
            width = 70;
            height = width;
        } else if (g->get_visible_world().area() < 4.5e+06 && g->get_visible_world().area() >= 1.7e+06) {
            width = 40;
            height = width;
        } else if (g->get_visible_world().area() < 1.7e+06 && g->get_visible_world().area() >= 600000) {
            width = 30;
            height = width;
        } else if (g->get_visible_world().area() < 600000 && g->get_visible_world().area() >= 300000) {
            width = 20;
            height = width;

        } else if (g->get_visible_world().area() < 300000 && g->get_visible_world().area() >= 80000) {

            width = 10;
            height = width;
        } else if (g->get_visible_world().area() < 80000) {
            width = 5;
            height = width;
        }
        g->fill_rectangle({x - width / 2, y - height / 2},
        {
            x + width / 2, y + height / 2
        });
    }
}

//*********** DRAWING ALL FEATURES ***********//

void draw_features(ezgl::renderer *g) {
    auto world = g->get_visible_world();
    auto visible_area = world.area();
    for (auto it = feature_areas.rbegin(); it != feature_areas.rend(); it++) {
        switch (getFeatureType(it->second)) {
            case ISLAND:
                draw_islands(g, it->second);
                break;
            case PARK:
                draw_parks(g, it->second);
                break;
            case GREENSPACE:
                draw_greenspaces(g, it->second);
                break;
            case GOLFCOURSE:
                draw_golfcourses(g, it->second);
                break;
            case LAKE:
                draw_lakes(g, it->second);
                break;
            default:
                break;
        }
    }
    if (visible_area <= (2e+08)) {
        draw_streams(g);
        draw_rivers(g);
    }
    for (int i = 0; i < getNumFeatures(); i++) {
        auto centroid = feature_centroids[i];
        if (getFeatureType(i) == BUILDING && visible_area <= (4e+06) && world.contains(centroid.first, centroid.second)) {
            draw_buildings(g, i);

        } else if (getFeatureType(i) == BEACH && world.contains(centroid.first, centroid.second)) {
            draw_beaches(g, i);
        }
    }
}

void draw_lakes(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            darkMode ? g->set_color(ezgl::dm_LAKE) : g->set_color(ezgl::LIGHT_SKY_BLUE);

            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_buildings(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            darkMode ? g->set_color(ezgl::dm_BUILD) : g->set_color(ezgl::GREY_55);

            g->fill_poly(feature_points[id]);

            g->set_line_width(3);
            darkMode ? g->set_color(ezgl::dm_STROKE) : g->set_color(ezgl::BUILD_STROKE);
            for (int i = 1; i < feature_points[id].size(); i++) {
                g->draw_line(feature_points[id][i - 1], feature_points[id][i]);
            }

        }
    }
}

void draw_beaches(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            g->set_color(ezgl::SAND);
            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_islands(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            darkMode ? g->set_color(ezgl::dm_CANV) : g->set_color(ezgl::GREY_75);
            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_streams(ezgl::renderer *g) {
    for (int i = 0; i < getNumFeatures(); i++) {
        if (getFeatureType(i) == STREAM) {
            if (feature_points[i][0] == feature_points[i][feature_points[i].size() - 1 ]) {
                g->set_color(ezgl::LIGHT_BLUE);
                g->fill_poly(feature_points[i]);
            } else {
                for (int j = 0; j < feature_points[i].size() - 1; j++) {
                    g->set_color(ezgl::LIGHT_BLUE);
                    g->set_line_width(1);
                    g->set_line_cap(ezgl::line_cap::round);
                    g->draw_line(feature_points[i][j], feature_points[i][j + 1]);
                }
            }
        }
    }
}

void draw_rivers(ezgl::renderer *g) {
    for (int i = 0; i < getNumFeatures(); i++) {
        if (getFeatureType(i) == STREAM) {
            if (feature_points[i][0] == feature_points[i][feature_points[i].size() - 1 ]) {
                darkMode ? g->set_color(ezgl::dm_RIV) : g->set_color(ezgl::LIGHT_SKY_BLUE);
                g->fill_poly(feature_points[i]);
            } else {
                for (int j = 0; j < feature_points[i].size() - 1; j++) {
                    darkMode ? g->set_color(ezgl::dm_RIV) : g->set_color(ezgl::LIGHT_SKY_BLUE);
                    g->set_line_width(3);
                    g->set_line_cap(ezgl::line_cap::round);
                    g->draw_line(feature_points[i][j], feature_points[i][j + 1]);
                }
            }
        }
    }
}

void draw_golfcourses(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            g->set_color(ezgl::NOTASDARK_GREEN);
            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_greenspaces(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            g->set_color(ezgl::NOTASDARK_GREEN);
            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_parks(ezgl::renderer *g, FeatureIdx id) {
    if (feature_points[id].size() > 1) {
        if (*(feature_points[id].begin()) == *(feature_points[id].rbegin())) {
            g->set_color(ezgl::DARK_GREEN);
            g->fill_poly(feature_points[id]);
        }
    }
}

void draw_feature_name(ezgl::renderer *g) {
    auto world = g->get_visible_world();
    for (int id = 0; id < getNumFeatures(); id++) {
        if (getFeatureType(id) != BUILDING) {
            auto centroid = feature_centroids[id];
            if (world.contains(centroid.first, centroid.second)) {
                if (getFeatureName(id) != "<noname>") {

                    darkMode ? g->set_color(ezgl::CUSTBLUE) : g->set_color(ezgl::BLACK);

                    g->set_font_size(10);

                    g->draw_text({centroid.first, centroid.second}, getFeatureName(id));
                }
            }
        }
    }
}

//*********** DRAWING POINTS OF INTEREST ***********//

void draw_POIs(ezgl::renderer *g) {
    auto world = g->get_visible_world();
    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        LatLon pos = getPOIPosition(i);
        double x = x_from_lon(pos.longitude());
        double y = y_from_lat(pos.latitude());
        if (world.contains({x, y})) {
        double rad_x = 3;
        double rad_y = rad_x;
        g->set_color(ezgl::CORAL);
        g->fill_elliptic_arc({x, y}, rad_x, rad_y, 0, 360);
    }
    }
}

void drawPOInames(ezgl::renderer *g) {
    auto world = g->get_visible_world();
    for (int individualPOI = 0; individualPOI < POIs.size(); individualPOI++) {

        float x = x_from_lon(getPOIPosition(individualPOI).longitude());
        float y = y_from_lat(getPOIPosition(individualPOI).latitude());

        if (world.contains({x, y})) {
        std::string individualPOIname = POIs[individualPOI].name;
        g->set_font_size(10);
        darkMode ? g->set_color(ezgl::CUSTBLUE) : g->set_color(ezgl::BLACK);
        g->draw_text({x, y + 5}, individualPOIname);
    }
    }
}

//*********** CONVERTING COORDINATE POINTS ***********//

float x_from_lon(float lon) {
    return lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian);

}

float y_from_lat(float lat) {
    return lat * kDegreeToRadian*kEarthRadiusInMeters;
}

float lon_from_x(float x) {
    return x / (kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian));
}

float lat_from_y(float y) {
    return y / (kDegreeToRadian * kEarthRadiusInMeters);
}

//*********** DRAWING DIFFERENT TYPES OF STREETS ***********//

void draw_highways(ezgl::renderer *g, int scale) {
    ezgl::color highwayColour;

    if (darkMode == false) {
        highwayColour = ezgl::HIGHWAY;
    } else {
        highwayColour = ezgl::dm_HIGHWAY;
    }

    draw_streets(g, highways, scale, highwayColour);
}

void draw_major_roads(ezgl::renderer *g, int scale) {

    ezgl::color majorColour;

    if (darkMode == false) {
        majorColour = ezgl::PERI;
    } else {
        majorColour = ezgl::dm_MAJOR;
    }

    draw_streets(g, major_roads, scale, majorColour);
}

void draw_minor_roads(ezgl::renderer *g) {
    ezgl::color minorColour;

    if (darkMode == false) {
        minorColour = ezgl::WHITE;
    } else {
        minorColour = ezgl::dm_MINOR;
    }
    draw_streets(g, minor_roads, 6, minorColour);
}



// Drawing streets (called by different types of roads)

void draw_streets(ezgl::renderer *g, std::vector<StreetSegmentIdx> streetsegs, int width, ezgl::color line_colour) {
    auto world = g->get_visible_world();
    bool oneWay = true;
    for (int i = 0; i < streetsegs.size(); i++) {
        auto seg = streetsegs[i];
        oneWay = getStreetSegmentInfo(seg).oneWay;
        if (getStreetSegmentInfo(seg).numCurvePoints == 0) {
            double xStart = x_from_lon(intersections[getStreetSegmentInfo(seg).from].position.longitude());
            double yStart = y_from_lat(intersections[getStreetSegmentInfo(seg).from].position.latitude());
            double xEnd = x_from_lon(intersections[getStreetSegmentInfo(seg).to].position.longitude());
            double yEnd = y_from_lat(intersections[getStreetSegmentInfo(seg).to].position.latitude());
            if (world.contains({xStart, yStart}) || world.contains({xEnd, yEnd})) {
            g->set_color(line_colour);
            g->set_line_width(width);
            g->draw_line({xStart, yStart},
            {
                xEnd, yEnd
            });

            // calling draw arrows function to indicate one way streets
            draw_arrows(g, xStart, yStart, xEnd, yEnd, oneWay);

        }

        } else {
            for (int j = -1; j < getStreetSegmentInfo(seg).numCurvePoints; j++) {
                if (j == -1) {


                    double xStart = x_from_lon(intersections[getStreetSegmentInfo(seg).from].position.longitude());
                    double yStart = y_from_lat(intersections[getStreetSegmentInfo(seg).from].position.latitude());
                    double xEnd = x_from_lon(getStreetSegmentCurvePoint(seg, 0).longitude());
                    double yEnd = y_from_lat(getStreetSegmentCurvePoint(seg, 0).latitude());
                    if (world.contains({xStart, yStart}) || world.contains({xEnd, yEnd})) {
                    g->set_color(line_colour);
                    g->set_line_width(width);
                    g->draw_line({xStart, yStart},
                    {
                        xEnd, yEnd
                    });


                    draw_arrows_curved_segments(g, xStart, yStart, xEnd, yEnd, oneWay);
                }
                } else {
                    auto first = getStreetSegmentCurvePoint(seg, j);
                    double xStart = x_from_lon(first.longitude());
                    double yStart = y_from_lat(first.latitude());

                    if (j == getStreetSegmentInfo(seg).numCurvePoints - 1) {
                        double xEnd = x_from_lon(intersections[getStreetSegmentInfo(seg).to].position.longitude());
                        double yEnd = y_from_lat(intersections[getStreetSegmentInfo(seg).to].position.latitude());
                        if (world.contains({xStart, yStart}) || world.contains({xEnd, yEnd})) {
                        g->set_color(line_colour);
                        g->set_line_width(width);
                        g->draw_line({xStart, yStart},
                        {
                            xEnd, yEnd
                        });

                    }
                    } else {
                        auto second = getStreetSegmentCurvePoint(seg, j + 1);
                        double xEnd = x_from_lon(second.longitude());
                        double yEnd = y_from_lat(second.latitude());
                        if (world.contains({xStart, yStart}) || world.contains({xEnd, yEnd})) {
                        g->set_color(line_colour);
                        g->set_line_width(width);
                        g->draw_line({xStart, yStart},
                        {
                            xEnd, yEnd
                        });
                    }

                    }
                }
            }
        }
    }
}

//*********** HIGHLIGHTING INTERSECTIONS ***********//

void intersectionsHighlight(ezgl::application* app, double x, double y) {

    LatLon Pos = LatLon(lat_from_y(y), lon_from_x(x));
    int ID = findClosestIntersection(Pos);

    if (intersections[ID].highlight == true) { //if the intersection clicked on has already been clicked on
        intersections[ID].highlight = false; //un-highlight the intersection
    } else {
        for (int i = 0; i < getNumIntersections(); i++) {
            intersections[i].highlight = false;

        }

        if (intersection_zoom == true) { //if zoom to intersection was pressed
            ezgl::rectangle new_world({x - 100, y - 100},
            {
                x + 100, y + 100
            });
            app->get_renderer()->set_visible_world(new_world);
            intersection_zoom = false;
        }

    }

    //    std::cout << "Mouse Clicked at (" << x << "," << y << ")\n";
    //    std::cout << "Closest Intersection: " << intersections[ID].name << "\n";
    std::string intersectionInfo = intersections[ID].name + "       Longitude: " + std::to_string(lon_from_x(x)) + "   Latitude: " + std::to_string(lat_from_y(y));
    app->update_message(intersectionInfo);

    intersections[ID].highlight = true;

}

//*********** DRAWING POP-UP INFORMATION BOX ***********//

void poiPopUpBox(ezgl::application* app, double x, double y) { //pop-up box to display information when a POI is clicked


    float roundedClickedPoiLat = round((lat_from_y(y))*10000) / 10000;
    float roundedClickedPoiLon = round((lon_from_x(x))*10000) / 10000;
    LatLon sel_point(roundedClickedPoiLat, roundedClickedPoiLon);

    for (int i = 0; i < getNumPointsOfInterest(); i++) {

        float roundedPoiLat = round((getPOIPosition(i).latitude())*10000) / 10000;
        float roundedPoiLon = round((getPOIPosition(i).longitude())*10000) / 10000;

        if (roundedClickedPoiLat == roundedPoiLat) {
            if (roundedClickedPoiLon == roundedPoiLon) {
                std::string poiName = getPOIName(i);
                std::string poiType = getPOIType(i);
                std::string poi_inter = getIntersectionName(findClosestIntersection(sel_point));


                std::string popUpBoxTitle = "Point of Interest Information";
                std::string message = "You have clicked on " + poiName + ". It is a " + poiType + " located at latitude: " + std::to_string(roundedPoiLat) + " and longitude: " + std::to_string(roundedPoiLon) + ".\nThe closest intersection to this POI is " + poi_inter + ".";
                popUpBox(app, popUpBoxTitle, message);

                app->refresh_drawing();


            }

        }

    }

}

//*********** DRAWING ARROWS TO INDICATE ONE WAY STREETS ***********//

void draw_arrows(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd, bool oneWay) {
    if (oneWayButton == true) {
        double xDistance = 0;
        double yDistance = 0;
        double angleOfRotation = 0;

        xDistance = xEnd - xStart;
        yDistance = yEnd - yStart;

        // if the street segment is not empty, calculate angle of rotation
        if (xDistance != 0) {
            angleOfRotation = atan(yDistance / xDistance) / kDegreeToRadian;
        } else {
            angleOfRotation = 90;

        }
        // x and y positions of arrow to be placed
        double xArrowPosition = ((xStart + xEnd) / 2) + 1;
        double yArrowPosition = ((yStart + yEnd) / 2) + 1;

        g->set_text_rotation(angleOfRotation);

        if (oneWay) {
            // if the starting point (x-coordinate) is less than ending point, direction is from->to, otherwise vice versa
            if (xStart < xEnd) {
                darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                g->set_font_size(35);
                g->draw_text({xArrowPosition, yArrowPosition}, "→");
            } else if (xStart > xEnd) {
                darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                g->set_font_size(35);
                g->draw_text({xArrowPosition, yArrowPosition}, "←");
            } else {
                // if the starting point (y-coordinate) is less than ending point, direction is from->to, otherwise vice versa
                if (yStart < yEnd) {
                    darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                    g->set_font_size(35);
                    g->draw_text({xArrowPosition, yArrowPosition}, "→");
                } else {
                    darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                    g->set_font_size(35);
                    g->draw_text({xArrowPosition, yArrowPosition}, "←");
                }
            }
        }
        g->set_text_rotation(0);
    }
}

void draw_arrows_curved_segments(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd, bool oneWay) {
    if (oneWayButton == true) {
        double xDistance = 0;
        double yDistance = 0;
        double angleOfRotation = 0;

        // street segment distances
        xDistance = xEnd - xStart;
        yDistance = yEnd - yStart;

        // if the street segment is not empty, calculate angle of rotation
        if (xDistance != 0) {
            angleOfRotation = atan(yDistance / xDistance) / kDegreeToRadian;
            if (angleOfRotation < 0) {
                angleOfRotation += 360;
            }
        } else {
            angleOfRotation = 90;

        }
        // x and y positions of arrow to be placed
        double xArrowPosition = ((xStart + xEnd) / 2) + 1;
        double yArrowPosition = ((yStart + yEnd) / 2) + 1;

        g->set_text_rotation(angleOfRotation);

        double xOffset = cos(atan(yDistance / xDistance)) / 2;
        double yOffset = sin(atan(yDistance / xDistance)) / 2;

        if (oneWay) {
            // if the starting point (x-coordinate) is less than ending point, direction is from->to, otherwise vice versa
            if (xStart < xEnd) {
                darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                g->set_font_size(35);
                g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "→");
            } else if (xStart > xEnd) {
                darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                g->set_font_size(35);
                g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "←");
            } else {
                // if the starting point (y-coordinate) is less than ending point, direction is from->to, otherwise vice versa
                if (yStart < yEnd) {
                    darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                    g->set_font_size(35);
                    g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "→");
                } else {
                    darkMode ? g->set_color(ezgl::TURQUOISE) : g->set_color(ezgl::MAGENTA);
                    g->set_font_size(35);
                    g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "←");
                }
            }
        }
        // resetting text rotation orientation
        g->set_text_rotation(0);
    }
}



//*********** Path Finding ***********//M3//

void pathCancel_button(GtkWidget */*widget*/, ezgl::application *application) { //Clear path shown on the map when button is clicked
    pathWay.clear();

    application->refresh_drawing();
}

void pathByClick_button(GtkWidget */*widget*/, ezgl::application *application) { //Button that allows finding a path by clicking on the map
    searchByClick = true;
    application->update_message("Please click on any two places on the map.");

    std::string popUpBoxTitle = "Path Find: Click ";
    std::string input = "Please click on any two places on the map. Once both are clicked, the path between them\n"
            "will be displayed.";
    popUpBox(application, popUpBoxTitle, input);
    //Creating a pop-up box for instructions to use Path Find: Click

    application->refresh_drawing();
}

void pathByClick(ezgl::application* app, double x, double y) {
    //Assigning the point clicked on the map
    pathWay.clear();
    LatLon Pos = LatLon(lat_from_y(y), lon_from_x(x));


    if (intersectionOne == 0) {
        intersectionOne = findClosestIntersection(Pos);
    } else if (intersectionTwo == 0) {
        intersectionTwo = findClosestIntersection(Pos);

        pathWay = findPathBetweenIntersections(intersectionOne, intersectionTwo, 15.0);
        travel_directions(pathWay,app);
        intA = intersectionOne;
        intB = intersectionTwo;

        float xA = x_from_lon(intersections[intA].position.longitude()); //determining the points for the resizing of the map when path is displayed
        float yA = y_from_lat(intersections[intA].position.latitude());

        float xB = x_from_lon(intersections[intB].position.longitude());
        float yB = y_from_lat(intersections[intB].position.latitude());
        float xMin, xMax, yMin, yMax;

        if (xA < xB) {
            xMin = xA;
            xMax = xB;
        } else {
            xMin = xB;
            xMax = xA;
        }
        if (yA < yB) {
            yMin = yA;
            yMax = yB;
        } else {
            yMin = yB;
            yMax = yA;
        }
        ezgl::rectangle new_world({xMin - 150, yMin - 150},
        {
            xMax + 150, yMax + 150
        }); //setting the resized window
        app->get_renderer()->set_visible_world(new_world);

        intersections[intA].highlight = false;
        intersections[intB].highlight = false;
        app->update_message("You have successfully clicked on two intersection points.");
        //Notifying the user of the intersection points they clicked on using a pop up box 


        std::string popUpBoxTitle = "Path Find: Click ";
        std::string input = "You have successfully clicked on two intersection points.\n\n"
                " 1: " + intersections[intersectionOne].name + "\n"
                "2: " + intersections[intersectionTwo].name + "\n\nThe path between them is now being displayed.";
        popUpBox(app, popUpBoxTitle, input);


        intersectionOne = 0;
        intersectionTwo = 0;
        searchByClick = false;

    }

    app->refresh_drawing();
}

void path_find_button(GtkWidget */*widget*/, ezgl::application *application) {
    //Finding the path between two intersections by using the search bars to input them

    GtkEntry* firstEntry = (GtkEntry *) application->get_object("FirstInput");
    GtkEntry* secondEntry = (GtkEntry *) application->get_object("SecondInput");

    std::string firstInput = gtk_entry_get_text(firstEntry); //Retrieving the first entry
    std::string secondInput = gtk_entry_get_text(secondEntry); //Retrieving the second entry

    std::stringstream inputOne(firstInput);
    std::stringstream inputTwo(secondInput);

    std::string IntOneStreetOne;
    std::string IntOneStreetTwo;
    std::string IntTwoStreetOne;
    std::string IntTwoStreetTwo;

    //Separating the entries in each of the search bars with the comma that separates the street inputs:
    std::getline(inputOne, IntOneStreetOne, ',');
    std::getline(inputOne, IntOneStreetTwo);
    std::getline(inputTwo, IntTwoStreetOne, ',');
    std::getline(inputTwo, IntTwoStreetTwo);

    if (firstInput.empty() || secondInput.empty()) {
        application->update_message("Please enter two street names in each entry field, separated by a comma before clicking on \'PathFinder\'.");

        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Whoops!! "
                "\n\nYou have clicked on pathFinder before filling out both input boxes with street names."
                "\nPlease try again and fill out both boxes first before pressing 'Path Find: Search'.";
        popUpBox(application, popUpBoxTitle, input);

        return;
    }
    if (IntOneStreetTwo.empty()) {
        invalidIntersection(application, 1);
        return;
    }
    if (IntOneStreetOne.empty()) {
        invalidIntersection(application, 1);
        return;
    }
    if (IntTwoStreetTwo.empty()) {
        invalidIntersection(application, 2);
        return;
    }
    if (IntTwoStreetOne.empty()) {
        invalidIntersection(application, 2);
        return;
    }


    std::vector<StreetIdx> firstVec = findStreetIdsFromPartialStreetName(IntOneStreetOne);
    std::vector<StreetIdx> secondVec = findStreetIdsFromPartialStreetName(IntOneStreetTwo);
    std::vector<StreetIdx> thirdVec = findStreetIdsFromPartialStreetName(IntTwoStreetOne);
    std::vector<StreetIdx> fourthVec = findStreetIdsFromPartialStreetName(IntTwoStreetTwo);

    //Creating error pop-up boxes
    if ((IntOneStreetOne == IntOneStreetTwo) && IntTwoStreetOne == IntTwoStreetTwo) {
        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Both street name inputs for intersection #1 are exactly the same,\n"
                "and both street name inputs for intersection #2 are exactly the\nsame.\nPlease specify your input.";
        popUpBox(application, popUpBoxTitle, input);
        return;
    }
    if (IntOneStreetOne == IntOneStreetTwo) {
        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Both street name inputs for intersection #1 are exactly the same. Please specify your input.";
        popUpBox(application, popUpBoxTitle, input);
        return;
    }
    if (IntTwoStreetOne == IntTwoStreetTwo) {
        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Both street name inputs for intersection #2 are exactly the same. Please specify your input.";
        popUpBox(application, popUpBoxTitle, input);
        return;
    }

    if (firstVec.size() == 0 || secondVec.size() == 0 || thirdVec.size() == 0 || fourthVec.size() == 0) {
        application->update_message("Invalid name(s)/Street(s) not found. Ensure that street names match an existing street name or the start of one.");

        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Invalid name(s)/Street(s) not found. Ensure that street names\nmatch an existing street name or the start of one.";
        popUpBox(application, popUpBoxTitle, input);

        return;
    }

    StreetIdx firstID = (firstVec)[0];
    StreetIdx secondID = (secondVec)[0];
    StreetIdx thirdID = (thirdVec)[0];
    StreetIdx fourthID = (fourthVec)[0];

    std::pair<int, int> fromStreetIDs;
    fromStreetIDs = std::make_pair(firstID, secondID);
    std::vector<int> intersFrom = findIntersectionsOfTwoStreets(fromStreetIDs);
    if (intersFrom.size() != 0) {
        intersectionFrom = intersFrom[0];
    } else {
        partialStreetSearch(firstVec, secondVec);
        intersectionFrom = partial_intersection;

        application->update_message("Your search for intersection #1 matched multiple intersections.");

        if (intersectionFrom != 0) {
            mulIntersect1 = true;
        } else if (intersectionFrom == 0) {
            application->update_message("Invalid intersection #1 Input");

            invalidIntersection(application, 1);
            return;
        }

    }

    std::pair<int, int> toStreetIDs;
    toStreetIDs = std::make_pair(thirdID, fourthID);
    std::vector<int> intersTo = findIntersectionsOfTwoStreets(toStreetIDs);
    if (intersTo.size() != 0) {
        intersectionTo = intersTo[0];
    } else {
        application->update_message("Your search for intersection #2 matched multiple intersections.");
        partialStreetSearch(thirdVec, fourthVec);
        intersectionTo = partial_intersection;

        if (intersectionTo != 0) {
            mulIntersect2 = true;
        }

        if (intersectionTo == 0) {
            application->update_message("Invalid intersection #2 Input");

            invalidIntersection(application, 2);
            return;
        }
    }

    if (intersectionFrom != 0 && intersectionTo != 0) {

        application->update_message("You have successfully searched two intersection points.");

        std::string popUpBoxTitle = "Path Find: Search ";
        std::string input = "Now displaying the path between \n\n 1: " + intersections[intersectionFrom].name + "\n2: " + intersections[intersectionTo].name;
        popUpBox(application, popUpBoxTitle, input);


        if ((mulIntersect2 == true) && (mulIntersect1 == true)) { //creating a popUp box to inform user about multiple intersections being found for both inputs
            std::string title = "Path Find: Search";
            std::string message = "Your search for intersection 1 matched multiple intersections. We picked the intersection we\nthought you meant, but if we are wrong please specify your search and click 'Path Find: Search' again.\n\n"
                    "Your search for intersection 2 also matched multiple intersections. We picked the intersection we\nthought you meant, but if we are wrong please specify your search and click 'Path Find: Search' again.";
            popUpBox(application, title, message);
        } else if (mulIntersect2 == true) { //creating a popUp box to inform user about multiple intersections being found for intersection #2 only
            multipleIntersection(application, 2);
            mulIntersect2 = false;
        } else if (mulIntersect1 == true) { //creating a popUp box to inform user about multiple intersections being found for intersection #1 only
            multipleIntersection(application, 1);
            mulIntersect1 = false;
        }

        intA = intersectionFrom;
        intB = intersectionTo;

        float xA = x_from_lon(intersections[intA].position.longitude());
        float yA = y_from_lat(intersections[intA].position.latitude());

        float xB = x_from_lon(intersections[intB].position.longitude());
        float yB = y_from_lat(intersections[intB].position.latitude());
        float xMin, xMax, yMin, yMax;


        if (xA < xB) {
            xMin = xA;
            xMax = xB;
        } else {
            xMin = xB;
            xMax = xA;
        }
        if (yA < yB) {
            yMin = yA;
            yMax = yB;
        } else {
            yMin = yB;
            yMax = yA;
        }
        ezgl::rectangle new_world({xMin - 150, yMin - 150},
        {
            xMax + 150, yMax + 150
        });
        application->get_renderer()->set_visible_world(new_world);


        pathWay = findPathBetweenIntersections(intersectionFrom, intersectionTo, 15);
        travel_directions(pathWay,application);
    }

    application->refresh_drawing();
}

IntersectionIdx partialStreetSearch(std::vector<StreetIdx> vectorA, std::vector<StreetIdx> vectorB) { //Searching for common intersections between two partial street names
    partial_intersection = 0;
    for (int a = 0; a < vectorA.size(); a++) {
        for (int b = 0; b < vectorB.size(); b++) {
            StreetIdx firstVecID = (vectorA)[a];
            StreetIdx secondVecID = (vectorB)[b];
            std::pair<int, int> partialIDs;
            partialIDs = std::make_pair(firstVecID, secondVecID);

            std::vector<int> intersFound = findIntersectionsOfTwoStreets(partialIDs);
            if (intersFound.size() != 0) {
                partial_intersection = intersFound[0];
                return partial_intersection;
            } else {
                partial_intersection = 0;
            }
        }
    }
    return partial_intersection;
}

void invalidIntersection(ezgl::application *application, int inputNumber) { //Pop-up box for if the inputted intersection is invalid

    std::string popUpBoxTitle = "Path Find: Search";
    std::string input = "Your search entry for intersection " + std::to_string(inputNumber) + " is invalid. Please try again and click 'Path Find: Search'.";
    popUpBox(application, popUpBoxTitle, input);


}

void multipleIntersection(ezgl::application *application, int inputNumber) { //Pop-up box for if the inputted intersection has multiple results
    std::string popUpBoxTitle = "Path Find: Search";
    std::string input = "Your search for intersection " + std::to_string(inputNumber) + " matched multiple intersections. We picked the intersection we\nthought you meant, but if we are wrong please specify your search and click 'Path Find: Search' again.";

    popUpBox(application, popUpBoxTitle, input);
}

void popUpBox(ezgl::application *application, std::string title, std::string display) {
    GObject *window;
    GtkWidget *content_area;
    GtkWidget *label;

    GtkWidget *dialog;
    window = application->get_object(application->get_main_window_id().c_str());

    const char * titleInput = title.c_str();
    dialog = gtk_dialog_new_with_buttons(titleInput, (GtkWindow*) window, GTK_DIALOG_MODAL, (" OK"), GTK_RESPONSE_ACCEPT, ("CANCEL"), GTK_RESPONSE_REJECT, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    const char * displayMessage = display.c_str();
    label = gtk_label_new(displayMessage);
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(dialog);

    g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), NULL);
}

void pathArrows(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd) { //Arrows(for path) that will be printed on segments without curve points
    double xDistance = 0;
    double yDistance = 0;
    double angleOfRotation = 0;

    xDistance = xEnd - xStart;
    yDistance = yEnd - yStart;

    // if the street segment is not empty, calculate angle of rotation
    if (xDistance != 0) {
        angleOfRotation = atan(yDistance / xDistance) / kDegreeToRadian;
    } else {
        angleOfRotation = 90;

    }
    // x and y positions of arrow to be placed
    double xArrowPosition = ((xStart + xEnd) / 2);
    double yArrowPosition = ((yStart + yEnd) / 2);


    g->set_text_rotation(angleOfRotation);

    // if the starting point (x-coordinate) is less than ending point, direction is from->to, otherwise vice versa
    if (xStart < xEnd) {
        g->set_color(ezgl::WHITE);
        g->set_font_size(35);
        g->draw_text({xArrowPosition, yArrowPosition}, "→");
    } else if (xStart > xEnd) {
        g->set_color(ezgl::WHITE);
        g->set_font_size(35);
        g->draw_text({xArrowPosition, yArrowPosition}, "←");
    } else {
        // if the starting point (y-coordinate) is less than ending point, direction is from->to, otherwise vice versa
        if (yStart < yEnd) {
            g->set_color(ezgl::WHITE);
            g->set_font_size(35);
            g->draw_text({xArrowPosition, yArrowPosition}, "→");
        } else {
            g->set_color(ezgl::WHITE);
            g->set_font_size(35);
            g->draw_text({xArrowPosition, yArrowPosition}, "←");
        }
    
    

    }
    g->set_text_rotation(0);
}

void pathArrowsCurve(ezgl::renderer *g, double xStart, double yStart, double xEnd, double yEnd) { //arrows(for path) when street segments contain curve points
    double xDistance = 0;
    double yDistance = 0;
    double angleOfRotation = 0;

    // street segment distances
    xDistance = xEnd - xStart;
    yDistance = yEnd - yStart;

    // if the street segment is not empty, calculate angle of rotation
    if (xDistance != 0) {
        angleOfRotation = atan(yDistance / xDistance) / kDegreeToRadian;
        if (angleOfRotation < 0) {
            angleOfRotation += 360;
        }
    } else {
        angleOfRotation = 90;

    }
    // x and y positions of arrow to be placed
    double xArrowPosition = ((xStart + xEnd) / 2);
    double yArrowPosition = ((yStart + yEnd) / 2);

    g->set_text_rotation(angleOfRotation);

    double xOffset = cos(atan(yDistance / xDistance)) / 2;
    double yOffset = sin(atan(yDistance / xDistance)) / 2;

    // if the starting point (x-coordinate) is less than ending point, direction is from->to, otherwise vice versa
    if (xStart < xEnd) {
        darkMode ? g->set_color(ezgl::WHITE) : g->set_color(ezgl::WHITE);
        g->set_font_size(35);
        g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "→");
    } else if (xStart > xEnd) {
        g->set_color(ezgl::WHITE);
        g->set_font_size(35);
        g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "←");
    } else {
        // if the starting point (y-coordinate) is less than ending point, direction is from->to, otherwise vice versa
        if (yStart < yEnd) {
            g->set_color(ezgl::WHITE);
            g->set_font_size(35);
            g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "→");
        } else {
            g->set_color(ezgl::WHITE);
            g->set_font_size(35);
            g->draw_text({xArrowPosition + xOffset, yArrowPosition + yOffset}, "←");
        }
    }
    // resetting text rotation orientation
    g->set_text_rotation(0);
}