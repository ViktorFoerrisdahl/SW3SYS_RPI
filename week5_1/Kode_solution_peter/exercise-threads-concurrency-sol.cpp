#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>

#define MAX_RANDOM 10000000

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

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, MAX_RANDOM);

    Node* head = new Node(dist(rng));
    Node* current = head;

    for (size_t i = 1; i < n_nodes; i++) {
        current->next = new Node(dist(rng));
        current = current->next;
    }
    return head;
}

// ===============================================
// Build array of Node pointers, each pointing to
// the beginning of a search sub-segment
// ===============================================
std::vector<Node*> build_segments(Node* head, size_t n_nodes, size_t n_threads) {
    std::vector<Node*> segments;
    if (!head || n_threads == 0) return segments;

    size_t chunk_size = n_nodes / n_threads;
    Node* current = head;
    size_t count = 0;

    while (current) {
        if (count % chunk_size == 0 && segments.size() < n_threads && count < n_nodes) {
            segments.push_back(current);
        }
        current = current->next;
        count++;
    }
    return segments;
}

// =============================================================
// Sequential Search for the value 'key'
// Starts at node 'begin' stops at 'end' (nullptr for full list)
// =============================================================
Node* sequential_search(Node* begin, Node* end, const int key) {
    if (begin == nullptr)
        return nullptr; // Empty

    Node* current = begin;
    while (current != nullptr && current != end) {
        if (current->value == key) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Not found
}

// ==============================
// Parallel Search
// ==============================
/* Wrapper function for sequential_search() as thread must return void */
void segment_search_thread(Node* begin, Node* end, const int key, Node** result)
{
    *result = sequential_search(begin, end, key);    
}

std::vector<Node*> parallel_search(std::vector<Node*> segments, const size_t n_threads, int key) {
    std::vector<Node*> results(n_threads);
    std::vector<std::thread> threads;
    size_t i = 0;
    // Create threads
    for (i = 0; i < n_threads - 1; i++)
    {
        /* Add segment_search_thread to the threads vector */
//        threads.emplace_back(segment_search_thread, segments[i], segments[i + 1], key, &results[i]);
        /* Alternative: Add thread but wrap sequential_search using a lambda instead of using segment_search_thread() */
        threads.emplace_back([](auto b, auto e, auto  k, Node** res)
                     { *res = sequential_search(b, e, k); }, segments[i], 
                     segments[i + 1], key, &results[i]);
    }

    // Last segment ends at nullptr
    threads.emplace_back(segment_search_thread, segments[i],  nullptr, key, &results[i]);

    // Join thread (results are available when they have joined)
    for (auto& t : threads) {
        t.join();
    }

    return results;
}

// ==============================
// Free Linked List Memory
// ==============================
void free_linked_list(Node* head) {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

// ==============================
// Main
// ==============================
int main() {
    size_t n_nodes = 10000000;
//    size_t n_nodes = 10;
    size_t n_threads = 4;

    // Build linked list
    std::cout << "Building Linked list" << std::endl;
    Node* head = build_linked_list(n_nodes);

    std::cout << "Creating segments" << std::endl;
    std::vector<Node*> segments = build_segments(head, n_nodes, n_threads);

    std::cout << "Created " << segments.size() << " segments" << std::endl;

    // Print linked list
    // Node *pcursor = head;
    // while (pcursor != nullptr)
    // {
    //     std::cout << pcursor->value << " ";
    //     pcursor = pcursor->next;
    // }
    // std::cout << std::endl;


    //Print segments
    // Node* cursor = head;
    // Node *end = nullptr;
    // for (int i = 0; i < segments.size(); i++)
    // {
    //     cursor = segments[i];
    //     if ((i + 1) < segments.size())
    //         end = segments[i + 1];
    //     else
    //         end = nullptr;

    //     while (cursor != end)
    //     {
    //         std::cout << cursor->value << " ";
    //         cursor = cursor->next;
    //     }
    //     std::cout << std::endl;
    // }

    // Lets assign the last node with a value outside the assigned values and use it as our search value (key)
    Node *ptr = head;
    while (ptr->next) // Stop when next ptr == nullptr
        ptr = ptr->next;
    ptr->value = MAX_RANDOM + 1;
    int key = ptr->value; // assign key to last value

    std::cout << "Searching for: " << key << std::endl;

    // Sequential search with timing
    auto start_seq = std::chrono::high_resolution_clock::now();
    Node *resultNode = sequential_search(head, nullptr, key);
    auto end_seq = std::chrono::high_resolution_clock::now();
    auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq).count();

    // Named lambda to locate index of a node
    int nodeIdx = -1;
    auto nodeToIdx = [&](Node* head, Node *node) 
    {
        while (head != nullptr)
        {
            head = head->next;
            ++nodeIdx;
            if (head == node)
                break;
        };
    };

    // Find index of node containing key
    nodeToIdx(head, resultNode);

    std::cout << "Sequential search, node found at idx: " << nodeIdx
                << " (time: " << seq_time << " ms)\n";

    // Parallel search with timing
    auto start_par = std::chrono::high_resolution_clock::now();
    int par_index = 0;
    std::vector<Node*> results = parallel_search(segments, n_threads, key);
    auto end_par = std::chrono::high_resolution_clock::now();
    auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par).count();

    // Find segment containing key 
    par_index = 0;
    for (auto result : results)
    {
        if(result)
            break;
        ++par_index;
    }

    // Find index in segment
    nodeIdx = -1;
    nodeToIdx(segments[par_index], resultNode);

    // Print result
    std::cout << "Parallel search result: Found in segment # " << par_index
                << " Index: " << nodeIdx << " (time: " << par_time << " ms)\n";

    // Cleanup
    free_linked_list(head);
    return 0;
}
