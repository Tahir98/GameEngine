#include <stdlib.h>
#include <iostream>

template <class Data>
class ArrayList {
	
private:
	unsigned int m_size = 0;
	unsigned int capacity = 32; // Default
	float growthRate = 2; // Default
	unsigned int maxCapacity = 100000; // Default
	Data* dataList;
public:
	ArrayList() {
		dataList = (Data*)malloc(sizeof(Data) * capacity);
	}

	ArrayList(const unsigned int c,const unsigned int mCapacity  = 100000,const float gRate = 2.0f) 
		: capacity(c), maxCapacity(mCapacity), growthRate(gRate) {
		if (capacity == 0) {
			std::cout << "Capacity can not be  0" << std::endl;
			capacity = 1;
		}

		if (mCapacity == 0) {
			std::cout << "Max capacity can not be  0" << std::endl;
			maxCapacity = 1;
		}

		if (growthRate <= 0.0f) {
			std::cout << "Growth rate must be higher than 0" << std::endl;
		}

		if (capacity > maxCapacity) {
			std::cout << "capacity has to be equal or lower than max capacity!!!" << std::endl;
			maxCapacity = capacity;
		}

		dataList = (Data*)malloc(sizeof(Data) * capacity);
	}

	~ArrayList() {
		free(dataList);
	}

	unsigned int size() const {
		return m_size;
	}
	void add(Data data) {
		if (m_size != capacity)
			dataList[m_size++] = data;
		else if (m_size == capacity) {
			if (capacity == maxCapacity) {
				std::cout << "Size of the ArrayList has reached the max capacity" << std::endl;
			}
			else {
				capacity = (unsigned int)(capacity * growthRate);
				if (capacity >= maxCapacity) {
					std::cout << "Capacity reach out max capacity" << std::endl;
					capacity = maxCapacity;
				}

				Data* temp = (Data*)malloc(sizeof(Data) * capacity);
				for (unsigned int i = 0; i < m_size; i++) {
					temp[i] = dataList[i];
				}

				free(dataList);
				dataList = temp;
			}
		}
	}


	void remove(const unsigned int i) {
		if (i >= m_size) {
			std::cout << "Index is higher than size" << std::endl;
		}
		else {
			for (int j = i; j < m_size - 1; j++) {
				dataList[j] = dataList[j + 1];
			}
			m_size--;
		}
	}

	void clear() { m_size = 0; }

	void setMaxCapacity(unsigned int mCapacity) {
		if (mCapacity == 0) {
			std::cout << "Max capacity can not be 0" << std::endl;
			mCapacity++;
		}
		maxCapacity = mCapacity;
		if (maxCapacity < capacity) {
			capacity = maxCapacity;
			if (m_size >= capacity)
				m_size = capacity - 1;
		}
	}

	Data* data()const {
		if (m_size != 0)
			return dataList;
		else return nullptr;
	}

	Data& operator[](unsigned int index) {
		if (index >= m_size)
			return dataList[index];
		return dataList[0];
	}
};





