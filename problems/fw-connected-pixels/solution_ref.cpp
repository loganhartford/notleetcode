#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <sstream>

using namespace std;

// provided class for images
class Image {
  public:
    Image(int rows, int cols, int initial_value) :
      rows_(rows), cols_(cols), data_(rows*cols, initial_value) {}

    int Rows() const {
      return rows_;
    }

    int Cols() const {
      return cols_;
    }

    bool IsValid(int row, int col) const {
      return (row >= 0 && row < rows_ && col >= 0 && col < cols_);
    }

    const int& At(int row, int col) const {
      AssertValid(row,col);
      return data_[row * cols_ + col];
    }

    void SetAt(int row, int col, int value) {
      AssertValid(row, col);
      data_[row * cols_ + col] = value;
    }

    void Show(int max_digits) const {
      std::cout << "Image:" << std::endl;
      std::cout << std::right;
      for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
          std::cout << std::setw(max_digits + 1) << At(row,col);
        }
        std::cout << std::endl;
      }
    }

  private:
    void AssertValid(int row, int col) const {
      if (!IsValid(row,col)) {
        std::stringstream ss;
        ss << "Bad access: " << row << "," << col << std::endl;
        throw std::out_of_range(ss.str());
      }
    }

    int rows_;
    int cols_;
    std::vector<int> data_;
};

// helper function to create test images of any size
// each pixel is either 0 or 1 with uniform probability
Image CreateTestImage(int rows, int cols, int max_value, int seed = 0) {
  std::srand(seed);
  Image result(rows,cols,0);
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      result.SetAt(row, col, std::rand() % (max_value + 1));
    }
  }
  return result;
}
// end support code

int CountConnectedPixels(Image image)
{
  const int rows = image.Rows();
  const int cols = image.Cols();
  const int kRowStep[4] = {-1, 1, 0, 0};
  const int kColStep[4] = {0, 0, -1, 1};

  std::vector<bool> visited(static_cast<size_t>(rows) * cols, false);
  std::vector<std::pair<int, int>> stack;
  int largest = 0;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (image.At(row, col) == 0 || visited[row * cols + col]) continue;

      // Flood fill this component with an explicit stack: an image large
      // enough to matter would overflow the call stack under recursion.
      int size = 0;
      visited[row * cols + col] = true;
      stack.push_back({row, col});
      while (!stack.empty()) {
        const std::pair<int, int> pixel = stack.back();
        stack.pop_back();
        ++size;

        for (int dir = 0; dir < 4; ++dir) {
          const int next_row = pixel.first + kRowStep[dir];
          const int next_col = pixel.second + kColStep[dir];
          if (!image.IsValid(next_row, next_col)) continue;
          if (visited[next_row * cols + next_col]) continue;
          if (image.At(next_row, next_col) == 0) continue;
          // Mark on push, not on pop, so a pixel is never queued twice.
          visited[next_row * cols + next_col] = true;
          stack.push_back({next_row, next_col});
        }
      }

      largest = std::max(largest, size);
    }
  }

  return largest;
}
