#ifndef __BIGINT_HPP__
#define __BIGINT_HPP__
#include <iostream>
#include <cstring>
const size_t INITIAL_CAPACITY = 15;
class BigString;
class BigInt;
class OutOfRange;
class InvalidSize;
class InvalidNumber;
class OutOfRange{
    public:
        OutOfRange(size_t LineNumber){std::cout << "\nBigString Out Of Range at:" << LineNumber <<"\n";}
};
class InvalidSize{
    public:
        InvalidSize(size_t LineNumber){std::cout<<"\nInvalid Size Of BigString at:" << LineNumber << "\n";}
};
class InvalidNumber{
    public:
        InvalidNumber(size_t LineNumber){std::cout << "\nInvalid Number For BigInt at:" << LineNumber << "\n";}
};
class DividedByZero{
    public:
        DividedByZero(size_t LinenNumber){std::cout << "\nDivided By Zero at:" << LinenNumber << "\n";}
};
class BigString{
    public:
        friend BigString operator+(const BigString &a, const BigString &b);
        friend bool operator==(const BigString &a, const BigString &b);
        friend bool operator!=(const BigString &a, const BigString &b);
        friend bool operator>(const BigString &a, const BigString &b);
        friend bool operator>=(const BigString &a, const BigString &b);
        friend bool operator<(const BigString &a, const BigString &b);
        friend bool operator<=(const BigString &a, const BigString &b);
        friend std::istream& operator>>(std::istream& in, BigString &s);
        friend std::ostream& operator<<(std::ostream& out, const BigString &s);
    private:
        char* data = nullptr;
        size_t sz = 0;
        size_t cap = 0;

        void doubleCapacity(){
            cap *= 2;
            char* tmp = new char[cap];
            for(size_t i = 0; i < sz; ++i) tmp[i] = data[i];
            delete [] data;
            data = tmp;
            tmp = nullptr;
        }
    public:
        BigString(){
            cap = INITIAL_CAPACITY;
            data = new char[INITIAL_CAPACITY];
        }
        BigString(const char* s){
            sz = strlen(s);
            cap = sz + 1;
            data = new char[cap];
            for(size_t i = 0; i < sz; ++i) data[i] = *(s + i);
        }
        BigString(const BigString &s){
            if(data == s.data) return;
            if(!sz){
                sz = 0;
                cap = INITIAL_CAPACITY;
                data = new char[INITIAL_CAPACITY];
            }
            else{
                sz = s.sz;
                cap = s.cap;
                data = new char[cap];
                for(size_t i = 0; i < sz; ++i) s.data[i] = data[i];
            }
        }
        BigString(BigString &&s){
            sz = s.sz;
            cap = s.cap;
            data = s.data;
            s.data = nullptr;
        }
        BigString(size_t size){
            if(size <= 0) throw InvalidSize(__LINE__);
            sz = size;
            cap = size;
            data = new char[cap];
        }
        ~BigString(){
            if(data) delete [] data;
            data = nullptr;
            sz = 0; cap = 0;
        }
    public:
        inline size_t size() const{
            return sz;
        }
        inline size_t length() const{
            return sz;
        }
        inline size_t capacity() const{
            return cap;
        }

    public:
        int insert(size_t pos, const BigString &o){
            if(pos < 0 || pos > sz - 1 || sz == 0) throw OutOfRange(__LINE__);
            cap = cap + o.cap;
            sz = sz + o.sz;
            char *tmp = new char[cap];
            for(size_t i = 0; i < pos; ++i) tmp[i] = data[i];
            for(size_t i = pos; i <= pos + o.sz; ++i) tmp[i] = o.data[i - pos];
            for(size_t i = pos; i < sz; ++i) tmp[i + o.sz] = data[i];
            delete [] data;
            data = tmp;
            tmp = nullptr;
            return pos;
        }

        int append(const char a){
            if(cap == sz) doubleCapacity();
            data[sz] = a;
            sz++;
            return sz - 1;
        }

        int find_first_of(const BigString &t){
            int *p = new int[t.sz];
            p[0] = -1;
            size_t i = 0,j = 1;
            for(i = 1; i < t.sz; ++i){
                j = i - 1;
                while(j >= 0 && t.data[p[j] + 1] != t.data[i]) j = p[j];
                if(j < 0) p[i] = -1;
                else p[i] = p[j] + 1;
            }
            i = 0, j = 0;
            while(i < sz && j < t.sz){
                if(data[i] == t.data[j]){
                    ++i; ++j;
                }
                else if(j == 0) ++i;
                else j = p[j - 1] + 1;
            }
            delete [] p;
            p = nullptr;
            if(j == t.sz) return i - j;
            else return -1;
        }
    public:
        char& operator[](size_t pos){
            if(pos < 0 || pos >= sz) throw OutOfRange(__LINE__);
            else return data[pos];
        }
        const char& operator[](size_t pos) const{
            if(pos < 0 || pos >= sz) throw OutOfRange(__LINE__);
            else return data[pos];
        }
        char& at(size_t pos){
            return this -> operator [](pos);
        }
        const char& at(size_t pos) const{
            return this -> operator [](pos);
        }
        BigString& operator=(const BigString &s){
            if(s.data == data) return *this;
            if(data){
                delete [] data;
                data = nullptr;
            }
            sz = s.sz;
            cap = s.cap;
            if(cap == 0){
                data = nullptr;
            }
            else{
                data = new char[cap];
                for(size_t i = 0; i < sz; ++i){
                    data[i] = s.data[i];
                }
            }
            return *this;
        }
        BigString& operator=(BigString &&s){
            sz = s.sz;
            cap = s.cap;
            data = s.data;
            s.data = nullptr;
            return *this;
        }

        BigString& operator=(const char* s){
            if(data){
                delete [] data;
                data = nullptr;
            }
            sz = strlen(s);
            cap = sz + 1;
            data = new char[cap];
            for(size_t i = 0 ;i < sz; ++i) data[i] = *(s + i);
            return *this;
        }
};

BigString operator+(const BigString &a, const BigString &b){
    BigString tmp(a.cap + b.cap);
    tmp.sz = a.sz + b.sz;
    size_t pos = 0;
    for(size_t i = 0; i < a.sz; ++i) tmp[pos++] = a[i];
    for(size_t i = 0; i < b.sz; ++i) tmp[pos++] = b[i];
    return tmp;
}

bool operator==(const BigString &a, const BigString &b){
    if(a.sz != b.sz) return 0;
    for(size_t i = 0; i < a.sz; ++i){
        if(a.data[i] != b.data[i]) return 0;
    }
    return 1;
}

bool operator!=(const BigString &a, const BigString &b){
    return !(a == b);
}

bool operator>(const BigString &a, const BigString &b){
    size_t k = a.sz <= b.sz ? a.sz : b.sz;
    for(size_t i = 0; i < k; ++i){
        if(a.data[i] < b.data[i]) return 0;
        if(a.data[i] > b.data[i]) return 1;
    }
    if(a.sz > b.sz) return 1;
    else return 0;
}

bool operator>=(const BigString &a, const BigString &b){
    size_t k = a.sz <= b.sz ? a.sz : b.sz;
    for(size_t i = 0; i < k; ++i){
        if(a.data[i] < b.data[i]) return 0;
        if(a.data[i] > b.data[i]) return 1;
    }
    if(a.sz >= b.sz) return 1;
    else return 0;
}

bool operator<(const BigString &a, const BigString &b){
    return !(a >= b);
}

bool operator<=(const BigString &a, const BigString &b){
    return !(a > b);
}

std::istream& operator>>(std::istream& in, BigString &s){
    s.sz = 0;
    while(true){
        if(in.peek() == '\n' || in.peek() == ' ' || in.peek() == EOF){
            in.get();
            if(s.sz == 0) continue;
            else break;
        }
        in.get(s.data[s.sz++]);
        if(s.sz == s.cap) s.doubleCapacity();
    }
    return in;
}

std::ostream& operator<<(std::ostream &out, const BigString &s){
    for(size_t i = 0; i < s.sz; ++i){
        out << s.data[i];
    }
    return out;
}


class BigInt{
    public:
        friend BigInt operator+(const BigInt &a, const BigInt &b);
        friend BigInt operator-(const BigInt &a, const BigInt &b);
        friend BigInt operator*(const BigInt &a, const BigInt &b);
        friend BigInt operator/(const BigInt &a, const BigInt &b);
        friend BigInt operator%(const BigInt &a, const BigInt &b);
        friend bool operator==(const BigInt &a, const BigInt &b);
        friend bool operator!=(const BigInt &a, const BigInt &b);
        friend bool operator>(const BigInt &a, const BigInt &b);
        friend bool operator>=(const BigInt &a, const BigInt &b);
        friend bool operator<(const BigInt &a, const BigInt &b);
        friend bool operator<=(const BigInt &a, const BigInt &b);
        friend std::istream& operator>>(std::istream &in, BigInt &a);
        friend std::ostream& operator<<(std::ostream &out, const BigInt &a);
    private:
        bool minus = 0;
        int *data = nullptr;
        size_t sz = 0;
        size_t cap = 0;
        void doubleCapacity(){
            cap *= 2;
            int *tmp = new int[cap];
            for(size_t i = 0; i < cap / 2; ++i) tmp[i] = data[i];
            for(size_t i = cap / 2; i < cap; ++i) tmp[i] = 0;
            delete [] data;
            data = tmp;
            tmp = nullptr;
        }
        static int absCompare(const BigInt &a, const BigInt &b) {
            if(a.sz != b.sz){
                return a .sz > b.sz ? 1 : 0;
            }
            else{
                for(size_t i = a.sz - 1; i >= 0; --i){
                   if(i >= a.sz) break;
                   if(a.data[i] > b.data[i]) return 1;
                   if(a.data[i] < b.data[i]) return 0;
                }
                return 2;
            }
        }
    public:
        BigInt(){
            sz = 1;
            cap = INITIAL_CAPACITY;
            data = new int[INITIAL_CAPACITY];
            for(size_t i = 0; i < cap; ++i) data[i] = 0;
        }
        BigInt(const long long &a) : BigInt(){
            sz = 0;
            long long b = a;
            if(b < 0){
                minus = 1;
                b = -b;
            }
            while(b > 0){
                data[sz++] = b % 10;
                if(sz == cap) doubleCapacity();
                b /= 10;
            }
        }
        BigInt(const char* s){
            bool zeroflag = 0;
            if(*s == '-'){
                minus = 1;
                ++s;
            }
            else minus = 0;
            while(*s == '0'){
                ++s;
                zeroflag = 1;
            }
            sz = strlen(s);
            cap = sz * 2;
            if(sz == 0 && zeroflag == 1){
                sz = 1;
                cap = 2;
                data = new int[cap];
                data[0] = 0;
                return;
            }
            else data = new int[cap];
            for(size_t i = strlen(s) - 1; i >= 0; --i){
                if(i > strlen(s) - 1) break; //if --(size_t) when size_t = 0, it will become +infinite.
                if(s[i] - '0' < 0 || s[i] - '0' > 9) throw InvalidNumber(__LINE__);
                data[sz - 1 - i] = s[i] - '0';
            }
        }
        BigInt(const BigInt &a){
            minus = a.minus;
            sz = a.sz;
            cap = a.cap;
            data = new int[cap];
            for(size_t i = 0; i < sz; ++i) data[i] = a.data[i];
        }
        BigInt(BigInt &&a){
            minus = a.minus;
            sz = a.sz;
            cap = a.cap;
            data = a.data;
            a.data = nullptr;
        }
        ~BigInt(){
            if(data) delete [] data;
            sz = 0; cap = 0; data = nullptr;
        }
    public:
        BigInt& operator=(const BigInt &a){
            if(data == a.data) return *this;
            if(data) delete [] data;
            minus = a.minus;
            sz = a.sz;
            cap = a.cap;
            data = new int[cap];
            for(size_t i = 0; i < sz; ++i) data[i] = a.data[i];
            return *this;
        }
        BigInt& operator=(BigInt &&a){
            if(data == a.data) return *this;
            if(data) delete [] data;
            minus = a.minus;
            sz = a.sz;
            cap = a.cap;
            data = a.data;
            a.data = nullptr;
            return *this;
        }
        BigInt& operator++(){
            if(minus == 1){
                if(sz == 1 && data[0] == 1){
                    minus = 0;
                    data[0] = 0;
                    return *this;
                }
                else{
                    data[0]--;
                    for(size_t i = 0; i < sz - 1; ++i){
                        if(data[i] < 0){
                            data[i] += 10;
                            data[i + 1]--;
                        }
                    }
                    if(data[sz - 1] == 0) sz--;
                    return *this;
                }
            }
            else{
                data[0]++;
                for(size_t i = 0; i < sz; ++i){
                    data[i + 1] += data[i] / 10;
                    data[i] %= 10;
                }
                if(data[sz] > 0){
                    sz++;
                    if(sz == cap) doubleCapacity();
                }
                return *this;
            }
        }
        BigInt operator++(int){
            BigInt res(*this);
            this->operator++();
            return res;
        }
        BigInt& operator--(){
            if(minus == 0){
                if(sz == 1 && data[0] == 1){
                    data[0]--;
                    return *this;
                }
                data[0]--;
                for(size_t i = 0; i < sz - 1; ++i){
                    if(data[i] < 0){
                        data[i] += 10;
                        data[i + 1]--;
                    }
                }
                if(data[sz - 1] == 0) sz--;
                return *this;
            }
            else{
                if(sz == 1 && data[0] == 0){
                    minus = 1;
                    data[0] = 1;
                    return *this;
                }
                else{
                    data[0]++;
                    for(size_t i = 0; i < sz; ++i){
                        data[i + 1] += data[i] / 10;
                        data[i] %= 10;
                    }
                    if(data[sz] > 0){
                        sz++;
                        if(sz == cap) doubleCapacity();
                    }
                return *this;
                }
            }
        }
        BigInt operator--(int){
            BigInt res(*this);
            this -> operator--();
            return res;
        }
    public:
        inline bool ifMinus() const{ return minus;}
        inline size_t size() const{return sz;}
        inline size_t length() const{return sz;}
    public:
        BigInt operator-() const{
            BigInt tmp(*this);
            tmp.minus = !minus;
            return tmp;
        }
        BigInt abs() const{
            BigInt res(*this);
            res.minus = 0;
            return res;
        }
};

BigInt operator+(const BigInt &a, const BigInt &b){
    if(a.minus == b.minus){
        BigInt res;
        const BigInt *hi = nullptr, *lo = nullptr;
        int carry = 0;
        int cmp =  0;
        res.minus = a.minus;
        cmp = BigInt::absCompare(a, b);
        if(cmp >= 1){hi = &a;lo = &b;}
        else{hi = &b;lo = &a;}
        if(res.data) delete [] res.data;
        res.cap = hi -> cap;
        res.data = new int[res.cap];
        for(size_t i = 0; i < res.cap; ++i) res.data[i] = 0;
        res.sz = hi -> sz;
        if(res.cap == res.sz) res.doubleCapacity();
        for(size_t i = 0; i < lo -> sz; ++i) res.data[i] = hi -> data[i] + lo -> data[i];
        for(size_t i = lo -> sz; i < hi -> sz; ++i) res.data[i] = hi -> data[i];
        for(size_t i = 0; i < hi -> sz; ++i){
            res.data[i] += carry;
            carry = res.data[i] / 10;
            res.data[i] %= 10;
        }
        if(carry){res.data[hi -> sz]++;res.sz++;}
        return res;
    }
    else{
        if(a.minus == 1) return b - (-a);
        else return a - (-b);
    }
}
BigInt operator-(const BigInt &a, const BigInt &b){
    if(a.minus == b.minus){
        BigInt res;
        const BigInt *hi = nullptr, *lo = nullptr;
        size_t pos = 0;
        int carry = 0;
        int cmp = BigInt::absCompare(a, b);
        if(cmp == 2){
            res.minus = 0;
            return res;
        }
        if(cmp == 1){hi = &a; lo = &b;}
        else{hi = &b; lo = &a;}
        if(a.minus == 0){
            if(cmp == 1) res.minus = 0;
            else res.minus = 1;
        }
        else{
            if(cmp == 1) res.minus = 1;
            else res.minus = 0;
        }
        res.cap = hi -> cap;
        if(res.data) delete [] res.data;
        res.data = new int[res.cap];
        for(size_t i = 0; i < res.cap; ++i) res.data[i] = 0;
        for(size_t i = 0; i < lo -> sz; ++i) res.data[i] = hi -> data[i] - lo -> data[i];
        for(size_t i = lo -> sz; i < hi -> sz; ++i) res.data[i] = hi -> data[i];
        for(size_t i = 0; i < hi -> sz; ++i){
            res.data[i] -= carry;
            if(res.data[i] < 0){
                res.data[i] += 10;
                carry = 1;
            }
            else carry = 0;
        }
        if(carry == 1) res.data[hi -> sz - 1]--;
        pos = hi -> sz;
        while(res.data[(pos--) - 1] == 0){}
        res.sz = pos + 1;
        return res;
    }
    else{
        return a + (-b);
    }
}
BigInt operator*(const BigInt &a, const BigInt &b){
    BigInt res;
    int carry = 0;
    if(res.data) delete [] res.data;
    res.minus = a.minus == b.minus ? 0 : 1;
    res.sz = a.sz + b.sz;
    res.cap = a.cap + b.cap;
    res.data = new int[res.cap];
    for(size_t i = 0; i < res.cap; ++i) res.data[i] = 0;
    for(size_t i = 0; i < b.sz; ++i){
        for(size_t j = 0; j < a.sz; ++j ){
            res.data[i + j] += carry + a.data[j] * b.data[i];
            carry = res.data[i + j] / 10;
            res.data[i + j] %= 10;
        }
        if(carry > 0){
            res.data[i + a.sz] += carry;
            carry = 0;
        }
    }
    while(res.sz > 0 && res.data[(res.sz--) - 1] == 0){}
    res.sz++;
    return res;
}
BigInt operator/(const BigInt &a, const BigInt &b){
    if(b.sz == 1 && b.data[0] == 0) throw DividedByZero(__LINE__);
    BigInt res;
    if(res.data){
        delete [] res.data;
        res.data = nullptr;
    }
    res.sz = 0;
    res.cap = 0;
    res.minus = a.minus == b.minus ? 0 : 1;
    int cmp = BigInt::absCompare(a, b);
    if(cmp == 0) return BigInt();
    else if(cmp == 2){
        res.sz = 1;
        res.cap = 2;
        res.data = new int[res.cap];
        for(size_t i = 0; i < res.cap; ++i) res.data[i] = 0;
        res.data[0] = 1;
        return res;
    }
    else{
        res.sz = a.sz - b.sz + 1;
        res.cap = res.sz * 2;
        res.data = new int[res.cap];
        for(size_t i = 0; i < res.cap; ++i) res.data[i] = 0;
        res.data[a.sz - b.sz] = 0;
        for(size_t i = res.sz - 1; i >= 0; --i){
            if(i >= res.sz) break;
            res.data[i] = 1;
            cmp = BigInt::absCompare(a, res * b);
            if(cmp == 1){
                int lastCmp = 1, currentCmp = -1;
                for(int j = 2; j <= 9; ++j){
                    res.data[i] = j;
                    currentCmp = BigInt::absCompare(a, res * b);
                    if(currentCmp == 2) return res;
                    else if(currentCmp == 0 && lastCmp == 1){
                        res.data[i]--;
                        lastCmp = currentCmp;
                        break;
                    }
                }
            }
            else if(cmp == 2) return res;
            else --res.sz;
        }
        return res;
    }
}
BigInt operator%(const BigInt &a, const BigInt &b){
    return a - a / b * b;
}
bool operator==(const BigInt &a, const BigInt &b){
    if(a.minus != b.minus) return 0;
    if(a.sz != b.sz) return 0;
    for(size_t i = 0; i < a.sz; ++i){
        if(a.data[i] != b.data[i]) return 0;
    }
    return 1;
}
bool operator!=(const BigInt &a, const BigInt &b){return !(a == b);}
bool operator>(const BigInt &a, const BigInt &b){
    if(a.minus != b.minus){
        if(a.minus > b.minus) return 0;
        else return 1;
    }
    else{
        if(a.minus == 0){
            if(a.sz != b.sz){
                if(a.sz > b.sz) return 1;
                else return 0;
            }
            else{
                for(size_t i = 0; i < a.sz; ++i){
                    if(a.data[i] > b.data[i]) return 1;
                    else if(a.data[i] < b.data[i]) return 0;
                }
                return 0;
            }
        }
        else{
            if(a.sz != b.sz){
                if(a.sz > b.sz) return 0;
                else return 1;
            }
            else{
                for(size_t i = 0; i < a.sz; ++i){
                    if(a.data[i] > b.data[i]) return 0;
                    else if(a.data[i] < b.data[i]) return 1;
                }
                return 0;
            }
        }
    }
}
bool operator>=(const BigInt &a, const BigInt &b){
    if(a.minus != b.minus){
        if(a.minus > b.minus) return 0;
        else return 1;
    }
    else{
        if(a.minus == 0){
            if(a.sz != b.sz){
                if(a.sz > b.sz) return 1;
                else return 0;
            }
            else{
                for(size_t i = 0; i < a.sz; ++i){
                    if(a.data[i] > b.data[i]) return 1;
                    else if(a.data[i] < b.data[i]) return 0;
                }
                return 1;
            }
        }
        else{
            if(a.sz != b.sz){
                if(a.sz > b.sz) return 0;
                else return 1;
            }
            else{
                for(size_t i = 0; i < a.sz; ++i){
                    if(a.data[i] > b.data[i]) return 0;
                    else if(a.data[i] < b.data[i]) return 1;
                }
                return 1;
            }
        }
    }
}
bool operator<(const BigInt &a, const BigInt &b){
    return !(a >= b);
}
bool operator<=(const BigInt &a, const BigInt &b){
    return !(a > b);
}
std::istream& operator>>(std::istream &in, BigInt &a){
    if(a.data){
        delete [] a.data;
        a.data = nullptr;
    }
    bool zeroflag = 0;
    while(in.peek() == '\n' || in.peek() == ' ' || in.peek() == EOF) in.get();
    if(in.peek() == '-'){a.minus = 1; in.get();}
    else{a.minus = 0;}
    if(in.peek() == '\n' || in.peek() == ' ' || in.peek() == EOF) throw InvalidNumber(__LINE__);
    int *tmp = new int[INITIAL_CAPACITY];
    size_t tmpcap = INITIAL_CAPACITY;
    size_t tmpsz = 0;
    while(in.peek() == '0'){
        zeroflag = 1;
        in.get();
    }
    while(true){
        if(in.peek() == '\n' || in.peek() == ' ' || in.peek() == EOF) break;
        if(in.peek() - '0' < 0 || in.peek() - '0' > 9) throw InvalidNumber(__LINE__);
        else{
            tmp[tmpsz++] = in.get() - '0';
            if(tmpsz == tmpcap){
                tmpcap *= 2;
                int *tmp1 = new int[tmpcap];
                for(size_t i = 0; i < tmpsz; ++i) tmp1[i] = tmp[i];
                delete []tmp;
                tmp = tmp1;
                tmp1 = nullptr;
            }
        }
    }
    if(zeroflag == 1 && tmpsz == 0){
        a.minus = 0;
        tmp[0] = 0;
        tmpsz = 1;
    }
    a.sz = tmpsz;
    a.cap = tmpcap;
    a.data = new int[tmpcap];
    for(size_t i = tmpsz - 1; i >= 0; --i) {
        if(i >= a.sz) break;
        a.data[tmpsz - 1 - i] = tmp[i];
    }
    delete [] tmp;
    return in;
}
std::ostream& operator<<(std::ostream &out, const BigInt &a){
    if(a.minus) out << "-";
    for(size_t i = a.sz - 1; i >= 0; --i){
        if(i >= a.sz) break;
        out << a.data[i];
    }
    return out;
}


#endif



























