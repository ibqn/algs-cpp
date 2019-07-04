#include <iostream>
#include <cstdlib>
#include <memory>


using namespace std;


template <typename T = int>
struct node
{
    unique_ptr<T> data;
    shared_ptr<node<T>> next;

    node(unique_ptr<T> data): data(move(data)), next(nullptr) {}
    ~node() {
        // cout << "destroy node (" << &data << ") with data: " << *data << endl;
        // cout << '#';
    }
};


template <typename T = int>
class cyclic_list
{
private:
    shared_ptr<node<T>> tail;
    int size;

public:
    cyclic_list() : tail(nullptr), size(0) {};

    cyclic_list(unique_ptr<T> data) : size(1) {
        push(move(data));
    };

    ~cyclic_list() {
        cout << "clear list" << endl;
        while(tail) {
            tail = move(tail->next);
        }
    };

    void push(unique_ptr<T> data) {
        auto tmp = make_shared<node<T>>(move(data));
        if (tail) {
            tmp->next = move(tail->next);
            tail = tail->next = move(tmp);
        } else {
            tail = tmp;
            tail->next = move(tmp);
        }

        size ++;
    }

    void cycle() {
        shared_ptr<node<T>> c = tail->next;
        do {
            cout << "val: " << *(c->data) << endl;
            c = c->next;
        } while (c != tail->next);
    }

    void cycle(int M) {
        shared_ptr<node<T>> c = tail;
        while(c != c->next) {
            for (int i = 1; i < M; i++) {
                c = c->next;
                cout << *c->data;
                cout << '.';
            }
            cout << " removing " << *c->next->data << endl;
            c->next = move(c->next->next);
        }
        cout << "left: " << *(c->data) << endl;

        tail = c;
    }
};


int main(int argc, char *argv[])
{
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    auto v0 = make_unique<int>(-1);
    auto n0{make_shared<node<int>>(move(v0))};

    auto v1 = make_unique<int>(-2);
    auto n1{make_shared<node<>>(move(v1))};
    n1->next = move(n0);

    auto v2 = make_unique<int>(-3);
    auto n2{make_shared<node<>>(move(v2))};
    n2->next = move(n1);

    cout << "create cyclic list" << endl;
    auto v = make_unique<int>(1);
    auto list = make_unique<cyclic_list<>>(move(v));


    for (int i = 2; i <= N; i++) {
        auto tmp = make_unique<int>(i);
        list->push(move(tmp));
    }

    list->cycle(M);
    list->cycle();

/*
    for (int i = 1; i <= 10'000'000; i++) {
        auto tmp = make_unique<int>(i);
        list->push(move(tmp));
    }
    list->cycle();
*/
    return EXIT_SUCCESS;
}
