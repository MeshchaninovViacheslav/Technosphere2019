//
// Created by User on 30.03.2019.
//

#ifndef TECHNOSPERE_MATRIX_H
#define TECHNOSPERE_MATRIX_H

class Matrix {
    size_t rows, columns;
    int **data = NULL;


    struct Row {
        int *data;
        size_t columns;

        Row(int *data, size_t columns) :
                data(data), columns(columns) {}

        int &operator[](size_t j) {
            if (j >= columns)
                throw std::out_of_range("");
            return data[j];
        }

        const int &operator[](size_t j) const {
            if (j >= columns)
                throw std::out_of_range("");
            return data[j];
        }
    };

public:
    Matrix(size_t rows, size_t columns) :
            rows(rows), columns(columns) {
        data = new int *[rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new int[columns];
        }
    }

    ~Matrix() {
        for (size_t i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
    }

    size_t getRows() const {
        return rows;
    }

    size_t getColumns() const {
        return columns;
    }

    Row operator[](const size_t i) {
        if (i >= rows)
            throw std::out_of_range("");
        Row tmp(data[i], columns);
        return tmp;
    }

    const Row operator[](const size_t i) const {
        if (i >= rows)
            throw std::out_of_range("");
        Row tmp(data[i], columns);
        return tmp;
    }

    Matrix &operator*=(const int n) {
        for (size_t i = 0; i != rows; ++i)
            for (size_t j = 0; j != columns; ++j)
                data[i][j] = data[i][j] * n;
        return *this;
    }

    bool operator==(const Matrix &other) const {
        if (other.getColumns() != columns || other.getRows() != rows)
            return 0;
        for (size_t i = 0; i != rows; ++i)
            for (size_t j = 0; j != columns; ++j)
                if (data[i][j] != other[i][j])
                    return 0;
        return 1;
    }

    bool operator!=(const Matrix &other) const {
        return !(*this == other);
    }
};

#endif //TECHNOSPERE_MATRIX_H
