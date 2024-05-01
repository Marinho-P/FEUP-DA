#ifndef PROJETODA2_EDGE_H
#define PROJETODA2_EDGE_H


class Edge {
    public:
        int getDestiny() const;
        void setDestiny(int destiny);
        double getDistance() const;
        void setDistance(double distance);
        Edge(int destiny, double distance);

private:
        int destiny;
        double distance;
};


#endif //PROJETODA2_EDGE_H
