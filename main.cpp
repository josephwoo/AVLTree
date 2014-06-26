#include <iostream>
#include <fstream>
#include "Node&Tree.h"

template<typename T>
void getFile(AVLTree<T>&);

int main(int argc, char const *argv[])
{
    AVLTree<int> avlTree ;
    getFile(avlTree) ;
    AVLTreeNode<int> *root = avlTree.getTreeRoot() ;
    
    AVLTreeNode<int> *nodeDeleted = avlTree.searchByData(root, 18) ;
    
    //avlTree.deleteByMerging(nodeDeleted ) ;
    avlTree.deleteByCopying(nodeDeleted ) ;

    avlTree.printTree() ;
    
    return 0;
}

template<typename T>
void getFile(AVLTree<T> &tree)
{
    ifstream inf ;
    int i(0) ;
    inf.open("input.txt", ios::in) ; 
    if (!inf)
        cerr<<"Input Open Error !" ;

    while(inf>>i)
    {
        tree.insertNode(i) ;
    }

    inf.close() ;
}
