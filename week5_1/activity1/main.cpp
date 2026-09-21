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


int main ()
{

    Node* linked_list_head = build_linked_list(1000000);

    int counter{};

    while (counter < 101)
    {
        std::cout <<"next adresse: " << linked_list_head << ", value: " << linked_list_head->value << std::endl;
        linked_list_head = linked_list_head->next;
        counter++;
    }


    return 0;
}