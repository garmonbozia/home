#ifndef MQUEUE_H
#define MQUEUE_H

#include <list>
#define VARIANT2

template<class T> class mQueue
{
public:
	int push( T new_item )
	{
		typename std::list<T>::iterator i = data_.begin();

		while ( i != data_.end() )
		{
			if ((*i) == new_item)
			{
				break;
			}
			i++;
		}

		if ( i != data_.end() )
		{
		
#ifdef VARIANT1
			return -1;
#else
			(*i) = new_item;
#endif		
		}
		else
		{
			data_.push_back(new_item);
		}

		return 0;
	}

	T pop(void)
	{
		T temp;
		temp = data_.front();
		data_.pop_front();

		return temp;
	}
	
	bool empty(void)
	{
		return data_.empty();
	}

private:
	std::list<T> data_;
};

#endif
