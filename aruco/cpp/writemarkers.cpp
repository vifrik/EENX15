#include <opencv2/opencv.hpp>
#include <cmath>

#include "eulerRotation.h"
#include "writevec.h"

using namespace cv;

void genMatrix(float x, float y, float rot, std::vector<Affine3d> &markers) {
    Vec3d r1(-M_PI_2, 0, M_PI_4 + rot);
    Vec3d t1(x - cos(rot) * (0.115 / 2), y - sin(rot) * (0.115 / 2), 0);
    Affine3d marker1(eulerRotation::eulerAnglesToRotationMatrix(r1), t1);
    markers.push_back(marker1);

    Vec3d r2(-M_PI_2, 0, -M_PI_4 + rot);
    Vec3d t2(x + cos(rot) * (0.115 / 2), y + sin(rot) * (0.115 / 2), 0);
    Affine3d marker2(eulerRotation::eulerAnglesToRotationMatrix(r2), t2);
    markers.push_back(marker2);
}

int main(int argc, char **argv) {
    std::vector<Affine3d> markers;

    genMatrix(2 * 0.3, 3 * 0.3, -M_PI_2 - M_PI_4, markers);
    genMatrix(6 * 0.3, 3 * 0.3, -M_PI_2 - M_PI_4, markers);
    genMatrix(2 * 0.3, -2 * 0.3, -M_PI_4, markers);
    genMatrix(0, 0, -M_PI_2, markers);

    FileStorage fs_write("markers.txt", 1);
    writevec::writeVectorAffine3d(fs_write, "markers", markers);
    fs_write.release();

    return 0;
}
