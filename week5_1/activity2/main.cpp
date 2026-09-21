#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>

// ==============================
// Linked List Node
// ==============================
struct Node {
    int value;
    Node* next;
    Node(int v) : value(v), next(nullptr) {}
};

// ==============================
// Build Linked List
// ==============================
Node* build_linked_list(size_t n_nodes) {
    if (n_nodes == 0) return nullptr;

    // Random number generator
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 100000);

    Node* head = new Node(dist(rng));
    Node* current = head;

    for (size_t i = 1; i < n_nodes; i++) {
        current->next = new Node(dist(rng));
        current = current->next;
    }
    return head;
}

Node* sequential_search(Node* begin, Node* end, int key)
{
    while(begin->next != end)
    {
        if (begin->value == key)
        {
            return begin;
        }
        begin = begin->next;
    }
    return nullptr;
}


int main ()
{

    Node* linked_list_head = build_linked_list(100000000);

    auto time_start = std::chrono::high_resolution_clock::now();

    Node* found_element = sequential_search(linked_list_head,nullptr,-1);

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
    std::cout << "duration: " << time_diff << " mikrosekunder " << std::endl;

    std::cout << "found 100 at " << found_element << std::endl;

    return 0;
}