#!/usr/bin/env python3

# This script uses ismnx to donwload a ground truth from the internet, transformsit to utm and saves it as a shapefile that fmm will understand,

import osmnx as ox
import numpy as np
import time
import os
from shapely.geometry import Polygon

NETWORK_FILE = "./montseny-network"


def save_graph_shapefile_directional(G, filepath=None, encoding="utf-8"):
    # default filepath if none was provided
    if filepath is None:
        filepath = os.path.join(ox.settings.data_folder, "graph_shapefile")

    # if save folder does not already exist, create it (shapefiles
    # get saved as set of files)
    if not filepath == "" and not os.path.exists(filepath):
        os.makedirs(filepath)
    filepath_nodes = os.path.join(filepath, "nodes.shp")
    filepath_edges = os.path.join(filepath, "edges.shp")

    # convert undirected graph to gdfs and stringify non-numeric columns
    gdf_nodes, gdf_edges = ox.utils_graph.graph_to_gdfs(G)
    gdf_nodes = ox.io._stringify_nonnumeric_cols(gdf_nodes)
    gdf_edges = ox.io._stringify_nonnumeric_cols(gdf_edges)
    # We need an unique ID for each edge
    gdf_edges["fid"] = np.arange(0, gdf_edges.shape[0], dtype="int")
    # save the nodes and edges as separate ESRI shapefiles
    gdf_nodes.to_file(filepath_nodes, encoding=encoding)
    gdf_edges.to_file(filepath_edges, encoding=encoding)


print("osmnx version", ox.__version__)

# Download by a bounding box
# bounds = (17.4110711999999985, 18.4494298999999984, 59.1412578999999994, 59.8280297000000019) # Stocklom
bounds = (2.335337, 2.432706, 41.783415, 41.849994)  # Montseny

x1, x2, y1, y2 = bounds
boundary_polygon = Polygon([(x1, y1), (x2, y1), (x2, y2), (x1, y2)])
network_lotlan = ox.graph_from_polygon(
    boundary_polygon, network_type="all", simplify=False
)
start_time = time.time()
network_utm = ox.project_graph(network_lotlan)
save_graph_shapefile_directional(network_utm, filepath=NETWORK_FILE)
print("--- %s seconds ---" % (time.time() - start_time))
