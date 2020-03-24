#include <iostream>
#include <fstream>

#include "table.h"

using namespace std;

void print_matrix(string **m, int numAttrs, int numEntries) {
    cout << "printing matrix..." << endl;
    for (int j = 0; j < numEntries; ++j) {
        for (int k = 0; k < numAttrs; ++k) cout << m[j][k] << "\t";
        cout << endl;
    }
}

void print_vector(string *m, int numAttrs) {
    cout << endl;
    for (int k = 0; k < numAttrs; ++k) cout << m[k] << "\t";
    cout << endl;
}


Table::Table() {
    attrs = nullptr;
    entries = nullptr;
    numAttrs = 0;
    numEntries = 0;

}

Table::Table(const Table &another) {
    numAttrs = another.numAttrs;
    numEntries = another.numEntries;
    attrs = new string[numAttrs];
    for (int i = 0; i < numAttrs; ++i) {
        attrs[i] = another.attrs[i];
    }
    entries = new string *[numEntries];
    for (int i = 0; i < numEntries; ++i) {
        entries[i] = new string[numAttrs];
        for (int j = 0; j < numAttrs; ++j) {
            entries[i][j] = another.entries[i][j];
        }
    }
}

Table::~Table() {
    delete[] attrs;

    for (int i = 0; i < numEntries; ++i) {
        delete[] entries[i];
    }
    delete[] entries;
}

bool Table::addAttribute(const string &attr, int index, const string &default_value) {
    if (index < -1 or index > numAttrs) return false;

//    get new attributes
    string *new_attrs = new string[numAttrs + 1];
    if (index == -1) {
        new_attrs[numAttrs] = attr;
        for (int i = 0, j = 0; j < numAttrs;) {
            new_attrs[i] = attrs[j];
            ++i;
            ++j;
        }
    } else {
        for (int i = 0, j = 0; i < numAttrs + 1;) {
            if (i == index) {
                new_attrs[i] = attr;
                ++i;
            } else {
                new_attrs[i] = attrs[j];
                ++i;
                ++j;
            }
        }
    }
    delete[] attrs;
    attrs = new_attrs;

//    get new entries
    if (numEntries != 0) {
        string **new_entries = new string *[numEntries];
        for (int i = 0; i < numEntries; ++i) new_entries[i] = new string[numAttrs + 1];
        for (int row = 0; row < numEntries; ++row) {
            if (index == -1) new_entries[row][numAttrs] = default_value;
            for (int i = 0, j = 0; i < numAttrs + 1;) {
                if (i == index) {
                    new_entries[row][i] = default_value;
                    ++i;
                } else {
                    new_entries[row][i] = entries[row][j];
                    ++i;
                    ++j;
                }
            }
        }


        for (int i = 0; i < numEntries; ++i) {
            delete[] entries[i];
        }
        delete[] entries;
        entries = new_entries;
    }
//    for (int k = 0; k < numAttrs; ++k) cout << attrs[k] << endl;
    numAttrs += 1;
    return true;
}

// TODO
bool Table::addEntry(const string *entry, int index) {
    if (index < -1 or index > numEntries) return false;

    string **new_entries = new string *[numEntries + 1];
    for (int i = 0; i < numEntries + 1; ++i) new_entries[i] = new string[numAttrs];

    if (index == -1) {
        for (int i = 0; i < numAttrs; ++i) {
            new_entries[numEntries][i] = entry[i];
        }
    }
    for (int row_new = 0, row_old = 0; row_old < numEntries;) {
        if (row_new == index) {
            for (int i = 0; i < numAttrs; ++i) {
                new_entries[row_new][i] = entry[i];
            }
            row_new++;
        } else {
            for (int i = 0; i < numAttrs; ++i) {
                new_entries[row_new][i] = entries[row_old][i];
            }
            ++row_new;
            ++row_old;
        }
    }
    for (int i = 0; i < numEntries; ++i) {
        delete[] entries[i];
    }
    delete[] entries;
    entries = new_entries;
    numEntries += 1;
    return true;
}


// TODO
bool Table::deleteAttribute(int index) {
    if (index < 0 or index >= numAttrs) return false;

//    delete everything if only one attribute
    if (numAttrs == 1) {
        cout << "deleting" << endl;
        delete[] attrs;
        for (int i = 0; i < numEntries; ++i) {
            delete[] entries[i];
        }
        delete[] entries;
        return true;
    }

//    delete one attribute
    string *new_attrs = new string[numAttrs - 1];
    for (int new_idx = 0, old_idx = 0; old_idx < numAttrs;) {
        if (old_idx == index) {
            ++old_idx;
        } else {
            new_attrs[new_idx] = attrs[old_idx];
            ++new_idx;
            ++old_idx;
        }
    }
    delete[] attrs;
    attrs = new_attrs;

//    delete new entries
    if (numEntries != 0) {
        string **new_entries = new string *[numEntries];
        for (int i = 0; i < numEntries; ++i) new_entries[i] = new string[numAttrs - 1];

        for (int row = 0; row < numEntries; ++row) {
            for (int i = 0, j = 0; j < numAttrs;) {
                if (j == index) {
                    ++j;
                } else {
                    new_entries[row][i] = entries[row][j];
                }
                ++i;
                ++j;
            }
        }


        for (int i = 0; i < numEntries; ++i) {
            delete[] entries[i];
        }
        delete[] entries;
        entries = new_entries;
    }
    numAttrs -= 1;
    return true;
}

// TODO
bool Table::deleteEntry(int index) {
    if (index < 0 or index > numEntries - 1) return false;

    string **new_entries = new string *[numEntries - 1];
    for (int i = 0; i < numEntries - 1; ++i) new_entries[i] = new string[numAttrs];

    for (int row_new = 0, row_old = 0; row_new < numEntries - 1;) {
        if (row_old == index) {
            row_old++;
        } else {
            for (int i = 0; i < numAttrs; ++i) {
                new_entries[row_new][i] = entries[row_old][i];
            }
            ++row_new;
            ++row_old;
        }
    }

    for (int i = 0; i < numEntries; ++i) {
        delete[] entries[i];
    }
    delete[] entries;
    entries = new_entries;
    numEntries -= 1;
    return true;
}

// TODO
bool Table::append(const Table &another) {
    if (numAttrs != another.numAttrs)
        return false;
    else {
        for (int i = 0; i < another.numAttrs; ++i) {
            if (attrs[i] != another.attrs[i]) return false;
        }
    }

    int new_numEntries = numEntries + another.numEntries;
    string **new_entries = new string *[new_numEntries];
    for (int i = 0; i < new_numEntries; ++i) new_entries[i] = new string[numAttrs];

    for (int i = 0; i < numEntries; ++i) {
        for (int j = 0; j < numAttrs; ++j) {
            new_entries[i][j] = entries[i][j];
        }
    }
    for (int i = numEntries; i < new_numEntries; ++i) {
        for (int j = 0; j < numAttrs; ++j) {
            new_entries[i][j] = another.entries[i - numEntries][j];
        }
    }

    for (int i = 0; i < numEntries; ++i) {
        delete[] entries[i];
    }
    delete[] entries;
    numEntries = new_numEntries;
    entries = new_entries;

    return true;
}

void Table::print() const {
    for (int j = 0; j < numAttrs; ++j) cout << this->attrs[j] << "\t";
    cout << endl;
    for (int j = 0; j < numEntries; ++j) {
        for (int k = 0; k < numAttrs; ++k) cout << this->entries[j][k] << "\t";
        cout << endl;
    }
    cout << "numAttrs: " << numAttrs << endl;
    cout << "numEntries: " << numEntries << endl << endl;
}

