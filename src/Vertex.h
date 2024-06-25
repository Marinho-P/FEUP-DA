#ifndef VERTEX_H
#define VERTEX_H
#include <cmath>

/**
 * @class Vertex
 * @brief Represents a geographical vertex with latitude, longitude, and an identifier.
 */
class Vertex {
    public:
        /**
         * @brief Calculates the distance between two vertexes using the Haversine formula.
         * @details Time complexity - O(1)
         * @param other The destination vertex.
         * @return The distance between the vertexes in meters.
         */
        double calculateDistanceToVertex(Vertex other);

        /**
         * @brief Gets the latitude of the vertex.
         * @return The latitude.
         */
        double getLatitude() const;

        /**
         * @brief Gets the longitude of the vertex.
         * @return The longitude.
         */
        double getLongitude() const;

        /**
         * @brief Gets the id of the vertex.
         * @return The id.
         */
        int getId() const;

        /**
         * @brief Constructs a Vertex object.
         * @param latitude The latitude of the vertex.
         * @param longitude The longitude of the vertex.
         * @param id The identifier of the vertex.
         * @details Time complexity - O(1)
         */
        Vertex(double latitude, double longitude, int id);

        /**
         * @brief Compares the distance of two vertices.
         * @param vertex The vertex to compare with.
         * @return True if this vertex has a smaller distance, false otherwise.
         */
        bool operator<(Vertex& vertex) const;

        /**
         * @brief Gets the distance of the vertex.
         * @return The distance.
         */
        double getDist() const;

        /**
         * @brief Sets the distance of the vertex.
         * @param dist The new distance.
         */
        void setDist(double dist);

        int queueIndex = 0; // required for MST (Minimum Spanning Tree) algorithms

    private:
        double latitude;
        double longitude;
        int id;
        double dist = INFINITY; // required for MST
};

#endif