/*
   Simple manual test for the AVL_tree library.
*/
#include <iostream>
using namespace std;

#include "AVL_tree.h"

typedef pair<uint16_t, uint16_t> dos;

int main()
{
    char chars1[]
        = { 'k', 'd', 'r', 'd', 'e', 'f', 'z', 's', 'e', 'i', 'w', 'l', 'm', 'n', 'b', 'a' };

    char chars2[] = {'A', 'N', '8', 'Y'};
    AVL_tree tree { dos { chars1[0], 0 } };

    for (size_t i = 1; i < sizeof(chars1) / sizeof(char); ++i) {
        tree.add(dos { chars1[i], 0 });
        tree.print_tree();
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
             << endl;
    }

    for (size_t i = 0; i < sizeof(chars1) / sizeof(char); ++i)
    {
        if (!tree.is_inside(dos{chars1[i], 0})) {
            cout << "character " << chars1[i] << " was not found!" << endl;
            return 1;
        }
    }

    for (size_t i = 0; i < sizeof(chars2) / sizeof(char); ++i)
    {
        if (tree.is_inside(dos{chars2[i], 0})) {
            cout << "character " << chars2[i] << " was found!" << endl;
            return 1;
        }
    }

    auto status = tree.remove(dos{'.', 0});
    if (AVL_tree::avl_status::VALUE_NOT_FOUND != status) {
        cout << "remove function didnt fail correctly!" << endl;
        return 1;
    }

    for (size_t i = 0; i < sizeof(chars1) / sizeof(char); ++i) {
        tree.remove(dos { chars1[i], 0 });
        tree.print_tree();
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
             << endl;
    }

    return 0;
}
