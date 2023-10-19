# Reads a geotiff file and prints metadata (TOP left coordinated in UMT, cell size, number of cells, and min and max heights). 
# TODO: HK-48: Format this to so it is a well-formatted python script. 
# TODO: HK-49: Do this but is c++ itself. 

import numpy as np
from osgeo import gdal

DEM_FILE = "dem_2m.tif"

# open geotiff
dem = gdal.Open(DEM_FILE)
elevGrid = dem.GetRasterBand(1).ReadAsArray().astype(float)
elevGrid[elevGrid < 0] = 0  # clamp to 0 non-data values
statistics = dem.GetRasterBand(1).GetStatistics(True, True)

# show print data
gt = dem.GetGeoTransform()
cellSize = np.array([gt[1], gt[5]])
gridOrigin = np.array([gt[0], gt[3]])
print(gt)
print("Grid TOP-LEFT coords", gridOrigin)
print("Grid cell size (m)", cellSize)
print("Grid cells (count)", elevGrid.shape)
print("Max Height", statistics[1], " Min Height ", statistics[0])
