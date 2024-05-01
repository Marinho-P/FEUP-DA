#ifndef PROJETODA2_VERTEX_H
#define PROJETODA2_VERTEX_H
#include <cmath>

class Vertex {
    public:
    /**
    * @brief Calculates the distance between two vertexes using the Haversine formula.
    * @details Time complexity - O(1)
    * @param other The destination vertex.
    * @return The distance between the vertexes in meters.
    */
        double calculateDistanceToVertex(Vertex other);
        double getLatitude() const;
        double getLongitude() const;
        int getId() const;

    Vertex(double latitude, double longitude, int id);

private:
        double latitude;
        double longitude;
        int id;
};


#endif //PROJETODA2_VERTEX_H
