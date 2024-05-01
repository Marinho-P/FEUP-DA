#include "Vertex.h"

/**
 * @brief Converts degrees to radians.
 * @details Time complexity - O(1)
 * @param degrees The angle in degrees.
 * @return The angle in radians.
 */
double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

double Vertex::calculateDistanceToVertex(Vertex other) {
    const double radius_earth = 6371000;

    double lat1Rad = toRadians(latitude);
    double lat2Rad = toRadians(other.getLatitude());

    double deltaLatRad = toRadians(other.getLatitude() - latitude);
    double deltaLonRad = toRadians(other.getLongitude() - longitude);

    double a = sin(deltaLatRad / 2) * sin(deltaLatRad / 2) +
               cos(lat1Rad) * cos(lat2Rad) * sin(deltaLonRad / 2) * sin(deltaLonRad / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return radius_earth * c;
}

double Vertex::getLatitude() const {
    return latitude;
}

double Vertex::getLongitude() const {
    return longitude;
}

int Vertex::getId() const {
    return id;
}

Vertex::Vertex(double latitude, double longitude, int id) : latitude(latitude), longitude(longitude), id(id) {}

bool Vertex::operator<(Vertex& vertex) const {
    return this->dist < vertex.dist;
}

double Vertex::getDist() const {
    return dist;
}

void Vertex::setDist(double dist) {
    Vertex::dist = dist;
}
