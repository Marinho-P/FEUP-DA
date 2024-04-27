#include "Coordinate.h"
#include <cmath>

/**
 * @brief Constructor for the Coordinate class that initializes an object with latitude and longitude.
 * @details Time complexity - O(1)
 * @param latitude The latitude coordinate.
 * @param longitude The longitude coordinate.
 */
Coordinate::Coordinate(double latitude, double longitude) {
    this->latitude = latitude;
    this->longitude = longitude;
}

/**
 * @brief Default constructor for the Coordinate class.
 * @details Time complexity - O(1)
 * Initializes the object with default values (0.0, 0.0).
 */
Coordinate::Coordinate() : latitude(0.0), longitude(0.0) {}

/**
 * @brief Getter for the latitude coordinate.
 * @details Time complexity - O(1)
 * @return The latitude coordinate.
 */
double Coordinate::getLatitude() const {
    return latitude;
}

/**
 * @brief Getter for the longitude coordinate.
 * @details Time complexity - O(1)
 * @return The longitude coordinate.
 */
double Coordinate::getLongitude() const {
    return longitude;
}

/**
 * @brief Converts degrees to radians.
 * @details Time complexity - O(1)
 * @param degrees The angle in degrees.
 * @return The angle in radians.
 */
double Coordinate::toRadians(double degrees) const {
    return degrees * M_PI / 180.0;
}

/**
 * @brief Calculates the distance between two coordinates using the Haversine formula.
 * @details Time complexity - O(1)
 * @param cord2 The second coordinate to calculate the distance to.
 * @return The distance between the coordinates in meters.
 */
double Coordinate::dist_coordinates(Coordinate cord2) const {
    const double radius_earth = 6371000;

    double lat1Rad = toRadians(latitude);
    double lat2Rad = toRadians(cord2.getLatitude());

    double deltaLatRad = toRadians(cord2.getLatitude() - latitude);
    double deltaLonRad = toRadians(cord2.getLongitude() - longitude);

    double a = sin(deltaLatRad / 2) * sin(deltaLatRad / 2) +
               cos(lat1Rad) * cos(lat2Rad) * sin(deltaLonRad / 2) * sin(deltaLonRad / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return radius_earth * c;
}






