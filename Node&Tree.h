#include <iostream>
#include <queue>
#include <stack>

using namespace std;

template <typename T>
class AVLTreeNode
{
private:
    T data ;
    size_t index ;
    int balanceValue ;

public:
    AVLTreeNode(T data)
    {
        this->index = 0 ;
        this->data = data ;
        this->balanceValue = 0 ;
        leftChild = NULL ;
        rightChild = NULL ;
    }
    //data Interface
    T getNodeData()
    {
        return this->data ;
    }
    void setNodeData(T d)
    {
        this->data = d ;
    }
    //index Interface
    size_t getNodeIndex()
    {
        return this->index ;
    }
    void setNodeIndex(size_t tag)
    {
        this->index = tag ;
    }

    int getDeep(AVLTreeNode<T>*) ;

    //balanceValue interface
    int getNodeBalanceValue()
    {
        return this->balanceValue ;
    }
    void setNodeBalanceValue(AVLTreeNode<T> *node)
    {
        int leftValue(0), rightValue(0) ;
        leftValue = getDeep(node->leftChild ) ;
        rightValue = getDeep(node->rightChild) ;

        this->balanceValue = rightValue - leftValue ;
    }

    AVLTreeNode<T> *leftChild ;
    AVLTreeNode<T> *rightChild ;

};
template<typename T>
int AVLTreeNode<T>::getDeep(AVLTreeNode<T> *node)
{
    if(node == NULL)
        return 0 ;
    else
    {
        if(getDeep(node->leftChild ) > getDeep(node->rightChild ) )
            return getDeep(node->leftChild ) +1 ;
        else
            return getDeep(node->rightChild ) +1 ;
    }
}


template<typename T>
class AVLTree
{
private:
    AVLTreeNode<T> *root ;
    void destoryTreeNode(AVLTreeNode<T> *node)
    {
        if (node)
        {
            destoryTreeNode(node->leftChild ) ;
            destoryTreeNode(node->rightChild ) ;
            delete node ;
        }
    }

public:
    AVLTree()
    {
        this->root = NULL ;
    }
    ~AVLTree()
    {
        cout<<"Destructor !"<<endl;
        destoryTreeNode(this->root) ;
    }
    AVLTreeNode<T>* getTreeRoot()
    {
        return this->root ;
    }

	//basic Functions
	void modifyIndexAndBalanceValue() ;    //modify index & balanceValue with levelOrder（从root遍历所有节点，因为每次变动，平衡值都可能会受到影响)
    void replace(AVLTreeNode<T> *father, AVLTreeNode<T> *replaceTarget ) ;
    void leftRotation(AVLTreeNode<T> *node);
    void rightRotation(AVLTreeNode<T> *node) ;
    void balanceTree() ;        //use postOrder
    void judegToRotation(AVLTreeNode<T> *node ) ;

    //Functions
	void printTree() ;		//levelOrder to print all treeNode
    void insertNode(T &value) ;
	AVLTreeNode<T>* searchByIndex(size_t index) ;
	AVLTreeNode<T>* searchByData(AVLTreeNode<T>*, T) ;
    void deleteByMerging(AVLTreeNode<T> *node) ;
    void deleteByCopying(AVLTreeNode<T> *node) ;
};

template<typename T>
void AVLTree<T>::printTree()
{
	AVLTreeNode<T> *ptr = this->root ;
	queue<AVLTreeNode<T> *>nodeQueue ;

	if(ptr != NULL)
	{
		nodeQueue.push(ptr) ;
	}
	while( !nodeQueue.empty() )
	{
		ptr = nodeQueue.front() ;
		cout << ptr->getNodeIndex() << ' ' << ptr->getNodeData() <<' ' <<ptr->getNodeBalanceValue() << endl ;
		nodeQueue.pop() ;

		if(ptr->leftChild != NULL)
		{
			nodeQueue.push(ptr->leftChild ) ;
		}
		if(ptr->rightChild != NULL )
		{
			nodeQueue.push(ptr->rightChild ) ;
		}
	}
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::searchByData(AVLTreeNode<T> *root, T key)
{
	AVLTreeNode<T> *current = root ;
	while( (NULL != current) && (key != current->getNodeData() ) )
	{
		current = ( key < current->getNodeData() ? searchByData(current->leftChild, key) : searchByData(current->rightChild, key) ) ;
	}

	return current ;
}

template<typename T>
AVLTreeNode<T>* AVLTree<T>::searchByIndex(size_t index)
{
	AVLTreeNode<T> *current = this->root ;
	queue<AVLTreeNode<T> *> nodeQueue ;

	if(current != NULL)
	{
		nodeQueue.push(current) ;
	}
	while( !nodeQueue.empty() )
	{
		current = nodeQueue.front() ;
		nodeQueue.pop() ;

		if(current->getNodeIndex() == index)
		{
			return current ;
		}

		if(current->leftChild != NULL)
		{
			nodeQueue.push(current->leftChild ) ;
		}
		if(current-> rightChild != NULL )
		{
			nodeQueue.push(current->rightChild ) ;
		}
	}

	return NULL ;
}

template<typename T>
void AVLTree<T>::modifyIndexAndBalanceValue()
{
    queue<AVLTreeNode<T> *> nodeQueue ;
    AVLTreeNode<T> *ptr = this->root ;
    if (ptr != NULL )
    {
        nodeQueue.push(ptr) ;
    }
    while(!nodeQueue.empty() )
    {
        ptr = nodeQueue.front() ;
        ptr->setNodeBalanceValue(ptr) ;
        nodeQueue.pop() ;

        if (ptr->leftChild )
        {
            ptr->leftChild->setNodeIndex(ptr->getNodeIndex()*2 +1) ;
            nodeQueue.push(ptr->leftChild ) ;
        }
        if(ptr->rightChild)
        {
            ptr->rightChild->setNodeIndex(ptr->getNodeIndex()*2 +2) ;
            nodeQueue.push(ptr->rightChild ) ;
        }
    }
}

template<typename T>
void AVLTree<T>::balanceTree()
{
    AVLTreeNode<T> *ptr = this->root , *father = this->root ;
    stack<AVLTreeNode<T> *> nodeStack ;

    while( ptr != NULL )
    {
        for ( ; ptr->leftChild != NULL; ptr = ptr->leftChild )
        {
            nodeStack.push(ptr) ;
        }

        while( ptr != NULL && (ptr->rightChild == NULL || ptr->rightChild->getNodeData() == father->getNodeData() ) )
        {
            judegToRotation(ptr) ;
            father = ptr ;
            if (nodeStack.empty() )
            {
                return ;
            }
            ptr = nodeStack.top() ;
            nodeStack.pop() ;
        }
        nodeStack.push(ptr) ;
        ptr = ptr->rightChild ;
    }
}

template<typename T>
void AVLTree<T>::judegToRotation(AVLTreeNode<T> *node )
{
    if (node->getNodeBalanceValue() >= 2 )
    {
        if (node->rightChild->getNodeBalanceValue() > 0 )
        {
            leftRotation(node ) ;
        }
        else
        {
            rightRotation(node->rightChild ) ;
            leftRotation(node ) ;
        }
    }
    else if (node->getNodeBalanceValue() <= -2 )
    {
        if (node->leftChild->getNodeBalanceValue() < 0 )
        {
            rightRotation(node ) ;
        }
        else
        {
            leftRotation(node->leftChild) ;
            rightRotation(node ) ;
        }
    }

    modifyIndexAndBalanceValue() ;
}

template<typename T>
void AVLTree<T>::leftRotation(AVLTreeNode<T> *node )
{
    AVLTreeNode<T> *newRoot = node->rightChild ;
    if (node->getNodeIndex() == 0 )
    {
        this->root = newRoot ;
        newRoot->setNodeIndex(0) ;
    }
    else
    {
	   AVLTreeNode<T> *father = searchByIndex( (node->getNodeIndex()-1 ) /2) ;
	   newRoot->setNodeIndex(node->getNodeIndex() ) ;
       replace(father, newRoot) ;
    }

	node->rightChild = newRoot->leftChild;
	newRoot->leftChild = node ;
}

template<typename T>
void AVLTree<T>::rightRotation(AVLTreeNode<T> *node )
{
    AVLTreeNode<T> *newRoot = node->leftChild ;
    if (node->getNodeIndex() == 0 )
    {
        newRoot->setNodeIndex(0) ;
        this->root = newRoot ;
    }
    else
    {
	   AVLTreeNode<T> *father = searchByIndex( (node->getNodeIndex()-1) /2 ) ;
       newRoot->setNodeIndex(node->getNodeIndex() ) ;
	   replace(father, newRoot) ;
    }

	node->leftChild = newRoot->rightChild ;
	newRoot->rightChild = node ;
}

template<typename T>
void AVLTree<T>::replace(AVLTreeNode<T> *father, AVLTreeNode<T> *replaceTarget )
{
    if ( replaceTarget->getNodeIndex() % 2 == 0 )   //判断被删除节点 是父亲节点的左/右孩子
    {
        father->rightChild = replaceTarget ;
    }
    else
    {
        father->leftChild = replaceTarget ;
    }
}

template<typename T>
void AVLTree<T>::insertNode(T &value)
{
    AVLTreeNode<T> *p = root , *father = NULL ;
    while(p)
    {
        father = p ;
        if (p->getNodeData() < value)
        {
            p = p->rightChild ;
        }else{
            p = p->leftChild ;
        }
    }
    if(root == NULL)
    {
        root = new AVLTreeNode<T>(value) ;

    }else if ( father->getNodeData() < value)
    {
        father->rightChild = new AVLTreeNode<T>(value) ;
        father->rightChild->setNodeIndex(father->getNodeIndex()*2+2) ;
    }else{
        father->leftChild = new AVLTreeNode<T>(value) ;
        father->leftChild->setNodeIndex(father->getNodeIndex()*2+1) ;
    }

    modifyIndexAndBalanceValue() ;
    balanceTree() ;
}

//合并删除
template<typename T>
void AVLTree<T>::deleteByMerging(AVLTreeNode<T> *node)
{
    AVLTreeNode<T> *temp = node ;
    if (node != NULL && node->getNodeIndex() == 0 )     //被删除节点是根节点情况
    {
        if (node->leftChild == NULL )
        {
            this->root = node->rightChild ;
            if (this->root != NULL)
            {
                this->root->setNodeIndex(0) ;
                modifyIndexAndBalanceValue() ;
            }
        }
        else if (node->rightChild == NULL )
        {
            this->root = node->leftChild ;
            if (this->root != NULL)
            {
                this->root->setNodeIndex(0) ;
                modifyIndexAndBalanceValue() ;
            }
        }
        else
        {
            temp = node->leftChild ;
            while(temp->rightChild )    //查找左子树中按中序遍历的最后一个节点，即左子树中最大的节点
            {
                temp = temp->rightChild ;
            }

            //将查找到的节点的右指针赋值为被删除节点的右子树的根
            temp->rightChild = node->rightChild ;       //移动被删除节点的右子树
            node->leftChild->setNodeIndex(0) ;
            this->root = node->leftChild ;

            modifyIndexAndBalanceValue(this->root) ;
            balanceTree() ;

            temp = node ;   //要删除被删除节点node
        }
    }

    if (node != NULL && node->getNodeIndex() != 0 )
    {
        AVLTreeNode<T> *father = NULL ;

        if (node->leftChild != NULL && node->rightChild == NULL )       //如果被删节点没有右子树，用其左子树的根节点来代替被删除节点
        {
            size_t fatherIndex = (node->getNodeIndex() - 1 ) /2 ;   //获取father的tag
            father = searchByIndex(fatherIndex ) ;        //查找到father节点
            replace(father, node->leftChild) ;

            modifyIndexAndBalanceValue() ;
            balanceTree() ;
        }
        else if (node->rightChild != NULL && node->leftChild == NULL )  //如果被删除节点没有左子树，用其右子树的根节点来代替被删除节点
        {
            father = searchByIndex( (node->getNodeIndex() -1) /2 ) ;        //用index查找到father节点
            replace(father, node->rightChild ) ;    //用右子树的根节点代替被删除节点

            modifyIndexAndBalanceValue() ;
            balanceTree() ;
        }
        else if( node->leftChild != NULL && node->rightChild != NULL )                          //如果被删除节点左右子树都存在
        {
            temp = node->leftChild ;
            while(temp->rightChild )    //查找左子树中按中序遍历的最后一个节点，即左子树中最大的节点
            {
                temp = temp->rightChild ;
            }

            //将查找到的节点的右指针赋值为被删除节点的右子树的根
            temp->rightChild = node->rightChild ;       //移动被删除节点的右子树
            father = searchByIndex( (node->getNodeIndex() -1) /2 ) ;        //用index查找到father节点
            node->leftChild->setNodeIndex(node->getNodeIndex() ) ;
            replace(father, node->leftChild ) ;

            modifyIndexAndBalanceValue() ;
            balanceTree() ;

            temp = node ;   //要删除被删除节点node
        }
        else
        {
            father = searchByIndex( (node->getNodeIndex() -1) /2 ) ;
            if (  node->getNodeIndex() % 2 == 0)    //判断被删除节点 是父亲节点的左/右孩子
            {
                father->rightChild = NULL ;
            }
            else
            {
                father->leftChild = NULL ;
            }

            modifyIndexAndBalanceValue() ;
            balanceTree() ;
        }
    }

    delete temp ;
}

//复制删除
template<typename T>
void AVLTree<T>::deleteByCopying(AVLTreeNode<T> *node)
{
    if (node != NULL )
    {
        AVLTreeNode<T> *father , *temp = node ;

        if (node->leftChild != NULL && node->rightChild == NULL )   //如果被删除节点没有右子树，用其左子树的根节点来代替被删除节点
        {
            if (node->getNodeIndex() == 0)
            {
                node->leftChild->setNodeIndex(0) ;
                this->root = node->leftChild ;
                modifyIndexAndBalanceValue() ;
            }
            else
            {
                size_t fatherIndex = (node->getNodeIndex() - 1 ) /2 ;   //获取father的tag
                father = searchByIndex(fatherIndex ) ;        //查找到father节点
                node->leftChild->setNodeIndex(node->getNodeIndex() ) ;
                replace(father, node->leftChild) ;

                modifyIndexAndBalanceValue() ;
                balanceTree() ;
            }
        }
        else if (node->rightChild != NULL && node->leftChild == NULL)   //如果被删除节点没有左子树，用其右子树的根节点来代替被删除节点
        {
            if (node->getNodeIndex() == 0)
            {
                node->rightChild->setNodeIndex(0) ;
                this->root = node->rightChild ;

                modifyIndexAndBalanceValue() ;
            }
            else
            {
                father = searchByIndex( (node->getNodeIndex() - 1 ) /2 ) ;        //用index查找到father节点
                node->rightChild->setNodeIndex(node->getNodeIndex() ) ;
                replace(father, node->rightChild ) ;    //用右子树的根节点代替被删除节点

                modifyIndexAndBalanceValue() ;
                balanceTree() ;
            }
        }
        else if (node->leftChild != NULL && node->rightChild != NULL )  //如果被删除节点左右子树都存在
        {
            temp = node->leftChild ;
            father = node ;

            while(temp->rightChild )    //查找左子树中关键码最大的节点
            {
                father = temp ;
                temp   = temp->rightChild ;
            }
            node->setNodeData(temp->getNodeData() ) ;   //将查找到的节点的数值赋给被删除节点
            //移动找到的节点的子树
            if (father->getNodeIndex() == node->getNodeIndex() )        //如果被删除节点执行while时 动了，则变更它的左孩子指针，指向孩子的左子树
            {
                father->leftChild = temp->leftChild ;
                modifyIndexAndBalanceValue() ;
                balanceTree() ;
            }
            else    //否则，变更有孩子的指针，指向孩子的左子树
            {
                father->rightChild = temp->leftChild ;
                modifyIndexAndBalanceValue() ;
                balanceTree() ;
            }
        }
        else
        {
            if (node->getNodeIndex() != 0)
            {
                father = searchByIndex( (node->getNodeIndex() -1) /2 ) ;
                if (  node->getNodeIndex() % 2 == 0)    //判断被删除节点 是父亲节点的左/右孩子
                {
                    father->rightChild = NULL ;
                }
                else
                {
                    father->leftChild = NULL ;
                }
            }
        }

        delete temp ;
    }
}
