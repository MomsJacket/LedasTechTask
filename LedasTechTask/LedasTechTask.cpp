#include <iostream>
#include <memory>

constexpr auto eps = 1e-8;

class Vector3D {
private:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
public:
    Vector3D(double x, double y, double z = 0.0) noexcept : x(x), y(y), z(z) {}
    double GetX() const noexcept {
        return x;
    }
    double GetY() const noexcept {
        return y;
    }
    double GetZ() const noexcept {
        return z;
    }
    double Length() const noexcept{
        return x * x + y * y + z * z;
    }
};

std::ostream& operator<<(std::ostream& os, const Vector3D& v) noexcept {
    os << "(" << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << ")";
    return os;
}

Vector3D CrossProduct(const Vector3D& a, const Vector3D& b) {
    return { a.GetY() * b.GetZ() - a.GetZ() * b.GetY(),
             a.GetZ() * b.GetX() - a.GetX() * b.GetZ(),
             a.GetX() * b.GetY() - a.GetY() * b.GetX() };
}

double ScalarProduct(const Vector3D& a, const Vector3D& b) {
    return a.GetX() * b.GetX() + a.GetY() * b.GetY() + a.GetZ() * b.GetZ();
}

Vector3D Subtract(const Vector3D& a, const Vector3D& b) {
    return { a.GetX() - b.GetX(), a.GetY() - b.GetY(), a.GetZ() - b.GetZ() };
}

class Segment3D {
private:
    Vector3D start;
    Vector3D end;
public: 
    Segment3D(const Vector3D& start, const Vector3D& end) noexcept : start(start), end(end) {}
    Vector3D GetStart() const noexcept{
        return start;
    }
    Vector3D GetEnd() const noexcept {
        return end;
    }
    double Length() const noexcept{
        Vector3D v = Subtract(end, start);
        return ScalarProduct(v, v);
    }
};


std::unique_ptr<Vector3D> Intercept(const Segment3D& seg1, const Segment3D& seg2) {
    // Using parametric definition of a line to find the intersection point
    const auto v_s1 = Subtract(seg1.GetEnd(), seg1.GetStart());
    const auto v_s2 = Subtract(seg2.GetEnd(), seg2.GetStart());
    const auto v_s1s2 = Subtract(seg2.GetStart(), seg1.GetStart());

    // Check if the two segments are parallel to each other
    const auto S1crossS2 = CrossProduct(v_s1, v_s2);
    const auto crossV_length = S1crossS2.Length();
    if (crossV_length < eps) {
        return nullptr;
    }
    // Finding the parameters in the parametric equations of the straight segments
    double t1 = ScalarProduct(CrossProduct(v_s1s2, v_s2), S1crossS2) / crossV_length;
    double t2 = ScalarProduct(CrossProduct(v_s1s2, v_s1), S1crossS2) / crossV_length;

    // Check if the point lies on the segment
    if (t1 >= 0.0 && t1 <= 1.0 && t2 >= 0.0 && t2 <= 1.0) {
        return std::make_unique<Vector3D>(
            seg1.GetStart().GetX() + t1 * v_s1.GetX(),
            seg1.GetStart().GetY() + t1 * v_s1.GetY(),
            seg1.GetStart().GetZ() + t1 * v_s1.GetZ()
        );
    }

    return nullptr;
}


int main()
{
    Vector3D v11(0, 3, 3);
    Vector3D v12(3, 3, 3);

    Vector3D v21(0, 0, 0);
    Vector3D v22(11, 5, 0);

    Segment3D s1(v11, v12);
    Segment3D s2(v21, v22);
    
    std::unique_ptr<Vector3D> intersection_point = Intercept(s1, s2);
    if (intersection_point) {
        std::cout << intersection_point;
    }
    else {
        std::cout << "No intersection point found.";
    }
}