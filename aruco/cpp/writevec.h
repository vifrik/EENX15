//
// Created by Viktor on 2022-02-24.
//

#ifndef ARUCOCPP_WRITEVEC_H
#define ARUCOCPP_WRITEVEC_H

#include <opencv2/opencv.hpp>
#include <string.h>

using namespace cv;

namespace writevec {
    void writeVectorAffine3d(FileStorage &fs, std::string name, std::vector<Affine3d> &data) {
        fs << name;
        fs << "{";
        for (int i = 0; i < data.size(); i++)
        {
            fs << name + "_" + std::to_string(i);
            Affine3d a = data[i];
            fs << a.matrix;
        }
        fs << "}";
    }
    void readVectorAffine3d(FileStorage &fns, std::string name, std::vector<Affine3d> &data) {
        data.clear();
        FileNode fn = fns[name];
        if (fn.empty()){
            return;
        }

        FileNodeIterator current = fn.begin(), it_end = fn.end();
        for (; current != it_end; ++current)
        {
            Affine3d a;
            Mat b;

            FileNode item = *current;
            item >> b;
            a.matrix = b;

            data.push_back(a);
        }
    }
};


#endif //ARUCOCPP_WRITEVEC_H
