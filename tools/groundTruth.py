# Converts a osm gpx file from lotLan coordinates to utm.
# TODO: HK-48: Format this to so it is a well-formatted python script. 
# TODO: HK-49: Do this but is c++ itself. 

from numpy import min_scalar_type
import utm
import xml.etree.ElementTree as ET

GROUND_TRUTH_INPUT_FILE = "groundTruthLatLon.gpx"
GROUND_TRUTH_OUTPUT_FILE = "groundTruthUtm.gpx"

print("Transforming file ", GROUND_TRUTH_INPUT_FILE, "into UTM coordinates...")

tree = ET.parse(GROUND_TRUTH_INPUT_FILE)
root = tree.getroot()

for way_node in root:
    if way_node.tag == "way":
        for point_node in way_node:
            attributes = point_node.attrib

            if point_node.tag == "bounds":
                min_lat = float(attributes["minlat"])
                min_lon = float(attributes["minlon"])
                max_lat = float(attributes["maxlat"])
                max_lon = float(attributes["maxlon"])

                min_easting, min_northing, _, _ = utm.from_latlon(min_lat, min_lon)
                max_easting, max_northing, _, _ = utm.from_latlon(max_lat, max_lon)
                point_node.set("mineast", str(min_easting))
                point_node.set("minnorth", str(min_northing))
                point_node.set("maxeast", str(max_easting))
                point_node.set("maxnorth", str(max_northing))

                attributes.pop("minlat")
                attributes.pop("minlon")
                attributes.pop("maxlat")
                attributes.pop("maxlon")

            elif point_node.tag == "nd":
                longitude = float(attributes["lon"])
                latitude = float(attributes["lat"])
                easting, northing, _, _ = utm.from_latlon(latitude, longitude)

                point_node.set("northing", str(northing))
                point_node.set("easting", str(easting))

                attributes.pop("lon")
                attributes.pop("lat")

tree.write(GROUND_TRUTH_OUTPUT_FILE)

print("Done")
