#include "task1.cpp"
#include <fstream>
#include <iostream>

#include <vector>

void write_results() {
  // Пример это 3 одинаковых круга, центры которых находятся на вершинах одного равностороннего треугольника
  double R = 4.5;
  double tryangle_length = 3.5;
  std::vector<Circle> circles;
  circles.push_back(Circle(0, 0, R));
  circles.push_back(Circle(tryangle_length, 0, R));
  circles.push_back(Circle(tryangle_length / 2.0, tryangle_length * sqrt(3) / 2, R));
  // S = 3 *  S_сегмента + S_треугольника
  double theta = 2.0 * acos(tryangle_length / (2.0 * R));
  double segment_area = 0.5 * R * R * (theta - sin(theta));
  double triangle_area = (sqrt(3) / 4.0) * tryangle_length * tryangle_length;
  double correct_S = 3.0 * segment_area + triangle_area;

  std::ofstream file("/Users/vladbax6/Codding/code_works/cpp/AISDSet1/results.csv");
  file << std::fixed << std::setprecision(15);
  file << "N,wide_area,narrow_area,wide_error,narrow_error\n";

  for (int n = 100; n <= 100000; n += 500) {
    double wide_area = estimate_area_wide(circles, n);
    double narrow_area = estimate_area_narrow(circles, n);

    double wide_error = wide_area - correct_S / correct_S;
    double narrow_error = narrow_area - correct_S / correct_S;

    file << n << "," << wide_area << "," << narrow_area << ","
         << wide_error << "," << narrow_error << "\n";
  }

  file.close();
}

int main() {

  write_results();

  return 0;
}
