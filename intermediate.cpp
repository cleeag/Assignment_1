#include <string>
#include "iostream"
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
    if (att_index == -1)
        return *this;
    else {
        EntryNode *current = head;
        while (current != nullptr) {
            if (mode == EQ) {
                if (current->entry[att_index] != value) {
                    current->prev->next = current->next;
                    EntryNode *tmp = current;
                    current = current->next;
                    delete tmp;
                }
            } else {
                if (!current->entry[att_index].find(value)) {
                    current->prev->next = current->next;
                    EntryNode *tmp = current;
                    current = current->next;
                    delete tmp;
                }
            }
        }
        return *this;
    }


}

// TODO
Intermediate &Intermediate::orderBy(const string &attr, enum order order) {
    int att_index = -1;
    for (int i = 0; i < numAttrs; ++i) {
        if (attrs[i] == attr) {
            att_index = i;
            break;
        }
    }
    if (att_index == -1)
        return *this;
    else {
        EntryNode *current = head;

        bool flipped = true;
        while (flipped) {
            flipped = false;
            while (current != nullptr) {
                if (current->entry[att_index].compare(current->next->entry[att_index]) == 0) {
                    current = current->next;
                    continue;
                }
                if (order == ASCENDING) {
                    if (current->entry[att_index].compare(current->next->entry[att_index]) > 0) {
                        EntryNode *tmp = current->next->next;
                        current->prev->next = current->next;
                        current->next->next = current;
                        current->next = tmp;
                        flipped = true;
                    }
                } else if (order == DESCENDING) {
                    if (current->entry[att_index].compare(current->next->entry[att_index]) < 0) {
                        EntryNode *tmp = current->next->next;
                        current->prev->next = current->next;
                        current->next->next = current;
                        current->next = tmp;
                        flipped = true;
                    }
                }
                current = current->next;
            }
        }
        return *this;
    }
}

// TODO
Intermediate &Intermediate::limit(unsigned int limit) {
    int length = 0;
    EntryNode *len_counter = head;
    while (len_counter->next != nullptr) {
        len_counter = len_counter->next;
        length += 1;
    }
    if (limit >= length) return *this;

    int left_length = length;
    EntryNode *current = tail;
    while (left_length > limit) {
        EntryNode *tmp = current->prev;
        delete current;
        current = tmp;
        left_length -= 1;
    }
    return *this;
}

// TODO
void Intermediate::update(const string &attr, const string &new_value) const {
    int att_index = -1;
    for (int i = 0; i < numAttrs; ++i) {
        if (attrs[i] == attr) {
            att_index = i;
            break;
        }
    }
    if (att_index != -1) {
        EntryNode *current = head;
        while (current->next != nullptr) {
            current->entry[att_index] = new_value;
            current = current->next;
        }
    }
}

// TODO
void Intermediate::select(const string *attrs, int numAttrs) const {
    if (this->numAttrs == 0) return;

    if (attrs == nullptr) {
        numAttrs = this->numAttrs;
        attrs = this->attrs;
    }

    int attr_idx[numAttrs];
    for (int i = 0; i < numAttrs; ++i) {
        for (int j = 0; j < this->numAttrs; ++j) {
            if (this->attrs[j] == attrs[i]) {
                attr_idx[i] = j;
            }
        }
    }
//    find logest string in column
    EntryNode *current = head;
    int max_len_by_attr[numAttrs];
    for (int i = 0; i < numAttrs; ++i) {
        max_len_by_attr[i] = attrs[i].length();
    }

    while (current->next != nullptr) {
        for (int i = 0; i < numAttrs; ++i) {
            string tmp = current->entry[attr_idx[i]];
            if (tmp.length() > max_len_by_attr[i]) max_len_by_attr[i] = tmp.length();
            current = current->next;
        }
    }

//    print it out
    current = head;
    for (int i = 0; i < numAttrs; ++i) {
        string tmp = this->attrs[attr_idx[i]];
        string tmp2 = _left_pad_until(tmp, max_len_by_attr[i]);
        cout << " | " << tmp2;
    }
    cout << " | " << endl;
    while (current->next != nullptr) {
        for (int i = 0; i < numAttrs; ++i) {
            string tmp = current->entry[attr_idx[i]];
            string tmp2 = _left_pad_until(tmp, max_len_by_attr[i]);
            cout << " | " << tmp2;
        }
        cout << " | " << endl;
    }

}