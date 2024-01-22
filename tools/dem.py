# Reads a geotiff file and prints metadata (TOP left coordinated in UMT, cell size, number of cells, and min and max heights).
# TODO: HK-48: Format this to so it is a well-formatted python script.
# TODO: HK-49: Do this but is c++ itself.

import numpy as np
from osgeo import gdal
import utm

DEM_FILE = "dem_2m.tif"

# open geotiff
dem = gdal.Open(DEM_FILE)
elev_grid = dem.GetRasterBand(1).ReadAsArray().astype(float)
elev_grid[elev_grid < 0] = 0  # clamp to 0 non-data values
statistics = dem.GetRasterBand(1).GetStatistics(True, True)

# show print data
gt = dem.GetGeoTransform()
cell_size = np.array([gt[1], gt[5]])
grid_top_left = np.array([gt[0], gt[3]])

grid_bottom_right = grid_top_left + np.multiply(elev_grid.shape, cell_size)

# TODO: HK-48 This can all go in one line (to_latlon accepts arrays).
lat_top_left, lon_top_left = utm.to_latlon(grid_top_left[0], grid_top_left[1], 31, "T")
lat_bottom_right, lon_bottom_right = utm.to_latlon(grid_bottom_right[0], grid_bottom_right[1], 31, "T")

print(gt)
print("Grid TOP-LEFT coords: Easting=", grid_top_left[0], "Northing=", grid_top_left[1])

print(
    "Grid BOTTOM-RIGHT coords: Easting=",
    grid_bottom_right[0],
    "Northing=",
    grid_bottom_right[1],
)
print("Grid TOP-LEFT coords: Latitude=", lat_top_left, "Longitude=", lon_top_left)
print(
    "Grid BOTTOM-RIGHT coords: Latitude=",
    lat_bottom_right,
    "Longitude=",
    lon_bottom_right,
)

print("Grid cell size (m)", cell_size)
print("Grid cells (count)", elev_grid.shape)
print("Max Height", statistics[1], " Min Height ", statistics[0])
