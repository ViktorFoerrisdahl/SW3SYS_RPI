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

/* ==============================
   build_segments
   head: Head pointer of linked list
   n_nodes: Number of nodes in the linked list
   n_threads: Number threads we want to run in parallel
   returns: Vector with pointers to the beginning of each segment
   ============================== */
std::vector<Node*> build_segments(Node* head, size_t n_nodes, size_t n_threads) {
    std::vector<Node*> segments;
    if (!head || n_threads == 0) return segments;

    size_t chunk_size = n_nodes / n_threads;
    Node* current = head;
    size_t count = 0;

    while (current) {
        if (count % chunk_size == 0 && segments.size() < n_threads) {
            segments.push_back(current);
        }
        current = current->next;
        count++;
    }
    return segments;
}

void segment_search_thread(Node* begin, Node* end, const int key, Node** result)
{

    *result = sequential_search(begin, end, key); 
}


int main ()
{

    int SIZE_LINKED_LIST = 1000000;
    int ANTAL_THREADS = 3;
    int NUMBER_TO_SEARCH_FOR = 100;

    Node* linked_list_head = build_linked_list(SIZE_LINKED_LIST);

    //vælger at bruge 4 threads:
    //opdeler vores linked liste i 4:
    std::vector<Node*> opdelinger = build_segments(linked_list_head, SIZE_LINKED_LIST, ANTAL_THREADS);

    std::vector<Node*> results(ANTAL_THREADS - 1);

    auto time_start = std::chrono::high_resolution_clock::now();
    
    std::thread t1(segment_search_thread,opdelinger[0],opdelinger[1],NUMBER_TO_SEARCH_FOR,&results[0]);
    std::thread t2(segment_search_thread,opdelinger[1],opdelinger[2],NUMBER_TO_SEARCH_FOR,&results[1]);
    std::thread t3(segment_search_thread,opdelinger[2],nullptr,NUMBER_TO_SEARCH_FOR,&results[2]);
    
    t1.join();
    t2.join();
    t3.join();

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    std::cout << "duration: " << time_diff << " ms" << std::endl;

    for (auto i = 0; i < ANTAL_THREADS; i++)
    {
        if (results[i] == nullptr)
        {
            std::cout << "result på plads " << i << " = " << "nullptr" << std::endl;
        }
        else 
        {
            std::cout << "result på plads " << i << " = " << results[i] << std::endl;
        }
    }

    return 0;
}