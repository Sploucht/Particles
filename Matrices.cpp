#include "Matrices.h"
namespace Matrices
{
  Matrix::Matrix(int _rows, int _cols)
  {
  rows = _rows;
  cols = _cols;
  a.resize(rows, vector<double>(cols));
  }
  Matrix operator+(const Matrix& a, const Matrix& b)
  {
    Matrix c(a.getRows(), a.getCols());
    if (a.getRows() == b.getRows() && a.getCols() == b.getCols())
    {
      for (int i = 0; i < a.getRows(); i++)
      {
        for (int j = 0; j < a.getCols(); j++)
        {
          c(i, j) = a(i, j) + b(i, j);
        }
      }
      return c;
    }
    else
    {
      throw runtime_error("Error: dimensions must agree");
    }
  }
        ///Matrix multiply. See description.
        ///usage: c = a * b;
  Matrix operator*(const Matrix& a, const Matrix& b)
  {
    Matrix c(a.getRows(), b.getCols());
    if (b.getRows() == a.getCols())
    {
      for (int k = 0; k < b.getCols(); k++)
      {
        for (int i = 0; i < a.getRows(); i++)
        {
          double runningSum = 0;
          for (int j = 0; j < a.getCols(); j++)
          {
            runningSum += a(i, j) * b(j, k);
          }
          c(i, k) = runningSum;
        }

      }
      return c;
    }
    else
    {
      throw runtime_error("Error: dimensions must agree");
    }
  }
        ///Matrix comparison. See description.
        ///usage: a == b
  bool operator==(const Matrix& a, const Matrix& b)
  {
    if (a.getRows() == b.getRows() && a.getCols() == b.getCols())
    {
      for (int i = 0; i < a.getRows(); i++)
      {
        for (int j = 0; j < a.getCols(); j++)
        {
          if (abs(a(i, j) - b(i, j) >= 0.001)) return false;
        }
      }
      return true;
    }
    else
    {
      return false;
    }
  }
          ///Matrix comparison. See description.
          ///usage: a != b
  bool operator!=(const Matrix& a, const Matrix& b)
  {
    if (a.getRows() == b.getRows() && a.getCols() == b.getCols())
    {
      for (int i = 0; i < a.getRows(); i++)
      {
        for (int j = 0; j < a.getCols(); j++)
        {
          if (abs(a(i, j) - b(i, j) >= 0.001)) return true;
        }
      }
      return false;
    }
    else
    {
      return true;
    }
  }
  ostream& operator<<(ostream& os, const Matrix& a)
  {
    for (int i = 0; i < a.getRows(); i++)
    {
      for (int j = 0; j < a.getCols(); j++)
      {
        os << setw(8) << a(i, j);
      }
      os << endl;
    }
    return os;
  }
        ///Output matrix.
        ///Separate columns by ' ' and rows by '\n'
      
}
