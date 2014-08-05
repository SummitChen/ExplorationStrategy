//
//  DoubleList.h
//  ExplorationStrategy
//
//  Created by Chen Si on 24/07/2014.
//
//

#ifndef ExplorationStrategy_DoubleList_h
#define ExplorationStrategy_DoubleList_h

namespace KERNEL{
    
    template<class Type>
    struct Node{
        Type data;
        Node<Type>* prior;
        Node<Type>* next;
    };
    
    template<class Type>
    class DoubleList{
    
    public:
        
        DoubleList();
        DoubleList(const DoubleList<Type>& otherList);
        ~DoubleList();
        
        //void createListForward();
        void initList();
        
        bool isEmptyList();
        int  length();
        
        void  destroyList();
        bool  getFirstData(Type& firstItem);
        bool  search(const Type newItem);
        void  insertFirst(const Type newItem);
        void  insertLast(const Type newItem);
        bool  insertBefore(const int pos, const Type newItem);
        bool  insertAfter(const int pos, const Type newItem);
        void  deleteNode(const Type deleteItem);
        void  deleteNode(const int pos, Type& deleteItem);
        Node<Type>* begin();
        Node<Type>* end();
        
        
        void  reverse();
        const DoubleList<Type>& operator=(const DoubleList<Type>& otherList);
        
    protected:
        int len;
        Node<Type>* Head;
        
    };
    
    template<class Type>
    DoubleList<Type>::DoubleList(){
        Head = new Node<Type>;
        Head->next = NULL;
        len = 0;
    }
    
    template<class Type>
    DoubleList<Type>::DoubleList(const DoubleList<Type>& otherList){
        
        Head = new Node<Type>;
        Head->next = NULL;
        Head->prior = NULL;
        len = 0;
        Node<Type>* current = Head;
        Node<Type>* otherListCurrent = otherList.Head->next;
        
        while (otherListCurrent != NULL) {
            
            Node<Type>* newNode = new Node<Type>;
            newNode->data = otherListCurrent->data;
            newNode->next = current->next;
            current->next = newNode;
            newNode->prior = current;
            current = current->next;
            otherListCurrent = otherListCurrent->next;
            len++;
            
        }
    }
    
    template<class Type>
    const DoubleList<Type>& DoubleList<Type>::operator=(const DoubleList<Type>&otherList){
        Node<Type>* current = Head;
        Node<Type>* otherListCurrent = otherList.Head->next;
        
        if ( this != &otherList) {
            
            if ( current->next != NULL) {
                initList();
            }
            
            while (otherListCurrent != NULL) {
                Node<Type>* newNode = new Node<Type>;
                newNode->data = otherListCurrent->data;
                newNode->next = current->next;
                current->next = newNode;
                newNode->prior = current;
                current = current->next;
                otherListCurrent = otherListCurrent->next;
                len++;
            }
        }
        
        return *this;
    }
    
    template<class Type>
    DoubleList<Type>::~DoubleList(){
        destroyList();
    }
    
    template<class Type>
    void DoubleList<Type>::initList(){
        destroyList();
        Head = new Node<Type>;
        Head->prior = NULL;
        Head->next = NULL;
        len = 0;
    }
    
    template<class Type>
    bool DoubleList<Type>::isEmptyList(){
        if ( Head->next == NULL) {
            return true;
        }else{
            return false;
        }
    }
    
    template<class Type>
    int DoubleList<Type>::length(){
        return len;
    }
    
    template<class Type>
    void DoubleList<Type>::destroyList(){
        Node<Type>* current;
        while (Head != NULL) {
            current = Head;
            Head = current->next;
            delete current;
            current = NULL;
        }
        
        Head = NULL;
        len = 0;
    }
    
    template<class Type>
    bool DoubleList<Type>::getFirstData(Type& firstItem){
        if (!isEmptyList()) {
            firstItem = (Head->next)->data;
            return true;
        }else{
            return false;
        }
    }
    
    template<class Type>
    bool DoubleList<Type>::search(const Type searchItem){
      
        Node<Type>* current;
        
        if (isEmptyList()) {
            return false;
        }else{
            current = Head->next;
            while ( current != NULL && current->data != searchItem ) {
                current = current->next;
            }
            if ( current != NULL) {
                return true;
            }else{
                return false;
            }
        }
    }
    
    template<class Type>
    void DoubleList<Type>::insertFirst(const Type newItem){
        Node<Type> *first = Head->next;
        Node<Type> *newNode = new Node<Type>;
        newNode->data = newItem;
        
        if ( !isEmptyList()) {
            first->prior = newNode;
            newNode->next = Head->next;
            newNode->prior = Head;
            Head->next = newNode;
        }else{
            newNode->next = Head->next;
            newNode->prior = Head;
            Head->next = newNode;
        }
    }
    
    template<class Type>
    void DoubleList<Type>::insertLast(const Type newItem){
       
        Node<Type>* newNode = new Node<Type>;
        newNode->data = newItem;
        Node<Type>* current = Head;
        while (current->next != NULL){
            current = current->next;
        }
        
        newNode->next = current->next;
        newNode->prior = current;
        current->next = newNode;
    }
    
    template<class Type>
    bool DoubleList<Type>::insertBefore(const int pos, const Type newItem){
        int i = 1;
        Node<Type>* current = Head->next;
        if ( pos < 1 || pos > len) {
            return false;
        }
        
        Node<Type>* newNode = new Node<Type>;
        newNode->data = newItem;
        
        if ( 1 == pos) {
            current->prior = newNode;
            newNode->next = Head->next;
            newNode->prior = Head;
            Head->next = newNode;
        }else{
            while ( i < pos - 1) {
                current = current->next;
                i ++;
            }
            
            newNode->next = current->next;
            current->next->prior = newNode;
            newNode->prior = current;
            current->next = newNode;
        }
        len++;
        
        return true;
    }
    
    template<class Type>
    bool DoubleList<Type>::insertAfter(const int pos, const Type newItem){
        int i = 1;
        Node<Type>* current = Head->next;
        if( pos < 1 || pos > len){
            return false;
        }
        Node<Type>* newNode = new Node<Type>;
        newNode->data = newItem;
        while( i < pos){
            current = current->next;
            i ++;
        }
        
        newNode->next = current->next;
        current->next->prior = newNode;
        newNode->prior = current;
        current->next = newNode;
        len++;
    }
    
    template<class Type>
    void DoubleList<Type>::deleteNode(const Type deleteItem)
    {
        Node<Type>* current = Head->next;
        Node<Type>* trailCurrent = Head;
        if ( isEmptyList()) {
            return;
        }else{
            while (current != NULL && current->data != deleteItem) {
                trailCurrent = current;
                current = current->next;
            }
            
            if ( current == NULL) {
                return;
            }else{
                if ( current->next != NULL) {
                    current->next->prior = trailCurrent;
                }
                
                trailCurrent->next = current->next;
                delete current;
                len--;
            }
        }
    }
    
    template<class Type>
    void DoubleList<Type>::deleteNode(const int pos, Type& deleteItem){
        int i = 1;
        Node<Type>* trailCurrent = Head;
        Node<Type>* current;
        
        if ( pos < 1 || pos > len) {
            deleteItem = -1;
            return;
        }
        while ( i < pos) {
            trailCurrent = trailCurrent->next;
            i ++;
        }
        
        current = trailCurrent->next;
        
        if ( current->next != NULL) {
            current->next->prior = trailCurrent;
        }
        
        trailCurrent->next = current->next;
        deleteItem = current->data;
        delete current;
        len--;
    }
    
    template<class Type>
    void DoubleList<Type>::reverse()
    {
        bool isFirst = true;
        Node<Type>* current = Head->next;
        Head->next = NULL;
        
        if ( current == NULL) {
            return;
        }else{
            while ( current != NULL) {
               
                Node<Type>* nextCurrent = current->next;
                current->next = Head->next;
                Head->next = current;
                current->prior = Head;
                if ( current->next != NULL) {
                    current->next->prior = current;
                }
                
                current = nextCurrent;
                
            }
        }
    }
    
    template<class Type>
    Node<Type>* DoubleList<Type>::begin(){
        return Head->next;
    }
    
    template<class Type>
    Node<Type>* DoubleList<Type>::end(){
        return NULL;
    }
}


#endif
