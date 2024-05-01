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
        bool operator<(Vertex& vertex) const;
        int queueIndex = 0; // required for MST
private:
        double latitude;
        double longitude;
public:
    double getDist() const;

    void setDist(double dist);

private:
    int id;
        double dist = INFINITY; // required for MST
};


#endif //PROJETODA2_VERTEX_H
