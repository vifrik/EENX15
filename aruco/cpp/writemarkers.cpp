#include <opencv2/opencv.hpp>
#include "math.h"
#include "writevec.h"
#include <math.h>

using namespace cv;

int main(int argc, char** argv) {
    std::vector<Affine3d> markers;

    Vec3d r1(-M_PI_2, 0, 0);
    Vec3d t1(1.0,0,0);
    Affine3d marker1(oMath::eulerAnglesToRotationMatrix(r1), t1);
    markers.push_back(marker1);

    Vec3d r2(-M_PI_2, 0, -M_PI_2);
    Vec3d t2(0,1.0,0);
    Affine3d marker2(oMath::eulerAnglesToRotationMatrix(r2), t2);
    markers.push_back(marker2);

    FileStorage fs_write("markers.txt",1);
    writevec::writeVectorAffine3d(fs_write, "markers", markers);
    fs_write.release();

    return 0;
}