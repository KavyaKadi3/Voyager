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
#include <iostream>
#include <string>

#include "m1.h"
#include "m2.h"
#include "OSMDatabaseAPI.h"

//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

//The default map to load if none is specified
std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
// The start routine of your program (main) when you are running your standalone
// mapper program. This main routine is *never called* when you are running 
//// ece297exercise (the unit tests) -- those tests have their own main routine
// and directly call your functions in /libstreetmap/src/ to test them.
// Don't write any code in this file that you want run by ece297exerise -- it 
// will not be called!
int main(int argc, char** argv) {

std::cout << "Welcome to Voyager! " << std::endl;
    std::string map_path;
    
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }

    int mapNameSelection;
    std::cout << "You can select one of the following maps to load (enter the associated number):\n" << std::endl;
    std::cout << " 1. Beijing, China           2. Cairo, Egypt       3. Cape-Town, South Africa  4. Golden-Horseshoe, Canada" << std::endl;
    std::cout << " 5. Hamilton, Canada         6. Hong Kong, China   7. Iceland                  8. Interlaken, Switzerland" << std::endl;
    std::cout << " 9. London, England         10. Moscow, Russia    11. New-Delhi, India        12. New-York, USA" << std::endl;
    std::cout << "13. Rio-De-Janeiro, Brazil  14. Saint-Helena      15. Singapore               16. Sydney, Australia" << std::endl;
    std::cout << "17. Tehran, Iran            18. Tokyo, Japan      19. Toronto, Canada" << std::endl;
    std::cout<< " " << std::endl;
    std::cout << "Enter the number of map you would like to load: " << std::endl;
    std::cin >> mapNameSelection;
    
    if (mapNameSelection == 1) {
        map_path = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
    }
    else if (mapNameSelection == 2) {
        map_path = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
    }
    else if (mapNameSelection == 3) {
        map_path = "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";
    }
    else if (mapNameSelection == 4) {
        map_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
    }
    else if (mapNameSelection == 5) {
        map_path = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
    }
    else if (mapNameSelection == 6) {
        map_path = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
    }
    else if (mapNameSelection == 7) {
        map_path = "/cad2/ece297s/public/maps/iceland.streets.bin";
    }
    else if (mapNameSelection == 8) {
        map_path = "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin";
    }
    else if (mapNameSelection == 9) {
        map_path = "/cad2/ece297s/public/maps/london_england.streets.bin";
    }
    else if (mapNameSelection == 10) {
        map_path = "/cad2/ece297s/public/maps/moscow_russia.streets.bin";
    }
    else if (mapNameSelection == 11) {
        map_path = "/cad2/ece297s/public/maps/new-delhi_india.streets.bin";
    }
    else if (mapNameSelection == 12) {
        map_path = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
    }
    else if (mapNameSelection == 13) {
        map_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
    }
    else if (mapNameSelection == 14) {
        map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
    }
    else if (mapNameSelection == 15) {
        map_path = "/cad2/ece297s/public/maps/singapore.streets.bin";
    }
    else if (mapNameSelection == 16) {
        map_path = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
    }
    else if (mapNameSelection == 17) {
        map_path = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
    }
    else if (mapNameSelection == 18) {
        map_path = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
    }
    else if (mapNameSelection == 19) {
        map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
    }    
    else{
      std::cout << "Invalid number, default map will now be loaded (Toronto, Canada) " << std::endl;
    }
    
    //Load the map and related data structures
    
    bool load_success = loadMap(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return ERROR_EXIT_CODE;
    }
   drawMap();
    std::cout << "Successfully loaded map '" << map_path << "'\n";

    //You can now do something with the map data


    //Clean-up the map data and related data structures
    std::cout << "Closing map\n";
    closeMap();

    return SUCCESS_EXIT_CODE;
    
}


