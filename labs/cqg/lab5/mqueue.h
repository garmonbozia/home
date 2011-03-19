#include <stdio.h>
#include <queue>

using namespace std;

template<class T> class mQueue : private queue<T>
{
public:
	void mpush(T& x)
	{   
		bool MATCH = false;
		int i      = 0;
                   
		while(i < queue<T>::size())
        {   
			if(x == queue<T>::front())
			{   
				MATCH = true;
				queue<T>::push(x);
			}   
            else
				queue<T>::push(queue<T>::front());
			queue<T>::pop();
			i++;
		}   
		if(!MATCH)
			queue<T>::push(x);
	}   

	void mpop()
	{   
		queue<T>::pop();
	}   
		
	T& mfront()
	{   
		return queue<T>::front();
	}   

	T& mback()
	{   
		return queue<T>::back();
	}  

	bool mempty()
	{
		return queue<T>::empty();
	}

	unsigned int msize ()
	{
		return queue<T>::size();
	}
		
};
