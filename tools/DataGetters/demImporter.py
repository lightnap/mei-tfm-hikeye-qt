#!/usr/bin/env python3


import requests
import utm

DEM_FILE = "./montsent-dem.tiff"


def main():

    print("Importing dem")

    # bounds = (17.4110711999999985, 18.4494298999999984, 59.1412578999999994, 59.8280297000000019) # Stocklom
    # bounds_latlon = (2.335337, 2.432706, 41.783415, 41.849994)  # Montseny

    # easting_min, northing_min, _, _ = utm.from_latlon(
    #    bounds_latlon[2], bounds_latlon[0]
    # )
    # easting_max, northing_max, _, _ = utm.from_latlon(
    #    bounds_latlon[3], bounds_latlon[1]
    # )

    easting_min = 444825.0
    northing_min = 4625885.0
    easting_max = 452859.0
    northing_max = 4633335.0

    url_base = "https://geoserveis.icgc.cat/icgc_mdt2m/wms/service?"
    url_service = "REQUEST=GetMap&SERVICE=WMS&VERSION=1.3.0&LAYERS=MET2m"
    url_image_format = (
        "&STYLES=&FORMAT=image/tiff&BGCOLOR=0xFFFFFF&TRANSPARENT=FALSE&CRS=EPSG:25831"
    )
    url_bounding_box = (
        "&BBOX="
        + str(easting_min)
        + ","
        + str(northing_min)
        + ","
        + str(easting_max)
        + ","
        + str(northing_max)
    )
    # url_size = "&WIDTH=3725&HEIGHT=4017"
    # url_size = "&WIDTH=817&HEIGHT=836"
    url_size = "&WIDTH=3100&HEIGHT=3017"
    full_url = url_base + url_service + url_image_format + url_bounding_box + url_size

    print(full_url)
    return
    response = requests.get(full_url)
    response.raise_for_status()

    with open(DEM_FILE, "wb") as file:
        file.write(response.content)

    print("Done")

    return 0


if __name__ == "__main__":
    main()
