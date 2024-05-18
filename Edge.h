#ifndef EDGE_H
#define EDGE_H

/**
 * @class Edge
 * @brief Represents an edge in a graph with a destination vertex and distance.
 */
class Edge {
    public:
        /**
         * @brief Gets the destination vertex of the edge.
         * @return The destination vertex.
         */
        int getDestiny() const;

        /**
         * @brief Gets the distance to the destination vertex.
         * @return The distance.
         */
        double getDistance() const;

        /**
         * @brief Constructs an Edge object.
         * @param destiny The destination vertex.
         * @param distance The distance to the destination vertex.
         */
        Edge(int destiny, double distance);

    private:
        int destiny;
        double distance;
};

#endif