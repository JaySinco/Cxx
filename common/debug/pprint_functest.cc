#include <vector>
#include <string>
#include <array>
#include <map>
#include <set>
#include <queue>
#include "pprint.hpp"

enum Level { LOW, MEDIUM, HIGH };
typedef std::array<std::array<int, 3>, 3> Mat3x3;

class Foo {};

class Date {
  unsigned int month, day, year;
public:
  Date(unsigned int m, unsigned int d, unsigned int y) : month(m), day(d), year(y) {}
  friend std::ostream& operator<<(std::ostream& os, const Date& dt);
};

std::ostream& operator<<(std::ostream& os, const Date& dt) {
  os << dt.month << '/' << dt.day << '/' << dt.year;
  return os;
}

struct Vector3 {
  float x, y, z;
};

struct Mesh {
  std::vector<Vector3> vertices;
};

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
  pprint::PrettyPrinter printer(os);
  printer.print_inline(std::make_tuple(v.x, v.y, v.z));
  return os;
}

std::ostream& operator<<(std::ostream& os, const Mesh& mesh) {
  pprint::PrettyPrinter printer(os);
  printer.print("Mesh {");
  printer.indent(2);
  printer.print_inline("vertices:", mesh.vertices);
  printer.print("}");
  return os;
}

int main() {
  // std::stringstream stream;
  pprint::PrettyPrinter printer(std::cout);

  //Fundamental Types
  printer.print(5);
  printer.print(3.14f);
  printer.print(2.718);
  printer.print(true);
  printer.print('x');
  printer.print("Hello, 世界");
  printer.print(nullptr);

  // Strings
  printer.quotes(true);
  printer.print("A", "B", "C");

  // Complex Numbers
  using namespace std::complex_literals;
  std::complex<double> foo = 1. + 2.5i;
  std::complex<double> bar = 9. + 4i;
  printer.print(foo, "*", bar, "=", (foo * bar));   // parameter packing

  // Enumeration Types
  Level current_level = MEDIUM;
  std::cout << "Current level: ";
  printer.print(current_level);

  // STL Sequence Containers
  Mat3x3 matrix;
  matrix[0] = {1, 2, 3};
  matrix[1] = {4, 5, 6};
  matrix[2] = {7, 8, 9};
  printer.print("Matrix =", matrix);

  // Compact Printing
  std::vector<std::map<std::string, int>> vec {{{"a", 1}, {"b", 2}}, {{"c", 3}, {"d", 4}}};
  printer.compact(true);
  printer.print("Vec =", vec);

  // STL Associative Containers
  printer.print(std::map<std::string, std::set<int>>{ 
      {"foo", {1, 2, 3, 3, 2, 1}}, {"bar", {7, 6, 5, 4}}});

  // STL Container Adaptors
  std::priority_queue<int> queue;
  for(int n : {1,8,5,6,3,4,0,9,7,2}) queue.push(n);
    printer.print(queue);

  // Fixed-size Heterogeneous Tuples
  auto get_student = [](int id) {
    if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
    throw std::invalid_argument("id");
  };
  printer.print({ get_student(0), get_student(1), get_student(2) });

  // Class Objects
  Foo obj;
  printer.print(obj);
  Date date(04, 07, 2019);
  printer.print("Today's date is", date);

  // User-defined types
  Mesh quads = {{
    {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 0, 0}, {1, 1, 0}, {0, 1, 0},
    {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 1, 1},
    }};
  printer.print(quads);
}