//
//  bimap.cpp
//  bimap
//
//  Created by Данил Шкарупин on 18.09.16.
//  Copyright © 2016 Данил Шкарупин. All rights reserved.
//

#include "bimap.hpp"
#include <vector>
using namespace std;

bimap::left_iterator bimap::insert(bimap::left_t const& left, right_t const& right) {
    
    if (root -> left_left == nullptr) {
        node* inserting = new node();
        
        inserting -> left_data = left;
        inserting -> right_data = right;
        inserting -> left_parent = root;
        root -> left_left = inserting;
        
        inserting -> right_data = right;
        inserting -> right_parent = root;
        root -> right_left = inserting;
        
        left_iterator trying_it;
        trying_it.current = inserting;
        return trying_it;
    }
    
    node *trying_left = root -> left_left, *trying_right = root -> right_left;
    
    int l_insert = 0, r_insert = 0;
    
    //find left
    while (true) {
        if (trying_left -> left_data == left) break;
        if (trying_left -> left_data < left) {
            if (trying_left -> left_right != nullptr) {
                trying_left = trying_left -> left_right;
            } else {
                l_insert = 2;
                break;
            }
        }
        if (trying_left -> left_data > left) {
            if (trying_left -> left_left != nullptr) {
                trying_left = trying_left -> left_left;
            } else {
                l_insert = 1;
                break;
            }
        }
    }
    
    //find right
    while (true) {
        if (trying_right -> right_data == right) break;
        if (trying_right -> right_data < right) {
            if (trying_right -> right_right != nullptr) {
                trying_right = trying_right -> right_right;
            } else {
                r_insert = 2;
                break;
            }
        }
        if (trying_right -> right_data > right) {
            if (trying_right -> right_left != nullptr) {
                trying_right = trying_right -> right_left;
            } else {
                r_insert = 1;
                break;
            }
        }
    }
    
    //if good - insert
    if (l_insert != 0 && r_insert != 0) {
        node* inserting = new node();
        
        inserting -> left_data = left;
        inserting -> right_data = right;
        inserting -> left_parent = trying_left;
        (l_insert == 1 ? trying_left -> left_left : trying_left -> left_right) = inserting;
        
        inserting -> right_data = right;
        inserting -> right_parent = trying_right;
        (r_insert == 1 ? trying_right -> right_left : trying_right -> right_right) = inserting;
        
        left_iterator trying_it;
        trying_it.current = inserting;
        return trying_it;
    } else {
        return end_left();
    }
}

void bimap::erase(bimap::left_iterator it) {
    if (it.current -> left_parent != nullptr) {
        bool l_parent, r_parent;
        if (it.current -> left_parent -> left_left == it.current) l_parent = 0;
        else l_parent = 1;
        if (it.current -> right_parent -> right_left == it.current) r_parent = 0;
        else r_parent = 1;
        //Если нет детей -- всё хорошо
        if (it.current -> left_left == nullptr && it.current -> left_right == nullptr) {
            (l_parent == false ? it.current -> left_parent -> left_left: it.current -> left_parent -> left_right) = nullptr;
        }
        //Если два ребенка -- подвесить левого вместо current правого к самому большому ребенку левого
        else if (it.current -> left_left != nullptr && it.current -> left_right != nullptr) {
            (l_parent == false ? it.current -> left_parent -> left_left : it.current -> left_parent -> left_right) = it.current -> left_left;
            it.current -> left_left -> left_parent = it.current -> left_parent;
            
            node* trying = it.current -> left_left;
            while (trying -> left_right != nullptr) trying = trying -> left_right;
            
            trying -> left_right = it.current -> left_right;
            it.current -> left_right -> left_parent = trying;
        }
        //Если один ребенок -- подвесить вместо current
        else {
            (l_parent == false ? it.current -> left_parent -> left_left : it.current -> left_parent -> left_right) = (it.current -> left_left != nullptr ? it.current -> left_left : it.current -> left_right);
            (it.current -> left_left != nullptr ? it.current -> left_left : it.current -> left_right) -> left_parent = it.current -> left_parent;
        }
        
        
        if (it.current -> right_left == nullptr && it.current -> right_right == nullptr) {
            (r_parent == false ? it.current -> right_parent -> right_left: it.current -> right_parent -> right_right) = nullptr;
        }
        //Если два ребенка -- подвесить левого вместо current правого к самому большому ребенку левого
        else if (it.current -> right_left != nullptr && it.current -> right_right != nullptr) {
            (r_parent == false ? it.current -> right_parent -> right_left : it.current -> right_parent -> right_right) = it.current -> right_left;
            it.current -> right_left -> right_parent = it.current -> right_parent;
            
            node* trying = it.current -> right_left;
            while (trying -> right_right != nullptr) trying = trying -> right_right;
            
            trying -> right_right = it.current -> right_right;
            it.current -> right_right -> right_parent = trying;
        }
        //Если один ребенок -- подвесить вместо current
        else {
            (r_parent == false ? it.current -> right_parent -> right_left : it.current -> right_parent -> right_right) = (it.current -> right_left != nullptr ? it.current -> right_left : it.current -> right_right);
            (it.current -> right_left != nullptr ? it.current -> right_left : it.current -> right_right) -> right_parent = it.current -> right_parent;
        }
        
        delete it.current;
    }
}
void bimap::erase(bimap::right_iterator it) {
    erase(it.flip());
}

bimap::left_iterator  bimap::find_left (bimap::left_t  const& left)  const {
    
    if (root -> left_left == nullptr) return end_left();
    
    node *trying = root -> left_left;
    
    while (true) {
        if (trying -> left_data == left) break;
        if (trying -> left_data < left) {
            if (trying -> left_right != nullptr) {
                trying = trying -> left_right;
            } else {
                return end_left();
            }
        }
        if (trying -> left_data > left) {
            if (trying -> left_left != nullptr) {
                trying = trying -> left_left;
            } else {
                return end_left();
            }
        }
    }
    
    left_iterator trying_it;
    trying_it.current = trying;
    return trying_it;
}
bimap::right_iterator bimap::find_right(bimap::right_t const& right) const {
    
    if (root -> right_left == nullptr) return end_right();
    
    node *trying = root -> right_left;
    
    while (true) {
        if (trying -> right_data == right) break;
        if (trying -> right_data < right) {
            if (trying -> right_right != nullptr) {
                trying = trying -> right_right;
            } else {
                return end_right();
            }
        }
        if (trying -> right_data > right) {
            if (trying -> right_left != nullptr) {
                trying = trying -> right_left;
            } else {
                return end_right();
            }
        }
    }
    
    right_iterator trying_it;
    trying_it.current = trying;
    return trying_it;
}

bimap::left_iterator bimap::begin_left() const {
    if (root -> left_left == nullptr) return end_left();
    node* trying = root -> left_left;
    while (trying -> left_left != nullptr) trying = trying -> left_left;
    
    left_iterator trying_it;
    trying_it.current = trying;
    return trying_it;
}
bimap::left_iterator bimap::end_left() const {
    left_iterator left;
    left.current = root;
    return left;
}

bimap::right_iterator bimap::begin_right() const {
    if (root -> right_left == nullptr) return end_right();
    node* trying = root -> right_left;
    while (trying -> right_left != nullptr) trying = trying -> right_left;
    
    right_iterator trying_it;
    trying_it.current = trying;
    return trying_it;
}
bimap::right_iterator bimap::end_right() const {
    right_iterator right;
    right.current = root;
    return right;
}

bimap::bimap() {
    root = new node();
}

void bimap::clear_all(bimap::node* current) {
    if (current -> left_left != nullptr) {
        clear_all(current -> left_left);
    }
    if (current -> left_right != nullptr) {
        clear_all(current -> left_right);
    }
    delete(current);
}

bimap::~bimap() {
    if (root -> left_left != nullptr) clear_all(root -> left_left);
    delete(root);
}

bimap::left_t const& bimap::left_iterator::operator*() const {
    return current -> left_data;
}

bimap::left_iterator& bimap::left_iterator::operator++() {
    if (current -> left_right != nullptr) {
        current = current -> left_right;
        while (current -> left_left != nullptr) current = current -> left_left;
    } else {
        while ((current -> left_parent != nullptr) && (current -> left_parent -> left_right == current)) current = current -> left_parent;
        if (current -> left_parent != nullptr) current = current -> left_parent;
    }
    return *this;
}
bimap::left_iterator bimap::left_iterator::operator++(int) {
    left_iterator res(*this);
    ++(*this);
    return res;
}

bimap::left_iterator& bimap::left_iterator::operator--() {
    if (current -> left_left != nullptr) {
        current = current -> left_left;
        while (current -> left_right != nullptr) current = current -> left_right;
    } else {
        while ((current -> left_parent != nullptr) && (current -> left_parent -> left_left == current)) current = current -> left_parent;
        if (current -> left_parent != nullptr) current = current -> left_parent;
    }
    return *this;
}
bimap::left_iterator bimap::left_iterator::operator--(int) {
    left_iterator res(*this);
    --(*this);
    return res;
}

bimap::right_iterator bimap::left_iterator::flip() const {
    right_iterator right;
    right.current = current;
    return right;
}

bool bimap::left_iterator::operator==(const left_iterator right) const {
    return current == right.current;
}
bool bimap::left_iterator::operator!=(const left_iterator right) const {
    return current != right.current;
}

bimap::right_t const& bimap::right_iterator::operator*() const {
    return current -> right_data;
}

bimap::right_iterator& bimap::right_iterator::operator++() {
    if (current -> right_right != nullptr) {
        current = current -> right_right;
        while (current -> right_left != nullptr) current = current -> right_left;
    } else {
        while ((current -> right_parent != nullptr) && (current -> right_parent -> right_right == current)) current = current -> right_parent;
        if (current -> right_parent != nullptr) current = current -> right_parent;
    }
    return *this;
}
bimap::right_iterator bimap::right_iterator::operator++(int) {
    right_iterator res(*this);
    ++(*this);
    return res;
}

bimap::right_iterator& bimap::right_iterator::operator--() {
    if (current -> right_left != nullptr) {
        current = current -> right_left;
        while (current -> right_right != nullptr) current = current -> right_right;
    } else {
        while ((current -> right_parent != nullptr) && (current -> right_parent -> right_left == current)) current = current -> right_parent;
        if (current -> right_parent != nullptr) current = current -> right_parent;
    }
    return *this;
}
bimap::right_iterator bimap::right_iterator::operator--(int) {
    right_iterator res(*this);
    --(*this);
    return res;
}

bimap::left_iterator bimap::right_iterator::flip() const {
    left_iterator left;
    left.current = current;
    return left;
}

bool bimap::right_iterator::operator==(const right_iterator right) const {
    return current == right.current;
}
bool bimap::right_iterator::operator!=(const right_iterator right) const {
    return current != right.current;
}

//int main() {
//    bimap a;
//    int sch = 0, genered;
//    vector < int > data;
//    for (int i = 0; i < 50000; i++) {
//        genered = rand();
//        data.push_back(genered);
//        a.insert(genered, rand());
//    }
//    int ind = 0;
//    bimap::left_iterator itt;
//    while (a.begin_left() != a.end_left()) {
//        itt = a.find_left(data[ind]);
//        a.erase(itt);
//        ++ind;
//        ++sch;
//    }
//
//    cout << sch << endl;
//    
//}

