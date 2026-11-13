#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


int equalPathsHeight(Node* root)
{
    if(root == NULL) return 0;

    if(root->left == NULL && root->right == NULL) {
        return 1;
    }

    int leftHeight = equalPathsHeight(root->left);
    int rightHeight = equalPathsHeight(root->right);

    if(leftHeight == -1 || rightHeight == -1) {
        return -1;
    }

    if(root->left != NULL &&
       root->right != NULL &&
       leftHeight != rightHeight)
    {
        return -1;
    }

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}


bool equalPaths(Node * root)
{
    return equalPathsHeight(root) != -1;
}
