#include <string>
#include "intermediate.h"
#include "table.h"

using namespace std;


// TODO
Intermediate::Intermediate(const Table &table) {
    attrs = table.attrs;
    numAttrs = table.numAttrs;
    head = new EntryNode;
    head->entry = table.entries[0];
    head->next = nullptr;
    head->prev = nullptr;
    EntryNode *p = head;
    for (int i = 1; i < table.numEntries; ++i) {
        EntryNode *n = new EntryNode;
        n->prev = p;
        n->next = nullptr;
        n->entry = table.entries[i];
        p->next = n;
        p = n;
    }
    tail = p;
}

// TODO
Intermediate::~Intermediate() {
    EntryNode *current = head;
    EntryNode *next_node = head->next;
    while (current != nullptr) {
        delete current;
        if (next_node == nullptr) {
            current = nullptr;
            break;
        }
        current = next_node;
        next_node = next_node->next;
    }
}

// TODO
Intermediate &Intermediate::where(const string &attr, enum compare mode, const string &value) {
    int att_index = -1;
    for (int i = 0; i < numAttrs; ++i) {
        if (attrs[i] == attr) {
            att_index = i;
            break;
        }
    }
    if(att_index == -1) return *this;

    EntryNode* current = head;
    while (current != nullptr){
        if (mode == EQ){
            if (current->entry[att_index] != value){
                current->prev->next = current->next;
                delete current;
            }
        }
        else{
            if (!current->entry[att_index].find(value)){
                current->prev->next = current->next;
                delete current;
            }
        }
    }
    return *this;
}

// TODO
Intermediate &orderBy(const string &attr, enum order order);

// TODO
Intermediate &limit(unsigned int limit);

// TODO
void update(const string &attr, const string &new_value) const;

// TODO
void select(const string *attrs = nullptr, int numAttrs = 0) const;