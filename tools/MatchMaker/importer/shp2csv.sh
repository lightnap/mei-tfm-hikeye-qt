#!/usr/bin/env bash
# This script takes the ground truth shapefile and transforms it to a csv by using ogr2ogr.
# The csv is something that we can then understand when parsing it

ogr2ogr -f "CSV" montseny-network/edges.csv montseny-network/edges.shp -lco GEOMETRY=AS_WKT
ogr2ogr -f "CSV" montseny-network/nodes.csv montseny-network/nodes.shp -lco GEOMETRY=AS_WKT

