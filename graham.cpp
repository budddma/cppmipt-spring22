#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace geometry {

class Point {
 public:
  int64_t x;
  int64_t y;

  Point(int64_t x, int64_t y) : x(x), y(y) {
  }
};

class Vector {
 public:
  int64_t x;
  int64_t y;

  Vector(int64_t x, int64_t y) : x(x), y(y) {
  }
  Vector(Point p, Point q) : x(q.x - p.x), y(q.y - p.y) {
  }
};

int64_t CrossProduct(const Vector &v1, const Vector &v2) {
  return v1.x * v2.y - v1.y * v2.x;
}

Vector operator-(const Point &p1, const Point &p2) {
  Vector res(p2, p1);
  return res;
}

}  // namespace geometry

int64_t operator^(const geometry::Vector &v1, const geometry::Vector &v2) {
  return geometry::CrossProduct(v1, v2);
}

std::vector<geometry::Point> Graham(std::vector<geometry::Point> points) {

  for (size_t i = 1; i < points.size(); ++i) {
    if (points[i].x < points[0].x || (points[i].x == points[0].x && points[i].y < points[0].y)) {
      std::swap(points[i], points[0]);
    }
  }

  geometry::Point p0(points[0].x, points[0].y);

  sort(points.begin() + 1, points.end(), [&](const geometry::Point &lhs, const geometry::Point &rhs) {
    int64_t cp = (lhs - p0) ^ (rhs - p0);
    return cp < 0 || (cp == 0 && lhs.y < rhs.y) || (cp == 0 && lhs.y == rhs.y && lhs.x < rhs.x);
  });

  std::vector<geometry::Point> hull = {points[0], points[1]};

  for (size_t i = 2; i < points.size(); ++i) {
    if (points[i].x == hull.back().x && points[i].y == hull.back().y) {
      continue;
    }
    while (!hull.empty()) {
      geometry::Point temp_p = hull.back();
      hull.pop_back();
      if (((points[i] - hull.back()) ^ (temp_p - hull.back())) >= 0) {
        hull.push_back(temp_p);
        break;
      }
    }
    hull.push_back(points[i]);
  }

  return hull;
}

int64_t DoubleSquare(std::vector<geometry::Point> points) {
  int64_t s = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    s += geometry::CrossProduct(geometry::Vector(points[i].x, points[i].y),
                                geometry::Vector(points[(i + 1) % points.size()].x, points[(i + 1) % points.size()].y));
  }
  return llabs(s);
}

int main() {

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n;
  std::cin >> n;
  std::vector<geometry::Point> points;

  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(geometry::Point(x, y));
  }

  std::vector<geometry::Point> hull = Graham(points);

  std::vector<geometry::Point> new_hull;
  new_hull.emplace_back(hull[0]);
  for (size_t i = 1; i < hull.size(); ++i) {
    if (geometry::CrossProduct(hull[i - 1] - hull[i], hull[i] - hull[(i + 1) % hull.size()]) != 0) {
      new_hull.emplace_back(hull[i]);
    }
  }

  std::cout << new_hull.size() << '\n';
  for (auto p : new_hull) {
    std::cout << p.x << ' ' << p.y << '\n';
  }

  std::cout << std::setprecision(1) << std::fixed << DoubleSquare(new_hull) / 2.0l;
  return 0;
}