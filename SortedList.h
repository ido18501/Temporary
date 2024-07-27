#pragma once
//read-only version
namespace mtm {

    template<typename T>
    class SortedList {
    private:
        int size;
        int maxSize;
        T **list;

        void add(const T &add, int index) {
            if (maxSize == size) {
                maxSize = maxSize <= 0 ? 4 : maxSize * 2;
                T **temp = new T *[maxSize];
                for (int i = 0; i < size; i++) {
                    temp[i] = list[i];
                }
                delete[] list;
                list = temp;
            }
            for (int i = size; i > index; --i) {
                list[i] = list[i - 1];
            }
            if ((list[index] = new T(add))) {
                size++;
            }
        }

        void free() {
            for (int i = 0; i < size; i++) {
                delete list[i];
            }
            delete[] list;
            list = nullptr;
            size = 0;
            maxSize = 0;
        }

        void copyFrom(const SortedList &source) {
            free();
            list = new T *[source.maxSize]; // Allocate memory for the array of pointers
            maxSize = source.maxSize;
            for (int i = 0; i < source.size; i++) {
                add(*source.list[i], i);
            }
        }


    public:
        SortedList() : size(0), maxSize(4) {
            list = new T *[maxSize];
        }

        ~SortedList() {
            free();
        }

        SortedList(const SortedList &source) : size(0), maxSize(source.maxSize),
                                               list(nullptr) {
            try {
                copyFrom(source);
            }
            catch (...) {
                free();
                throw;
            }
        }

        SortedList &operator=(const SortedList &source) {
            if (this == &source) {
                return *this;
            }
            SortedList temp(source);
            copyFrom(temp);
            return *this;
        }

        T operator[](int i) const {
            if (i < 0 || i >= size) {
                throw std::out_of_range("out of array bounds");
            }
            return *list[i];
        }

        int length() const {
            return size;
        }

        class ConstIterator;

        ConstIterator begin() const {
            return ConstIterator(this, 0);
        }

        ConstIterator end() const {
            return ConstIterator(this, size);
        }

        void insert(const T &add) {
            if (size == 0) {
                this->add(add, 0);
                return;
            }
            int low = 0;
            int high = size - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                if (*list[mid] > add) {
                    if (mid == size - 1 || !(*list[mid + 1] > add)) {
                        while (mid != size - 1 && !(add > *list[mid + 1])) {
                            mid++;
                        }
                        this->add(add, mid + 1);
                        return;
                    }
                    low = mid + 1;
                } else {
                    if (mid == 0 || !(add > *list[mid - 1])) {
                        while (mid != size - 1 && !(add > *list[mid])) {
                            mid++;
                        }
                        this->add(add, mid);
                        return;
                    }
                    high = mid - 1;
                }
            }
        }

        void remove(ConstIterator remove) {
            int index = remove.getIndex();
            if (index < 0 || index >= size) {
                return;
            }
            delete list[index];
            for (int i = index; i < size - 1; ++i) {
                list[i] = list[i + 1];
            }
            --size;
        }

        template<typename Func>
        SortedList apply(Func func) const {
            SortedList res;
            for (int i = 0; i < size; i++) {
                T temp = func(*list[i]);
                res.insert(temp);
            }
            return res;
        }

        template<typename Func>
        SortedList filter(Func func) const {
            SortedList res;
            for (int i = 0; i < size; i++) {
                if (func(*list[i])) {
                    res.insert(*list[i]);
                }
            }
            return res;
        }

    };

    template<class T>
    class SortedList<T>::ConstIterator {
    private:
        const SortedList<T> *sortedList;
        int index;

    public:
        ConstIterator(const SortedList<T> *sortedList, int index) : sortedList(
                sortedList), index(index) {}

        const T &operator*() const {
            if (index < 0 || index >= sortedList->size) {
                throw std::out_of_range("Iterator out of range");
            }
            return *sortedList->list[index];
        }

        bool operator!=(const ConstIterator &compare) const {
            return index != compare.index;
        }

        ConstIterator &operator++() {
            if (this->index >= (*sortedList).length()) {
                throw std::out_of_range("Iterator out of range");
            }
            ++index;
            return *this;
        }

        int getIndex() const {
            return index;
        }

        const T *getCurrent() const {
            return sortedList->list[index];
        }
    };

}