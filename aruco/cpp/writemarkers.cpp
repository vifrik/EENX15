#include <opencv2/opencv.hpp>
#include <cmath>

#include "eulerRotation.h"
#include "writevec.h"

using namespace cv;

int main(int argc, char **argv) {
    std::vector<Affine3d> markers;
    Vec3d r1(-M_PI_2, 0, M_PI_4);
    Vec3d t1(1 - (0.115/2), 0, 0);
    Affine3d marker1(eulerRotation::eulerAnglesToRotationMatrix(r1), t1);
    markers.push_back(marker1);

    Vec3d r2(-M_PI_2, 0, -M_PI_4);
    Vec3d t2(1 + (0.115/2), 0, 0);
    Affine3d marker2(eulerRotation::eulerAnglesToRotationMatrix(r2), t2);
    markers.push_back(marker2);

    Vec3d r3(0, 0, M_PI);
    Vec3d t3(1.0, 2.0, 0);
    Affine3d marker3(eulerRotation::eulerAnglesToRotationMatrix(r3), t3);
    markers.push_back(marker3);

    Vec3d r4(0, 0, 0);
    Vec3d t4(0, 0, 0);
    Affine3d marker4(eulerRotation::eulerAnglesToRotationMatrix(r4), t4);
    markers.push_back(marker4);

    FileStorage fs_write("markers.txt", 1);
    writevec::writeVectorAffine3d(fs_write, "markers", markers);
    fs_write.release();

    return 0;
}
