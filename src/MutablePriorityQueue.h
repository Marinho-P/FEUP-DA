#ifndef MUTABLEPRIORITYQUEUE_H
#define MUTABLEPRIORITYQUEUE_H
#include <vector>

/**
 * @class MutablePriorityQueue
 * @brief A mutable priority queue implemented as a binary heap.
 * @details Template class T must have an accessible field int queueIndex and operator< defined.
 * @tparam T The type of elements in the priority queue.
 */
template <class T>
class MutablePriorityQueue {
    std::vector<T *> H;
    void heapifyUp(unsigned i);
    void heapifyDown(unsigned i);
    inline void set(unsigned i, T * x);

    public:
        /**
         * @brief Constructs an empty MutablePriorityQueue.
         */
        MutablePriorityQueue();

        /**
         * @brief Inserts an element into the priority queue.
         * @param x The element to insert.
         * @details Time complexity - O(log n)
         */
        void insert(T * x);

        /**
         * @brief Extracts and returns the minimum element from the priority queue.
         * @return The minimum element.
         * @details Time complexity - O(log n)
         */
        T * extractMin();

        /**
         * @brief Decreases the key of a given element.
         * @param x The element whose key is to be decreased.
         * @details Time complexity - O(log n)
         */
        void decreaseKey(T * x);

        /**
        * @brief Checks if the priority queue is empty.
        * @return True if the priority queue is empty, false otherwise.
        * @details Time complexity - O(1)
        */
        bool empty();
};



// Index calculations
#define vetor(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}


template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}


template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if(H.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}


template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}


template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex);
}


/**
 * @brief Moves the element at index i up to restore the heap property.
 * @param i The index of the element to move up.
 * @details Time complexity - O(log n)
 */
template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[vetor(i)]) {
        set(i, H[vetor(i)]);
        i = vetor(i);
    }
    set(i, x);
}


/**
 * @brief Moves the element at index i down to restore the heap property.
 * @param i The index of the element to move down.
 * @details Time complexity - O(log n)
 */
template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}


/**
 * @brief Sets the element at index i to x and updates its queue index.
 * @param i The index to set.
 * @param x The element to set.
 * @details Time complexity - O(1)
 */
template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x;
    x->queueIndex = i;
}

#endif