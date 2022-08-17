// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include <thread>

#include <open3d/Open3D.h>
namespace {
bool read_and_visualize_mesh(const std::string& file_name) {
  auto mesh_ptr = std::make_shared<open3d::geometry::TriangleMesh>();
  if (open3d::io::ReadTriangleMesh(file_name, *mesh_ptr)) {
    open3d::utility::LogWarning("Successfully read %s\n", file_name.c_str());
    if (mesh_ptr->triangles_.size() == 0) {
      open3d::utility::LogWarning("Contains 0 triangles, will read as point cloud\n");
      return false;
    }
  } else {
    open3d::utility::LogError("Failed to read %s\n\n", file_name.c_str());
    return false;
  }
  mesh_ptr->ComputeVertexNormals();
  open3d::visualization::DrawGeometries({mesh_ptr}, "Mesh " + file_name, 1600, 900);
  return true;
}

bool read_and_visualize_point_cloud(const std::string& file_name) {
  auto cloud_ptr = std::make_shared<open3d::geometry::PointCloud>();
  if (open3d::io::ReadPointCloud(file_name, *cloud_ptr)) {
    open3d::utility::LogWarning("Successfully read %s\n", file_name.c_str());
  } else {
    open3d::utility::LogError("Failed to read %s\n\n", file_name.c_str());
    return false;
  }
  cloud_ptr->NormalizeNormals();
  open3d::visualization::DrawGeometries({cloud_ptr}, "PointCloud " + file_name, 1600, 900);
  return true;
}

void visualize_sphere_mesh() {
  auto sphere = open3d::geometry::TriangleMesh::CreateSphere(1.0);
  sphere->ComputeVertexNormals();
  sphere->PaintUniformColor({0.0, 1.0, 0.0});
  open3d::visualization::DrawGeometries({sphere});
}

bool auto_visualize(const std::string& file_name) {
  bool rc = true;
  if (!read_and_visualize_mesh(file_name)) {
    rc = read_and_visualize_point_cloud(file_name);
  }
  return rc;
}
}

int main(int argc, char *argv[]) {
  open3d::utility::SetVerbosityLevel(open3d::utility::VerbosityLevel::Debug);

  if (argc < 2) {
    visualize_sphere_mesh();
  } else {
    std::string file_name(argv[1]);
    std::string file_type = file_name.substr(static_cast<size_t>(file_name.size()) - 3);
    if (file_type == "pcd") {
      read_and_visualize_point_cloud(file_name);
    } else if (file_type == "ply") {
      read_and_visualize_mesh(file_name);
    }
  }
  return 0;
}
