#ifndef solution_hpp
#define solution_hpp

#include <iostream>
#include <ctime>
#include <string>
#include <memory>
#include <queue>
#include <vector>

using namespace std;

template <class T>
class List {
    int size = 0;
    struct element {
        bool deleted = false;
        T val;
        vector<shared_ptr<element>> next;
    };
    shared_ptr<element> head = nullptr;
    const int maxLevel = 10;
public:
    List();
    void push(T a);
    T get(int i);
    int Size();
    bool find(T a);
    void erase(int i);
};

template<class T>
List<T>::List() {
    push(numeric_limits<int>::min());
    size--;
}

template<class T>
void List<T>::push(T a) {
    shared_ptr<element> p;
    shared_ptr<element> newElem(new element);
    if (!this->head) {
        newElem->val = a;
        newElem->next.resize(maxLevel);
        this->head = newElem;
    } else {
        newElem->val = a;
        newElem->next.resize(maxLevel);
        queue<shared_ptr<element>> queue;
        for (int l = 0; l < maxLevel; l++) {
            while (true) {
                p = this->head;
                int currLevel = maxLevel - 1;
                while (true) {
                    while (p->next[currLevel] &&
                           !p->next[currLevel]->deleted && p->next[currLevel]->val < a)
                    p = p->next[currLevel];
                    currLevel--;
                    if (currLevel < l) break;
                }
                auto next = p->next[l];
                shared_ptr<element> p1 = this->head;
                currLevel = maxLevel - 1;
                while (true) {
                    while (p1->next[currLevel]
                           && !p1->next[currLevel]->deleted
                           && p1->next[currLevel]->val < a)
                    p1 = p1->next[currLevel];
                    currLevel--;
                    if (currLevel < l)break;
                }
                if (p == p1 && !p->deleted) {
                    if (!p->next[l] || p->next[l]->val >= newElem->val) {
                        newElem->next[l] = next;
                        if (atomic_compare_exchange_weak_explicit(&p->next[l], &next, newElem, memory_order_release, memory_order_relaxed)) {
                            break;
                        }
                    }
                }
            }
            if (rand() % (l + 2) == 0)
            break;
        }
    }
    size++;
}

template<class T>
inline T List<T>::get(int i) {
    i++;
    shared_ptr<element> p = head;
    for (int t = 0; t < i; t++)
    p = p->next[0];
    return p->val;
}

template<class T>
inline int List<T>::Size() {
    return size;
}

template<class T>
inline bool List<T>::find(T a) {
    int currLevel = maxLevel - 1;
    shared_ptr<element> p = head;
    while (true) {
        while (p->next[currLevel] != nullptr && p->next[currLevel]->val < a) {
            p = p->next[currLevel];
        }
        if (p->next[currLevel] == nullptr || p->next[currLevel]->val > a)
        currLevel--;
        else
        return true;
        if (currLevel < 0)
        return false;
    }
}

template<class T>
inline void List<T>::erase(int i) {
    i++;
    element* del;
    if (size > i) {
        shared_ptr<element> p;
        shared_ptr<element> delItem;
        for (int l = 0; l < maxLevel; l++) {
            while (true) {
                p = this->head;
                for (int t = 1; (t < i && l == 0) || (p->next[l] && l>0 && p->next[l] != delItem); t++)
                p = p->next[l];
                auto next = p->next[l];
                if (next == nullptr)
                break;
                shared_ptr<element> p1 = this->head;
                for (int t(1); (t < i && l == 0) || (p1->next[l] && l>0 && p1->next[l] != delItem); t++)
                p1 = p1->next[l];
                if (p == p1) {
                    if (l == 0) {
                        delItem = next;
                        delItem->deleted = true;
                    }
                    if (atomic_compare_exchange_weak_explicit(&p1->next[l], &p->next[l], next->next[l], memory_order_release, memory_order_relaxed)) { break; }
                }
            }
        }
        size--;
        del = delItem.get();
        find(delItem->val);
    }
}


#endif /* solution_hpp */