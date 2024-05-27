#!/usr/bin/env python3

"""
Converts gpx track files into csv files undestandable to fmm.

This sricpt takes a directory full of a gpx files and creates a directoy of
filesin a csv format so that the fmm project understands as a gps track input.
Each track segment of the gpx is printed in a different line of the csv,
to be considered a different track for fmm.
"""

import gpxpy
import os
import utm
import csv

TRACK_GPX_DIRECTORY = "tracks_gpx"
TRACK_CSV_DIRECTORY = "tracks_csv"


def gpx2csv(gpx_file_path, csv_file_path):

    with open(gpx_file_path, "r") as f:
        gpx = gpxpy.parse(f)

    with open(csv_file_path, "w") as f:
        csv_file = csv.writer(
            f, delimiter=";", quotechar='"', quoting=csv.QUOTE_MINIMAL
        )

        csv_file.writerow(["id", "geom", "dates"])

        index = 1
        for track in gpx.tracks:
            for segment in track.segments:
                segment_points = []
                segment_times = []
                for point in segment.points:
                    easting, northing, _, _ = utm.from_latlon(
                        point.latitude, point.longitude
                    )
                    segment_points.append(str(easting) + " " + str(northing))
                    segment_times.append(point.time.timestamp())

                segment_wkt = (
                    "LINESTRING(" + ",".join(str(x) for x in segment_points) + ")"
                )
                times_str = ",".join(str(x) for x in segment_times)
                csv_file.writerow([str(index), segment_wkt, times_str])
                index = index + 1
    return


def main():

    directory = os.fsencode(TRACK_GPX_DIRECTORY)

    for file in os.listdir(directory):
        filename = os.fsdecode(file)

        track_gpx_file = TRACK_GPX_DIRECTORY + "/" + filename
        track_csv_file = TRACK_CSV_DIRECTORY + "/" + filename.split(".")[0] + ".csv"

        print("Creating file" + track_csv_file + "...")
        gpx2csv(track_gpx_file, track_csv_file)


main()
