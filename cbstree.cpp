// ============================================================================
// File: cbstree.cpp
// ============================================================================
// This header file contains the implementation of the CBSTree class. It uses 
// the template parameter "NodeType" for the type of values that are stored in 
// the tree.
// ============================================================================

#include    <fstream>
#include    <iostream>
#include    <cstdlib>
using namespace std;
#include    "cbstree.h"
#include    "math.h"

// ==== CBSTree::CBSTree ======================================================
//
// This is the copy constructor for the CBSTree class.
//
// Access: public
//
// Input:
//      other [IN]  -- a constant reference to a CBSTree object.
//
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>::CBSTree(const CBSTree<NodeType>  &other)
{
    // perform deep copy if they are not the same tree.
    m_root = NULL;
    if (m_root != other.m_root)
    {
        DestroyTree();
        CopyTree(other.m_root);
    }

}  // end of "CBSTree<NodeType>::CBSTree"



// ==== CBSTree::CopyTree =====================================================
//
// This recursive function creates a copy of a CBSTree. It receives a pointer
// to the source tree's root, creates a copy and returns a pointer to the root
// of the copy.
//
// Access: private
//
// Input:
//      sourcePtr [IN]          -- a pointer to the source tree's root
//      
//  Output:
//      A pointer to the root of the copied tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*    CBSTree<NodeType>::CopyTree(
                                        const CTreeNode<NodeType>  *sourcePtr)
{
    // traverse the source tree in pre-order to get the value that will assign
    // to new tree.
    if (NULL != sourcePtr)
    {
        InsertItem(sourcePtr->m_value);
        CopyTree(sourcePtr->m_left);
        CopyTree(sourcePtr->m_right);
    }
    return m_root;

}  // end of "CBSTree<NodeType>::CopyTree"



// ==== CBSTree::CountNodes ===================================================
//
// This recursive function derives the current height and number of nodes in
// the tree.  The height is a zero-based integer value, which represents the
// length of the longest path from the root to a leaf (counting the edges, not
// the nodes).  This function is called by CBSTree::GetTreeInfo so that the
// client may determine the total number of nodes and the height of the tree.
//
// Access: protected
//
// Input:
//      nodePtr [IN]        -- a pointer to a tree node; initially this is the 
//                             root
//
//      currDepth [IN]      -- the depth of the parent node (initial value is -1)
//
//      numNodes [IN/OUT]   -- a reference to an int that will contain the 
//                             total number of nodes in the tree (initial value 
//                             is zero, set by the caller)
//
// Output:
//      The length of the longest path from the initial root.
// 
// ============================================================================

template    <typename  NodeType>
int     CBSTree<NodeType>::CountNodes(const CTreeNode<NodeType>  *nodePtr
                                                        , int  currDepth
                                                        , int  &numNodes )const
{
    int		leftHeight = 0;
    int		rightHeight = 0;
    
    // make sure there is a tree node coming into the function.
    if (nodePtr == NULL)
    {
        numNodes = 0;
        return -1;
    }
    
    // count number of node and height of the tree.
    else
    {
        if (NULL != nodePtr->m_left)
        {
            leftHeight = CountNodes(nodePtr->m_left, currDepth + 1, numNodes);
            ++numNodes;
        }
        if (NULL != nodePtr->m_right)
        {
            rightHeight = CountNodes(nodePtr->m_right, currDepth + 1, numNodes);
            ++numNodes;
        }
    }
    
    // pick the longest path from the parent node to one of the leaf
    if ((currDepth < leftHeight) || (currDepth < rightHeight))
    {
        if (leftHeight > rightHeight)
        {
            currDepth = leftHeight;
        }
        else 
        {
            currDepth = rightHeight;
        }
    }
    return currDepth;
    
}  // end of "CBSTree::CountNodes"



// ==== CBSTree::Delete =======================================================
//
// This function deletes a target node from the tree.  The function finds the 
// correct location for the target node by calling itself recursively. It then
// returns the address of the (potentially new) root of the tree.
//
// Access: protected
//
// Input:
//      targetItem [IN]     -- a pointer to a NodeType object that contains the
//                             search key value
//
//      nodePtr [IN]        -- a pointer to a tree node (initially this is 
//                             usually the root).
//
//      bItemDeleted [OUT]  -- a reference to a bool that will indicate if the
//                             target item was actually removed from the tree;
//                             if that's the case it will have a value of true,
//                             else it will have a value of false
//
// Output:
//      A pointer to the (potentially new) root of the tree.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Delete(
                                        const NodeType  &targetItem
                                        , CTreeNode<NodeType>  *nodePtr
                                        , bool  &bItemDeleted)
{
    CTreeNode<NodeType>		*childPtr = NULL;
    CTreeNode<NodeType>		*tempPtr = NULL;
    
    // make sure the tree node is valid.
    if (NULL == nodePtr)
    {
        return NULL;
    }
    
    // perform recursive to the tree node that has the target.
    if (targetItem.GetDistance() < nodePtr->m_value.GetDistance())
    {
        nodePtr->m_left = Delete(targetItem, nodePtr->m_left, bItemDeleted);
    }
    else if (targetItem.GetDistance() > nodePtr->m_value.GetDistance())
    {
        nodePtr->m_right = Delete(targetItem, nodePtr->m_right, bItemDeleted);
    }
        
    // delete item and adjust pointer if neccessary.
    else
    {
        
        // delete tree node with two children.
        if ((NULL != nodePtr->m_left) && (NULL != nodePtr->m_right))
        {
            tempPtr = FindMinNode(nodePtr->m_right);
            nodePtr->m_value = tempPtr->m_value;
            nodePtr->m_right = Delete(nodePtr->m_value, nodePtr->m_right
                                                      , bItemDeleted);
        }
            
        // delete tree node with zero or one child.
        else
        {
            if (nodePtr->m_right)
            {
                childPtr = nodePtr->m_right;
            }
            else if (nodePtr->m_left)
            {
                childPtr = nodePtr->m_left;
            }
            delete nodePtr;
            bItemDeleted = true;
            return childPtr;
        }
    }
    return nodePtr;

}  // end of "CBSTree<NodeType>::Delete"



// ==== CBSTree::DeleteItem ===================================================
//
// This function allows the caller to delete a target node from the tree.  The
// input parameter points to a fully-initialized tree node value.
//
// Access: public
//
// Input:
//      targetItem [IN]  -- a const pointer to an initialized  NodeType object
//
// Output:
//      A value of false if the target item is not in the tree, otherwise a 
//      value of true is returned and the item is removed from the tree.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::DeleteItem(const NodeType  &targetItem)
{
    bool	bResult = false;
    
    // make sure the target is in the tree, then delete the target.
    if (false == RetrieveItem(targetItem))
    {
        return false;
    }
    else
    {
        m_root = Delete(targetItem, m_root, bResult);
    }
    return bResult;
    
}  // end of "CBSTree<NodeType>::DeleteItem"



// ==== CBSTree::DestroyNodes =================================================
//
// This function performs a recursive postorder descent down the tree, 
// releasing all allocated memory.
//
// Access: protected
//
// Input:
//      nodePtr [IN/OUT]    -- a pointer to a tree node (initially the root)
//      
// Output:
//      Nothing.
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::DestroyNodes(CTreeNode<NodeType>  *const nodePtr)
{
    // perform recursive postorder delete. We will call 'Delete' function
    // to release memory when we get to the node that we want to delete.
    if (NULL != nodePtr)
    {
        DestroyNodes(nodePtr->m_left);
        DestroyNodes(nodePtr->m_right);
        bool	bResult = false;
        nodePtr->m_left = NULL;
        nodePtr->m_right = NULL;
        Delete(nodePtr->m_value, nodePtr, bResult);
    }

}  // end of "CBSTree<ItemType>::DestroyNodes"



// ==== CBSTree::FindMinNode ==================================================
//
// This function finds the node with the smallest value, using the input node
// pointer as a starting point.  It returns a pointer to the target node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a tree node
//
// Output:
//      A pointer to the target node.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::FindMinNode(
                            CTreeNode<NodeType>  *nodePtr) const
{
    // Get the left most item of the input node pointer.
    if (NULL != nodePtr)
    {
        while (nodePtr->m_left)
        {
            nodePtr = nodePtr->m_left;
        }
    }
    return nodePtr;
}  // end of "CBSTree<NodeType>::FindMinNode"



// ==== CBSTree::GetTreeInfo ==================================================
//
// This function allows the caller to get the current height of the tree.
//
// Access: public
//
// Input:
//      numNodes [OUT]  -- a reference to an int that will contain the total 
//                         number of nodes currently in the tree
//
//      height [OUT]    -- a reference to an int that will contain the height 
//                         of the tree; this is a zero-based value that 
//                         represents the longest path from the root to a leaf 
//                         (counting edges, not the nodes)
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::GetTreeInfo(int  &numNodes, int  &height) const
{
    // if there is at least one node in the tree, hand control over to 
    // 'CountNodes' member function to get the exact number of node and
    // height of the tree. otherwise, the tree will have the height of -1
    // and zero number of node by default.
    if (m_root != NULL)
    {
        numNodes = 1;
        height = 0;
        height = CountNodes(m_root, height, numNodes);
    }
    else
    {
        numNodes = 0;
        height = -1;
    }

}  // end of "CBSTree::GetTreeInfo"



// ==== CBSTree::InOrder ======================================================
//
// This function performs an in-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a NodeType object (this is a recursive
//                         function, initially this points to the root).
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a 
//                         const reference to a NodeType object as input, and 
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrder(const CTreeNode<NodeType> *const nodePtr
                                        , void (*fPtr)(const NodeType&)) const
{
    // Perform an in-order traversal through the tree
    if (NULL != nodePtr)
    {
        InOrder(nodePtr->m_left, fPtr);
        fPtr(nodePtr->m_value);
        InOrder(nodePtr->m_right, fPtr);
    }

}  // end of "CBSTree<NodeType>::InOrder"



// ==== CBSTree::InOrderTraversal =============================================
//
// This function allows the caller to execute an in-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const
//                     reference to a NodeType object as input, and returns 
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::InOrderTraversal(void  (*fPtr)(const NodeType&)) const
{
    
    // call 'InOrder' function to perform in-order traversal
    if (NULL != m_root)
    {
        InOrder(m_root, fPtr);
    }
    
}  // end of "CBSTree<NodeType>::InOrderTraversal"



// ==== CBSTree::Insert =======================================================
//
// This function inserts a new node into the tree.  It finds the correct
// location for the new node by calling itself recursively. If the new record
// is unique, a copy is created and inserted into the tree. Then the address of
// the (potentially new) root of the tree is returned.
//
// If the root data member of this class is NULL upon entry, it is initialized
// with the value of the nodePtr parameter.
//
// Access: protected
//
// Input:
//      newItem [IN]    -- a pointer to a NodeType object; it is assumed that 
//                         the object is fully initialized and ready to be 
//                         inserted (node of neighbor)
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually 
//                         the root)
//
//      treeHeight [IN] -- the current height of tree(useful for k-d tree)
// Output:
//      A pointer to the (potentially new) root of the tree
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Insert(const NodeType  &newItem
			, CTreeNode<NodeType>  *nodePtr, const int treeHeight)
{
    // get pointer to function that return the coordinate
    int currDim = treeHeight % DIMENSIONAL;
    double (FieldNode::*coordFunc)() const = NULL;
    if (currDim == 0)
      coordFunc = &FieldNode::GetXCoord;
    else if (currDim == 1)
      coordFunc = &FieldNode::GetYCoord;
    else if (currDim == 2)
      coordFunc = &FieldNode::GetZCoord;

    // add a new node to the tree.
    if (NULL == nodePtr)
    {
        nodePtr = new CTreeNode<NodeType>;
        nodePtr->m_value = newItem;
        nodePtr->m_left = nodePtr->m_right = NULL;
    }
    // apply k-d tree insert algorithm
    else if ((newItem.*coordFunc)() < (nodePtr->m_value.*coordFunc)())
    {
	   nodePtr->m_left = Insert(newItem, nodePtr->m_left, treeHeight + 1);
    }
    else
    {
	   nodePtr->m_right = Insert(newItem, nodePtr->m_right, treeHeight + 1);
    }
    return nodePtr;
    
}  // end of "CBSTree<NodeType>::Insert"



// ==== CBSTree::InsertItem ===================================================
//
// This function allows the caller to insert a new node into the tree.  The
// input parameter points to a fully-initialized tree node object.
//
// Access: public
//
// Input:
//      newItem [IN]    -- a pointer to a NodeType object; it is assumed that 
//                         the object is fully initialized and ready to be 
//                         inserted
//
// Output:
//      A value of true if the item was successfully inserted into the tree, 
//      false otherwise.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::InsertItem(const NodeType  &newItem)
{
    // make sure the 'newItem' is not exist in the tree, then perform 
    // insertion by calling protected member function.
    if (true == RetrieveItem(newItem))
    {
        return false;
    }
    else
    {
	   m_root = Insert(newItem, m_root, 0);
        if (NULL == m_root)
        {
            return false;
        }
    }
    return true;
    
}  // end of "CBSTree<NodeType>::InsertItem"



// ==== CBSTree::PostOrder ====================================================
//
// This function performs a post-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a NodeType object (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a 
//                         const reference to a NodeType object as input, and 
//                         returns nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrder(const CTreeNode<NodeType>  *const nodePtr
                                        , void (*fPtr)(const NodeType&)) const
{
    // Performs a post-order traversal through the tree
    if (NULL != nodePtr)
    {
        PostOrder(nodePtr->m_left, fPtr);
        PostOrder(nodePtr->m_right, fPtr);
        fPtr(nodePtr->m_value);
    }

}  // end of "CBSTree<NodeType>::PostOrder"



// ==== CBSTree::PostOrderTraversal ===========================================
//
// This function allows the caller to execute a post-order traversal through
// the tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const 
//                     reference to a NodeType object as input, and returns 
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PostOrderTraversal(void  (*fPtr)(const NodeType&)) const
{
    // execute a post-order traversal through the tree
    if (NULL != m_root)
    {
        PostOrder(m_root, fPtr);
    }
}  // end of "CBSTree<NodeType>::PostOrderTraversal"



// ==== CBSTree::PreOrder =====================================================
//
// This function performs a pre-order traversal through the tree, calling the
// "fPtr" parameter for each node.
//
// Access: protected
//
// Input:
//      nodePtr [IN]    -- a pointer to a NodeType object (this is a recursive
//                         function, initially this points to the root)
//
//      fPtr [IN]       -- a pointer to a non-member function that takes a 
//                         const reference NodeType object as input, and returns 
//                         nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrder(const CTreeNode<NodeType>  *const nodePtr
                                    , void  (*fPtr)(const NodeType&)) const
{
    // perform pre-order traversal.
    if (NULL != nodePtr)
    {
        fPtr(nodePtr->m_value);
        PreOrder(nodePtr->m_left, fPtr);
        PreOrder(nodePtr->m_right, fPtr);
    }

}  // end of "CBSTree<NodeType>::PreOrder"



// ==== CBSTree::PreOrderTraversal ============================================
//
// This function allows the caller to execute a pre-order traversal through the
// tree, and have the "fPtr" parameter called for each node in the tree.
//
// Access: public
//
// Input:
//      fPtr [IN]   -- a pointer to a non-member function that takes a const 
//                     reference to a NodeType object as input, and returns 
//                     nothing
//
// Output:
//      Nothing
//
// ============================================================================

template    <typename  NodeType>
void    CBSTree<NodeType>::PreOrderTraversal(void (*fPtr)(const NodeType&)) const
{
    // call protected member function to perform pre-order traversal.
    if (NULL != m_root)
    {
        PreOrder(m_root, fPtr);
    }

}  // end of "CBSTree<NodeType>::PreOrderTraversal"



// ==== CBSTree::Retrieve =====================================================
//
// This function finds the node in the tree whose value equals that of the
// tree node pointer parameter.  If locates the target node by calling itself
// recursively.  If the node does not exist in the tree, a value of NULL is
// returned.
//
// Access: public
//
// Input:
//      target [IN]     -- a reference to a NodeType object; it is assumed that
//                         the object is fully initialized so a search can be
//                         performed
//
//      nodePtr [IN]    -- a pointer to a tree node (initially this is usually 
//                         the root)
//
//      height [IN]     -- tree height         
// Output:
//      If a tree node is found that has the same value as the target
//      parameter, then a pointer to  the node in the tree is returned.
//      Otherwise, a value of NULL is returned.
//
// ============================================================================

template    <typename  NodeType>
CTreeNode<NodeType>*  CBSTree<NodeType>::Retrieve(const NodeType  &target
		  , CTreeNode<NodeType>  *nodePtr, const int height) const
{
        int currDim = height % DIMENSIONAL;

	// get pointer to function
	double (FieldNode::*coordFunc)() const = NULL;
	if (currDim == 0)
	  coordFunc = &FieldNode::GetXCoord;
	else if (currDim == 1)
	  coordFunc = &FieldNode::GetYCoord;
	else if (currDim == 2)
	  coordFunc = &FieldNode::GetZCoord;

    // check to see if recursive reach base case (if this base case is true,
    // then the target is not in the tree).
    if (NULL == nodePtr)
    {
        return NULL;
    }

    if ((nodePtr->m_value.GetXCoord() == target.GetXCoord())
	&& (nodePtr->m_value.GetYCoord() == target.GetYCoord())
	&& (nodePtr->m_value.GetZCoord() == target.GetZCoord()))
    {
	   return nodePtr;
    }
    
    // perform searching for target.
    if ((target.*coordFunc)() < (nodePtr->m_value.*coordFunc)())
    {
	   nodePtr = Retrieve(target, nodePtr->m_left, height + 1);
    }
    else 
    {
	   nodePtr = Retrieve(target, nodePtr->m_right, height + 1);
    }
    return nodePtr;

}  // end of "CBSTree<NodeType>::Retrieve"



// ==== CBSTree::RetrieveItem =================================================
//
// This function allows the caller to retrieve a target node from the tree.
// The input parameter points to a fully-initialized tree node value.
//
// Access: public
//
// Input:
//      target [IN]     -- a pointer to a NodeType object; it is assumed that
//                         the object is fully initialized so a search can be
//                         performed
//
// Output:
//      A value of true if the target item is found, false if not.
//
// ============================================================================

template    <typename  NodeType>
bool    CBSTree<NodeType>::RetrieveItem(const NodeType  &target) const
{
    // check condition of the tree.
    if (NULL == m_root)
    {
        return false;
    }
        
    // call protected member function to retrieve target.
    if (NULL == Retrieve(target, m_root, 0))
    {
        return false;
    }
    return true;
    
}  // end of "CBSTree<NodeType>::RetrieveItem"



// ==== CBSTree::operator= ====================================================
// 
// This is the overloaded assignment operator for the CBSTree class. It makes
// sure that dynamically allocated memory is handled properly, and that a 
// deep copy occurs.
// 
// Input:
//      rhs [IN]    -- a const reference to an existing CBSTree object
// 
// Output:
//      A reference to the calling object.
// 
// ============================================================================

template    <typename  NodeType>
CBSTree<NodeType>&  CBSTree<NodeType>::operator=(const CBSTree<NodeType> &rhs)
{
    // perform deep copy if they are not the same tree.
    if (m_root != rhs.m_root)
    {
        DestroyTree();
        CopyTree(rhs.m_root);
    }
    return *this;
    
}  // end of "CBSTree<NodeType>::operator="



// === CBSTree:NeighborTraversal ==============================================
// This function will call function 'Neighbor'  to get nearest neighbor
// 
// Input: -- fPtr: this is pointer to function of the function that going to 
//                 display neighbor information.
//        -- num: number of nearest neighbor user wants.
//
// Output: nothing
//
// ============================================================================

template    <typename  NodeType>
void CBSTree<NodeType>::NeighborTraversal(void (*fPtr)(const NodeType&)
					  , const NodeType &target, int &num) 
{
  vector<NodeType> listN;
  vector<NodeType> listN2;
    if (NULL != m_root)
    {
        Retrieve(target, m_root, 0);
	    NaiveNeighbor(m_root, fPtr, target, listN, 0);
	    OptNeighbor(m_root, target, listN2, 0);
    }
    for (auto it = listN.begin(); it != listN.end(); ++it)
    {
	    cout << (*it).GetName() << " " << (*it).GetDistance() << endl;
    }
    cout << "####################\n";
    for (auto it = listN2.begin(); it != listN2.end(); ++it)
    {
	    cout << (*it).GetName() << " " << (*it).GetDistance() << endl;
    }

}



// === CBSTree::NaiveNeighbor =================================================
// This function will do naive traversal to find neighbors
//
// Input: -- fPtr: this is pointer to function. It will call this function to
//                 display neighbor information.
//        -- numNeighbor: number of neighbor user wants.
//        -- height: current tree level
// Output: Nothing
//
// ============================================================================

template    <typename  NodeType>
void CBSTree<NodeType>::NaiveNeighbor(CTreeNode<NodeType> *nodePtr
		 , void (*fPtr)(const NodeType&), const NodeType &target
		 , vector<NodeType> &listN, const int height) 
{
	if (nodePtr == NULL)
	{
	    return;
	}

	NaiveNeighbor(nodePtr->m_left, fPtr, target, listN, height);
	
    int currDim = height % DIMENSIONAL;

	// get pointer to function that return coordinate
	double (FieldNode::*coordFunc)() const = NULL;
	if (currDim == 0)
	  coordFunc = &FieldNode::GetXCoord;
	else if (currDim == 1)
	  coordFunc = &FieldNode::GetYCoord;
	else if (currDim == 2)
	  coordFunc = &FieldNode::GetZCoord;

	// get distance to neighbor
	double dist = 0;
	dist = pow(nodePtr->m_value.GetXCoord() - target.GetXCoord(), 2);
	dist += pow(nodePtr->m_value.GetYCoord() - target.GetYCoord(), 2);
	dist += pow(nodePtr->m_value.GetZCoord() - target.GetZCoord(), 2);
	dist = sqrt(dist);
	nodePtr->m_value.SetDistance(dist);
	// get 6 nearest neighbor
	if ((listN.size() < NUM_NEAREST_NEIGH) && (nodePtr->m_value.GetDistance() > 0))
	{
	    listN.push_back(nodePtr->m_value);
	}
	else
	{
	   // look for biggest value and remove it
	   int index = 0;
	   double currLongest = nodePtr->m_value.GetDistance();
	   int updateIndex = -1;

	   for (auto it = listN.begin(); it != listN.end(); ++it)
	   {
	       if (currLongest < (*it).GetDistance())
	       {
		      currLongest = (*it).GetDistance();
		      updateIndex = index;
           }
	       ++index;
	   }
	   if ((updateIndex > -1) && (nodePtr->m_value.GetDistance() > 0))
	   {
	       listN.erase(listN.begin() + updateIndex);
	       listN.push_back(nodePtr->m_value);
	   }
	}

	NaiveNeighbor(nodePtr->m_right, fPtr, target, listN, height);
	
} // end of "CBSTree::NaiveNeighbor"



template    <typename  NodeType>
void CBSTree<NodeType>::OptNeighbor(CTreeNode<NodeType> *nodePtr
    , const NodeType &target, vector<NodeType> &listN, const int height)
{
    // get pointer to function that return the coordinate
    int currDim = height % DIMENSIONAL;
    double (FieldNode::*coordFunc)() const = NULL;
    if (currDim == 0)
      coordFunc = &FieldNode::GetXCoord;
    else if (currDim == 1)
      coordFunc = &FieldNode::GetYCoord;
    else if (currDim == 2)
      coordFunc = &FieldNode::GetZCoord;

    if (NULL == nodePtr)
    {
	   return;
    }
    // get 6 neighbors
    double dist = 0;
    dist = pow(nodePtr->m_value.GetXCoord() - target.GetXCoord(), 2);
    dist += pow(nodePtr->m_value.GetYCoord() - target.GetYCoord(), 2);
    dist += pow(nodePtr->m_value.GetZCoord() - target.GetZCoord(), 2);
    dist = sqrt(dist);
    nodePtr->m_value.SetDistance(dist);
    if ((listN.size() < NUM_NEAREST_NEIGH) && (nodePtr->m_value.GetDistance() > 0))
    {
	   listN.push_back(nodePtr->m_value);
    }
    else
    {
    	// look for biggest value and remove it
    	int index = 0;
    	double currLongest = nodePtr->m_value.GetDistance();
    	int updateIndex = -1;

    	for (auto it = listN.begin(); it != listN.end(); ++it)
    	{
    	   if (currLongest < (*it).GetDistance())
    	   {
    	       currLongest = (*it).GetDistance();
    		  updateIndex = index;
    	   }
    	   ++index;
    	}
    	if ((updateIndex > -1) && (nodePtr->m_value.GetDistance() > 0))
    	{
            listN.erase(listN.begin() + updateIndex);
    	    listN.push_back(nodePtr->m_value);
    	}
    }

    // traverse to find nearest neighbor
    if ((target.*coordFunc)() < (nodePtr->m_value.*coordFunc)())
    {
    	OptNeighbor(nodePtr->m_left, target, listN, height + 1);
    	// detemine if we need to look at the other side
    	double biggest = (*(listN.begin())).GetDistance();
    	for (auto it = listN.begin(); it != listN.end(); ++it)
    	{
    	   if ((*it).GetDistance() > biggest)
    	   {
    		  biggest = (*it).GetDistance();
    	   }
    	}
    	if ((biggest > abs((nodePtr->m_value.*coordFunc)() - (target.*coordFunc)()))
    	    || (listN.size() < NUM_NEAREST_NEIGH))
    	{
    	   OptNeighbor(nodePtr->m_right, target, listN, height + 1);
    	}
    }
    else
    {
    	OptNeighbor(nodePtr->m_right, target, listN, height + 1);
    	// detemine if we need to look at the other side
    	double biggest = (*(listN.begin())).GetDistance();
    	for (auto it = listN.begin(); it != listN.end(); ++it)
    	{
    	   if ((*it).GetDistance() > biggest)
    	   {
    		  biggest = (*it).GetDistance();
    	   }
    	}
    	if ((biggest > abs((nodePtr->m_value.*coordFunc)() - (target.*coordFunc)()))
    	    || (listN.size() < NUM_NEAREST_NEIGH))
    	{
    	   OptNeighbor(nodePtr->m_left, target, listN, height + 1);
    	}
    }
}
