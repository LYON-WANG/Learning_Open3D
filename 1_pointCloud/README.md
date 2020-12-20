# Learning_Open3D
The examples included above are based on the tutorial from Open3D using both Python and C++. Hope this document can help you! 

## Geometry Point Cloud Function Summary:
  - Read a point cloud from file  .xyz  .xyzn  .xyzrgb   .pts   .ply   .pcd 
  ```
    Python: xxx = 03d.io.read_point_cloud("file path") 
    C++: 
  ```
  - visualize the point cloud
    [(Python Parameters)](http://www.open3d.org/docs/release/python_api/open3d.visualization.draw_geometries.html)
  ```
    Python: o3d.visualization.draw_geometries([xxx], zoom=float....)
    C++: open3d::visualization::DrawGeometries(geometry_ptrs, window_name, width, height, left, top);
  ```
  - Voxel Downsampling uses a regular voxel grid
  ```
    Python: xxx = xxx.voxel_down_sample(voxel_size = float)
    C++: auto downsampled = cloud_ptr -> VoxelDownSample(float);
  ```
  - Compute normal for every point parameter[radius: searching radius, max_nn: maximum nearest neighbor]
  ```
    Python: xxx.estimate_normals()
    C++: 
  ```
  - Reads a json file that specifies polygon selection area
  ```
    Python: xxx = o3d.visualization.read_selection_polygon_volume("file path")
    C++: 
  ```
  - Filter out(Crop) points
  ```
    Python: xxx.crop_point_cloud(yyy)
    C++: 
  ```
  - Paint point cloud [xxx]
  ```
    Python: xxx.paint_uniform_color([R,G,B])
    C++: 
  ```
  - Compute the distance from the source point cloud [xxx] to a target point cloud [yyy]
  ```
    Python: xxx.compute_point_cloud_distance(yyy)
    C++: 
  ```
  - Get axis aligned/oriented bounding box
  ```
    Python: xxx.get_axis_aligned_bounding_box()
    Python: xxx.get_oriented_bounding_box()
    C++: cloud_ptr-> GetAxisAlignedBoundingBox();
    C++: cloud_ptr -> GetOrientedBoundingBox();
  ```
  - Comput the convex hull of a point cloud [xxx], which contains all points
  ```
    Python: xxx.compute_convex_hull()
    C++: 
  ```
  - DBSCAN Clustering
  ```
  Python: xxx.cluster_dbscan(eps, min_points)
  C++: 
  ```
  - Plane segmentation
   
   [distance_threshold: defines the maximum distance a point can have to an estimated plane to be considered an inlier]

   [ransac_n: defines the number of points that are randomly sampled to estimate a plane]
   
   [num_iteration: defines how often a random plane is sampled and verified]
  ```
  Python: xxx.segment_plane(distance_threshold, ransac_n, num_iterations)
  C++:
  ```
  - Hidden point removal
  ```
  Python: xxx.hidden_point_removal()
  C++:
  ```