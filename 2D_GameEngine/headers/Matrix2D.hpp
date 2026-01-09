#ifndef Matrix2D_hpp
#define Matrix2D_hpp

#include <vector>
#include <iostream>
#include <stdexcept>

/**
 * @brief Template class for a 2D Matrix (e.g. Map grid).
 * 
 * @tparam T Type of data stored (int, float, etc.)
 * @tparam Rows Number of rows
 * @tparam Cols Number of columns
 */
template <typename T, int Rows, int Cols>
class Matrix2D {
private:
    T data[Rows][Cols];
public:
    Matrix2D() {
        for(int i=0; i<Rows; ++i)
            for(int j=0; j<Cols; ++j)
                data[i][j] = T();
    }
    
    void set(int r, int c, T val) {
        if(r >= 0 && r < Rows && c >= 0 && c < Cols)
            data[r][c] = val;
    }
    
    T get(int r, int c) const {
        if(r >= 0 && r < Rows && c >= 0 && c < Cols)
            return data[r][c];
        return T(); // Default return
    }
    
    void fill(T val) {
        for(int i=0; i<Rows; ++i)
            for(int j=0; j<Cols; ++j)
                data[i][j] = val;
    }
    
    // Direct raw array access filling if needed (helper)
    void loadFromRaw(const int raw[Rows][Cols]) {
        for(int i=0; i<Rows; ++i)
            for(int j=0; j<Cols; ++j) // Assuming T is compatible with int
                data[i][j] = static_cast<T>(raw[i][j]);
    }
};

#endif /* Matrix2D_hpp */
