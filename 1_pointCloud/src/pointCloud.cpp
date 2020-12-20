#include <iostream>
#include <memory>
#include <thread>
#include <random>
#include <cstdlib>

#include <Eigen/Dense>

#include "open3d/Open3D.h"

int main(int argc, char *argv[]) 
{
    //----- Create a pointer to store the point cloud -----//
    auto cloud_ptr = std::make_shared<open3d::geometry::PointCloud>();
    open3d::visualization::Visualizer visualizer; //Create a visualizer object
    // Create a pointcloud from a file Return an empty pointcloud, if fail to read the file.
    cloud_ptr = open3d::io::CreatePointCloudFromFile(argv[1]);
    if (open3d::io::ReadPointCloud(argv[1], *cloud_ptr))
    {
        open3d::utility::LogInfo("Successfully read {}", argv[1]);
    }
    else
    {
        open3d::utility::LogInfo("Failed to read {}", argv[1]);
        return 1;
    }
    // Returns 'true' if the point cloud contains points.
    bool pointcloud_has_normal = cloud_ptr->HasNormals();
    if(pointcloud_has_normal){ 
        open3d::utility::LogInfo("Pointcloud has normals.");
    }else{
        open3d::utility::LogInfo("Pointcloud has no normals.");
    }


    //----- Down sampling -----//
    open3d::utility::LogInfo("Downsample the point cloud with a voxel of 0.05");
    auto downsampled = cloud_ptr -> VoxelDownSample(0.05);
    
    //----- Vertex  normal estimation -----//
    open3d::utility::LogInfo("Recompute the normal of the downsampled point cloud");
    //Compute normal for every point parameter[radius: searching radius, max_nn: maximum nearest neighbor]
    cloud_ptr -> EstimateNormals(open3d::geometry::KDTreeSearchParamHybrid(0.1, 30));
    open3d::utility::LogInfo("Print a normal vector of the 0th point:");
    std::cout << cloud_ptr->normals_[0] << std::endl;


    //----- Crop point cloud -----//


    //----- Get axis aligned/oriented bounding box -----//
    open3d::geometry::AxisAlignedBoundingBox bounding_box_aligned = cloud_ptr -> GetAxisAlignedBoundingBox();
    std::shared_ptr<open3d::geometry::AxisAlignedBoundingBox> bounding_box(
                new open3d::geometry::AxisAlignedBoundingBox(bounding_box_aligned));
    bounding_box -> color_ = Eigen::Vector3d(1, 0, 0); // Set bounding box color


    //----- DBSCAN Clustering -----//
    // [Group local point cloud clusters together]
    std::vector<int>labels = cloud_ptr -> ClusterDBSCAN(0.02, 10, true);

    int max_label = *max_element(labels.begin(),labels.end());
    open3d::utility::LogInfo("point cloud has {:d} clusters.", max_label+1);
    std::vector<Eigen::Vector3d> vColor; // Define color vector
	vColor.reserve(1 + max_label);
    // Generate random color
	for (int i = 0; i <= max_label; i++){
		double R = (rand()%(200-0)+0)/255.0;
		double G = (rand()%(128-100)+100)/255.0;
		double B = (rand()%(250-25)+25)/255.0;
		Eigen::Vector3d c = {R, G, B};
		vColor.push_back(c);
    }
    // Color each point cloud cluster
	std::vector<Eigen::Vector3d> pcColor;
	pcColor.reserve(labels.size());
    for (int i = 0; i < labels.size(); i++)
	{
		int label = labels[i];
		if (label <0)
		{
			Eigen::Vector3d c = {0,0,0}; // noise=-1 set color to black
			pcColor.push_back(c);
		}
		else
		{
			Eigen::Vector3d c = vColor[label];
			pcColor.push_back(c);
		}
	}
    // Paint point cloud
    cloud_ptr -> colors_ = pcColor;


    //----- Plane segmentation -----//
    double distance_threshold = 0.01;
    int ransac_n = 3;
    int num_iterations = 1000;    
    std::tuple<Eigen::Vector4d, std::vector<size_t>> vRes = 
                    cloud_ptr -> SegmentPlane(distance_threshold, ransac_n, num_iterations); // Return plane model and inliers
    // [a b c d] plane model
	Eigen::Vector4d para = std::get<0>(vRes);
    // Inliers
    std::vector<size_t> selectedIndex = std::get<1>(vRes);
    
    // Paint inliers red
    std::shared_ptr<open3d::geometry::PointCloud> inPC = cloud_ptr -> SelectByIndex(selectedIndex, false);
	const Eigen::Vector3d colorIn = {1,0,0};
	inPC->PaintUniformColor(colorIn);
    // Paint inliers black
	std::shared_ptr<open3d::geometry::PointCloud> outPC = cloud_ptr -> SelectByIndex(selectedIndex, true);
	//const Eigen::Vector3d colorOut = { 0,0,0 };
	//outPC->PaintUniformColor(colorOut);


    //----- Point cloud Visualization -----//
    //open3d::visualization::DrawGeometries({downsampled}, "PointCloud", 1600, 900);
    visualizer.CreateVisualizerWindow("Open3D Test", 1600, 900);
    //visualizer.AddGeometry(cloud_ptr);
    visualizer.AddGeometry(inPC);
    visualizer.AddGeometry(outPC);
    visualizer.AddGeometry(bounding_box);
    visualizer.Run();
    visualizer.DestroyVisualizerWindow();

    return 0;
}