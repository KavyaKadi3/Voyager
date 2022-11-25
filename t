[33mcommit c7189b469e488dfd41fb10ad8a4fd0b7823e6d5b[m[33m ([m[1;36mHEAD -> [m[1;32mmaster[m[33m, [m[1;31morigin/master[m[33m, [m[1;31morigin/HEAD[m[33m)[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Tue Feb 9 21:04:28 2021 -0500

    Modified m1.cpp to add to the findFeatureArea function.

[33mcommit 3eb3b332eb47169500ceddec22b1dca397860ab0[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Tue Feb 9 03:45:18 2021 -0500

    Added (and pre-loaded in loadMap) vectors for street segments, streets, street names, streets and their segments, and street segment curve points. Also updated closeMap to clear all of the pre-loaded data structures.

[33mcommit 0ebc27a43260e7e3617cd79c512bc2362e00df28[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Tue Feb 9 00:40:17 2021 -0500

    Modified m1.cpp to add changes to findStreetLength function and findFeatureArea function.

[33mcommit c31384e3d617f50d4aa3d9ae9b3f9be42a93798a[m
Merge: 7740d80 664459b
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Mon Feb 8 23:11:01 2021 -0500

    Modified m1.cpp to edit findStreetLength function.

[33mcommit 7740d806411e5949dd0d5f39e091a2bdc2c0e6da[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Mon Feb 8 23:09:17 2021 -0500

    Modified m1.cpp to make changes to findStreetLength function.

[33mcommit 664459bacdd86d7a97493b5dd43123994d4612c0[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Sun Feb 7 14:37:31 2021 -0500

    Implemented findStreetNamesOfIntersection. I'll probably end up having to test all or most of my functions at once, unfortunately.

[33mcommit dcddea6b72c7efac59a20e36ff964ac1846af050[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Sun Feb 7 14:34:31 2021 -0500

    Completed function findIntersectionsOfTwoStreets and made changes to findIntersectionsOfStreet to make it faster

[33mcommit ae8c3fbfe7ebd599e02d9216b3ec5e534ce7cb01[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Sat Feb 6 14:04:21 2021 -0500

    Modified m1.cpp by removing uncessary lines of code that was causing errors

[33mcommit f311d8af6270a3248defec09de071f2b1fe63713[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Sat Feb 6 13:57:36 2021 -0500

    Reworked loadMap for intersection_street_segments, implemented closeMap. Will test when I can figure out how to get around Netbeans being uncooperative.

[33mcommit 82a83d879527f068e41d462fe0f047f0ffb3f7e5[m
Merge: deb2832 9659298
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Sat Feb 6 13:42:03 2021 -0500

    Modified m1.cpp file to change findStreetSegmentLength and find streetLength functions.

[33mcommit deb2832236505709975edc7fb44d77d0e2a5a314[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Sat Feb 6 13:30:28 2021 -0500

    Modified m1.cpp file to make changes to and finalize findStreetSegmentLength fucntion & began findStreetLength function.

[33mcommit 9659298a586ebacbc5f2e6796069aed37f0c86ad[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Sat Feb 6 13:27:31 2021 -0500

    Modified m1.cpp by including function definition for findInteresectionsOfStreet.

[33mcommit 243e34cdfd5697dbffb5a912004bf7ac4c43f800[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Fri Feb 5 00:34:01 2021 -0500

    Modified m1.cpp to add the finalized version of the findStreetSegmentTravelTime function.

[33mcommit 20365c07cf142c9d4f53507508f8275373e5356e[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Fri Feb 5 00:11:52 2021 -0500

    Added an else statement to findStreetSegmentLength function to account for possibility of invalid input.

[33mcommit 044381a6d21287b64348f5fbf4c93bc8c48afd96[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Fri Feb 5 00:02:50 2021 -0500

    Modified m1.cpp to add the completed findStreetSegmentLength function.

[33mcommit 16bab4a1bd37d19f0195891f2c11cf46ad0d52f6[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Thu Feb 4 23:26:18 2021 -0500

    Modified loadMap to resize vector upon call and completed findStreetSegmentsOfIntersection. Both need to be tested.

[33mcommit da18e4d72ab6340256c4468824094f31d50e8382[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Thu Feb 4 22:06:35 2021 -0500

    Added conditional logic to loadMap that exits the function immediately and returns false if the map load from the BINunsuccessful.

[33mcommit 65b252da394619a272e5b4f947fd13813ef7b8df[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Thu Feb 4 22:07:32 2021 -0500

    Modified m1.cpp by including definition for findClosestIntersection

[33mcommit 11100fa7ab4a8bf969c27559a0bc77cc2a3d29c5[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Thu Feb 4 11:28:20 2021 -0500

    Completed findClosestPOI function in m1.cpp

[33mcommit ae78e70989f2981a92a700677cfcde09470f4f3c[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Thu Feb 4 10:16:45 2021 -0500

    Updating the streetSegmentLength function.

[33mcommit e2e0586733d75d59db989b49a32ea4a584b3a271[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Thu Feb 4 10:23:13 2021 -0500

    Modified findClosestPOI to implement the POI vectot

[33mcommit 8a6319d2b9dc28f6579117df1088abce888f6014[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Thu Feb 4 10:14:13 2021 -0500

    Fixed bug in loadMap by removing an unnecessary line of code on line 62 of m1.cpp.

[33mcommit 925e71ec7b303790cc8158677add50d3402cb077[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Thu Feb 4 09:22:56 2021 -0500

    Began implementing findClosestPOI function

[33mcommit 0f89fc6cb6eb2705b85889b4accb91900447778e[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Thu Feb 4 02:05:12 2021 -0500

    Finished up the Intersections and Segments part of loadMap, added and finished (I think) loads for POIs and features. Will unit test ASAP.

[33mcommit 7212f91cb0498163ce3131906bd3e2e424115e18[m
Merge: 15d738b 2380a65
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Thu Feb 4 00:01:53 2021 -0500

    Made changes to m1.cpp by defining findDistanceBetweenTwoPoints

[33mcommit 15d738b9505cf8822634e08e040ddaef8516f91e[m
Author: Mahiliny <mahiliny.santhirakumar@mail.utoronto.ca>
Date:   Wed Feb 3 23:53:21 2021 -0500

    Modified m1.cpp by defining findDistanceBetweenTwoPoints

[33mcommit 2380a652ee8623cebc58d3810e5fe7ec85b0aaed[m
Author: Taha Khokhar <taha.khokhar@mail.utoronto.ca>
Date:   Tue Feb 2 13:56:52 2021 -0500

    Added progress on importMap, including a global variable for intersection street segments. Not complete yet.

[33mcommit d7a3ab0b21f3052309b5424ecb98b90a18ccecb9[m
Author: Kavya <kavya.kadi@mail.utoronto.ca>
Date:   Fri Jan 29 14:14:08 2021 -0500

    Initial Milestone 1 commit by /nfs/ug/fast1/ece297s/.public_v276/bin/.scripts/ece297init for kadikavy
