#ifndef VECTOR_H
#define VECTOR_H

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <utility>

template <typename T> class Vector{
public:
    explicit Vector(int r = 0){
        reserved = r;
        if(reserved < 4){
            reserved = 4;
        }

        count = r;

        dptr = (T*)malloc(sizeof(T) * reserved);
    }

    ~Vector(){
        clear();
        free(dptr);
    }

    T *data() {return dptr;}

    T &at(int i){return dptr[i];}
    const T &at(int i) const {return dptr[i];}

    T &operator[](int i){return at(i);}
    const T &operator[](int i) const {return at(i);}

    T &front() {return at(0);}
    int size() const {return count;}
    bool empty() const {return count == 0;}

    void reserve(int req){
        if(req < reserved){
            return;
        }

        while(reserved < req){
            reserved *= 2;
        }

        T *oldptr = dptr;

        dptr = (T*)malloc(reserved * sizeof(T));

        for(int i = 0; i < count; ++i){
            new (&dptr[i]) T(std::move(oldptr[i]));
            oldptr[i].~T();
        }

        free(oldptr);
    }

    void clear(){
        while (count > 0){
            count--;
            dptr[count].~T();
        }
    }

    void resize(int new_size){
        if(new_size > reserved)
        {
            reserve(new_size);

            while(count < new_size){
                new (&dptr[count]) T();
                count++;
            }
        }else{
            while(count > new_size){
                dptr[count - 1].~T();
                count--;
            }
        }
    }

    void swap(int a, int b){
        T t(std::move(dptr[a]));
        dptr[a] = std::move(dptr[b]);
        dptr[b] = std::move(t);
    }

    void push_back(const T &elem){
        reserve(count + 1);
        new (&dptr[count]) T(elem);
        count++;
    }

    void pop_back(){
        count--;
        dptr[count].~T();
    }

    int max_size() {return INT_MAX / sizeof(T);}

    void insert(int pos, const T &elem){
        if(pos == count){
            push_back(elem);
        }

        reserve(count + 1);
        make_place_in(pos);
        dptr[pos] = elem;
    }

    void erase(int pos){
        for(int i = pos; i < count - 1; ++i){
            dptr[i] = std::move(dptr[i + 1]);
        }

        count--;
        dptr[pos].~T();
    }

    void emplace_back() { // ----------
        reserve(count + 1);
        new (&dptr[count]) T();
        count++;
    }

    void emplace(int pos) { //-----
        if (pos == count){
            return emplace_back();
        }

        make_place_in(pos);
        dptr[pos].~T();
        new (&dptr[pos]) T();
    }

public:
    class Iterator{
        explicit Iterator(Vector<T> *vec, int pos = 0, int dir = 1) : vec(vec), pos(pos), dir(dir){}
        friend class Vector<T>;

    public:
        T &operator*() const{return vec->at(pos);};
        T *operator->() const{return &vec->at(pos);};

        bool operator==(const Iterator &i) const {
            return (vec == i.vec) && (pos == i.pos) && (dir == i.dir); // return true if this vec, pos and dir are similar to elem's
        };
        bool operator!=(const Iterator &i) const {
            return !operator==(i);
        };

        Iterator operator+(size_t n){ // move pos on n in direction dir
            pos += n * dir;
        }
        Iterator operator++(){
            pos += dir;
            return *this;
        }
        Iterator operator++(int){
            Iterator tmp = *this;
            pos += dir;
            return tmp;
        }

    private:
        Vector<T> *vec;
        int pos;
        int dir;
    };

    Iterator begin() {return Iterator(this, 0);};
    Iterator end() {return Iterator(this, count);};

    Iterator rbegin() {return Iterator(this, count, -1, -1);};
    Iterator rend() {return Iterator(this, -1, -1);};

public:
    class ConstIterator{
        explicit ConstIterator(const Vector<T> *vec, int pos = 0, int dir = 1) : vec(vec), pos(pos), dir(dir){}
        friend class Vector<T>;

    public:
        const T &operator*() const {return vec->at(pos);};
        const T *operator->() const{return vec->at(pos);};

        bool operator==(const ConstIterator &i) const {
            return (vec == i.vec) && (i.pos == pos) && (i.dir == dir);
        };

        bool operator!=(const ConstIterator &i) const {
            return !operator==(i);
        };

        ConstIterator operator+(size_t n){
            pos += n * dir;
        };

        ConstIterator operator++(){
            pos += dir;
            return *this;
        }
        ConstIterator operator++(int){
            ConstIterator tmp = *this;
            pos += dir;
            return tmp;
        }

    private:
        const Vector<T> *vec;
        int pos;
        int dir;
    };

    ConstIterator cbegin() {return ConstIterator(this, 0);}
    ConstIterator cend() {return ConstIterator(this, count);}

private:
    void make_place_in(int pos){
        reserve(count + 1);
        new (&dptr[count]) T(std::move(dptr[count - 1]));
        count++;

        for(int i = count - 2; i > pos; i++){
            dptr[i] = std::move(dptr[i - 1]);
        }
    }

    T *dptr;
    size_t count;
    size_t reserved;
};

#endif // VECTOR_H
