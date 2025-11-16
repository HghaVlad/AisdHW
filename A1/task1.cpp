#include <iostream>
#include <random>
#include <vector>

struct Circle {
  double x, y, r;

 public:
  Circle(double x, double y, double r) : x(x), y(y), r(r) {
  }
};

double estimate_area_wide(std::vector<Circle>circles, int num_points = 100000) {

  double min_x = circles[0].x;
  double max_x = circles[0].x;
  double min_y = circles[0].y;
  double max_y = circles[0].y;
  double max_r = circles[0].r;

  for (int i = 1; i < 3; i++) {
    min_x = std::min(min_x, circles[i].x);
    max_x = std::max(max_x, circles[i].x);
    min_y = std::min(min_y, circles[i].y);
    max_y = std::max(max_y, circles[i].y);
    max_r = std::max(max_r, circles[i].r);
  }

  double left = min_x - max_r;
  double right = max_x + max_r;
  double bottom = min_y - max_r;
  double top = max_y + max_r;
  double square_area = (right - left) * (top - bottom);


  std::mt19937 rng(4131);
  std::uniform_real_distribution<double> dist_x(left, right);
  std::uniform_real_distribution<double> dist_y(bottom, top);

  int count = 0;
  for (int i = 0; i < num_points; i++) {
    double x = dist_x(rng);
    double y = dist_y(rng);

    bool in_all_circles = true;
    for (Circle &circle : circles) {
      double dx = x - circle.x;
      double dy = y - circle.y;
      if (dx * dx + dy * dy >= circle.r * circle.r) {
        in_all_circles = false;
        break;
      }
    }

    if (in_all_circles) {
      count++;
    }
  }

  double area_estimate = (static_cast<double>(count) / num_points) * square_area;
  return area_estimate;
}

double estimate_area_narrow(std::vector<Circle> circles, int num_points = 100000) {
  double left = -std::numeric_limits<double>::max();
  double right = std::numeric_limits<double>::max();
  double bottom = -std::numeric_limits<double>::max();
  double top = std::numeric_limits<double>::max();

  for (int i = 0; i < 3; i++) {
    left = std::max(left, circles[i].x - circles[i].r);
    right = std::min(right, circles[i].x + circles[i].r);
    bottom = std::max(bottom, circles[i].y - circles[i].r);
    top = std::min(top, circles[i].y + circles[i].r);
  }

  if (left >= right || bottom >= top) {
    return 0.0;
  }

  double square_area = (right - left) * (top - bottom);


  std::mt19937 rng(4131);
  std::uniform_real_distribution<double> dist_x(left, right);
  std::uniform_real_distribution<double> dist_y(bottom, top);

  int count = 0;
  for (int i = 0; i < num_points; i++) {
    double x = dist_x(rng);
    double y = dist_y(rng);

    bool in_all_circles = true;
    for (Circle &circle : circles) {
      double dx = x - circle.x;
      double dy = y - circle.y;
      if (dx * dx + dy * dy >= circle.r * circle.r) {
        in_all_circles = false;
        break;
      }
    }

    if (in_all_circles) {
      count++;
    }
  }

  double area_estimate = (static_cast<double>(count) / num_points) * square_area;
  return area_estimate;
}