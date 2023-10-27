/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){

    data = vector<T>();
    n1 = 0;
    n2 = -1;
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{  
        n2++;
        data.push_back(newItem);
        
    
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    T item = data[n1];
    n1++;
    if(n2 - n1 < n1) { 
        vector<T> new_data;

        for(int i = n1; i <= n2; i++) {
            new_data.push_back(data[i]);
        }
        data = new_data; //  since data is held by value, deallocation will be done by complier?
        // no need for free?
        n2 = n2-n1;
        n1 = 0;
    }

    return item;
    
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
       T item = data[n2];
    n2--;
    if(n2 - n1 < n1) { 
        vector<T> new_data;
        for(int i = n1; i <= n2; i++) {
            new_data.push_back(data[i]);
        }
        data = new_data; //  since data is held by value, deallocation will be done by complier? 
        n2 = n2 - n1;
        n1 = 0;
    } else {
        data.pop_back();
    }

    return item;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n2];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return ((n2 - n1) <= -1); //expected for (n2-n1) be at least -1 or greater, added '<' for unexpected behavior
}
